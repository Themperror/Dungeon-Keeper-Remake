#include "ThempSystem.h"
#include "ThempEntity.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempResources.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "ThempLevelConfig.h"
#include <DirectXMath.h>
#include <unordered_map>
#include <array>
using namespace Themp;

//first FP sprite, then W sprite
const std::vector<int> EntityIDs
{
	/*Entity_Gold0 =*/ 2220,
	/*Entity_Gold0 =*/ 2225,
	/*Entity_Gold1 =*/ 2221,
	/*Entity_Gold1 =*/ 2226,
	/*Entity_Gold2 =*/ 2222,
	/*Entity_Gold2 =*/ 2227,
	/*Entity_Gold3 =*/ 2223,
	/*Entity_Gold3 =*/ 2228,
	/*Entity_Gold4 =*/ 2223,
	/*Entity_Gold4 =*/ 2229,
	/*Dungeon_Heart*/  2232,
	/*Dungeon_Heart*/  2233,
	/*Entity_LairFly_FP,*/ 2225,
	/*Entity_LairFly_W,*/ 2225,
	/*Entity_LairBeetle_FP*/ 2225,
	/*Entity_LairBeetle_W,*/ 2225,
	/*Entity_Corpse_NULL,			*/
	/*Entity_Corpse_WIZARD,			*/
	/*Entity_Corpse_BARBARIAN,		*/
	/*Entity_Corpse_ARCHER,			*/
	/*Entity_Corpse_MONK,			*/
	/*Entity_Corpse_DWARF,			*/
	/*Entity_Corpse_KNIGHT,			*/
	/*Entity_Corpse_AVATAR,			*/
	/*Entity_Corpse_TUNNELLER,		*/
	/*Entity_Corpse_WITCH,			*/
	/*Entity_Corpse_GIANT,			*/
	/*Entity_Corpse_FAIRY,			*/
	/*Entity_Corpse_THIEF,			*/
	/*Entity_Corpse_SAMURAI,		*/
	/*Entity_Corpse_HORNY,			*/
	/*Entity_Corpse_SKELETON,		*/
	/*Entity_Corpse_TROLL,			*/
	/*Entity_Corpse_DRAGON,			*/
	/*Entity_Corpse_DEMONSPAWN,		*/
	/*Entity_Corpse_FLY,			*/
	/*Entity_Corpse_DARK_MISTRESS,	*/
	/*Entity_Corpse_SORCEROR,		*/
	/*Entity_Corpse_BILE_DEMON,		*/
	/*Entity_Corpse_IMP,			*/
	/*Entity_Corpse_BEETLE,			*/
	/*Entity_Corpse_VAMPIRE,		*/
	/*Entity_Corpse_SPIDER,			*/
	/*Entity_Corpse_HELLHOUND,		*/
	/*Entity_Corpse_GHOST,			*/
	/*Entity_Corpse_TENTACLE,		*/
	/*Entity_Corpse_ORC,			*/
	/*Entity_Corpse_FLOATING_SPIRIT,*/
};
const std::vector<std::string> EntityIDToName =
{
	"Entity_Gold0_FP",
	"Entity_Gold0_W" ,
	"Entity_Gold1_FP",
	"Entity_Gold1_W" ,
	"Entity_Gold2_FP",
	"Entity_Gold2_W" ,
	"Entity_Gold3_FP",
	"Entity_Gold3_W" ,
	"Entity_Gold4_FP",
	"Entity_Gold4_W" ,
	"Entity_DungeonHeart_FP",
	"Entity_DungeonHeart_W",
	"Entity_LairFly_FP",
	"Entity_LairFly_W",
	"Entity_LairBeetle_FP",
	"Entity_LairBeetle_W",
};

Themp::Entity::~Entity()
{
	if (m_EntityCB)
	{
		m_EntityCB->Release();
		m_EntityCB = nullptr;
	}
}

Themp::Entity::Entity(Entity::EntityType entityType)
{
	m_EntityID = entityType;
	m_EntitySpriteIndex = EntityIDs[entityType];
	m_Renderable = new Object3D();
	m_Renderable->CreateQuad("creature", false);
	Material* material = Resources::TRes->GetUniqueMaterial( "", "creature");
	//Material* material = Resources::TRes->GetMaterial(EntityIDToName[m_EntityID], "", "creature", false);
	m_Renderable->SetMaterial(material);

	SetSprite(m_EntitySpriteIndex);

	m_EntityCBData._AnimIndex = 0;
	m_EntityCBData._NumAnim = m_Sprite->numAnim;
	m_EntityCBData._SpriteWidth = m_Sprite->texture->m_Width;
	m_EntityCBData._isFrozen = false;
	m_EntityCBData._isFlipped = true;
	if (!m_EntityCB)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CreatureConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_EntityCBData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the buffer.
		Themp::System::tSys->m_D3D->m_Device->CreateBuffer(&cbDesc, &InitData, &m_EntityCB);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		Themp::System::tSys->m_D3D->m_DevCon->Map(m_EntityCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_EntityCBData, sizeof(CreatureConstantBuffer));
		Themp::System::tSys->m_D3D->m_DevCon->Unmap(m_EntityCB, NULL);
	}
	
	m_Renderable->m_Meshes[0]->m_ConstantBuffer = m_EntityCB;
}
void Entity::SetSpriteFromType(Entity::EntityType entityType)
{
	m_EntityID = entityType;
	m_EntitySpriteIndex = EntityIDs[entityType];
	SetSprite(m_EntitySpriteIndex);
}
void Entity::SetSprite(int SpriteID)
{
	m_Sprite = FileManager::GetCreatureSprite(SpriteID);
	m_EntitySpriteIndex = SpriteID;
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Sprite->texture);
	m_EntityCBData._NumAnim = m_Sprite->numAnim;
}
void Entity::ResetScale()
{
	const Camera::CameraType camType = System::tSys->m_Game->m_Camera->GetProjection();
	if (m_EntityID >= EntityIDs.size())
	{
		SetSprite(m_EntityID + (camType != Camera::Orthographic));
	}
	else
	{
		SetSprite(EntityIDs[m_EntityID + (camType != Camera::Orthographic)]);
	}
	m_Renderable->SetScale((float)m_Sprite->unscaledW / 32.0f, (float)m_Sprite->unscaledH / 32.0f, 1);
}
void Entity::Update(float delta)
{
	m_AnimationTime += delta;
	if (m_AnimationTime > 1.0f / 10.0f)
	{
 		if (m_AnimationTime > 2 * (1.0f / 10.0f))
		{
			m_AnimationTime = 2 * (1.0f / 10.0f);
		}
		m_AnimationTime -= 1.0f / 10.0f;
		m_AnimationIndex++;
		m_AnimationIndex = m_AnimationIndex % (int)m_EntityCBData._NumAnim;
		m_EntityCBData._AnimIndex = m_AnimationIndex;

		//const Camera::CameraType camType = System::tSys->m_Game->m_Camera->GetProjection();
		//SetSprite(EntityIDs[m_EntityID + (camType != Camera::Orthographic)]);
		ResetScale();
		m_Renderable->isDirty = true;
	}
	//DebugDraw::Line(m_Renderable->m_Position, m_Renderable->m_Position + m_Direction * 3);
}
void Entity::Draw(D3D& d3d)
{
	if (!m_Renderable->isVisible)return;
	if (m_Renderable->isDirty)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		d3d.m_DevCon->Map(m_EntityCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_EntityCBData, sizeof(CreatureConstantBuffer));
		d3d.m_DevCon->Unmap(m_EntityCB, NULL);
	}
	m_Renderable->Draw(d3d);
}
void Entity::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}