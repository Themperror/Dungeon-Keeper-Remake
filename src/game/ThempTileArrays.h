#pragma once
#include <DirectXMath.h>
#include "../Library/micropather.h"
#include <array>
#include <unordered_map>
using namespace DirectX;

#define MAP_SIZE_HEIGHT (8)
#define MAP_SIZE_SUBTILES ((85 * 3) + 1)
#define MAP_SIZE_TILES (85)
#define MAP_SIZE_SUBTILES_RENDER (85 * 3)

#define L8(x) ((x)&0xFF)

static constexpr int Type_Rock = 0;
static constexpr int Type_Gold = 1;
static constexpr int Type_Earth = 2;
static constexpr int Type_Earth_Torch = 3;
static constexpr int Type_Wall0 = 4;
static constexpr int Type_Wall1 = 5;
static constexpr int Type_Wall2 = 6;
static constexpr int Type_Wall3 = 7;
static constexpr int Type_Wall4 = 8;
static constexpr int Type_Wall5 = 9;
static constexpr int Type_Unclaimed_Path = 10;
static constexpr int Type_Claimed_Land = 11;
static constexpr int Type_Lava = 12;
static constexpr int Type_Water = 13;
static constexpr int Type_Portal = 14;
static constexpr int Type_Treasure_Room = 16;
static constexpr int Type_Library = 18;
static constexpr int Type_Prison = 20;
static constexpr int Type_Torture_Room = 22;
static constexpr int Type_Training_Room = 24;
static constexpr int Type_Dungeon_Heart = 26;
static constexpr int Type_Workshop = 28;
static constexpr int Type_Scavenger_Room = 30;
static constexpr int Type_Temple = 32;
static constexpr int Type_Graveyard = 34;
static constexpr int Type_Hatchery = 36;
static constexpr int Type_Lair = 38;
static constexpr int Type_Barracks = 40;
static constexpr int Type_Wooden_DoorH = 42;
static constexpr int Type_Wooden_DoorV = 43;
static constexpr int Type_Braced_DoorH = 44;
static constexpr int Type_Braced_DoorV = 45;
static constexpr int Type_Iron_DoorH = 46;
static constexpr int Type_Iron_DoorV = 47;
static constexpr int Type_Magic_DoorH = 48;
static constexpr int Type_Magic_DoorV = 49;
static constexpr int Type_Bridge = 51;
static constexpr int Type_Gem = 52;
static constexpr int Type_Guardpost = 53;

//not real types currently for future doors unless I think of something better
static constexpr int Type_Wooden_DoorH_Locked = 54;
static constexpr int Type_Wooden_DoorV_Locked = 55;
static constexpr int Type_Braced_DoorH_Locked = 56;
static constexpr int Type_Braced_DoorV_Locked = 57;
static constexpr int Type_Iron_DoorH_Locked = 58;
static constexpr int Type_Iron_DoorV_Locked = 59;
static constexpr int Type_Magic_DoorH_Locked = 60;
static constexpr int Type_Magic_DoorV_Locked = 61;


const std::unordered_map<uint16_t, std::string> RoomTypeToString
{
	{Type_Portal		 , "Portal" },
	{Type_Treasure_Room	 , "Treasure Room" },
	{Type_Library		 , "Library" },
	{Type_Prison		 , "Prison" },
	{Type_Torture_Room	 , "Torture Chamber" },
	{Type_Training_Room	 , "Training Room" },
	{Type_Dungeon_Heart	 , "Dungeon Heart" },
	{Type_Workshop		 , "Workshop" },
	{Type_Scavenger_Room , "Scavenge Room" },
	{Type_Temple		 , "Temple" },
	{Type_Graveyard		 , "Graveyard" },
	{Type_Hatchery		 , "Hatchery" },
	{Type_Lair			 , "Lair" },
	{Type_Barracks		 , "Barracks" },
	{Type_Guardpost		 , "Guardpost" },
	{Type_Bridge		 , "Bridge" },
};


static bool IsWall(uint16_t type)
{
	return type >= Type_Wall0 && type <= Type_Wall5;
}
static bool IsWalkable(uint16_t type)
{
	return (type > Type_Wall5 && type != Type_Gem);
}
static bool IsMineable(uint16_t type)
{
	return (type >= Type_Gold && type <= Type_Wall5 || type == Type_Gem);
}
static bool IsMineableForPlayer(uint16_t type, uint8_t tileOwner, uint8_t player)
{
	if ((type >= Type_Gold && type < Type_Wall0 || type == Type_Gem))
	{
		return true;
	}
	else if (type >= Type_Wall0 && type <= Type_Wall5)
	{
		return tileOwner == player;
	}
	return false;
}
static bool IsClaimableRoom(uint16_t type)
{
	return ((type >= Type_Portal && type <= Type_Barracks || type == Type_Bridge || type == Type_Guardpost) && type != Type_Dungeon_Heart);
}
static uint16_t TypeToTexture(uint16_t type)
{
	switch (type & 0xFF)
	{
	case Type_Earth: 
		return 1;
		break;
	case Type_Water:
		return 2;
		break;
	case Type_Lava:
		return 8;
		break;
	case Type_Gold:
		return 3;
		break;
	case Type_Wall0:
	case Type_Wall1:
	case Type_Wall2:
	case Type_Wall3:
	case Type_Wall4:
	case Type_Wall5:
		return 6;
		break;
	case Type_Unclaimed_Path:
		return 4;
		break;
	case Type_Claimed_Land:
	case Type_Portal:
		return 5;
		break;
	case Type_Treasure_Room:
		return 9;
	case Type_Lair:
		return 10;
	case Type_Hatchery:
		return 11;
	default:
		return 0; 
		break;
	}
}

static constexpr int Owner_PlayerRed = 0;
static constexpr int Owner_PlayerBlue = 1;
static constexpr int Owner_PlayerGreen = 2;
static constexpr int Owner_PlayerYellow = 3;
static constexpr int Owner_PlayerWhite = 4;
static constexpr int Owner_PlayerNone = 5;

//3 bits
static constexpr int N_DIFF = 0b00;
static constexpr int N_SAME = 0b01;
static constexpr int N_WATER = 0b10;
static constexpr int N_LAVA = 0b11;
static constexpr int N_WALKABLE = 0b100;

namespace Themp
{
	class Entity;

	struct Block
	{
		Block()
		{
			randValue = 0;
		}
		Block(bool a, XMFLOAT2 u, XMFLOAT2 v)
		{
			active = a;
			uv[0] = u; //X (side)
			uv[1] = u; //Y (side)
			uv[2] = v; //Z (top)
			randValue = 0;
		}
		Block(bool a, XMFLOAT2 u,XMFLOAT2 v,XMFLOAT2 w)
		{
			active = a;
			uv[0] = u; //X (side)
			uv[1] = v; //Y (side)
			uv[2] = w; //Z (top)
			randValue = 0;
		}
		bool active = false;
		int randValue;
		XMFLOAT2 uv[3];
	};
	struct PathFindTile
	{
		PathFindTile()
		{
			cost = FLT_MAX;
			height = 8;
			walkable = false;
		}
		PathFindTile(float a_Cost, uint8_t a_Height, bool a_Walkable)
		{
			walkable = a_Walkable;
			if (a_Cost > 1000.0f || a_Walkable == false)
			{
				cost = FLT_MAX;
				walkable = false;
			}
			else
			{
				cost = a_Cost;
			}
			height = a_Height;
		}
		float cost = FLT_MAX;
		uint8_t height = 8;
		bool walkable = false;
	};
	struct RenderTile
	{
		RenderTile()
		{

		}
		RenderTile(std::array<std::array<std::array<Block, 8>, 3>, 3> tiles, std::array<std::array<PathFindTile, 3>, 3> pathTiles)
		{	
			subTile = tiles;
			pathSubTiles = pathTiles;
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 8; k++)
					{
						activeBlocks += subTile[i][j][k].active;
					}
				}
			}
		}
		std::array<std::array<std::array<Block,8>,3>,3> subTile;
		//Block subTile[3][3][8];
		std::array<std::array<PathFindTile, 3>, 3> pathSubTiles;
		//PathFindTile pathSubTiles[3][3];
		uint16_t activeBlocks = 0;
	}; 
	struct Tile
	{
		Tile()
		{
			visible = false;
			owner = Owner_PlayerNone;
			type = Type_Rock;
			numBlocks = 6 * 3 * 3;
			marked[0] = false;
			marked[1] = false;
			marked[2] = false;
			marked[3] = false;
			health = 1024;
			areaCode = 0;
			roomID = INT32_MAX;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					pathSubTiles[i][j] = PathFindTile();
					placedEntities[i][j] = nullptr;
				}
			}
		}
		uint16_t GetType()
		{
			return (type & 0xFF);
		}
		bool visible;
		uint8_t owner;
		uint16_t type;
		uint16_t numBlocks;
		bool marked[4];
		int32_t health;
		int32_t roomID;
		uint32_t areaCode;
		std::array<std::array<Entity*, 3>, 3> placedEntities;
		std::array<std::array<PathFindTile, 3>, 3> pathSubTiles;
	};
	struct NeighbourSubTiles
	{
		struct Direction
		{
			uint8_t height;
			uint8_t owner;
			uint16_t type;
			uint32_t areaCode;
			float cost;
		};
		//Anonymous union to bind all directions into our array
		//Keeping them in the "upper" scope of this struct ( Allows T.All[N] )
		union
		{
			Direction All[8];
			//Anonymous struct to avoid adding a layer ( Allows T.NorthWest etc.. )
			struct
			{
				//First few loose variables
				Direction NorthWest, NorthEast, SouthWest, SouthEast;
				union //Anonymous union again ( Allows T.Axii[N] )
				{
					Direction Axii[4];
					struct //Another anonymous struct ( Allows T.North etc..)
					{
						Direction North, South, West, East;
					};
				};
			};
		};
	};

	class TileMap : public micropather::Graph
	{
	public:
		Tile m_Tiles[MAP_SIZE_TILES][MAP_SIZE_TILES];

		/**
		Return the least possible cost between 2 states. For example, if your pathfinding
		is based on distance, this is simply the straight distance between 2 points on the
		map. If you pathfinding is based on minimum time, it is the minimal travel time
		between 2 points given the best possible terrain.
		*/
		virtual float LeastCostEstimate(void* stateStart, void* stateEnd);

		/**
		Return the exact cost from the given state to all its neighboring states. This
		may be called multiple times, or cached by the solver. It *must* return the same
		exact values for every call to MicroPather::Solve(). It should generally be a simple,
		fast function with no callbacks into the pather.
		*/
		virtual void AdjacentCost(void* state, MP_VECTOR< micropather::StateCost >* adjacent, bool ignoreWalls);

		/**
		This function is only used in DEBUG mode - it dumps output to stdout. Since void*
		aren't really human readable, normally you print out some concise info (like "(1,2)")
		without an ending newline.
		*/
		virtual void  PrintStateInfo(void* state);
	};
	
	struct TileNeighbours
	{
		unsigned char North : 3;
		unsigned char NorthEast : 3;
		unsigned char East : 3;
		unsigned char SouthEast : 3;
		unsigned char South : 3;
		unsigned char SouthWest : 3;
		unsigned char West : 3;
		unsigned char NorthWest : 3;
	};



	const XMINT2 AxiiDirections[4] = { XMINT2(0,1), XMINT2(0,-1), XMINT2(-1,0), XMINT2(1,0), };
	//I have since learned that this following is UB in C++, It works for now, I'll figure out something at some point for this..
	struct TileNeighbourTiles
	{
		//Anonymous union to bind all directions into our array
		//Keeping them in the "upper" scope of this struct ( Allows T.All[N] )
		union
		{
			Tile* All[8];
			//Anonymous struct to avoid adding a layer ( Allows T.NorthWest etc.. )
			struct
			{
				//First few loose variables
				Tile *NorthWest, *NorthEast, *SouthWest, *SouthEast;
				union //Anonymous union again ( Allows T.Axii[N] )
				{
					Tile* Axii[4];
					struct //Another anonymous struct ( Allows T.North etc..)
					{
						Tile *North, *South, *West, *East;
					};
				};
			};
		};
	};
	
	constexpr int Tile_ROCK = 0;
	constexpr int Tile_FULLBLOCK = 1;
	constexpr int Tile_GROUND = 2;
	constexpr int Tile_LIQUID = 3;
	constexpr int Tile_DUNGEON_HEART = 4;
	constexpr int Tile_PORTAL = 14;
	constexpr int Tile_LIBRARY = 24;
	constexpr int Tile_BARRACKS = 34;
	constexpr int Tile_PILLAR_ROOM = 44;
	const RenderTile RenderTileMap[] =
	{
		#include "Rock.h" //0
		#include "FullBlock.h" //1
		#include "GroundBlock.h" //2
		#include "LiquidBlock.h" //3
		#include "DungeonHeart.h"//4 - 13
		#include "Portal.h" //14 - 23
		#include "Library.h"//24 - 33
		#include "Barracks.h"//34 - 43
		#include "RoomWithPillars.h"//44 - 53
	};

	struct TileTextures
	{
		//following a 3x3 grid starting left up (0,0) (1,0) (2,0) (0,1) etc..
		std::vector<std::vector<XMFLOAT2>> top;
		//following a 3x4 grid starting left up (0,0) (1,0) (2,0) (0,1) etc..
		std::vector<std::vector<XMFLOAT2>> wall;
		//tiles next to liquids (normal , lava , water)
		std::vector<std::vector<XMFLOAT2>> edge; 
	};
	const XMFLOAT2 t_WallOwners[6] =
	{
		{ 3,13 },
		{ 4,13 },
		{ 5,13 },
		{ 6,13 },
		{ 7,13 },
		{ 3,0 },
	};
	const XMFLOAT2 t_FloorOwners[6] =
	{
		{ 0,34 },
		{ 1,34 },
		{ 2,34 },
		{ 3,34 },
		{ 7,34 },
		{ 6,34 },
	};

	const TileTextures t_Black =
	{
		//top
		{
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
		},
		//side
		{
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black

				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
		},
		//edge
		{
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
		},
	};
	const TileTextures t_Earth =
	{
		//top
		{
			{
				XMFLOAT2(2,0),
			},
			{
				XMFLOAT2(3,0),
			},
			{
				XMFLOAT2(4,0),
			},
		},
		//side
		{
			{
				XMFLOAT2(6,0),
				XMFLOAT2(7,0),
				XMFLOAT2(0,1),
				XMFLOAT2(1,1),
				XMFLOAT2(2,0),
				XMFLOAT2(2,1),
				XMFLOAT2(1,1),
				XMFLOAT2(2,0),
				XMFLOAT2(2,1),
				XMFLOAT2(3,1),
				XMFLOAT2(4,1),
				XMFLOAT2(5,1),
			},
			{
				XMFLOAT2(6,0),
				XMFLOAT2(7,0),
				XMFLOAT2(0,1),
				XMFLOAT2(1,1),
				XMFLOAT2(3,0),
				XMFLOAT2(2,1),
				XMFLOAT2(1,1),
				XMFLOAT2(3,0),
				XMFLOAT2(2,1),
				XMFLOAT2(3,1),
				XMFLOAT2(4,1),
				XMFLOAT2(5,1),
			},
			{
				XMFLOAT2(6,0),
				XMFLOAT2(7,0),
				XMFLOAT2(0,1),
				XMFLOAT2(1,1),
				XMFLOAT2(4,0),
				XMFLOAT2(2,1),
				XMFLOAT2(1,1),
				XMFLOAT2(4,0),
				XMFLOAT2(2,1),
				XMFLOAT2(3,1),
				XMFLOAT2(4,1),
				XMFLOAT2(5,1),
			},
		},
		//edge
		{
			{
				XMFLOAT2(4,1), //normal
			},
			{
				XMFLOAT2(0,9), //lava
				XMFLOAT2(6,9), //lava
			},
			{
				XMFLOAT2(0,5), //water
				XMFLOAT2(1,5), //water
				XMFLOAT2(2,5), //water
				XMFLOAT2(3,5), //water
				XMFLOAT2(4,5), //water
				XMFLOAT2(5,5), //water
				XMFLOAT2(6,5), //water
				XMFLOAT2(7,5), //water
			},
		},
	};
	const TileTextures t_Water =
	{
		//top
		{
			{
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
				XMFLOAT2(4,6), //black
				XMFLOAT2(5,6), //black
				XMFLOAT2(6,6), //black
				XMFLOAT2(7,6), //black
			},
		},
		//side
		{
			{
				XMFLOAT2(4,15),XMFLOAT2(5,15),XMFLOAT2(6,15),
				XMFLOAT2(7,15),XMFLOAT2(0,16),XMFLOAT2(1,16),
				XMFLOAT2(2,16),XMFLOAT2(3,16),XMFLOAT2(4,16),
				XMFLOAT2(5,16),XMFLOAT2(6,16),XMFLOAT2(7,16),
			},
		},
		//edge
		{
			{
				XMFLOAT2(0,0), //normal
			},
		},
	};
	const TileTextures t_Gold =
	{
		//top
		{
			{
				XMFLOAT2(0,10), 
				XMFLOAT2(1,10), 
				XMFLOAT2(2,10), 
				XMFLOAT2(3,10), 
			},
			{
				XMFLOAT2(4,10), 
				XMFLOAT2(5,10), 
				XMFLOAT2(6,10), 
				XMFLOAT2(7,10), 
			},
			{
				XMFLOAT2(0,11), 
				XMFLOAT2(1,11), 
				XMFLOAT2(2,11), 
				XMFLOAT2(3,11), 
			},
		},
		//side
		{
			{
				XMFLOAT2(0,10),
				XMFLOAT2(1,10),
				XMFLOAT2(2,10),
				XMFLOAT2(3,10),
			},
			{
				XMFLOAT2(4,10),
				XMFLOAT2(5,10),
				XMFLOAT2(6,10),
				XMFLOAT2(7,10),
			},
			{
				XMFLOAT2(0,11),
				XMFLOAT2(1,11),
				XMFLOAT2(2,11),
				XMFLOAT2(3,11),
			},
			{
				XMFLOAT2(4,11),
				XMFLOAT2(5,11),
				XMFLOAT2(6,11),
				XMFLOAT2(7,11),
			},
		},
		//edge
		{
			{
				XMFLOAT2(0,6),
				XMFLOAT2(1,6),
				XMFLOAT2(2,6),
				XMFLOAT2(3,6),
			},
			{
				XMFLOAT2(4,6),
				XMFLOAT2(5,6),
				XMFLOAT2(6,6),
				XMFLOAT2(7,6),
			},
			{
				XMFLOAT2(0,7),
				XMFLOAT2(1,7),
				XMFLOAT2(2,7),
				XMFLOAT2(3,7),
			},
		},
	};
	const TileTextures t_UnclaimedPath =
	{
		//top
		{
			{
				XMFLOAT2(3,3), //clear path
				XMFLOAT2(4,3), //clear path
				XMFLOAT2(5,3), //clear path
			},
			{
				XMFLOAT2(3,3), //rumbly path
				XMFLOAT2(4,3), //rumbly path
				XMFLOAT2(5,3), //rumbly path
				XMFLOAT2(6,3), //rumbly path
				XMFLOAT2(7,3), //rumbly path
			},
		},
		//side
		{
			{
				XMFLOAT2(4,15),XMFLOAT2(5,15),XMFLOAT2(6,15),
				XMFLOAT2(7,15),XMFLOAT2(0,16),XMFLOAT2(1,16),
				XMFLOAT2(2,16),XMFLOAT2(3,16),XMFLOAT2(4,16),
				XMFLOAT2(5,16),XMFLOAT2(6,16),XMFLOAT2(7,16),
			},
			{
				XMFLOAT2(0,17),XMFLOAT2(1,17),XMFLOAT2(2,17),
				XMFLOAT2(3,17),XMFLOAT2(4,17),XMFLOAT2(5,17),
				XMFLOAT2(6,17),XMFLOAT2(7,17),XMFLOAT2(0,18),
				XMFLOAT2(1,18),XMFLOAT2(2,18),XMFLOAT2(3,18),
			},
			{
				XMFLOAT2(4,18),XMFLOAT2(5,18),XMFLOAT2(6,18),
				XMFLOAT2(7,18),XMFLOAT2(0,19),XMFLOAT2(1,19),
				XMFLOAT2(2,19),XMFLOAT2(3,19),XMFLOAT2(4,19),
				XMFLOAT2(5,19),XMFLOAT2(6,19),XMFLOAT2(7,19),
			},
		},
		//edge
		{
			{
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
		},
	};
	const TileTextures t_ClaimedPath =
	{
		//top
		{
			{
				XMFLOAT2(3,22),
				XMFLOAT2(4,22),
				XMFLOAT2(5,22),
				XMFLOAT2(6,22),
				XMFLOAT2(0,0),
				XMFLOAT2(7,22),
				XMFLOAT2(0,23),
				XMFLOAT2(1,23),
				XMFLOAT2(2,23),
			},
			{
				XMFLOAT2(0,22), //random mid-pieces
				XMFLOAT2(1,22), //random mid-pieces
				XMFLOAT2(2,22), //random mid-pieces
			},
		},
		//side
		{
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(4,15),XMFLOAT2(5,15),XMFLOAT2(6,15),
				XMFLOAT2(7,15),XMFLOAT2(0,16),XMFLOAT2(1,16),
				XMFLOAT2(2,16),XMFLOAT2(3,16),XMFLOAT2(4,16),
				XMFLOAT2(5,16),XMFLOAT2(6,16),XMFLOAT2(7,16),
			},
			{
				XMFLOAT2(0,17),XMFLOAT2(1,17),XMFLOAT2(2,17),
				XMFLOAT2(3,17),XMFLOAT2(4,17),XMFLOAT2(5,17),
				XMFLOAT2(6,17),XMFLOAT2(7,17),XMFLOAT2(0,18),
				XMFLOAT2(1,18),XMFLOAT2(2,18),XMFLOAT2(3,18),
			},
			{
				XMFLOAT2(4,18),XMFLOAT2(5,18),XMFLOAT2(6,18),
				XMFLOAT2(7,18),XMFLOAT2(0,19),XMFLOAT2(1,19),
				XMFLOAT2(2,19),XMFLOAT2(3,19),XMFLOAT2(4,19),
				XMFLOAT2(5,19),XMFLOAT2(6,19),XMFLOAT2(7,19),
			},
		},
		//edge
		{
			{
				XMFLOAT2(4,23),
				XMFLOAT2(5,23),
				XMFLOAT2(6,23),
			},
		},
	};
	const TileTextures t_Wall =
	{
		//top
		{
			{
				XMFLOAT2(2,13), //corner
				XMFLOAT2(3,14), //side
				XMFLOAT2(3,0), //normal
			},
		},
		//side
		{
			{
				XMFLOAT2(0,0),
			},
		},
		//edge
		{
			//
			{
				XMFLOAT2(4,23),
			},
			{
				XMFLOAT2(5,23),
			},
			{
				XMFLOAT2(6,23),
			},
		},
	};
	const TileTextures t_Marked =
	{
		//top
		{
			//Normal
			{
				XMFLOAT2(0,63),
				XMFLOAT2(1,63),
				XMFLOAT2(2,63),
				XMFLOAT2(3,63),
			},
			//Gold
			{
				XMFLOAT2(4,63),
				XMFLOAT2(5,63),
				XMFLOAT2(6,63),
				XMFLOAT2(7,63),
			},
		},
		//side
		{
			{
				XMFLOAT2(0,0),
			},
		},
		//edge
		{
			//
			{
				XMFLOAT2(0,0), //black
			}
		},
	};
	const TileTextures t_Lava =
	{
		//top
		{
			{
				XMFLOAT2(0,7),
				XMFLOAT2(1,7),
				XMFLOAT2(2,7),
				XMFLOAT2(3,7),
				XMFLOAT2(4,7),
				XMFLOAT2(5,7),
				XMFLOAT2(6,7),
				XMFLOAT2(7,7),
			},
			{
				XMFLOAT2(0,8),
				XMFLOAT2(1,8),
				XMFLOAT2(2,8),
				XMFLOAT2(3,8),
				XMFLOAT2(4,8),
				XMFLOAT2(5,8),
				XMFLOAT2(6,8),
				XMFLOAT2(7,8),
			},
		},
		//side
		{
			{
				XMFLOAT2(7,9),XMFLOAT2(7,9),XMFLOAT2(7,9),
				XMFLOAT2(7,9),XMFLOAT2(7,9),XMFLOAT2(7,9),
				XMFLOAT2(7,9),XMFLOAT2(7,9),XMFLOAT2(7,9),
				XMFLOAT2(7,9),XMFLOAT2(7,9),XMFLOAT2(7,9),
			},

		},
		//edge
		{
			{
				XMFLOAT2(0,9),
			},
			{
				XMFLOAT2(6,9),
			},
		},
	};
	const TileTextures t_Lair =
	{
		//top
		{
			{
				XMFLOAT2(0,4),
				XMFLOAT2(1,4),
				XMFLOAT2(2,4),
				XMFLOAT2(3,4),
				XMFLOAT2(7,33), //mid
				XMFLOAT2(4,4),
				XMFLOAT2(5,4),
				XMFLOAT2(6,4),
				XMFLOAT2(7,4),
			},
		},
		//side
		{
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(4,14),XMFLOAT2(5,14),XMFLOAT2(6,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(2,2),XMFLOAT2(3,2),XMFLOAT2(4,2),
				XMFLOAT2(0,14),XMFLOAT2(5,2),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(6,2),XMFLOAT2(2,14),
				XMFLOAT2(7,2),XMFLOAT2(0,3),XMFLOAT2(1,3),
			},
		},
		//edge
		{
			{
				XMFLOAT2(4,23),
				XMFLOAT2(5,23),
				XMFLOAT2(6,23),
			},
		},
	};
	const TileTextures t_Treasury =
	{
		//top
		{
			{
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
				XMFLOAT2(0,26), //mid
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
				XMFLOAT2(0,26),
			},
		},
		//side
		{
			{
				XMFLOAT2(0,27),XMFLOAT2(1,27),XMFLOAT2(2,27), //redflag, to prevent access out of bounds for unimplemented situations
				//room 3x3 side
				XMFLOAT2(2,26),XMFLOAT2(3,26),XMFLOAT2(4,26),
				XMFLOAT2(5,26),XMFLOAT2(6,26),XMFLOAT2(7,26),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
			{
				XMFLOAT2(0,27),XMFLOAT2(1,27),XMFLOAT2(2,27), //redflag, to prevent access out of bounds for unimplemented situations
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(0,14),XMFLOAT2(1,14),XMFLOAT2(2,14),
				XMFLOAT2(1,15),XMFLOAT2(2,15),XMFLOAT2(3,15),
			},
		},
		//edge
		{
			{
				XMFLOAT2(4,23),
				XMFLOAT2(5,23),
				XMFLOAT2(6,23),
			},
		},
	};
	const TileTextures t_Hatchery =
	{
		//top
		{
			{
				XMFLOAT2(4,3), //center
				XMFLOAT2(3,14), //edge piece
			},
		},
		//side
		{
			{   
				XMFLOAT2(0,27),XMFLOAT2(1,27),XMFLOAT2(2,27), //redflag, to prevent access out of bounds for unimplemented situations
				//room 3x3 side
				XMFLOAT2(4,59),XMFLOAT2(5,59),XMFLOAT2(6,59),
				XMFLOAT2(7,59),XMFLOAT2(0,60),XMFLOAT2(1,60),
				XMFLOAT2(2,60),XMFLOAT2(3,60),XMFLOAT2(4,60),
			},
			{   //flag colors on top
				XMFLOAT2(0,27),XMFLOAT2(1,27),XMFLOAT2(2,27), //red
				XMFLOAT2(0,64),XMFLOAT2(1,64),XMFLOAT2(2,64), //blue
				XMFLOAT2(3,64),XMFLOAT2(4,64),XMFLOAT2(5,64), //green
				XMFLOAT2(6,64),XMFLOAT2(7,64),XMFLOAT2(0,65), //yellow
				XMFLOAT2(1,65),XMFLOAT2(2,65),XMFLOAT2(3,65), //white
				XMFLOAT2(1,65),XMFLOAT2(2,65),XMFLOAT2(3,65), //unowned
			},
		},
		//edge
		{
			{
				XMFLOAT2(4,23), //normal
				XMFLOAT2(5,23), //lava
				XMFLOAT2(6,23), //water
				XMFLOAT2(6,60), //Inner edge
			},
		},
	};
	const std::vector<TileTextures> BlockTextures =
	{
		t_Black,//0
		t_Earth,//1
		t_Water,//2
		t_Gold, //3
		t_UnclaimedPath, //4
		t_ClaimedPath, //5
		t_Wall, //6
		t_Marked, //7
		t_Lava, //8
		t_Treasury, //9 
		t_Lair, //10
		t_Hatchery, //11
	};
};