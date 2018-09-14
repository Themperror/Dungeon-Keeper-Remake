#include "ThempSystem.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempMainMenu.h"
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
	m_Camera->SetPosition(-0.100051 , 43.199852, -273.101563);
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
		//printf("DT: %f \n", dt);
		bool changedAValue = false;

		ImGui::BeginMainMenuBar();

		ImGui::PushID(1);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7, 0.1, 0.1, 0.8));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9, 0.1, 0.1, 1.0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0, 0.0, 0.0, 1.0));
		if (ImGui::Button("Quit", ImVec2(60, 20)))
		{
			Themp::System::tSys->m_Quitting = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
		const char* MSAA[4] = { "x1","x2","x4","x8" };
	
		if (ImGui::Checkbox("Wireframe", &Themp::System::tSys->m_D3D->m_Wireframe))
		{

		}
		ImGui::EndMainMenuBar();
	
		if (changedAValue)
		{
			Themp::System::tSys->m_D3D->dirtySystemBuffer = true;
		}

		//left mouse button
		if (m_Keys[1023])
		{
			totalMouseX += m_CursorDeltaX*mouseSensitivity;
			totalMouseY += m_CursorDeltaY*mouseSensitivity;

			totalMouseY = totalMouseY > 90.0f ? 90.0f : totalMouseY < -90.0f ? -90.0f : totalMouseY;
			m_Camera->Rotate(totalMouseX, totalMouseY);
		}
		double speedMod = 0.05;
		if (m_Keys[VK_SHIFT])
		{
			speedMod = .3;
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
		if (m_Keys['Y'])
		{
			XMFLOAT3 p = m_Camera->GetPosition();
			XMFLOAT3 f = m_Camera->GetForward();
		}
	}

	m_Camera->Update(dt);
	m_Camera->UpdateMatrices();
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
