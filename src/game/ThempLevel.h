#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../Library/micropather.h"
#include "ThempCreatureData.h"
#include "ThempFileManager.h"
namespace Themp
{
	class D3D;
	class VoxelObject;
	class Object3D;
	class Object2D;
	class Creature;
	struct Sprite;
	class LevelData;
	class LevelScript;
	class PlayerBase;

	struct AvailableCreatureInPool
	{
		CreatureData::CreatureType type;
		int amount;
	};

	class Level
	{
	public:
		
		~Level();
		Level(int levelIndex);
		void AvailableRoomsChanged();
		void Update(float delta);
		int PathFind(DirectX::XMINT2 A, DirectX::XMINT2 B, micropather::MPVector<void*>& outPath, float & outCost, bool AllowDoors);
		void UpdateMinimap();
		void SpawnCreature(uint8_t player);
		Object2D* m_Cursor = nullptr;
		Object2D* m_MinimapObject = nullptr;
		int UnownedRoomColorIndex = 0;
		float UnOwnedRoomColorTimer = 0;
		GUITexture m_MinimapData;
		VoxelObject* m_MapObject = nullptr;
		LevelData* m_LevelData = nullptr;
		LevelScript* m_LevelScript = nullptr;
		micropather::MicroPather* m_Pather = nullptr;

		float m_CreatureGenerateTurnTimer = 0;
		int m_CreatureGenerateTurns = 0;
		
		//Using the player owner ID's this would result in a max of 4 real players (Red,Blue,Yellow and Green) and 2 CPU's (Neutral and Good)
		//Red is 'always' a real player, the other 3 potentially a CPU player or disabled.
		PlayerBase* m_Players[6] = {nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

		std::vector<AvailableCreatureInPool> m_AvailableCreatures;
		static Level* s_CurrentLevel;
	};
};
