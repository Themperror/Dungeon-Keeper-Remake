#include "ThempSystem.h"
#include "ThempCreature.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
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

using namespace Themp;

Themp::Creature::~Creature()
{

}

Themp::Creature::Creature(int creatureSpriteIndex)
{
	m_Sprite = FileManager::GetCreatureSprite(creatureSpriteIndex);
	m_Renderable = new Object3D();
	m_Renderable->CreateQuad("ScreenSpace", false);
	size_t uniqueMatID = (size_t)this;
	char buf[16] = { 0 };
	_i64toa(uniqueMatID, buf, 16);
	Material* material = Resources::TRes->GetMaterial(buf, "", "Video", false);
	material->SetTexture(&m_Sprite->textures[0]);
	m_Renderable->SetMaterial(material);
}
void Creature::NextFrame()
{
	currentAnim++;
	if (currentAnim >= m_Sprite->numAnim)
	{
		currentAnim = 0;
	}
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(&m_Sprite->textures[currentAnim]);
}
void Creature::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}