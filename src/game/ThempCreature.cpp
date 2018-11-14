#include "ThempSystem.h"
#include "ThempCreature.h"
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
#include "../Engine/ThempDebugDraw.h"
#include "ThempLevelConfig.h"
#include <DirectXMath.h>
#include <unordered_map>
#include <array>
using namespace Themp;

//2 types (FP or World), 5 directions, 12 states
const std::array<std::array<std::array<CreatureData::ImpAnimations, 12>, 5>, 2> ImpAnimState =
{
	//5 directional sprites
	std::array<std::array<CreatureData::ImpAnimations, 12>, 5>
	{
		std::array<CreatureData::ImpAnimations, 12>
		{
			CreatureData::IMP_FP_WALK_Forward,
			CreatureData::IMP_FP_DROPPING_Forward,
			CreatureData::IMP_FP_MINE_Forward,
			CreatureData::IMP_FP_HIT_Forward,
			CreatureData::IMP_FP_IDLE_Forward,
			CreatureData::IMP_FP_CLAIM_Forward,
			CreatureData::IMP_FP_DRAG_Forward,
			CreatureData::IMP_FP_IDLE2_Forward,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Forward,
		},
		{
			CreatureData::IMP_FP_WALK_DiagForwardRight,
			CreatureData::IMP_FP_DROPPING_DiagForwardRight,
			CreatureData::IMP_FP_MINE_DiagForwardRight,
			CreatureData::IMP_FP_HIT_DiagForwardRight,
			CreatureData::IMP_FP_IDLE_DiagForwardRight,
			CreatureData::IMP_FP_CLAIM_DiagForwardRight,
			CreatureData::IMP_FP_DRAG_DiagForwardRight,
			CreatureData::IMP_FP_IDLE2_DiagForwardRight,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_DiagForwardRight,
		},
		{
			CreatureData::IMP_FP_WALK_Right,
			CreatureData::IMP_FP_DROPPING_Right,
			CreatureData::IMP_FP_MINE_Right,
			CreatureData::IMP_FP_HIT_Right,
			CreatureData::IMP_FP_IDLE_Right,
			CreatureData::IMP_FP_CLAIM_Right,
			CreatureData::IMP_FP_DRAG_Right,
			CreatureData::IMP_FP_IDLE2_Right,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Right,
		},
		{
			CreatureData::IMP_FP_WALK_DiagBackRight,
			CreatureData::IMP_FP_DROPPING_DiagBackRight,
			CreatureData::IMP_FP_MINE_DiagBackRight,
			CreatureData::IMP_FP_HIT_DiagBackRight,
			CreatureData::IMP_FP_IDLE_DiagBackRight,
			CreatureData::IMP_FP_CLAIM_DiagBackRight,
			CreatureData::IMP_FP_DRAG_DiagBackRight,
			CreatureData::IMP_FP_IDLE2_DiagBackRight,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_DiagBackRight,
		},
		{
			CreatureData::IMP_FP_WALK_Back,
			CreatureData::IMP_FP_DROPPING_Back,
			CreatureData::IMP_FP_MINE_Back,
			CreatureData::IMP_FP_HIT_Back,
			CreatureData::IMP_FP_IDLE_Back,
			CreatureData::IMP_FP_CLAIM_Back,
			CreatureData::IMP_FP_DRAG_Back,
			CreatureData::IMP_FP_IDLE2_Back,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Back,
		},
	},
	//5 directional sprites
	std::array<std::array<CreatureData::ImpAnimations, 12>, 5>
	{
		std::array<CreatureData::ImpAnimations, 12>
		{
			CreatureData::IMP_W_WALK_Forward,
			CreatureData::IMP_W_DROPPING_Forward,
			CreatureData::IMP_W_MINE_Forward,
			CreatureData::IMP_W_HIT_Forward,
			CreatureData::IMP_W_IDLE_Forward,
			CreatureData::IMP_W_CLAIM_Forward,
			CreatureData::IMP_W_DRAG_Forward,
			CreatureData::IMP_W_IDLE2_Forward,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Forward,
		},
		{
			CreatureData::IMP_W_WALK_DiagForwardRight,
			CreatureData::IMP_W_DROPPING_DiagForwardRight,
			CreatureData::IMP_W_MINE_DiagForwardRight,
			CreatureData::IMP_W_HIT_DiagForwardRight,
			CreatureData::IMP_W_IDLE_DiagForwardRight,
			CreatureData::IMP_W_CLAIM_DiagForwardRight,
			CreatureData::IMP_W_DRAG_DiagForwardRight,
			CreatureData::IMP_W_IDLE2_DiagForwardRight,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_DiagForwardRight,
		},
		{
			CreatureData::IMP_W_WALK_Right,
			CreatureData::IMP_W_DROPPING_Right,
			CreatureData::IMP_W_MINE_Right,
			CreatureData::IMP_W_HIT_Right,
			CreatureData::IMP_W_IDLE_Right,
			CreatureData::IMP_W_CLAIM_Right,
			CreatureData::IMP_W_DRAG_Right,
			CreatureData::IMP_W_IDLE2_Right,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Right,
		},
		{
			CreatureData::IMP_W_WALK_DiagBackRight,
			CreatureData::IMP_W_DROPPING_DiagBackRight,
			CreatureData::IMP_W_MINE_DiagBackRight,
			CreatureData::IMP_W_HIT_DiagBackRight,
			CreatureData::IMP_W_IDLE_DiagBackRight,
			CreatureData::IMP_W_CLAIM_DiagBackRight,
			CreatureData::IMP_W_DRAG_DiagBackRight,
			CreatureData::IMP_W_IDLE2_DiagBackRight,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_DiagBackRight,
		},
		{
			CreatureData::IMP_W_WALK_Back,
			CreatureData::IMP_W_DROPPING_Back,
			CreatureData::IMP_W_MINE_Back,
			CreatureData::IMP_W_HIT_Back,
			CreatureData::IMP_W_IDLE_Back,
			CreatureData::IMP_W_CLAIM_Back,
			CreatureData::IMP_W_DRAG_Back,
			CreatureData::IMP_W_IDLE2_Back,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Back,
		},
	},
};

//2 types (FP or World), 5 directions, 12 states
const std::array<std::array<std::array<CreatureData::Animations, 12>, 5>, 2> CreatureAnimState =
{ 
	std::array<std::array<CreatureData::Animations, 12>, 5>
	{
		std::array<CreatureData::Animations, 12>
		{
			CreatureData::FP_WALK_Forward,
			CreatureData::FP_DROPPING_Forward,
			CreatureData::FP_MELEE_Forward,
			CreatureData::FP_HIT_Forward,
			CreatureData::FP_BEG_Forward,
			CreatureData::FP_DIE,
			CreatureData::FP_HAND,
			CreatureData::FP_HAPPY,
			CreatureData::FP_SLEEP,
			CreatureData::FP_SLAPPED_Forward,
			CreatureData::FP_EAT,
			CreatureData::FP_PISS_Forward,
		},
		{

			CreatureData::FP_WALK_DiagForwardRight,
			CreatureData::FP_DROPPING_DiagForwardRight,
			CreatureData::FP_MELEE_DiagForwardRight,
			CreatureData::FP_HIT_DiagForwardRight,
			CreatureData::FP_BEG_DiagForwardRight,
			CreatureData::FP_DIE,
			CreatureData::FP_HAND,
			CreatureData::FP_HAPPY,
			CreatureData::FP_SLEEP,
			CreatureData::FP_SLAPPED_DiagForwardRight,
			CreatureData::FP_EAT,
			CreatureData::FP_PISS_DiagForwardRight,
		},
		{
			CreatureData::FP_WALK_Right,
			CreatureData::FP_DROPPING_Right,
			CreatureData::FP_MELEE_Right,
			CreatureData::FP_HIT_Right,
			CreatureData::FP_BEG_Right,
			CreatureData::FP_DIE,
			CreatureData::FP_HAND,
			CreatureData::FP_HAPPY,
			CreatureData::FP_SLEEP,
			CreatureData::FP_SLAPPED_Right,
			CreatureData::FP_EAT,
			CreatureData::FP_PISS_Right,
		},
		{
			CreatureData::FP_WALK_DiagBackRight,
			CreatureData::FP_DROPPING_DiagBackRight,
			CreatureData::FP_MELEE_DiagBackRight,
			CreatureData::FP_HIT_DiagBackRight,
			CreatureData::FP_BEG_DiagBackRight,
			CreatureData::FP_DIE,
			CreatureData::FP_HAND,
			CreatureData::FP_HAPPY,
			CreatureData::FP_SLEEP,
			CreatureData::FP_SLAPPED_DiagBackRight,
			CreatureData::FP_EAT,
			CreatureData::FP_PISS_DiagBackRight,
		},
		{
			CreatureData::FP_WALK_Back,
			CreatureData::FP_DROPPING_Back,
			CreatureData::FP_MELEE_Back,
			CreatureData::FP_HIT_Back,
			CreatureData::FP_BEG_Back,
			CreatureData::FP_DIE,
			CreatureData::FP_HAND,
			CreatureData::FP_HAPPY,
			CreatureData::FP_SLEEP,
			CreatureData::FP_SLAPPED_Back,
			CreatureData::FP_EAT,
			CreatureData::FP_PISS_Back,
		},
	},
	{
		std::array<CreatureData::Animations, 12>
		{
			CreatureData::W_WALK_Forward,
			CreatureData::W_DROPPING_Forward,
			CreatureData::W_MELEE_Forward,
			CreatureData::W_HIT_Forward,
			CreatureData::W_BEG_Forward,
			CreatureData::W_DIE,
			CreatureData::W_HAND,
			CreatureData::W_HAPPY,
			CreatureData::W_SLEEP,
			CreatureData::W_SLAPPED_Forward,
			CreatureData::W_EAT,
			CreatureData::W_PISS_Forward,
		},
		{

			CreatureData::W_WALK_DiagForwardRight,
			CreatureData::W_DROPPING_DiagForwardRight,
			CreatureData::W_MELEE_DiagForwardRight,
			CreatureData::W_HIT_DiagForwardRight,
			CreatureData::W_BEG_DiagForwardRight,
			CreatureData::W_DIE,
			CreatureData::W_HAND,
			CreatureData::W_HAPPY,
			CreatureData::W_SLEEP,
			CreatureData::W_SLAPPED_DiagForwardRight,
			CreatureData::W_EAT,
			CreatureData::W_PISS_DiagForwardRight,
		},
		{
			CreatureData::W_WALK_Right,
			CreatureData::W_DROPPING_Right,
			CreatureData::W_MELEE_Right,
			CreatureData::W_HIT_Right,
			CreatureData::W_BEG_Right,
			CreatureData::W_DIE,
			CreatureData::W_HAND,
			CreatureData::W_HAPPY,
			CreatureData::W_SLEEP,
			CreatureData::W_SLAPPED_Right,
			CreatureData::W_EAT,
			CreatureData::W_PISS_Right,
		},
		{
			CreatureData::W_WALK_DiagBackRight,
			CreatureData::W_DROPPING_DiagBackRight,
			CreatureData::W_MELEE_DiagBackRight,
			CreatureData::W_HIT_DiagBackRight,
			CreatureData::W_BEG_DiagBackRight,
			CreatureData::W_DIE,
			CreatureData::W_HAND,
			CreatureData::W_HAPPY,
			CreatureData::W_SLEEP,
			CreatureData::W_SLAPPED_DiagBackRight,
			CreatureData::W_EAT,
			CreatureData::W_PISS_DiagBackRight,
		},
		{
			CreatureData::W_WALK_Back,
			CreatureData::W_DROPPING_Back,
			CreatureData::W_MELEE_Back,
			CreatureData::W_HIT_Back,
			CreatureData::W_BEG_Back,
			CreatureData::W_DIE,
			CreatureData::W_HAND,
			CreatureData::W_HAPPY,
			CreatureData::W_SLEEP,
			CreatureData::W_SLAPPED_Back,
			CreatureData::W_EAT,
			CreatureData::W_PISS_Back,
		},
	},
};


const std::unordered_map<CreatureData::CreatureType, CreatureData::CreatureSpriteIndex> TypeToSprite =
{
	{CreatureData::CREATURE_NULL, CreatureData::CreatureSprite_Ghost },
	{CreatureData::CREATURE_WIZARD, CreatureData::CreatureSprite_Wizard },
	{CreatureData::CREATURE_BARBARIAN, CreatureData::CreatureSprite_Barbarian },
	{CreatureData::CREATURE_ARCHER, CreatureData::CreatureSprite_Archer },
	{CreatureData::CREATURE_MONK, CreatureData::CreatureSprite_Monk },
	{CreatureData::CREATURE_DWARF, CreatureData::CreatureSprite_Dwarf },
	{CreatureData::CREATURE_KNIGHT, CreatureData::CreatureSprite_Knight },
	{CreatureData::CREATURE_AVATAR, CreatureData::CreatureSprite_Avatar },
	{CreatureData::CREATURE_TUNNELLER, CreatureData::CreatureSprite_Tunneler },
	{CreatureData::CREATURE_WITCH, CreatureData::CreatureSprite_Witch },
	{CreatureData::CREATURE_GIANT, CreatureData::CreatureSprite_Giant },
	{CreatureData::CREATURE_FAIRY, CreatureData::CreatureSprite_Fairy },
	{CreatureData::CREATURE_THIEF, CreatureData::CreatureSprite_Thief },
	{CreatureData::CREATURE_SAMURAI, CreatureData::CreatureSprite_Samurai },
	{CreatureData::CREATURE_HORNY, CreatureData::CreatureSprite_Horny },
	{CreatureData::CREATURE_SKELETON, CreatureData::CreatureSprite_Skeleton },
	{CreatureData::CREATURE_TROLL, CreatureData::CreatureSprite_Troll },
	{CreatureData::CREATURE_DRAGON, CreatureData::CreatureSprite_Dragon },
	{CreatureData::CREATURE_DEMONSPAWN, CreatureData::CreatureSprite_DemonSpawn },
	{CreatureData::CREATURE_FLY, CreatureData::CreatureSprite_Fly },
	{CreatureData::CREATURE_DARK_MISTRESS, CreatureData::CreatureSprite_DarkMistress },
	{CreatureData::CREATURE_SORCEROR, CreatureData::CreatureSprite_Warlock },
	{CreatureData::CREATURE_BILE_DEMON, CreatureData::CreatureSprite_BileDemon},
	{CreatureData::CREATURE_IMP, CreatureData::CreatureSprite_Imp },
	{CreatureData::CREATURE_BEETLE, CreatureData::CreatureSprite_Beetle },
	{CreatureData::CREATURE_VAMPIRE, CreatureData::CreatureSprite_Vampire },
	{CreatureData::CREATURE_SPIDER, CreatureData::CreatureSprite_Spider },
	{CreatureData::CREATURE_HELLHOUND, CreatureData::CreatureSprite_Hellhound },
	{CreatureData::CREATURE_GHOST, CreatureData::CreatureSprite_Ghost },
	{CreatureData::CREATURE_TENTACLE, CreatureData::CreatureSprite_Tentacle },
	{CreatureData::CREATURE_ORC, CreatureData::CreatureSprite_Orc },
	{CreatureData::CREATURE_FLOATING_SPIRIT, CreatureData::CreatureSprite_Ghost },
};


Themp::Creature::~Creature()
{
	if (m_CreatureCB)
	{
		m_CreatureCB->Release();
		m_CreatureCB = nullptr;
	}
}

Themp::Creature::Creature(CreatureData::CreatureType creatureIndex)
{
	m_CreatureID = creatureIndex;
	m_CreatureData = LevelConfig::creatureData[m_CreatureID];
	m_CreatureSpriteIndex = TypeToSprite.find(creatureIndex)->second;
	m_AnimState = CreatureData::AnimationState::Walking;
	m_Renderable = new Object3D();
	m_Renderable->CreateQuad("creature", false);
	Material* material = Resources::TRes->GetUniqueMaterial("", "creature");
	m_Renderable->SetMaterial(material);

	SetSprite(m_CreatureSpriteIndex);
	m_Speed = 12.0;

	m_Direction = XMFLOAT3(1, 0, 0);

	m_CreatureCBData._AnimIndex = 0;
	m_CreatureCBData._NumAnim = m_Sprite->numAnim;
	m_CreatureCBData._SpriteWidth = m_Sprite->texture->m_Width;
	m_CreatureCBData._isFrozen = false;
	m_CreatureCBData._isFlipped = false;
	if (!m_CreatureCB)
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
		InitData.pSysMem = &m_CreatureCBData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the buffer.
		Themp::System::tSys->m_D3D->m_Device->CreateBuffer(&cbDesc, &InitData, &m_CreatureCB);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		Themp::System::tSys->m_D3D->m_DevCon->Map(m_CreatureCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_CreatureCBData, sizeof(CreatureConstantBuffer));
		Themp::System::tSys->m_D3D->m_DevCon->Unmap(m_CreatureCB, NULL);
	}
	for (int i = 0; i < 10; i++)
	{
		m_PowerCooldownTimer[i] = 0.0f;
	}
	m_Renderable->m_Meshes[0]->m_ConstantBuffer = m_CreatureCB;
}
void Creature::SetSprite(int SpriteID)
{
	m_Sprite = FileManager::GetCreatureSprite(SpriteID);
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Sprite->texture);
	m_CreatureCBData._NumAnim = m_Sprite->numAnim;
}
void Creature::Update(float delta)
{
	for (int i = 0; i < 10; i++)
	{
		if(m_PowerCooldownTimer[i] > 0.0f)
			m_PowerCooldownTimer[i] -= delta;
	}
	if (m_ImpSpecialTimer > 0.0f)
	{
		m_ImpSpecialTimer -= delta;
	}
	if (m_CreatureID == CreatureData::CREATURE_IMP)
	{
		//Check if current task is still completeable or reachable
		//for example, if an enemy imp has not taken over a neighbouring tile, so that claiming his currently tasked tile would be impossible (or reinforcing a wall)
		//if so, set current order on invalid and RemoveTask();
		//
		//uint16_t targetTileType = Level::s_CurrentLevel->m_LevelData->m_Map.m_Tiles[m_Order.targetTilePos.y][m_Order.targetTilePos.x].type & 0xFF;
		//if (targetTileType >= Type_Earth && targetTileType <= )
		//CreatureTaskManager::UnlistCreatureFromTask(this);
		////or
		//CreatureTaskManager::RemoveClaimingTask()
		//CreatureTaskManager::RemoveReinforcingTask()
		//
		

		GetTask();
 		if (m_Order.valid)
		{
			int pathingResult = micropather::MicroPather::SOLVED;
			if (LevelData::PathsInvalidated || m_Path.size() == 0 || m_JustSlapped)
			{
				m_JustSlapped = false;
				const XMINT3 subTilePos = LevelData::WorldToSubtile(m_Renderable->m_Position);
				float PathCost = 0;
				m_CurrentPathIndex = 0;
				pathingResult = System::tSys->m_Game->m_CurrentLevel->PathFind(XMINT2(subTilePos.x, subTilePos.z), m_Order.subTilePos, m_Path, PathCost, true);
				if (pathingResult != micropather::MicroPather::SOLVED && pathingResult != micropather::MicroPather::START_END_SAME)
				{
					System::Print("Imp could not find path, resetting world pathing");
					LevelData::PathsInvalidated = true;
					CreatureTaskManager::UnlistCreatureFromTask(this);
					StopOrder();
				}
			}
			m_PathLerpTime += delta * m_Speed;
			if (pathingResult == micropather::MicroPather::SOLVED || pathingResult == micropather::MicroPather::START_END_SAME)
			{
				if (m_PathLerpTime >= 1.0f)
				{
					m_PathLerpTime = 0.0f;
					m_CurrentPathIndex++;
				}
				XMFLOAT3 currentPos = m_Renderable->m_Position;
				const XMINT3 currentSubTilePos = LevelData::WorldToSubtile(currentPos);
				currentPos.y = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight(currentSubTilePos.z / 3, currentSubTilePos.x / 3, currentSubTilePos.z % 3,currentSubTilePos.x % 3);
				float OldPathX = m_Renderable->m_Position.x;
				float OldPathY = m_Renderable->m_Position.z;
			
				if (m_CurrentPathIndex < m_Path.size())
				{
					if (m_CurrentPathIndex > 0)
					{
						OldPathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex - 1]) & 0xFFFFFFF);
						OldPathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex - 1]) >> 32) & 0xFFFFFFF);
					}
					const float PathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex]) & 0xFFFFFFF);
					const float PathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex]) >> 32) & 0xFFFFFFF);
					

					const XMFLOAT2 nPos = Lerp(XMFLOAT2(OldPathX, OldPathY), XMFLOAT2(PathX, PathY), m_PathLerpTime);
					XMFLOAT2 dir = nPos - XMFLOAT2(OldPathX,OldPathY);
					if (dir.x != 0 || dir.y != 0)
					{
						dir = Normalize(dir);
						m_Direction.x = dir.x;
						m_Direction.z = dir.y;
					}
					const int8_t height = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight(nPos.y/3, nPos.x/3, ((int)nPos.y) % 3, ((int)nPos.x) % 3);
					m_Renderable->SetPosition(nPos.x, height, nPos.y);
					m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Walking;
				}
				else
				{
					if (m_Order.orderType == 0)//Mine
					{
						m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Attacking;
						if (m_ImpSpecialTimer <= 0.0f)
						{
							const InstanceData& digInstance = LevelConfig::instanceData[INSTANCE_DIG];
							m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(digInstance.Time + digInstance.ActionTime + digInstance.ResetTime);
							if (Level::s_CurrentLevel->m_LevelData->MineTile(m_Order.targetTilePos.y, m_Order.targetTilePos.x))
							{
								StopOrder();
								CreatureTaskManager::RemoveMiningTask(m_Owner, m_Order.tile);
								GetTask();
							}
						}
					}
					else if(m_Order.orderType == 1) //Claim
					{
						m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Claiming;
						if (m_ImpSpecialTimer <= 0.0f)
						{
							const InstanceData& prettyPathInstance = LevelConfig::instanceData[INSTANCE_PRETTY_PATH];
							m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(prettyPathInstance.Time + prettyPathInstance.ActionTime + prettyPathInstance.ResetTime);
							if (Level::s_CurrentLevel->m_LevelData->ReinforceTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x))
							{
								Level::s_CurrentLevel->m_LevelData->ClaimTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x);
								StopOrder();
								CreatureTaskManager::RemoveClaimingTask(m_Owner, m_Order.tile);
								GetTask();
							}
						}
					}
					else //Reinforce
					{
						m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Claiming;
						if (m_ImpSpecialTimer <= 0.0f)
						{
							const InstanceData& reinforceInstance = LevelConfig::instanceData[INSTANCE_REINFORCE];
							m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(reinforceInstance.Time + reinforceInstance.ActionTime + reinforceInstance.ResetTime);
							if (Level::s_CurrentLevel->m_LevelData->ReinforceTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x))
							{
								Level::s_CurrentLevel->m_LevelData->ClaimTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x);
								StopOrder();
								CreatureTaskManager::RemoveReinforcingTask(m_Owner, m_Order.tile);
								GetTask();
							}
						}
					}
				}
			}
		}
		else
		{
			//Do Idle stuff
			m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Idling;
		}
	}

	m_AnimationTime += delta;
	if (m_AnimationTime > 1.0f / 10.0f)
	{
 		if (m_AnimationTime > 2 * (1.0f / 10.0f))
		{
			m_AnimationTime = 2 * (1.0f / 10.0f);
		}
		m_AnimationTime -= 1.0f / 10.0f;
		m_AnimationIndex++;
		m_AnimationIndex = m_AnimationIndex % (int)m_CreatureCBData._NumAnim;
		m_CreatureCBData._AnimIndex = m_AnimationIndex;
	}	
	if (m_CreatureSpriteIndex == CreatureData::CreatureSprite_Imp)
	{
		DoAnimationDirectionsImp();
	}
	else
	{
		DoAnimationDirections();
	}
	m_Renderable->isDirty = true;

	DebugDraw::Line(m_Renderable->m_Position, m_Renderable->m_Position + m_Direction * 3);
}
void Creature::GetTask()
{
	if (!m_Order.valid)
	{
		XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
		int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;
		m_Order = CreatureTaskManager::GetSoloMiningTask(this, areaCode);
		if (!m_Order.valid)
		{
			m_Order = CreatureTaskManager::GetClaimingTask(this, areaCode);
			if (!m_Order.valid)
			{
				m_Order = CreatureTaskManager::GetMiningTask(this, areaCode);
				if (!m_Order.valid)
				{
					m_Order = CreatureTaskManager::GetReinforcingTask(this, areaCode);
				}
			}
		}
	}
}
void Creature::StopOrder()
{
	m_Order.valid = false;
	m_Path.clear();
	m_CurrentPathIndex = 0;
	m_PathLerpTime = 0;
}
void Creature::DoAnimationDirectionsImp()
{
	XMFLOAT3 camDir;
	XMFLOAT3 camRight;
	Camera* cam = System::tSys->m_Game->m_Camera;
	Camera::CameraType camType = cam->GetProjection();
	if (camType == Camera::Orthographic)
	{
		camDir = cam->GetForward();
	}
	else
	{
		XMFLOAT3 temp = cam->GetPosition() - m_Renderable->m_Position;
		temp.y = 0;
		camDir = Normalize(temp);
	}
	camRight = cam->GetRight();
	camRight.y = 0;
	camRight = Normalize(camRight);

	//override for now (testing)
	camType = Camera::CameraType::Orthographic;

	XMFLOAT3 creatureDir = XMFLOAT3(m_Direction.x, 0, m_Direction.z);
	creatureDir = Normalize(creatureDir);
	XMFLOAT3 creatureRight = Cross(XMFLOAT3(0, 1, 0), creatureDir);
	int angle = round((2.0f * Dot(camDir, creatureDir)) + 2.0f);
	int direction = Dot(camDir, creatureRight) < 0.0f ? -1.0 : 1.0;
	if (direction < 0)
	{
		angle += 4;
	}

	if (angle > 4 && angle < 8)
	{
		m_CreatureCBData._isFlipped = true;
	}
	else
	{
		m_CreatureCBData._isFlipped = false;
	}

	if (angle == 0 || angle == 4 && direction == -1)
	{
		SetSprite(m_CreatureSpriteIndex + ImpAnimState[camType == Camera::Orthographic][4][m_ImpAnimState]);
	}
	else if (angle >= 1 && angle <= 4)
	{
		SetSprite(m_CreatureSpriteIndex + ImpAnimState[camType == Camera::Orthographic][4 - angle][m_ImpAnimState]);
	}
	else if (angle >= 5)
	{
		SetSprite(m_CreatureSpriteIndex + ImpAnimState[camType == Camera::Orthographic][8 - angle][m_ImpAnimState]);
	}
}
void Creature::DoAnimationDirections()
{
	XMFLOAT3 camDir;
	XMFLOAT3 camRight;
	Camera* cam = System::tSys->m_Game->m_Camera;
	Camera::CameraType camType = cam->GetProjection();
	if (camType == Camera::Orthographic)
	{
		camDir = cam->GetForward();
	}
	else
	{
		XMFLOAT3 temp = cam->GetPosition() - m_Renderable->m_Position;
		temp.y = 0;
		camDir = Normalize(temp);
	}
	camRight = cam->GetRight();
	camRight.y = 0;
	camRight = Normalize(camRight);
	//override for now
	camType = Camera::CameraType::Orthographic;

	XMFLOAT3 creatureDir = XMFLOAT3(m_Direction.x, 0, m_Direction.z);
	creatureDir = Normalize(creatureDir);
	XMFLOAT3 creatureRight = Cross(XMFLOAT3(0,1,0), creatureDir);
	int angle = round((2.0f * Dot(camDir, creatureDir)) + 2.0f);
	int direction = Dot(camDir, creatureRight) < 0.0f ? -1.0 : 1.0;
	if (direction < 0)
	{
		angle += 4;
	}
	if (angle > 4 && angle < 8)
	{
		m_CreatureCBData._isFlipped = true;
	}
	else
	{
		m_CreatureCBData._isFlipped = false;
	}

	if (angle == 0 || angle == 4 && direction == -1)
	{
		SetSprite(m_CreatureSpriteIndex + CreatureAnimState[camType == Camera::Orthographic][4][m_AnimState]);
	}
	else if (angle >= 1 && angle <= 4)
	{
		SetSprite(m_CreatureSpriteIndex + CreatureAnimState[camType == Camera::Orthographic][4 - angle][m_AnimState]);
	}
	else if (angle >= 5)
	{
		SetSprite(m_CreatureSpriteIndex + CreatureAnimState[camType == Camera::Orthographic][8 - angle][m_AnimState]);
	}
}
void Creature::Draw(D3D& d3d)
{
	if (!m_Renderable->isVisible)return;
	if (m_Renderable->isDirty)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		d3d.m_DevCon->Map(m_CreatureCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_CreatureCBData, sizeof(CreatureConstantBuffer));
		d3d.m_DevCon->Unmap(m_CreatureCB, NULL);
	}
	m_Renderable->Draw(d3d);
}
void Creature::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}