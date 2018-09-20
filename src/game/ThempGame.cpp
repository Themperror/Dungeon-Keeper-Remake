#include "ThempSystem.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempMainMenu.h"
#include "ThempLevel.h"
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
#include <DirectXMath.h>

float totalMouseX = 0, totalMouseY = -20;
float mouseSensitivity = 0.15f;

void Themp::Game::Start()
{
	m_Camera = new Themp::Camera();
	m_Camera->SetPosition(0 , 10, 0);
	m_Camera->SetTarget(XMFLOAT3(-0.100051, 43.199852, -273.101563));
	m_Camera->Rotate(0, -20);
	m_Camera->SetAspectRatio(Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH] / Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT]);
	m_Camera->SetFoV(75);
	m_Camera->SetNear(0.1f);
	m_Camera->SetFar(1000.0f);

	m_FileManager = new FileManager();
	m_MainMenu = new MainMenu();

	m_MainMenu->Start();
}
void Themp::Game::TranslateMousePos(int inX, int inY, float& outX, float& outY)
{
	outX = (float)inX / (float)Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH];
	outX *= 4;
	outX -= 2;
	outY = (float)inY / (float)Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT];
	outY *= 4;
	outY -= 2;
	outY = -outY;
}
void Themp::Game::Update(double dt)
{
	DebugDraw::Update((float)dt);
	m_Camera->SetAspectRatio(Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH] / Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT]);
	
	if (!m_MainMenu->m_IsDone)
	{
		m_MainMenu->Update(dt);
	}
	else
	{
		m_CurrentLevel->Update(dt);
	}

	m_Camera->Update(dt);
	m_Camera->UpdateMatrices();
}
void Themp::Game::LoadLevel(int levelIndex)
{
	m_CurrentLevel = new Level(levelIndex);
}
void Themp::Game::Stop()
{
	m_MainMenu->Stop();
	delete m_MainMenu;
	delete m_Camera;
	delete m_FileManager;
	m_Camera = nullptr;
	for (int i = 0; i < m_Objects3D.size(); i++)
	{
		m_Objects3D[i] = nullptr;
	}
	m_Objects3D.clear();
}

void Themp::Game::AddObject3D(Object3D * obj)
{
	m_Objects3D.push_back(obj);
}
