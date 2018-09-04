#include "ThempSystem.h"
#include "ThempMainMenu.h"
#include "ThempGame.h"
#include "ThempObject2D.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include "../Engine/ThempVideo.h"
#include "ThempFileManager.h"
#include <DirectXMath.h>

Themp::Material* videoMat = nullptr;
Themp::Material* loadingMat = nullptr;
void Themp::MainMenu::Start()
{
	memset(m_MapFiles, 0, sizeof(Object2D*) * 22);
	m_Video = new Themp::Video();
	FileManager::FileData videoData = FileManager::GetFileData(L"LDATA\\INTROMIX.SMK");
	if (!m_Video->Load(videoData.data, videoData.size))
	{
		System::Print("Could not find ldata\\intromix.smk");
	}
	m_VideoObject = new Object3D();

	m_VideoObject->CreateQuad("", false);

	std::vector<std::string> textures = { "" };
	std::vector<std::uint8_t> types = { Material::DIFFUSE };
	videoMat = Themp::System::tSys->m_Resources->GetMaterial(std::string("Video"), textures, types, "Video", false);
	videoMat->m_SamplerStates[1] = D3D::DefaultTextureSamplerFiltered;
	m_VideoObject->SetMaterial(videoMat);
	Themp::System::tSys->m_Game->AddObject3D(m_VideoObject);

	wchar_t buf[8];
	m_MapFiles[0] = new Object2D(L"LDATA\\DKMAP00",true);
	for (size_t i = 1; i < 22; i++)
	{
		std::wstring path = L"LDATA\\DKMAP";
		memset(buf, 0, 8);
		_itow(i, buf, 10);
		if (i < 10)
		{
			path.append(1,L'0');
		}
		path.append(buf);
		m_MapFiles[i] = new Object2D(path, true);
	}
	//m_LoadingScreenTexture = new Object2D("ldata/LOADING", true);
	m_LoadingScreenTexture = new Object2D(L"DATA\\FRONTEND", true);
	m_TortureTexture = new Object2D(L"LDATA\\TORTURE", true);
	m_LoadingScreen = new Object3D();

	m_LoadingScreen->CreateQuad("", false);

	loadingMat = Themp::System::tSys->m_Resources->GetMaterial(std::string("LoadingScreen"), textures, types, "ScreenSpace", false);
	m_LoadingScreen->SetMaterial(loadingMat);
	Themp::System::tSys->m_Game->AddObject3D(m_LoadingScreen);
	m_LoadingScreen->isVisible = false;

	loadingMat->SetTexture(m_LoadingScreenTexture->m_Tex);
}

//temporary
int tIndex = -2;

void Themp::MainMenu::Update(double dt)
{
	Game* g = Themp::System::tSys->m_Game;
	switch (m_State)
	{
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
			m_LoadingScreen->isVisible = true;
			if (g->m_Keys['N'] == 2)
			{
				tIndex++;
				if (tIndex == 22)
				{
					tIndex = -2;
				}
				if (tIndex >= 0)
				{
					loadingMat->SetTexture(m_MapFiles[tIndex]->m_Tex);
				}
				else
				{
					if (tIndex == -2)
					{
						loadingMat->SetTexture(m_LoadingScreenTexture->m_Tex);
					}
					else
					{
						loadingMat->SetTexture(m_TortureTexture->m_Tex);
					}
				}
			}
			//create loading screen and render

			//load assets

			//create Main Menu layout
		}
		else
		{
			m_State = MenuState::Menu;
		}
		break;
	case MenuState::Menu:
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
		for (size_t i = 0; i < 22; i++)
		{
			if (m_MapFiles[i])
			{
				delete m_MapFiles[i];
			}
		}
		m_VideoObject->isVisible = false;
		m_IsDone = true;
	}
}