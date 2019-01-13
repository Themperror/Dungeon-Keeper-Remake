#include "ThempSystem.h"
#include "ThempCreature.h"
#include "ThempCreatureTaskManager.h"
#include "ThempGame.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempLevelConfig.h"
#include "ThempResources.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempFunctions.h"
#include <DirectXMath.h>

using namespace Themp;

std::unordered_map<Tile*,CreatureTaskManager::Task> CreatureTaskManager::MiningTasks[4];
std::unordered_map<Tile*, CreatureTaskManager::Task> CreatureTaskManager::ClaimingTasks[4];
std::unordered_map<Tile*, CreatureTaskManager::Task> CreatureTaskManager::ReinforcingTasks[4];
std::unordered_map<Creature*,CreatureTaskManager::Task> CreatureTaskManager::TaskedImps[4];


void CreatureTaskManager::Update(float delta)
{

}



void Themp::CreatureTaskManager::RemoveMiningTask(uint8_t player, Tile*  tile)
{
	auto it = MiningTasks[player].find(tile);
	int numCreatures = 0;
	if (it != MiningTasks[player].end())
	{
		for (size_t j = 0; j < 12; j++)
		{
			if (it->second.takenPositions[j])
			{
				it->second.takenPositions[j]->StopOrder();
				numCreatures++;
				auto taskIt = TaskedImps[player].find(it->second.takenPositions[j]);
				if(taskIt != TaskedImps[player].end())
					TaskedImps[player].erase(taskIt);
			}
		}
		MiningTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::RemoveClaimingTask(uint8_t player, Tile*  tile)
{
	auto it = ClaimingTasks[player].find(tile);
	int numCreatures = 0;
	if (it != ClaimingTasks[player].end())
	{
		if (it->second.takenPositions[0])
		{
			it->second.takenPositions[0]->StopOrder();

			auto taskIt = TaskedImps[player].find(it->second.takenPositions[0]);
			if (taskIt != TaskedImps[player].end())
				TaskedImps[player].erase(taskIt);
		}
		ClaimingTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::RemoveReinforcingTask(uint8_t player, Tile* tile)
{
	auto it = ReinforcingTasks[player].find(tile);
	int numCreatures = 0;
	if (it != ReinforcingTasks[player].end())
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (it->second.takenPositions[j])
			{
				it->second.takenPositions[j]->StopOrder();
				auto taskIt = TaskedImps[player].find(it->second.takenPositions[j]);
				if (taskIt != TaskedImps[player].end())
					TaskedImps[player].erase(taskIt);
			}
		}
		ReinforcingTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::AddMiningTask(uint8_t player, XMINT2 tilePos, Tile* tile)
{
	uint16_t type = tile->GetType();
	if (player != tile->owner && type >= Type_Wall0  && type <= Type_Wall5) return;

	if (type < Type_Gold && type > Type_Wall5 && type != Type_Gem) return;

	auto foundIt = MiningTasks[player].find(tile);
	if (foundIt == MiningTasks[player].end())
	{
		MiningTasks[player][tile] = Task(tilePos, tile);
	}
}
void Themp::CreatureTaskManager::AddClaimingTask(uint8_t player, XMINT2 tilePos, Tile* tile)
{
	auto foundIt = ClaimingTasks[player].find(tile);
	if (foundIt == ClaimingTasks[player].end())
	{
		ClaimingTasks[player][tile] = Task(tilePos, tile);
	}
}

void Themp::CreatureTaskManager::AddReinforcingTask(uint8_t player, XMINT2 tilePos, Tile* tile)
{
	auto foundIt = ReinforcingTasks[player].find(tile);
	if (foundIt == ReinforcingTasks[player].end())
	{
		ReinforcingTasks[player][tile] = Task(tilePos, tile);
	}
}

CreatureTaskManager::Order Themp::CreatureTaskManager::GetMiningTask(Creature* requestee, int areaCode)
{
	uint8_t player = requestee->m_Owner;
	const XMINT2 subTileOffsets[4] =
	{
		XMINT2(0,3),
		XMINT2(3,0),
		XMINT2(0,-1),
		XMINT2(-1,0),
	};
	const XMINT2 masks[4] =
	{
		XMINT2(1,0),
		XMINT2(0,1),
		XMINT2(1,0),
		XMINT2(0,1),
	};

	for (auto i = MiningTasks[player].begin(); i != MiningTasks[player].end(); i++)
	{
		Task& task = i->second;

		Themp::TileNeighbours neighbours = System::tSys->m_Game->m_CurrentLevel->m_LevelData->CheckNeighbours(Type_Earth, task.tilePosition.y, task.tilePosition.x);
		Themp::TileNeighbourTiles neighbourTiles = System::tSys->m_Game->m_CurrentLevel->m_LevelData->GetNeighbourTiles(task.tilePosition.y, task.tilePosition.x);

		int Walkable[4] =
		{
			(neighbours.North == N_WALKABLE || neighbours.North == N_WATER) && neighbourTiles.North->areaCode == areaCode,
			(neighbours.East == N_WALKABLE || neighbours.East == N_WATER) && neighbourTiles.East->areaCode == areaCode,
			(neighbours.South == N_WALKABLE || neighbours.South == N_WATER) && neighbourTiles.South->areaCode == areaCode,
			(neighbours.West == N_WALKABLE || neighbours.West == N_WATER) && neighbourTiles.West->areaCode == areaCode,
		};
		if (task.assignedCreatures >= (Walkable[0] * 3 + Walkable[1] * 3 + Walkable[2] * 3 + Walkable[3] * 3))
		{
			continue;
		}
		XMINT2 taskSubTile = XMINT2(task.tilePosition.x * 3, task.tilePosition.y * 3);
		XMINT2 creatureSubtilePos;
		int cameFrom = -1;
		int creatureSpotIndex = -1;
		for (int k = 0; k < 4; k++)
		{
			if (Walkable[k])
			{
				for (int j = 0; j < 3; j++)
				{
					if (!task.takenPositions[k * 3 + j])
					{
						creatureSubtilePos = XMINT2(taskSubTile.x + (subTileOffsets[k].x) + j * masks[k].x, taskSubTile.y + (subTileOffsets[k].y) + j * masks[k].y);
						task.takenPositions[k * 3 + j] = requestee;
						creatureSpotIndex = k * 3 + j;
						cameFrom = k;
						goto SPOTFOUND;
					}
				}
			}
		}
		if (cameFrom == -1)
		{
			continue;
		}
	SPOTFOUND:
		XMINT2 creatureTilePos = XMINT2(creatureSubtilePos.x / 3, creatureSubtilePos.y / 3);
		if (LevelData::m_Map.m_Tiles[creatureTilePos.y][creatureTilePos.x].areaCode != areaCode)
		{
			task.takenPositions[creatureSpotIndex] = nullptr;
			continue;
		}

		i->second.assignedCreatures++;
		TaskedImps[player][requestee] = task;
		return Order(true, creatureSubtilePos, task.tilePosition, Order_Mine, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None, nullptr);
}
CreatureTaskManager::Order Themp::CreatureTaskManager::GetSoloMiningTask(Creature* requestee, int areaCode)
{
	const uint8_t player = requestee->m_Owner;
	const XMINT2 subTileOffsets[4] =
	{
		XMINT2(0,3),
		XMINT2(3,0),
		XMINT2(0,-1),
		XMINT2(-1,0),
	};
	const XMINT2 masks[4] =
	{
		XMINT2(1,0),
		XMINT2(0,1),
		XMINT2(1,0),
		XMINT2(0,1),
	};

	for (auto i = MiningTasks[player].begin(); i != MiningTasks[player].end(); i++)
	{
		Task& task = i->second;
		if (task.assignedCreatures != 0)
		{
			continue;
		}
		Themp::TileNeighbours neighbours = System::tSys->m_Game->m_CurrentLevel->m_LevelData->CheckNeighbours(Type_Earth, task.tilePosition.y, task.tilePosition.x);
		Themp::TileNeighbourTiles neighbourTiles = System::tSys->m_Game->m_CurrentLevel->m_LevelData->GetNeighbourTiles(task.tilePosition.y, task.tilePosition.x);

		int Walkable[4] =
		{
			(neighbours.North == N_WALKABLE || neighbours.North == N_WATER) && neighbourTiles.North->areaCode == areaCode,
			(neighbours.East == N_WALKABLE || neighbours.East == N_WATER) && neighbourTiles.East->areaCode == areaCode,
			(neighbours.South == N_WALKABLE || neighbours.South == N_WATER) && neighbourTiles.South->areaCode == areaCode,
			(neighbours.West == N_WALKABLE || neighbours.West == N_WATER) && neighbourTiles.West->areaCode == areaCode,
		};
		
		const XMINT2 taskSubTile = XMINT2(task.tilePosition.x * 3, task.tilePosition.y * 3);
		XMINT2 creatureSubtilePos;
		int cameFrom = -1;
		int creatureSpotIndex = -1;
		for (int k = 0; k < 4; k++)
		{
			if (Walkable[k])
			{
				for (int j = 0; j < 3; j++)
				{
					if (!task.takenPositions[k * 3 + j])
					{
						creatureSubtilePos = XMINT2(taskSubTile.x + (subTileOffsets[k].x) + j * masks[k].x, taskSubTile.y + (subTileOffsets[k].y) + j * masks[k].y);
						task.takenPositions[k * 3 + j] = requestee;
						creatureSpotIndex = k * 3 + j;
						cameFrom = k;
						goto SPOTFOUND;
					}
				}
			}
		}
		if (cameFrom == -1)
		{
			continue;
		}
	SPOTFOUND:
		const XMINT2 creatureTilePos = XMINT2(creatureSubtilePos.x / 3, creatureSubtilePos.y / 3);
		if (LevelData::m_Map.m_Tiles[creatureTilePos.y][creatureTilePos.x].areaCode != areaCode)
		{
			task.takenPositions[creatureSpotIndex] = nullptr;
			continue;
		}

		i->second.assignedCreatures++;
		TaskedImps[player][requestee] = task;
		return Order(true, creatureSubtilePos, task.tilePosition, Order_Mine, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None, nullptr);
}

CreatureTaskManager::Order Themp::CreatureTaskManager::GetClaimingTask(Creature* requestee, int areaCode)
{
	const uint8_t player = requestee->m_Owner;
	for (auto i = ClaimingTasks[player].begin(); i != ClaimingTasks[player].end(); i++)
	{
		Task& task = i->second;
		if (task.assignedCreatures == 0)
		{
			XMFLOAT3 creaturePos = LevelData::TileToWorld(XMINT2(task.tilePosition.x, task.tilePosition.y));
			if (LevelData::m_Map.m_Tiles[task.tilePosition.y][task.tilePosition.x].areaCode != areaCode)
			{
				continue;
			}
			task.takenPositions[0] = requestee;
			i->second.assignedCreatures++;
			TaskedImps[player][requestee] = task;
			return Order(true, XMINT2((int)creaturePos.x,(int)creaturePos.z), task.tilePosition, Order_Claim, task.tile);
		}
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None, nullptr);
}

CreatureTaskManager::Order Themp::CreatureTaskManager::GetReinforcingTask(Creature* requestee, int areaCode)
{
	const uint8_t player = requestee->m_Owner;
	const XMINT2 subTileOffsets[4] =
	{
		XMINT2(0,3),
		XMINT2(3,0),
		XMINT2(0,-1),
		XMINT2(-1,0),
	};

	for (auto i = ReinforcingTasks[player].begin(); i != ReinforcingTasks[player].end(); i++)
	{
		Task& task = i->second;
		Themp::TileNeighbours neighbours = System::tSys->m_Game->m_CurrentLevel->m_LevelData->CheckNeighbours(Type_Earth, task.tilePosition.y, task.tilePosition.x);
		Themp::TileNeighbourTiles neighbourTiles = System::tSys->m_Game->m_CurrentLevel->m_LevelData->GetNeighbourTiles(task.tilePosition.y, task.tilePosition.x);

		int walkable[4] =
		{
			neighbours.North == N_WALKABLE && neighbourTiles.North->owner == player && neighbourTiles.North->areaCode == areaCode,
			neighbours.East == N_WALKABLE && neighbourTiles.East->owner == player && neighbourTiles.East->areaCode == areaCode,
			neighbours.South == N_WALKABLE && neighbourTiles.South->owner == player && neighbourTiles.South->areaCode == areaCode,
			neighbours.West == N_WALKABLE && neighbourTiles.West->owner == player && neighbourTiles.West->areaCode == areaCode,
		};
		
		if (task.assignedCreatures >= (walkable[0] + walkable[1] + walkable[2] + walkable[3]))
		{
			continue;
		}
		const XMINT2 taskSubTile = XMINT2(task.tilePosition.x * 3, task.tilePosition.y * 3);
		XMINT2 creaturePos;
		int cameFrom = -1;
		for (int j = 0; j < 4; j++)
		{
			if (walkable[j] && !task.takenPositions[j])
			{
				creaturePos = XMINT2(taskSubTile.x + subTileOffsets[j].x, taskSubTile.y + subTileOffsets[j].y);
				cameFrom = j;
				break;
			}
		}
		if (cameFrom == -1 || LevelData::m_Map.m_Tiles[creaturePos.y / 3][creaturePos.x / 3].areaCode != areaCode)
		{
			continue;
		}
		task.takenPositions[cameFrom] = requestee;
		i->second.assignedCreatures++;
		TaskedImps[player][requestee] = task;
		return Order(true, creaturePos, task.tilePosition, Order_Reinforce, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None,nullptr);
}
CreatureTaskManager::Order Themp::CreatureTaskManager::GetRandomMovementOrder(Creature* requestee, int areaCode)
{
	struct TileAndPos { Tile* t; XMINT2 p; };
	//since there's no method for looking through a list of nearby walkable tiles we'll do it naively, this luckily isn't too much and isn't called that often.
	std::vector<TileAndPos> walkableTiles;
	walkableTiles.reserve(4 * 4);

	const int range = 4;
	const XMINT2 tilePos = LevelData::WorldToTile(requestee->m_Renderable->m_Position);
	const int minY = tilePos.y - range >= range ? tilePos.y - range : range;
	const int minX = tilePos.x - range >= range ? tilePos.x - range : range;
	const int maxY = tilePos.y + range < MAP_SIZE_TILES ? tilePos.y + range : MAP_SIZE_TILES - range - 1;
	const int maxX = tilePos.x + range < MAP_SIZE_TILES ? tilePos.x + range : MAP_SIZE_TILES - range - 1;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			Tile* t = &LevelData::m_Map.m_Tiles[y][x];
			if (IsWalkable(t->GetType()) && t->areaCode == areaCode)
			{
				walkableTiles.push_back({ t,XMINT2(x,y) });
			}
		}
	}
	int randomTile = rand() % walkableTiles.size();
	TileAndPos& t = walkableTiles[randomTile];
	//really subpar subtile selection but fuck it, its 9 subtiles, no tiles with more than 2 unwalkable subtiles, so the chances this will cause lagg is minimal.
	while (true)
	{
		int randX = rand() % 3;
		int randY = rand() % 3;
		if (t.t->pathSubTiles[randY][randX].walkable)
		{
			return Order(true, LevelData::TileToSubtile(t.p) + XMINT2(randX-1, randY-1), t.p, Order_IdleMovement, t.t);
		}
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None, nullptr);
}
bool Themp::CreatureTaskManager::IsTreasuryAvailable(Creature* requestee, int areaCode)
{
	const uint8_t owner = requestee->m_Owner;
	std::unordered_map<int, LevelData::Room>& rooms = Level::s_CurrentLevel->m_LevelData->m_Rooms[owner];
	auto& it = rooms.begin();
	while (it != rooms.end())
	{
		if (it->second.areaCode == areaCode && it->second.roomType == Type_Treasure_Room)
		{
			if (it->second.roomFillPercentage != 100)
			{
				return true;
			}
		}
		it++;
	}
	return false;
}
CreatureTaskManager::Order Themp::CreatureTaskManager::GetAvailableTreasury(Creature* requestee, int areaCode)
{
	const uint8_t owner = requestee->m_Owner;
	std::unordered_map<int, LevelData::Room>& rooms = Level::s_CurrentLevel->m_LevelData->m_Rooms[owner];
	auto& it = rooms.begin();
	while (it != rooms.end())
	{
		if (it->second.areaCode == areaCode && it->second.roomType == Type_Treasure_Room)
		{
			if (it->second.roomFillPercentage != 100)
			{
				const int roomMaxGoldPerTile = LevelConfig::gameSettings[GameSettings::GAME_GOLD_PILE_MAXIMUM].Value * it->second.roomEfficiency / 100;
				//find a suitable tile
				auto& tileIt = it->second.tiles.begin();
				while (tileIt != it->second.tiles.end())
				{
					if (tileIt->second.tileValue < roomMaxGoldPerTile)
					{
						return Order(true, XMINT2(tileIt->second.x*3 + 1, tileIt->second.y* 3 + 1), XMINT2(tileIt->second.x, tileIt->second.y), Order_DeliverGold, tileIt->first);
					}
					tileIt++;
				}
			}
		}
		it++;
	}

	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), Order_None, nullptr);
}
void Themp::CreatureTaskManager::UnlistImpFromTask(Creature * requestee)
{
	const uint8_t player = requestee->m_Owner;
	auto it = TaskedImps[player].find(requestee);
	if (it != TaskedImps[player].end())
	{
		auto mineIt = MiningTasks[player].find(it->second.tile);
		auto claimIt = ClaimingTasks[player].find(it->second.tile);
		auto reinforceIt = ReinforcingTasks[player].find(it->second.tile);
		if (mineIt != MiningTasks[player].end())
		{
			for (int j = 0; j < 12; j++)
			{
				if (mineIt->second.takenPositions[j] == requestee)
				{
					mineIt->second.takenPositions[j]->StopOrder();
					mineIt->second.takenPositions[j] = nullptr;
					mineIt->second.assignedCreatures--;
					
					break;
				}
			}
			TaskedImps[player].erase(it);
			return;
		}
		else if (claimIt != ClaimingTasks[player].end())
		{
			claimIt->second.takenPositions[0]->StopOrder();
			claimIt->second.takenPositions[0] = nullptr;
			claimIt->second.assignedCreatures--;
			TaskedImps[player].erase(it);
			return;
		}
		else if (reinforceIt != ReinforcingTasks[player].end())
		{
			for (int j = 0; j < 4; j++)
			{
				if (reinforceIt->second.takenPositions[j] == requestee)
				{
					reinforceIt->second.takenPositions[j]->StopOrder();
					reinforceIt->second.takenPositions[j] = nullptr;
					reinforceIt->second.assignedCreatures--;
					break;
				}
			}
			TaskedImps[player].erase(it);
			return;
		}
	}
	requestee->StopOrder();
}


//Locate the players Dungeon Heart, and walk to it (and heal it a bit)
CreatureTaskManager::Activity CreatureTaskManager::GetDungeonEnteredActivity(Creature* requestee, int areaCode)
{
	const uint8_t owner = requestee->m_Owner;
	std::unordered_map<int, LevelData::Room>& rooms = Level::s_CurrentLevel->m_LevelData->m_Rooms[owner];
	auto& it = rooms.begin();
	while (it != rooms.end())
	{
		if (it->second.areaCode == areaCode && it->second.roomType == Type_Dungeon_Heart)
		{
			for (auto& tile : it->second.tiles)
			{
				//find the middle tile (naive method atm), though its only 9 tiles anyhow
				if (tile.first->type == (tile.first->GetType() + (5 << 8)))
				{
					return Activity(true, XMINT2(tile.second.x*3 +1, tile.second.y*3 +1), XMINT2(tile.second.x, tile.second.y), Activity_GoToHeart, tile.first);
				}
			}
		}
		it++;
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}
CreatureTaskManager::Activity CreatureTaskManager::GetFoodActivity(Creature* requestee, int areaCode)
{
	const uint8_t owner = requestee->m_Owner;
	std::unordered_map<int, LevelData::Room>& rooms = Level::s_CurrentLevel->m_LevelData->m_Rooms[owner];
	auto& it = rooms.begin();
	while (it != rooms.end())
	{
		if (it->second.areaCode == areaCode && it->second.roomType == Type_Hatchery)
		{
			//should target a random chicken inside the hatchery to feed upon, might do that when we are actually inside the hatchery though..

			//Wow this is a shitty way to get a reference to a random tile..
			auto& tileIt = it->second.tiles.begin();
			int off = (rand() % it->second.tiles.size());
			for (int i = 0; i < off; i++)
			{
				tileIt++;
			}
			LevelData::Room::RoomTile& tile = tileIt->second;
			return Activity(true, XMINT2(tile.x * 3 + 1, tile.y * 3 + 1), XMINT2(tile.x, tile.y), Activity_GetFood, tile.tile);
		}
		it++;
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}
CreatureTaskManager::Activity CreatureTaskManager::GetCreateLairActivity(Creature* requestee, int areaCode)
{
	const uint8_t owner = requestee->m_Owner;
	if (requestee->m_LairLocation.x == -1 && requestee->m_LairLocation.y == -1)
	{
		std::unordered_map<int, LevelData::Room>& rooms = Level::s_CurrentLevel->m_LevelData->m_Rooms[owner];
		auto& it = rooms.begin();
		while (it != rooms.end())
		{
			if (it->second.areaCode == areaCode && it->second.roomType == Type_Lair)
			{
				for (auto& tile = it->second.tiles.begin(); tile != it->second.tiles.end(); tile++)
				{
					if (tile->second.tileValue == 0)
					{
						tile->second.tileValue = 1;
						return Activity(true, XMINT2(tile->second.x * 3 + 1, tile->second.y * 3 + 1), XMINT2(tile->second.x, tile->second.y), Activity_CreateLair, tile->first);
					}
				}
			}
			it++;
		}
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}
CreatureTaskManager::Activity CreatureTaskManager::GetSleepActivity(Creature* requestee, int areaCode)
{
	if (requestee->m_LairLocation.x != -1 && requestee->m_LairLocation.y != -1)
	{
		XMFLOAT3 subTilePos = LevelData::TileToWorld(requestee->m_LairLocation);
		return Activity(true, XMINT2(subTilePos.x, subTilePos.z), requestee->m_LairLocation, Activity_GoToBed, &Level::s_CurrentLevel->m_LevelData->m_Map.m_Tiles[requestee->m_LairLocation.y][requestee->m_LairLocation.x]);
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}
CreatureTaskManager::Activity CreatureTaskManager::GetRandomMovementActivity(Creature* requestee, int areaCode)
{
	struct TileAndPos { Tile* t; XMINT2 p; };
	//since there's no method for looking through a list of nearby walkable tiles we'll do it naively, this luckily isn't too much and isn't called that often.
	std::vector<TileAndPos> walkableTiles;
	walkableTiles.reserve(4 * 4);


	const int range = 4;
	const XMINT2 tilePos = LevelData::WorldToTile(requestee->m_Renderable->m_Position);
	const int minY = tilePos.y - range >= range ? tilePos.y - range : range;
	const int minX = tilePos.x - range >= range ? tilePos.x - range : range;
	const int maxY = tilePos.y + range < MAP_SIZE_TILES ? tilePos.y + range : MAP_SIZE_TILES - range - 1;
	const int maxX = tilePos.x + range < MAP_SIZE_TILES ? tilePos.x + range : MAP_SIZE_TILES - range - 1;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			Tile* t = &LevelData::m_Map.m_Tiles[y][x];
			if (IsWalkable(t->GetType()) && t->areaCode == areaCode)
			{
				walkableTiles.push_back({ t,XMINT2(x,y) });
			}
		}
	}
	if (walkableTiles.size() > 0)
	{
		int randomTile = rand() % walkableTiles.size();
		TileAndPos& t = walkableTiles[randomTile];
		//really subpar subtile selection but fuck it, its 9 subtiles, no tiles with more than 2 unwalkable subtiles, so the chances this will cause lagg is minimal.
		while (true)
		{
			int randX = rand() % 3;
			int randY = rand() % 3;
			if (t.t->pathSubTiles[randY][randX].walkable)
			{
				return Activity(true, LevelData::TileToSubtile(t.p)+XMINT2(randX-1,randY-1), t.p, Activity_IdleMovement, t.t);
			}
		}
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}
CreatureTaskManager::Activity CreatureTaskManager::GetActivityByJob(Creature* requestee, int areaCode, CreatureData::Jobs job)
{
	switch (job)
	{
		case CreatureData::Jobs::Null: break;
		case CreatureData::Jobs::Tunnel: break;
		case CreatureData::Jobs::Dig: break;
		case CreatureData::Jobs::Research: break;
		case CreatureData::Jobs::Train: break;
		case CreatureData::Jobs::Manufacture: break;
		case CreatureData::Jobs::Scavenge: break;
		case CreatureData::Jobs::KinkyTorture: break;
		case CreatureData::Jobs::Fight: break;
		case CreatureData::Jobs::SeekTheEnemy: break;
		case CreatureData::Jobs::Guard: break;
		case CreatureData::Jobs::Group: break;
		case CreatureData::Jobs::Barrack: break;
		case CreatureData::Jobs::Temple: break;
		case CreatureData::Jobs::FreezePrisoners: break;
		case CreatureData::Jobs::Explore: 
			return GetExploreActivity(requestee, areaCode);
			break;
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}

CreatureTaskManager::Activity CreatureTaskManager::GetExploreActivity(Creature* requestee, int areaCode)
{
	LevelData* l = Level::s_CurrentLevel->m_LevelData;
	if (l->m_UnexploredTiles.size() > 0)
	{
		for (auto& it = l->m_UnexploredTiles.begin(); it != l->m_UnexploredTiles.end(); it++)
		{
			if (l->HasWalkableNeighbour(it->second.y, it->second.x,areaCode))
			{
				XMINT2 tile = l->GetWalkableNeighbour(it->second.y, it->second.x, areaCode);
				return Activity(true, LevelData::TileToSubtile(tile), tile, Activity_Explore, it->first);
			}
		}
	}
	return Activity(false, XMINT2(-1, -1), XMINT2(-1, -1), Activity_None, nullptr);
}