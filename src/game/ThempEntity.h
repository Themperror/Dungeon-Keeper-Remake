#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "ThempTileArrays.h"
namespace Themp
{
	class D3D;
	class Object3D;
	struct Sprite;
	class Entity
	{
	public:
		enum EntityType
		{
			Entity_Gold0_FP,
			Entity_Gold0_W,
			Entity_Gold1_FP,
			Entity_Gold1_W,
			Entity_Gold2_FP,
			Entity_Gold2_W,
			Entity_Gold3_FP,
			Entity_Gold3_W,
			Entity_Gold4_FP,
			Entity_Gold4_W,
			Entity_DungeonHeart_FP,
			Entity_DungeonHeart_W,
			Entity_LairFly_FP,
			Entity_LairFly_W,
			Entity_LairBeetle_FP,
			Entity_LairBeetle_W,
			Entity_Corpse_NULL,
			Entity_Corpse_WIZARD,
			Entity_Corpse_BARBARIAN,
			Entity_Corpse_ARCHER,
			Entity_Corpse_MONK,
			Entity_Corpse_DWARF,
			Entity_Corpse_KNIGHT,
			Entity_Corpse_AVATAR,
			Entity_Corpse_TUNNELLER,
			Entity_Corpse_WITCH,
			Entity_Corpse_GIANT,
			Entity_Corpse_FAIRY,
			Entity_Corpse_THIEF,
			Entity_Corpse_SAMURAI,
			Entity_Corpse_HORNY,
			Entity_Corpse_SKELETON,
			Entity_Corpse_TROLL,
			Entity_Corpse_DRAGON,
			Entity_Corpse_DEMONSPAWN,
			Entity_Corpse_FLY,
			Entity_Corpse_DARK_MISTRESS,
			Entity_Corpse_SORCEROR,
			Entity_Corpse_BILE_DEMON,
			Entity_Corpse_IMP,
			Entity_Corpse_BEETLE,
			Entity_Corpse_VAMPIRE,
			Entity_Corpse_SPIDER,
			Entity_Corpse_HELLHOUND,
			Entity_Corpse_GHOST,
			Entity_Corpse_TENTACLE,
			Entity_Corpse_ORC,
			Entity_Corpse_FLOATING_SPIRIT,
		};
		struct CreatureConstantBuffer
		{
			float _AnimIndex;
			float _NumAnim;
			float _SpriteWidth;
			float _cdummy0;
			uint32_t _isFrozen;
			uint32_t _isFlipped;
			uint32_t _isHovered;
			uint32_t _isFighting;
		};//64

		~Entity();

		//spriteIndex = CreatureData::Creature_X
		Entity(EntityType spriteIndex);
		void SetSpriteFromType(Entity::EntityType entityType);
		void SetSprite(int SpriteID);
		void ResetScale();
		void Update(float delta);
		void Draw(D3D& d3d);
		void SetVisibility(bool val);
		
		Object3D* m_Renderable = nullptr;
		Sprite* m_Sprite = nullptr;

		EntityType m_EntityID;
		int m_EntitySpriteIndex;
		float m_AnimationTime = 0;
		int m_AnimationIndex = 0;

		CreatureConstantBuffer m_EntityCBData;
		ID3D11Buffer* m_EntityCB = nullptr;
	};
};
