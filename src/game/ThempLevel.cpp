#include "ThempSystem.h"
#include "ThempLevel.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempObject2D.h"
#include "ThempGUIButton.h"
#include "ThempFont.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include <DirectXMath.h>

using namespace Themp;

Level::~Level()
{

}
XMFLOAT2 cursorOffset = XMFLOAT2(0.03, 0);
Level::Level(int levelIndex)
{
	m_Cursor = new Object2D(Object2D::Source::sMENU_CURSOR, 1, true);
	m_Cursor->SetVisibility(true);
	System::tSys->m_Game->AddObject3D(m_Cursor->m_Renderable);
}
void Level::Update(float delta)
{
	Game* g = Themp::System::tSys->m_Game;

	float uiMouseX = 0, uiMouseY = 0;
	Game::TranslateMousePos(g->m_CursorDeltaX, g->m_CursorDeltaY, uiMouseX, uiMouseY);
	m_Cursor->m_Renderable->SetPosition(uiMouseX + cursorOffset.x, uiMouseY - cursorOffset.y, 0.01);
}