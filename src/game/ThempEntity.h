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
		};
		struct CreatureConstantBuffer
		{
			float _AnimIndex; //4
			float _NumAnim; //8
			float _SpriteWidth; //12
			uint32_t _isFrozen; //16
			uint32_t _isFlipped; //16
			uint32_t _dummy0; //16
			uint32_t _dummy1; //16
			uint32_t _dummy2; //16
		};//64

		~Entity();

		//spriteIndex = CreatureData::Creature_X
		Entity(EntityType spriteIndex);
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
