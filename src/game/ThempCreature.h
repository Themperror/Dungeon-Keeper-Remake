#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "ThempCreatureData.h"
#include "ThempCreatureTaskManager.h"
#include "ThempTileArrays.h"
#include <micropather.h>
namespace Themp
{
	class D3D;
	class Object3D;
	struct Sprite;
	class Creature
	{
	public:

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

		~Creature();

		//spriteIndex = CreatureData::Creature_X
		Creature(CreatureData::CreatureType spriteIndex);
		void SetSprite(int SpriteID);
		void Update(float delta);
		void GetTask();
		void StopOrder();
		void DoAnimationDirectionsImp();
		void DoAnimationDirections();
		void Draw(D3D& d3d);
		void SetVisibility(bool val);

		Object3D* m_Renderable = nullptr;
		Sprite* m_Sprite = nullptr;

		CreatureData::CreatureType m_CreatureID = CreatureData::CREATURE_IMP;
		CreatureData::CreatureSpriteIndex m_CreatureSpriteIndex = CreatureData::CreatureSprite_Imp;
		CreatureData::AnimationState m_AnimState = CreatureData::AnimationState::Walking;
		CreatureData::ImpAnimationState m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Walking;
		DirectX::XMFLOAT3 m_Direction;
		float m_Speed = 0;
		uint8_t m_Owner = Owner_PlayerRed;
		float m_AnimationTime = 0;
		int m_AnimationIndex = 0;
		int m_CurrentPathIndex = 0;
		float m_PathLerpTime = 0;
		bool m_JustSlapped = false;

		float m_PowerCooldownTimer[10];
		float m_ImpSpecialTimer = 0.0f; //Dig, Claim etc..

		CreatureTaskManager::Order m_Order = CreatureTaskManager::Order(false, XMINT2(-1, -1), XMINT2(-1, -1), -1, nullptr);
		micropather::MPVector<void*> m_Path;
		CreatureConstantBuffer m_CreatureCBData;
		CreatureData m_CreatureData;
		ID3D11Buffer* m_CreatureCB = nullptr;
	};
};
