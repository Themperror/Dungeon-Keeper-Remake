#pragma once
#include <vector>
#include <unordered_map>
#include "ThempCreatureData.h"
#include <DirectXMath.h>

using namespace DirectX;
namespace Themp
{
	class Creature;
	struct Tile;

	class CreatureTaskManager
	{
	public:
		CreatureTaskManager() = delete;
		~CreatureTaskManager() = delete;
		enum OrderType { Order_None, Order_Mine, Order_Claim, Order_Reinforce, Order_DeliverGold , Order_IdleMovement };
		enum ActivityType { Activity_None, Activity_GoToHeart, Activity_Sleep, Activity_GetFood, Activity_GoToBed,Activity_Eat, Activity_Train, Activity_Explore, Activity_Tunnel, Activity_CreateLair, Activity_IdleMovement,Activity_Fight};

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
			bool valid = false;
			uint8_t orderType = 0;
			XMINT2 subTilePos;
			XMINT2 targetTilePos;
			Tile* tile = nullptr;
		};
		struct Activity
		{
			Activity(bool validTask, XMINT2 subtile, XMINT2 targetTile, uint8_t typeActivity, Tile* tilePtr)
			{
				subTilePos = subtile;
				targetTilePos = targetTile;
				valid = validTask;
				activityType = typeActivity;
				tile = tilePtr;
			}
			bool valid = false;
			uint8_t activityType = 0;
			XMINT2 subTilePos;
			XMINT2 targetTilePos;
			Tile* tile = nullptr;
		};

		static void Update(float delta);
		static Activity GetDungeonEnteredActivity(Creature* requestee, int areaCode);
		static Activity GetFoodActivity(Creature* requestee, int areaCode);
		static Activity GetCreateLairActivity(Creature* requestee, int areaCode);
		static Activity GetSleepActivity(Creature* requestee, int areaCode);
		static Activity GetRandomMovementActivity(Creature * requestee, int areaCode);
		static Activity GetActivityByJob(Creature * requestee, int areaCode, CreatureData::Jobs job);
		static Activity GetExploreActivity(Creature * requestee, int areaCode);
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
		static Order GetRandomMovementOrder(Creature * requestee, int areaCode);
		static bool IsTreasuryAvailable(Creature * requestee, int areaCode);
		static Order GetAvailableTreasury(Creature * requestee, int areaCode);
		static void UnlistImpFromTask(Creature* requestee);

		static std::unordered_map<Tile*,Task> MiningTasks[4];
		static std::unordered_map<Tile*, Task> ClaimingTasks[4];
		static std::unordered_map<Tile*, Task> ReinforcingTasks[4];
		static std::unordered_map<Creature*, Task> TaskedImps[4];
	};
};
