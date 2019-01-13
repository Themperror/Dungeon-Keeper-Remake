#include "ThempSystem.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempMainMenu.h"
#include "ThempLevel.h"
#include "Creature/ThempCreature.h"
#include "ThempEntity.h"
#include "ThempResources.h"
#include "ThempLevelConfig.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempDebugDraw.h"

float totalMouseX = 0, totalMouseY = 0;
float oldMouseX=0, oldMouseY = 0;
float mouseSensitivity = 0.15f;

void Themp::Game::Start()
{
	m_Camera = new Themp::Camera();
	m_Camera->SetPosition(0,10,0);
	m_Camera->SetTarget(XMFLOAT3(0,0,0));
	m_Camera->Rotate(0, 0);
	m_Camera->SetAspectRatio(Themp::D3D::s_D3D->m_ScreenWidth / Themp::D3D::s_D3D->m_ScreenHeight);
	m_Camera->SetFoV(75);
	m_Camera->SetProjection(Camera::CameraType::Perspective);
	m_Camera->SetNear(0.1f);
	m_Camera->SetFar(1000.0f);

	System::Print("Creating Filemanager!");
	m_FileManager = new FileManager();
	System::Print("Creating MainMenu!");
	m_MainMenu = new MainMenu();

	System::Print("Loading Creature.txt!");
	if (!LevelConfig::LoadConfiguration())
	{
		System::Print("Game::Start || Was not able to load data\\creature.txt!");
		Stop();
		return;
	}

	System::Print("Starting MainMenu!");
	m_MainMenu->Start();
}
void Themp::Game::TranslateMousePos(int inX, int inY, float& outX, float& outY)
{
	outX = (float)inX / Themp::D3D::s_D3D->m_ScreenWidth;
	outX *= 4;
	outX -= 2;
	outY = (float)inY / Themp::D3D::s_D3D->m_ScreenHeight;
	outY *= 4;
	outY -= 2;
	outY = -outY;
}
float OrthoSize = 10;
bool IsOrtho = true;
void Themp::Game::Update(double dt)
{
	m_CursorWindowedX = m_CursorWindowedX < 0 ? 0 : m_CursorWindowedX > D3D::s_D3D->m_ScreenWidth  ? D3D::s_D3D->m_ScreenWidth  : m_CursorWindowedX;
	m_CursorWindowedY = m_CursorWindowedY < 0 ? 0 : m_CursorWindowedY > D3D::s_D3D->m_ScreenHeight ? D3D::s_D3D->m_ScreenHeight : m_CursorWindowedY;

	DebugDraw::Update((float)dt);
	m_Camera->SetAspectRatio(Themp::D3D::s_D3D->m_ScreenWidth / Themp::D3D::s_D3D->m_ScreenHeight);
	m_CursorDeltaX = oldMouseX - m_CursorWindowedX;
	m_CursorDeltaY = oldMouseY - m_CursorWindowedY;

	if (!m_MainMenu->m_IsDone)
	{
		m_MainMenu->Update(dt);
	}
	else
	{
		m_CurrentLevel->Update((float)dt);
	}

	//left mouse button
	if (m_Keys[257])
	{
		totalMouseX += m_CursorDeltaX * mouseSensitivity;
		totalMouseY += m_CursorDeltaY * mouseSensitivity;

		totalMouseY = totalMouseY > 90.0f ? 90.0f : totalMouseY < -90.0f ? -90.0f : totalMouseY;
		m_Camera->Rotate(totalMouseX, totalMouseY);
	}
	float speedMod = 0.05f;
	if (m_Keys[VK_SHIFT])
	{
		speedMod = 0.2f;
	}
	m_Camera->SetSpeed(speedMod);
	if (m_Keys['W'])
	{
		m_Camera->MoveForward();
	}
	if (m_Keys['S'])
	{
		m_Camera->MoveBackward();
	}
	if (m_Keys['A'])
	{
		m_Camera->MoveLeft();
	}
	if (m_Keys['D'])
	{
		m_Camera->MoveRight();
	}
	//spacebar
	if (m_Keys[32] || m_Keys['E'])
	{
		m_Camera->MoveUp();
	}
	if (m_Keys['X'] || m_Keys['Q'])
	{
		m_Camera->MoveDown();
	}

	m_Camera->Update((float)dt);
	m_Camera->UpdateMatrices();
	oldMouseX = m_CursorWindowedX;
	oldMouseY = m_CursorWindowedY;
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
	if (m_CurrentLevel)
	{
		delete m_CurrentLevel;
	}
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
void Themp::Game::AddCreature(Creature* creature)
{
	m_Creatures.push_back(creature);
}
void Themp::Game::AddEntity(Entity* entity)
{
	m_Entities.push_back(entity);
}
