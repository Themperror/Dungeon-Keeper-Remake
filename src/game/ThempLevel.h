#pragma once
#include <vector>
#include <stack>
#include <DirectXMath.h>
#include "../Library/micropather.h"
#include "Creature/ThempCreatureData.h"
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
	class LevelUI;

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
		int PathFindThroughWalls(DirectX::XMINT2 A, DirectX::XMINT2 B, micropather::MPVector<void*>& outPath, float & outCost, bool AllowDoors);
		void UpdateMinimap();
		void SpawnCreature(uint8_t player);


		bool m_IsCompleted = false;
		bool m_Ended = false;

		float m_CreatureGenerateTurnTimer = 0;
		float UnOwnedRoomColorTimer = 0;

		uint16_t m_SelectedBuilding = 0;
		bool m_BuildMode = false;

		Creature* m_HoveringCreature = nullptr;
		Creature* m_ClickedCreature = nullptr;
		DirectX::XMINT2 m_HoveringTile = DirectX::XMINT2(-1, -1);
		std::stack<Creature*> m_HeldCreatures;
		std::vector<std::string> m_Messages;
		std::string m_Objective = "";

		int m_CreatureGenerateTurns = 0;
		int UnownedRoomColorIndex = 0;

		VoxelObject* m_MapObject = nullptr;
		LevelData* m_LevelData = nullptr;
		LevelScript* m_LevelScript = nullptr;
		LevelUI* m_LevelUI = nullptr;
		micropather::MicroPather* m_Pather = nullptr;
		micropather::MicroPather* m_PatherThroughWalls = nullptr;
		Object2D* m_Cursor = nullptr;
		bool m_ShowUI = true;
		
		//Using the player owner ID's this would result in a max of 4 real players (Red,Blue,Yellow and Green) and 2 CPU's (Neutral and Good)
		//Red is 'always' a real player, the other 3 potentially a CPU player or disabled.
		PlayerBase* m_Players[6] = {nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

		std::vector<AvailableCreatureInPool> m_AvailableCreatures;
		static Level* s_CurrentLevel;
	};
};
