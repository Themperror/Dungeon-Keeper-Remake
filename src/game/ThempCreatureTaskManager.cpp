#include "ThempSystem.h"
#include "ThempCreature.h"
#include "ThempCreatureTaskManager.h"
#include "ThempGame.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempResources.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempFunctions.h"
#include <DirectXMath.h>

using namespace Themp;

std::unordered_map<Tile*,CreatureTaskManager::Task> CreatureTaskManager::MiningTasks[4];
std::unordered_map<Tile*, CreatureTaskManager::Task> CreatureTaskManager::ClaimingTasks[4];
std::unordered_map<Tile*, CreatureTaskManager::Task> CreatureTaskManager::ReinforcingTasks[4];
std::unordered_map<Creature*,CreatureTaskManager::Task> CreatureTaskManager::TaskedCreatures[4];

Themp::CreatureTaskManager::~CreatureTaskManager()
{
	
}

Themp::CreatureTaskManager::CreatureTaskManager()
{

}
void CreatureTaskManager::Update(float delta)
{

}



void Themp::CreatureTaskManager::RemoveMiningTask(uint8_t player, Tile*  tile)
{
	auto it = MiningTasks[player].find(tile);
	if (it != MiningTasks[player].end())
	{
		for (size_t j = 0; j < 12; j++)
		{
			if (it->second.takenPositions[j])
			{
				it->second.takenPositions[j]->StopOrder();
				
				auto taskIt = TaskedCreatures->find(it->second.takenPositions[j]);
				if(taskIt != TaskedCreatures->end())
					TaskedCreatures[player].erase(taskIt);
			}
		}
		MiningTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::RemoveClaimingTask(uint8_t player, Tile*  tile)
{
	auto it = ClaimingTasks[player].find(tile);
	if (it != ClaimingTasks[player].end())
	{
		if (it->second.takenPositions[0])
		{
			it->second.takenPositions[0]->StopOrder();

			auto taskIt = TaskedCreatures->find(it->second.takenPositions[0]);
			if (taskIt != TaskedCreatures->end())
				TaskedCreatures[player].erase(taskIt);
		}
		ClaimingTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::RemoveReinforcingTask(uint8_t player, Tile* tile)
{
	auto it = ReinforcingTasks[player].find(tile);
	if (it != ReinforcingTasks[player].end())
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (it->second.takenPositions[j])
			{
				it->second.takenPositions[j]->StopOrder();
				auto taskIt = TaskedCreatures->find(it->second.takenPositions[j]);
				if (taskIt != TaskedCreatures->end())
					TaskedCreatures[player].erase(taskIt);
			}
		}
		ReinforcingTasks[player].erase(tile);
	}
}
void Themp::CreatureTaskManager::AddMiningTask(uint8_t player, XMINT2 tilePos, Tile* tile)
{
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
			(neighbours.North == N_WALKABLE || neighbours.North == N_WATER) && neighbourTiles.North.areaCode == areaCode,
			(neighbours.East == N_WALKABLE || neighbours.East == N_WATER) && neighbourTiles.East.areaCode == areaCode,
			(neighbours.South == N_WALKABLE || neighbours.South == N_WATER) && neighbourTiles.South.areaCode == areaCode,
			(neighbours.West == N_WALKABLE || neighbours.West == N_WATER) && neighbourTiles.West.areaCode == areaCode,
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
		TaskedCreatures[player][requestee] = task;
		return Order(true, creatureSubtilePos, task.tilePosition, 0, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), 0, nullptr);
}
CreatureTaskManager::Order Themp::CreatureTaskManager::GetSoloMiningTask(Creature* requestee, int areaCode)
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
		if (task.assignedCreatures != 0)
		{
			continue;
		}
		Themp::TileNeighbours neighbours = System::tSys->m_Game->m_CurrentLevel->m_LevelData->CheckNeighbours(Type_Earth, task.tilePosition.y, task.tilePosition.x);
		Themp::TileNeighbourTiles neighbourTiles = System::tSys->m_Game->m_CurrentLevel->m_LevelData->GetNeighbourTiles(task.tilePosition.y, task.tilePosition.x);

		int Walkable[4] =
		{
			(neighbours.North == N_WALKABLE || neighbours.North == N_WATER) && neighbourTiles.North.areaCode == areaCode,
			(neighbours.East == N_WALKABLE || neighbours.East == N_WATER) && neighbourTiles.East.areaCode == areaCode,
			(neighbours.South == N_WALKABLE || neighbours.South == N_WATER) && neighbourTiles.South.areaCode == areaCode,
			(neighbours.West == N_WALKABLE || neighbours.West == N_WATER) && neighbourTiles.West.areaCode == areaCode,
		};
		
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
		TaskedCreatures[player][requestee] = task;
		return Order(true, creatureSubtilePos, task.tilePosition, 0, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), 0, nullptr);
}

CreatureTaskManager::Order Themp::CreatureTaskManager::GetClaimingTask(Creature* requestee, int areaCode)
{
	uint8_t player = requestee->m_Owner;
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
			TaskedCreatures[player][requestee] = task;
			return Order(true, XMINT2(creaturePos.x,creaturePos.z), task.tilePosition, 1, task.tile);
		}
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), 1, nullptr);
}

CreatureTaskManager::Order Themp::CreatureTaskManager::GetReinforcingTask(Creature* requestee, int areaCode)
{
	uint8_t player = requestee->m_Owner;
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
			neighbours.North == N_WALKABLE && neighbourTiles.North.owner == player && neighbourTiles.North.areaCode == areaCode,
			neighbours.East == N_WALKABLE && neighbourTiles.East.owner == player && neighbourTiles.East.areaCode == areaCode,
			neighbours.South == N_WALKABLE && neighbourTiles.South.owner == player && neighbourTiles.South.areaCode == areaCode,
			neighbours.West == N_WALKABLE && neighbourTiles.West.owner == player && neighbourTiles.West.areaCode == areaCode,
		};
		
		if (task.assignedCreatures >= (walkable[0] + walkable[1] + walkable[2] + walkable[3]))
		{
			continue;
		}
		XMINT2 taskSubTile = XMINT2(task.tilePosition.x * 3, task.tilePosition.y * 3);
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
		TaskedCreatures[player][requestee] = task;
		return Order(true, creaturePos, task.tilePosition, 2, task.tile);
	}
	return Order(false, XMINT2(-1, -1), XMINT2(-1, -1), 2,nullptr);
}

void Themp::CreatureTaskManager::UnlistCreatureFromTask(Creature * requestee)
{
	uint8_t player = requestee->m_Owner;
	auto it = TaskedCreatures[player].find(requestee);
	if (it != TaskedCreatures->end())
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
			TaskedCreatures[player].erase(it);
			return;
		}
		else if (claimIt != ClaimingTasks[player].end())
		{
			claimIt->second.takenPositions[0]->StopOrder();
			claimIt->second.takenPositions[0] = nullptr;
			claimIt->second.assignedCreatures--;
			TaskedCreatures[player].erase(it);
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
			TaskedCreatures[player].erase(it);
			return;
		}
	}
}
