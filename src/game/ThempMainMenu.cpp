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

Themp::Material* videoMat = nullptr;
Themp::Material* loadingMat = nullptr;

Themp::Object2D* cursor = nullptr;

void Themp::MainMenu::Start()
{
	memset(m_MapFiles, 0, sizeof(Object2D*) * 22);

	m_SplashScreenTexture = new Object2D(L"DATA\\LEGAL", L".RAW");
	m_SplashScreenTexture->SetVisibility(true);
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

	//Load map files
	wchar_t buf[8];
	m_MapFiles[0] = new Object2D(L"LDATA\\DKMAP00", L".RAW");
	m_MapFiles[0]->SetVisibility(false);
	m_MapWindowFiles[0] = new Object2D(L"LDATA\\DKWIND00", L".DAT");
	m_MapWindowFiles[0]->SetVisibility(false);
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
		m_MapFiles[i]->SetVisibility(false);

		m_MapWindowFiles[i] = new Object2D(pathwnd, L".DAT");
		m_MapWindowFiles[i]->SetVisibility(false);

		Themp::System::tSys->m_Game->AddObject3D(m_MapFiles[i]->m_Renderable);
		//Themp::System::tSys->m_Game->AddObject3D(m_MapWindowFiles[i]->m_Renderable);
	}
	//Load Loading screen 
	m_LoadingScreenTexture = new Object2D(L"LDATA\\LOADING", L".RAW");
	m_LoadingScreenTexture->SetVisibility(false);
	m_LoadingScreenTexture->m_Renderable->m_Scale = XMFLOAT3(2, 2, 2);
	Themp::System::tSys->m_Game->AddObject3D(m_LoadingScreenTexture->m_Renderable);

	//Load Main Menu texture
	m_MenuBackgroundTexture = new Object2D(L"LDATA\\FRONT", L".RAW");
	m_MenuBackgroundTexture->m_Renderable->m_Scale = XMFLOAT3(2, 2, 2);
	m_MenuBackgroundTexture->SetVisibility(false);
	m_MenuBackgroundTexture->m_Renderable->m_Position.z = 1.0;
	Themp::System::tSys->m_Game->AddObject3D(m_MenuBackgroundTexture->m_Renderable);

	//Load Torture texture
	m_TortureTexture = new Object2D(L"LDATA\\TORTURE", L".RAW");
	m_TortureTexture->SetVisibility(false);
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
		float zOffsets[3] = { 0.05,0,0 }; //this is such a shitty hack, but I NEED the other buttons to draw over the first one..
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

	m_TextObject = new Font("Main Menu", Font::FontTexID::MENU_NORMAL0,false, XMFLOAT3(0, 1.65, 0.4));

	//Load cursor sprite
	cursor = new Object2D(Object2D::sMENU_MAIN,94,true);
	cursor->m_Renderable->SetScale(0.2, 0.2, 0.2);
	cursor->SetVisibility(false);
	System::tSys->m_Game->AddObject3D(cursor->m_Renderable);

	//skip video
	m_DoPlayVideo = false;
	m_VideoObject->isVisible = false;
	m_SplashScreenTexture->SetVisibility(false);
	m_State = MenuState::Loading;
}

void TranslateMousePos(int inX, int inY, float& outX, float& outY)
{
	outX = (float)inX / (float)Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH];
	outX *= 4;
	outX -= 2;
	outY = (float)inY / (float)Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT];
	outY *= 4;
	outY -= 2;
	outY = -outY;
}


Themp::GUIButton* InitialClick = nullptr;
int cursorIndex = 0;
float waitingTime = 0;
void Themp::MainMenu::Update(double dt)
{
	Game* g = Themp::System::tSys->m_Game;

	float uiMouseX = 0, uiMouseY = 0;
	TranslateMousePos(g->m_CursorDeltaX, g->m_CursorDeltaY, uiMouseX, uiMouseY);
	cursor->m_Renderable->SetPosition(uiMouseX + 0.2, uiMouseY - 0.2, 0.1);
	//System::Print("Cursor X: %f, Cursor Y: %f", g->m_CursorDeltaX, g->m_CursorDeltaY);
	switch (m_State)
	{
	case MenuState::Splash:
		waitingTime+=dt;
		if (waitingTime > 5.0 || (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2))
		{
			m_State = MenuState::Intro;
			waitingTime = 0;
			m_SplashScreenTexture->SetVisibility(false);
			m_Video->videoTimer.GetDeltaTimeReset();
		}
		break;
	case MenuState::Intro:

		if (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2)
		{
			m_DoPlayVideo = false;
			m_VideoObject->isVisible = false;
			m_State = MenuState::Loading;
		}
		if (m_DoPlayVideo && m_Video)
		{
			int videoStatus = m_Video->Play();
			if (m_Video->m_Tex)
			{
				videoMat->SetTexture(m_Video->m_Tex);
			}
			if (videoStatus == 2) //video done
			{
				m_State = MenuState::Loading;
				m_DoPlayVideo = false;
				m_VideoObject->isVisible = false;
			}
		}
		break;
	case MenuState::Loading:
		if (!m_IsLoaded)
		{
			//loading screen and render
			m_LoadingScreenTexture->m_Renderable->isVisible = true;
			m_IsLoaded = true;
			//load assets

			//create Main Menu layout
		}
		else
		{
			//can possibly remove? No need for an actual "Loading" screen
			waitingTime += dt;
			if (waitingTime > 3000 || (g->m_Keys[VK_SPACE] == 2 || g->m_Keys[VK_ESCAPE] == 2))
			{
				m_LoadingScreenTexture->m_Renderable->isVisible = false;
				m_State = MenuState::Menu;
				waitingTime = 0;
			}
		}
		break;
	case MenuState::Menu:

		cursor->SetVisibility(true);
		m_MenuBackgroundTexture->SetVisibility(true);

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
						if (InitialClick == m_GUIButtons[i])
						{
							switch (i)
							{
							case 0: //Start New Game
								m_State = MenuState::CampaignSelect;
								m_CurrentLevel = 0;
								m_MapFiles[m_CurrentLevel]->SetVisibility(true);
								m_MapFiles[m_CurrentLevel]->m_Renderable->SetPosition(0, 0, 0.9);
								m_MapFiles[0]->SetScale(0.7, 0.7);
								m_MapWindowFiles[0]->SetVisibility(true);
								m_MapWindowFiles[0]->m_Renderable->SetPosition(0, 0, 0.8);
								m_MapWindowFiles[0]->SetScale(0.8,0.8);
								for (size_t j = 0; j < 7; j++)
								{
									m_GUIButtons[j]->SetVisibility(false);
								}
								for (size_t j = 0; j < 3; j++)
								{
									m_MainMenuBar[j]->SetVisibility(false);
								}
								m_MenuBackgroundTexture->SetVisibility(false);
								m_TextObject->SetVisibility(false);
								break;
							case 1: //Continue
								m_State = MenuState::CampaignSelect;
								//m_CurrentLevel = LastCompletedLevel+1;
								//continueing = true;
								break;
							case 2: //Load Game
								m_State = MenuState::LoadGame;
								break;
							case 3: //Multiplayer
								//Nothing Happens, We're not gonna do MP until we're completely done
								break;
							case 4: //Options
								m_State = MenuState::Options;
								break;
							case 5: //High Score Table
								m_State = MenuState::HighScore;
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
		if (ImGui::Button("Next GUI"))
		{
			Texture* tex = FileManager::GetMenuCursorTexture((cursorIndex++))->texture;
			cursor->m_Renderable->m_Meshes[0]->m_Material->SetTexture(tex);
			cursor->m_Tex = tex;
			Themp::System::Print("Cursor Tex Index: %i",cursorIndex);
		}
		break;
	case MenuState::CampaignSelect:
		//show campaign stuff to visible
		//enable clickboxes
		//if clicked - stop menu -> load level
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

void Themp::MainMenu::Stop()
{
	if (!m_IsDone)
	{
		if (m_Video)
		{
			delete m_Video;
		}
		if (m_LoadingScreenTexture)
		{
			delete m_LoadingScreenTexture;
		}
		if (m_TortureTexture)
		{
			delete m_TortureTexture;
		}
		if (m_MenuBackgroundTexture)
		{
			delete m_MenuBackgroundTexture;
		}
		if (m_SplashScreenTexture)
		{
			delete m_SplashScreenTexture;
		}
		for (size_t i = 0; i < 22; i++)
		{
			if (m_MapFiles[i])
			{
				delete m_MapFiles[i];
			}
			if (m_MapWindowFiles[i])
			{
				delete m_MapWindowFiles[i];
			}
		}
		for (size_t i = 0; i < 7; i++)
		{
			if (m_GUIButtons[i])
			{
				delete m_GUIButtons[i];
			}
		}
		if (m_TextObject)
		{
			delete m_TextObject;
		}
		delete cursor;
		m_VideoObject->isVisible = false;
		m_IsDone = true;
	}
}