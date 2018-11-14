#include "ThempSystem.h"
#include "ThempTileArrays.h"
#include <DirectXMath.h>
#include "../Library/micropather.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
using namespace DirectX;

/**
Return the least possible cost between 2 states. For example, if your pathfinding
is based on distance, this is simply the straight distance between 2 points on the
map. If you pathfinding is based on minimum time, it is the minimal travel time
between 2 points given the best possible terrain.
*/
float Themp::TileMap::LeastCostEstimate(void* stateStart, void* stateEnd)
{
	int startX = (((uint64_t)stateStart) & 0xFFFFFFF);
	int startY = ((((uint64_t)stateStart) >> 32) & 0xFFFFFFF);

	int endX = (((uint64_t)stateEnd) & 0xFFFFFFF);
	int endY = ((((uint64_t)stateEnd) >> 32) & 0xFFFFFFF);


	float dx = (float)(startX - endX);
	float dy = (float)(startX - endY);
	return sqrt(dx*dx + dy*dy) + 1.0;
}

/**
Return the exact cost from the given state to all its neighboring states. This
may be called multiple times, or cached by the solver. It *must* return the same
exact values for every call to MicroPather::Solve(). It should generally be a simple,
fast function with no callbacks into the pather.
*/
void Themp::TileMap::AdjacentCost(void* state, MP_VECTOR< micropather::StateCost >* adjacent)
{
	//const XMINT2 directions[4] = { XMINT2(1,0) ,XMINT2(-1,0),XMINT2(0,1),XMINT2(0,-1), };
	uint64_t startX = (((uint64_t)state) & 0xFFFFFFFF);
	uint64_t startY = ((((uint64_t)state) >> 32) & 0xFFFFFFFF);
	


	NeighbourSubTiles nb = Level::s_CurrentLevel->m_LevelData->GetNeighbourSubTiles(startY, startX);
	if (nb.North.cost < 1000.0f)
	{
		(*adjacent).push_back({ (void*)(((startY+1) << 32) | (startX)),nb.North.cost  });
		if (nb.NorthEast.cost < 1000.0f && nb.East.cost < 1000.0f)
		{
			(*adjacent).push_back({ (void*)(((startY+1) << 32) | (startX+1)),nb.NorthEast.cost + 0.5f });
		}
		if (nb.NorthWest.cost < 1000.0f && nb.West.cost < 1000.0f)
		{
			(*adjacent).push_back({ (void*)(((startY+1) << 32) | (startX-1)),nb.NorthWest.cost + 0.5f });
		}
	}
	if (nb.South.cost < 1000.0f)
	{
		(*adjacent).push_back({ (void*)(((startY-1) << 32) | (startX)),nb.South.cost });
		if (nb.SouthEast.cost < 1000.0f && nb.East.cost < 1000.0f)
		{
			(*adjacent).push_back({ (void*)(((startY-1) << 32) | (startX+1)),nb.SouthEast.cost + 0.5f });
		}
		if (nb.SouthWest.cost < 1000.0f && nb.West.cost < 1000.0f)
		{
			(*adjacent).push_back({ (void*)(((startY-1) << 32) | (startX-1)),nb.SouthWest.cost + 0.5f });
		}
	}
	if (nb.East.cost < 1000.0f)
	{
		(*adjacent).push_back({ (void*)(((startY) << 32) | (startX+1)),nb.East.cost });
	}
	if (nb.West.cost < 1000.0f)
	{
		(*adjacent).push_back({ (void*)(((startY) << 32) | (startX-1)),nb.West.cost });
	}
}

/**
This function is only used in DEBUG mode - it dumps output to stdout. Since void*
aren't really human readable, normally you print out some concise info (like "(1,2)")
without an ending newline.
*/
void Themp::TileMap::PrintStateInfo(void* state)
{
	System::Print("(X: %3i, Y: %3i)", (((uint64_t)state) & 0xFFFFFFFF), ((((uint64_t)state)>>32) & 0xFFFFFFFF));
}