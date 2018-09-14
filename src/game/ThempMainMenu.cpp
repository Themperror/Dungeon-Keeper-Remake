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
	for (size_t i = 1; i < 22; i++)
	{
		std::wstring path = L"LDATA\\DKMAP";
		memset(buf, 0, 8);
		_itow(i, buf, 10);
		if (i < 10)
		{
			path.append(1, L'0');
		}
		path.append(buf);
		m_MapFiles[i] = new Object2D(path, L".RAW");
		m_MapFiles[i]->SetVisibility(false);
		Themp::System::tSys->m_Game->AddObject3D(m_MapFiles[i]->m_Renderable);
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


	for (size_t i = 0; i < 7; i++)
	{
		int normal[3] = {13, 14, 15 };
		int hover[3] = { 4, 5, 6 };
		int click[3] = { 7, 8, 9 };
		float xOffsets[3] = { -0.75f,0,0.75f };
		float yOffsets[3] = { 0,0,0 };
		float zOffsets[3] = { 0.05,0,0 }; //this is such a shitty hack, but I NEED the other buttons to draw over the first one..
		float xSizes[3] = { 0.19,0.19 ,0.19 };
		float ySizes[3] = { 0.075 ,0.075 ,0.075 };
		m_GUIButtons[i] = new GUIButton(Object2D::MENU_MAIN, normal,hover,click,xOffsets,yOffsets,zOffsets, 3, true);
		for (size_t j = 0; j < 3; j++)
		{
			m_GUIButtons[i]->SetSize(xSizes[j], ySizes[j],j);
		}
		m_GUIButtons[i]->SetPosition(0, 0.945 - (i * 0.415),0.5);
		m_GUIButtons[i]->SetVisibility(true);
	}
	//m_GUIButtons[0]->SetVisibility(false);


	//Load cursor sprite
	cursor = new Object2D(Object2D::MENU_CURSOR,0,true);
	cursor->m_Renderable->SetScale(0.2, 0.2, 0.2);
	cursor->SetVisibility(false);
	System::tSys->m_Game->AddObject3D(cursor->m_Renderable);

	//skip video
	//m_DoPlayVideo = false;
	//m_VideoObject->isVisible = false;
	//m_State = MenuState::Loading;
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

		cursor->m_Renderable->SetPosition(uiMouseX+0.2, uiMouseY-0.2, 0.1);
		for (size_t i = 0; i < 7; i++)
		{
			if (m_GUIButtons[i])
			{
				m_GUIButtons[i]->SetVisibility(true);
				if (m_GUIButtons[i]->Update(uiMouseX, uiMouseY, g->m_Keys[256], InitialClick))
				{
					if (g->m_Keys[256] == 2)
					{
						InitialClick = m_GUIButtons[i];
					}
					if (g->m_Keys[256] == -1) // just up
					{
						if (InitialClick == m_GUIButtons[i])
						{
							Themp::System::Print("Clicked on the button!");
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
			cursor->m_Renderable->m_Meshes[0]->m_Material->SetTexture(FileManager::GetMenuCursorTexture(cursorIndex++)->texture);
		}
		//spriteTest->NextFrame();

		

		//render Main Menu layout
		//-Start New Game - > CampaignSelect;

		//-Continue Game - > Load most recent save;

		//-Load Game - > LoadGame

		//-Multiplayer - > disabled

		//-Options - > Options


		//ifbutton presses - set state to that button menu or quite
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
		}
		for (size_t i = 0; i < 7; i++)
		{
			if (m_GUIButtons[i])
			{
				delete m_GUIButtons[i];
			}
		}
		
		delete cursor;
		m_VideoObject->isVisible = false;
		m_IsDone = true;
	}
}