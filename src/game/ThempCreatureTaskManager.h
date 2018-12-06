#pragma once
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>

using namespace DirectX;
namespace Themp
{
	class Creature;
	struct Tile;

	class CreatureTaskManager
	{
	public:
		CreatureTaskManager();
		~CreatureTaskManager();
		enum OrderType { Order_None, Order_Mine, Order_Claim, Order_Reinforce, Order_DeliverGold };
		struct Task
		{
			Task()
			{
				tile = nullptr;
				assignedCreatures = -1;
				tilePosition = XMINT2(-1, -1);
				for (int i = 0; i < 12; i++)
				{
					takenPositions[i] = nullptr;
				}
			}
			Task(XMINT2 tilePos,Tile* tilePtr)
			{ 
				assignedCreatures = 0;
				tilePosition = tilePos;
				tile = tilePtr;
				for (int i = 0; i < 12; i++)
				{
					takenPositions[i] = nullptr;
				}
			}
			int assignedCreatures;
			XMINT2 tilePosition;
			Tile* tile;
			Creature* takenPositions[12];
		};
		struct Order
		{
			Order(bool validTask , XMINT2 subtile, XMINT2 targetTile, uint8_t typeOrder, Tile* tilePtr)
			{
				subTilePos = subtile;
				targetTilePos = targetTile;
				valid = validTask;
				orderType = typeOrder;
				tile = tilePtr;
			}
			bool valid;
			uint8_t orderType = 0;
			XMINT2 subTilePos;
			XMINT2 targetTilePos;
			Tile* tile;
		};

		static void Update(float delta);
		static void RemoveMiningTask(uint8_t player, Tile* tile);
		static void RemoveClaimingTask(uint8_t player, Tile* tile);
		static void RemoveReinforcingTask(uint8_t player, Tile* tile);
		static void AddMiningTask(uint8_t player, XMINT2 tilePos, Tile* tile);
		static void AddClaimingTask(uint8_t player, XMINT2 tilePos, Tile* tile);
		static void AddReinforcingTask(uint8_t player, XMINT2 tilePos, Tile* tile);
		static Order GetMiningTask(Creature * requestee, int areaCode);
		static Order GetSoloMiningTask(Creature * requestee, int areaCode);
		static Order GetClaimingTask(Creature* requestee, int areaCode);
		static Order GetReinforcingTask(Creature* requestee, int areaCode);
		static bool IsTreasuryAvailable(Creature * requestee, int areaCode);
		static Order GetAvailableTreasury(Creature * requestee, int areaCode);
		static void UnlistCreatureFromTask(Creature* requestee);

		static std::unordered_map<Tile*,Task> MiningTasks[4];
		static std::unordered_map<Tile*, Task> ClaimingTasks[4];
		static std::unordered_map<Tile*, Task> ReinforcingTasks[4];
		static std::unordered_map<Creature*, Task> TaskedCreatures[4];
	};
};
