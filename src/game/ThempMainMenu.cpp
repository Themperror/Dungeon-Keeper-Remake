#include "ThempSystem.h"
#include "ThempMainMenu.h"
#include "ThempGame.h"
#include "ThempObject2D.h"
#include "ThempResources.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempVideo.h"
#include "ThempFileManager.h"
#include "ThempGUIButton.h"
#include "ThempFont.h"
#include <imgui.h>
#include <DirectXMath.h>

using namespace Themp;
Themp::Material* videoMat = nullptr;
Themp::Material* loadingMat = nullptr;

XMFLOAT2 levelFlagPositions[22] =
{
	{ -10,0 },
	{ 1.0f,-0.83f },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
	{ 0,0 },
};
void Themp::MainMenu::Start()
{
	memset(m_MapFiles, 0, sizeof(Object2D*) * 22);

	m_SplashScreenTexture = new Object2D(L"DATA\\LEGAL", L".RAW");
	m_SplashScreenTexture->m_Renderable->m_Scale = XMFLOAT3(2, 2, 2);
	Themp::System::tSys->m_Game->AddObject3D(m_SplashScreenTexture->m_Renderable);

	//Set up video
	m_Video = new Themp::Video(&FileManager::GetFileData(L"LDATA\\INTROMIX.SMK"));
	if (!m_Video->isLoaded)
	{
		System::Print("Could not find ldata\\intromix.smk");
	}
	m_VideoObject = new Object3D();
	m_VideoObject->CreateQuad("", false);
	m_VideoObject->m_Scale = XMFLOAT3(2, 2, 2);
	Themp::System::tSys->m_Game->AddObject3D(m_VideoObject);

	//Create Video material
	std::vector<std::string> textures = { "" };
	std::vector<std::uint8_t> types = { Material::DIFFUSE };
	videoMat = Themp::System::tSys->m_Resources->GetMaterial(std::string("Video"), textures, types, "ScreenSpace", false);
	m_VideoObject->SetMaterial(videoMat);
	videoMat->SetTexture(m_Video->m_Tex);

	//Load map files
	wchar_t buf[8];
	m_MapFiles[0] = new Object2D(L"LDATA\\DKMAP00", L".RAW");
	m_MapWindowFiles[0] = new Object2D(L"LDATA\\DKWIND00", L".DAT");
	Themp::System::tSys->m_Game->AddObject3D(m_MapFiles[0]->m_Renderable);
	Themp::System::tSys->m_Game->AddObject3D(m_MapWindowFiles[0]->m_Renderable);
	for (size_t i = 1; i < 22; i++)
	{
		std::wstring path = L"LDATA\\DKMAP";
		std::wstring pathwnd = L"LDATA\\DKWIND";

		memset(buf, 0, 8);
		_itow(i, buf, 10);

		if (i < 10)
		{
			path.append(1, L'0');
			pathwnd.append(1, L'0');
		}

		path.append(buf);
		pathwnd.append(buf);

		m_MapFiles[i] = new Object2D(path, L".RAW");

		m_MapWindowFiles[i] = new Object2D(pathwnd, L".DAT");

		Themp::System::tSys->m_Game->AddObject3D(m_MapFiles[i]->m_Renderable);
		Themp::System::tSys->m_Game->AddObject3D(m_MapWindowFiles[i]->m_Renderable);
	}
	//Load Loading screen 
	m_LoadingScreenTexture = new Object2D(L"LDATA\\LOADING", L".RAW");
	m_LoadingScreenTexture->m_Renderable->m_Scale = XMFLOAT3(2, 2, 2);
	Themp::System::tSys->m_Game->AddObject3D(m_LoadingScreenTexture->m_Renderable);

	//Load Main Menu texture
	m_MenuBackgroundTexture = new Object2D(L"LDATA\\FRONT", L".RAW");
	m_MenuBackgroundTexture->m_Renderable->m_Scale = XMFLOAT3(2, 2, 2);
	m_MenuBackgroundTexture->m_Renderable->m_Position.z = 1.0;
	Themp::System::tSys->m_Game->AddObject3D(m_MenuBackgroundTexture->m_Renderable);

	//Load Torture texture
	m_TortureTexture = new Object2D(L"LDATA\\TORTURE", L".RAW");
	Themp::System::tSys->m_Game->AddObject3D(m_TortureTexture->m_Renderable);

	m_MainMenuBar[0] = new Object2D(Object2D::sMENU_MAIN, 13, true);
	m_MainMenuBar[1] = new Object2D(Object2D::sMENU_MAIN, 14, true);
	m_MainMenuBar[2] = new Object2D(Object2D::sMENU_MAIN, 15, true);
	Themp::System::tSys->m_Game->AddObject3D(m_MainMenuBar[2]->m_Renderable);
	Themp::System::tSys->m_Game->AddObject3D(m_MainMenuBar[1]->m_Renderable);
	Themp::System::tSys->m_Game->AddObject3D(m_MainMenuBar[0]->m_Renderable);

	m_MainMenuBar[0]->m_Renderable->SetPosition(-0.75f, 1.645, 0.5);
	m_MainMenuBar[1]->m_Renderable->SetPosition(0, 1.645, 0.5);
	m_MainMenuBar[2]->m_Renderable->SetPosition(0.75f, 1.645, 0.5);

	std::string buttonTexts[7] = { "Start New Game", "Continue Game", "Load Game", "Multiplayer", "Options", "High Score Table", "Quit" };
	bool disabled[7] = { false,true,true,true,false,false,false };
	for (size_t i = 0; i < 7; i++)
	{
		int normal[3] = {13, 14, 15 };
		int hover[3] = { 1, 2, 3 };
		int click[3] = { 1, 2, 3 };
		float xOffsets[3] = { -0.75f,0,0.75f };
		float yOffsets[3] = { 0,0,0 };
		float zOffsets[3] = { 0.05f,0,0 }; //this is such a shitty hack, but I NEED the other buttons to draw over the first one..
		m_GUIButtons[i] = new GUIButton(Object2D::sMENU_MAIN, normal,hover,click,xOffsets,yOffsets,zOffsets, 3, true, buttonTexts[i], Font::MENU_NORMAL1, Font::MENU_NORMAL2, Font::MENU_NORMAL3);
		for (size_t j = 0; j < 3; j++)
		{
			//m_GUIButtons[i]->SetSize(xSizes[j], ySizes[j], j);
			m_GUIButtons[i]->SetSize(1,1,1,1, j);
		}
		m_GUIButtons[i]->SetPosition(0, 0.945 - (i * 0.415),0.5);
		m_GUIButtons[i]->SetVisibility(true);
		m_GUIButtons[i]->m_IsDisabled = disabled[i];
	}
	m_LevelSelectFlag = new GUIButton(Object2D::sMENU_LEVELFLAG, 1,6,1, true);
	m_TextObject = new Font("Main Menu", Font::FontTexID::MENU_NORMAL0,false, XMFLOAT3(0, 1.65, 0.4));

	//Load cursor sprite
	m_Cursor = new Object2D(Object2D::sMENU_MAIN,94,true);
	//cursor->m_Renderable->SetScale(0.2, 0.2, 0.2);
	System::tSys->m_Game->AddObject3D(m_Cursor->m_Renderable);

	SetEverythingHidden();
	//GoToSplash();
	GoToMenu();
}




Themp::GUIButton* InitialClick = nullptr;
int cursorIndex = 0;
int blockType = 0;
float waitingTime = 0;
XMFLOAT2 mouseOffset = XMFLOAT2(0.166, 0.18);
void Themp::MainMenu::Update(double dt)
{
	Game* g = Themp::System::tSys->m_Game;

	float uiMouseX = 0, uiMouseY = 0;
	Game::TranslateMousePos(g->m_CursorWindowedX, g->m_CursorWindowedY, uiMouseX, uiMouseY);
	m_Cursor->m_Renderable->SetPosition(uiMouseX+ mouseOffset.x,uiMouseY - mouseOffset.y, 0.01);

	const float scrollSpeed = 2.0; //map scroll speed

	switch (m_State)
	{
	case MenuState::Splash:
		waitingTime+=dt;
		if (waitingTime > 3.0 || (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2))
		{
			GoToIntro();
			waitingTime = 0;
			m_Video->videoTimer.GetDeltaTimeReset();
		}
		break;
	case MenuState::Intro:

		if (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2)
		{
			m_DoPlayVideo = false;
			GoToLoading();
		}
		if (m_DoPlayVideo && m_Video)
		{
			int videoStatus = m_Video->Play();
			if (videoStatus == 2) //video done
			{
				GoToLoading();
				m_DoPlayVideo = false;
			}
		}
		break;
	case MenuState::Loading:
		//can possibly remove? No need for an actual "Loading" screen
		waitingTime += dt;
		if (waitingTime > 1.5 || (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2))
		{
			m_IsLoaded = true;
			GoToMenu();
			waitingTime = 0;
		}
		break;
	case MenuState::Menu:
		for (size_t i = 0; i < 7; i++)
		{
			if (m_GUIButtons[i]->m_IsVisible)
			{
				if (m_GUIButtons[i]->Update(dt, uiMouseX, uiMouseY, g->m_Keys[256], InitialClick))
				{
					if (g->m_Keys[256] == 2)
					{
						InitialClick = m_GUIButtons[i];
					}
					if (g->m_Keys[256] == -1) // just up
					{
						if (InitialClick == m_GUIButtons[i] && !m_GUIButtons[i]->m_IsDisabled)
						{
							switch (i)
							{
							case 0: //Start New Game
								m_CurrentLevel = 1;
								GoToCampaignSelect();
								break;
							case 1: //Continue
								GoToContinue();
								//m_CurrentLevel = LastCompletedLevel+1;
								//continueing = true;
								break;
							case 2: //Load Game
								GoToLoadGame();
								break;
							case 3: //Multiplayer
								GoToMultiplayer();
								//Nothing Happens, We're not gonna do MP until we're completely done
								break;
							case 4: //Options
								GoToOptions();
								break;
							case 5: //High Score Table
								GoToHighScore();
								break;
							case 6: //Quit
								System::tSys->m_Quitting = true;
								break;
							}
						}
						else
						{
							InitialClick = nullptr;
						}
					}
				}
			}
		}
		if (ImGui::Button("Next Tex"))
		{
			m_Cursor->SetTexture(FileManager::GetBlockTexture(blockType));
			m_Cursor->SetScale(1, 1);
		}
		if(ImGui::SliderInt("Block Type", &blockType, 0, 7))
		{
			m_Cursor->SetTexture(FileManager::GetBlockTexture(blockType));
			m_Cursor->SetScale(1, 1);
		}
		break;
	case MenuState::CampaignSelect:
		if (ImGui::Button("Next Level"))
		{
			for (size_t i = 0; i < 22; i++)
			{
				m_MapWindowFiles[i]->SetVisibility(false);
				m_MapFiles[i]->SetVisibility(false);
			}
			m_CurrentLevel++;
			if (m_CurrentLevel >= 22)
			{
				m_CurrentLevel = 0;
			}
			m_MapWindowFiles[m_CurrentLevel]->SetVisibility(true);
			m_MapFiles[m_CurrentLevel]->SetVisibility(true);
			m_MapWindowFiles[m_CurrentLevel]->m_Renderable->SetPosition(0, 0, 0.8);
			m_MapFiles[m_CurrentLevel]->m_Renderable->SetPosition(0, 0, 0.9);
			m_LevelSelectFlag->SetPosition(0, 0, 0.85f);
		}
		if (g->m_Keys[VK_ESCAPE] == 2)
		{
			GoToMenu();
			return;
		}
		//show campaign stuff to visible
		//enable clickboxes
		//if clicked - stop menu -> load level
		XMFLOAT3 borderPos = m_MapWindowFiles[m_CurrentLevel]->m_Renderable->m_Position;
		XMFLOAT3 mapPos = m_MapFiles[m_CurrentLevel]->m_Renderable->m_Position;
		borderPos.z = 0.8;
		mapPos.z = 0.9;
		if (uiMouseX < -1.8)
		{
			borderPos.x += dt * scrollSpeed  * 0.5;
			mapPos.x += dt * scrollSpeed;
			if (borderPos.x > 0.99)
			{
				borderPos.x = 0.99;
				mapPos.x = 1.99;
			}
		}
		else if (uiMouseX > 1.8)
		{
			borderPos.x -= dt * scrollSpeed * 0.5;
			mapPos.x -= dt * scrollSpeed;
			if (borderPos.x < -0.99)
			{
				borderPos.x = -0.99;
				mapPos.x = -1.99;
			}
		}
		if (uiMouseY > 1.8)
		{
			borderPos.y -= dt * scrollSpeed * 0.75;
			mapPos.y -= dt * scrollSpeed;
			if (borderPos.y < -0.99)
			{
				borderPos.y = -0.99;
				mapPos.y = -1.33;
			}
		}
		else if (uiMouseY < -1.8)
		{
			borderPos.y += dt * scrollSpeed * 0.75;
			mapPos.y += dt * scrollSpeed;
			if (borderPos.y > 0.99)
			{
				borderPos.y = 0.99;
				mapPos.y = 1.33;
			}
		}
		m_MapWindowFiles[m_CurrentLevel]->m_Renderable->SetPosition(borderPos);
		m_MapFiles[m_CurrentLevel]->m_Renderable->SetPosition(mapPos);
		m_LevelSelectFlag->SetPosition(mapPos.x + levelFlagPositions[m_CurrentLevel].x, mapPos.y + levelFlagPositions[m_CurrentLevel].y, 0.75f);
		if (m_LevelSelectFlag->Update(dt, uiMouseX, uiMouseY, g->m_Keys[256], InitialClick))
		{
			if (g->m_Keys[256] == 2)
			{
				InitialClick = m_LevelSelectFlag;
			}
			if (g->m_Keys[256] == -1 && InitialClick == m_LevelSelectFlag) // just up
			{
				Stop();
				g->LoadLevel(m_CurrentLevel);
				System::Print("Clicked the flag!");
			}
		}
		else
		{
			InitialClick = nullptr;
		}
		break;
	case MenuState::LoadGame:
		//show save slots
		//if clicked -> load save
		break;
	case MenuState::Options:
		//show Options Menu
		break;
	case MenuState::HighScore:
		//show Highscore Menu
		break;
	}
}
void MainMenu::GoToSplash()
{
	SetEverythingHidden();
	m_SplashScreenTexture->SetVisibility(true);
	m_State = MenuState::Splash;
}
void MainMenu::GoToCampaignSelect()
{
	SetEverythingHidden();
	m_State = MenuState::CampaignSelect;
	m_MapFiles[m_CurrentLevel]->SetVisibility(true);
	m_MapFiles[m_CurrentLevel]->m_Renderable->SetPosition(0, 0, 0.9);
	m_MapWindowFiles[m_CurrentLevel]->SetVisibility(true);
	m_MapWindowFiles[m_CurrentLevel]->m_Renderable->SetPosition(0, 0, 0.8);
	m_LevelSelectFlag->SetVisibility(true);
	m_Cursor->SetVisibility(true);
	m_Cursor->SetTexture(FileManager::GetMenuCursorTexture(0));
	m_Cursor->SetScale(1, 1);
	mouseOffset = XMFLOAT2(0.03, 0);
}
void MainMenu::GoToContinue()
{

}
void MainMenu::GoToLoadGame()
{

}
void MainMenu::GoToMultiplayer()
{

}
void MainMenu::GoToHighScore()
{

}
void MainMenu::GoToOptions()
{

}
void MainMenu::GoToLoading()
{
	SetEverythingHidden();
	m_State = MenuState::Loading;
	m_LoadingScreenTexture->SetVisibility(true);
}
void MainMenu::GoToIntro()
{
	SetEverythingHidden();
	m_State = MenuState::Intro;
	m_VideoObject->isVisible = true;
}
void MainMenu::GoToMenu()
{
	SetEverythingHidden();
	m_State = MenuState::Menu;
	m_MenuBackgroundTexture->SetVisibility(true);
	m_Cursor->SetVisibility(true);
	m_Cursor->SetTexture(FileManager::GetMenuGUITexture(94));
	m_Cursor->SetScale(1, 1);
	for (size_t i = 0; i < 7; i++)
	{
		m_GUIButtons[i]->SetVisibility(true);
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_MainMenuBar[i]->SetVisibility(true);
	}
	m_TextObject->SetVisibility(true);
	mouseOffset = XMFLOAT2(0.166, 0.18);
}
void MainMenu::SetEverythingHidden()
{
	m_VideoObject->isVisible = false;
	m_LoadingScreenTexture->SetVisibility(false);
	m_TortureTexture->SetVisibility(false);
	m_MenuBackgroundTexture->SetVisibility(false);
	m_SplashScreenTexture->SetVisibility(false);
	m_LevelSelectFlag->SetVisibility(false);
	m_TextObject->SetVisibility(false);
	m_Cursor->SetVisibility(false);
	for (size_t i = 0; i < 22; i++)
	{
		m_MapWindowFiles[i]->SetVisibility(false);
		m_MapFiles[i]->SetVisibility(false);
	}
	for (size_t j = 0; j < 7; j++)
	{
		m_GUIButtons[j]->SetVisibility(false);
	}
	for (size_t j = 0; j < 3; j++)
	{
		m_MainMenuBar[j]->SetVisibility(false);
	}
}
void MainMenu::Stop()
{
	if (!m_IsDone)
	{
		if (m_Video)
		{
			delete m_Video;
			m_Video = nullptr;
		}
		if (m_LoadingScreenTexture)
		{
			delete m_LoadingScreenTexture;
			m_LoadingScreenTexture = nullptr;
		}
		if (m_TortureTexture)
		{
			delete m_TortureTexture;
			m_TortureTexture = nullptr;
		}
		if (m_MenuBackgroundTexture)
		{
			delete m_MenuBackgroundTexture;
			m_MenuBackgroundTexture = nullptr;
		}
		if (m_SplashScreenTexture)
		{
			delete m_SplashScreenTexture;
			m_SplashScreenTexture = nullptr;
		}
		for (size_t i = 0; i < 22; i++)
		{
			if (m_MapFiles[i])
			{
				delete m_MapFiles[i];
				m_MapFiles[i] = nullptr;
			}
			if (m_MapWindowFiles[i])
			{
				delete m_MapWindowFiles[i];
				m_MapWindowFiles[i] = nullptr;
			}
		}
		for (size_t i = 0; i < 7; i++)
		{
			if (m_GUIButtons[i])
			{
				delete m_GUIButtons[i];
				m_GUIButtons[i] = nullptr;
			}
		}
		if (m_LevelSelectFlag)
		{
			delete m_LevelSelectFlag;
			m_LevelSelectFlag = nullptr;
		}
		if (m_TextObject)
		{
			delete m_TextObject;
			m_TextObject = nullptr;
		}
		if (m_Cursor)
		{
			delete m_Cursor;
			m_Cursor = nullptr;
		}
		System::tSys->m_Game->m_Objects3D.clear();
		m_VideoObject->isVisible = false;
		m_IsDone = true;
	}
}