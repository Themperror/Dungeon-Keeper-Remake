#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#define MAP_SIZE_HEIGHT (8)
#define MAP_SIZE_SUBTILES (85 * 3 + 1)
#define MAP_SIZE_TILES (85)
#define MAP_SIZE_SUBTILES_RENDER (85 * 3)

#define L8(x) ((x)&0xFF)

static const int Type_Rock = 0;
static const int Type_Gold = 1;
static const int Type_Earth = 2;
static const int Type_Earth_Torch = 3;
static const int Type_Wall0 = 4;
static const int Type_Wall1 = 5; //These remain unused for now, I change them to Wall0 during level loading.
static const int Type_Wall2 = 6; 
static const int Type_Wall3 = 7; 
static const int Type_Wall4 = 8; //These remain unused for now, I change them to Wall0 during level loading.
static const int Type_Wall5 = 9; //These remain unused for now, I change them to Wall0 during level loading.
static const int Type_Unclaimed_Path = 10;
static const int Type_Claimed_Land = 11;
static const int Type_Lava = 12;
static const int Type_Water = 13;
static const int Type_Portal = 14;
static const int Type_Treasure_Room = 16;
static const int Type_Library = 18;
static const int Type_Prison = 20;
static const int Type_Torture_Room = 22;
static const int Type_Training_Room = 24;
static const int Type_Dungeon_Heart = 26;
static const int Type_Workshop = 28;
static const int Type_Scavenger_Room = 30;
static const int Type_Temple = 32;
static const int Type_Graveyard = 34;
static const int Type_Hatchery = 36;
static const int Type_Lair = 38;
static const int Type_Barracks = 40;
static const int Type_Wooden_DoorH = 42;
static const int Type_Wooden_DoorV = 43;
static const int Type_Braced_DoorH = 44;
static const int Type_Braced_DoorV = 45;
static const int Type_Iron_DoorH = 46;
static const int Type_Iron_DoorV = 47;
static const int Type_Magic_DoorH = 48;
static const int Type_Magic_DoorV = 49;
static const int Type_Bridge = 51;
static const int Type_Gem = 52;
static const int Type_Guardpost = 53;

static bool IsWall(uint16_t type)
{
	return type >= Type_Wall0 && type <= Type_Wall5;
}
static uint16_t TypeToTexture(uint16_t type)
{
	switch (type)
	{
	case Type_Earth: 
		return 1;
		break;
	case Type_Water:
		return 2;
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
		return 5;
		break;
	default:
		return 0; 
		break;
	}
}


static const int Owner_PlayerRed = 0;
static const int Owner_PlayerBlue = 1;
static const int Owner_PlayerGreen = 2;
static const int Owner_PlayerYellow = 3;
static const int Owner_PlayerWhite = 4;
static const int Owner_PlayerNone = 5;

//3 bits
static const int N_DIFF = 0b00;
static const int N_SAME = 0b01;
static const int N_WATER = 0b10;
static const int N_LAVA = 0b11;
static const int N_WALKABLE = 0b100;

namespace Themp
{
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
	struct RenderTile
	{
		Block subTile[3][3][8];
		uint16_t activeBlocks = 0;
	};
	struct SubTile
	{
		bool walkable;
		uint8_t height;
	};
	struct Tile
	{
		uint8_t owner;
		uint16_t type;
		uint16_t numBlocks;
		SubTile sub[3][3];
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

	struct TileNeighbourTiles
	{
		struct Direction
		{
			uint8_t owner;
			uint16_t type;
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
				Direction NorthWest,NorthEast,SouthWest,SouthEast;
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


	const int Tile_ROCK = 0;
	const int Tile_FULLBLOCK = 1;
	const int Tile_GROUND = 2;
	const int Tile_LIQUID = 3;
	const int Tile_DUNGEON_HEART = 4;
	const int Tile_PORTAL = 13;
	const RenderTile RenderTileMap[] =
	{
		//Rock
		{
			{
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),

				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(true,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
				Block(false,XMFLOAT2(2,9),XMFLOAT2(2,9)),
			},
			6 * 3 * 3,
		},
		//FullBlock
		{
			{
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(false,XMFLOAT2(3,0),XMFLOAT2(3,0)),
			},
			6*3*3,
		},
		//Ground Block
		{
			{
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(0,0)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(0,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(true ,XMFLOAT2(6,23),XMFLOAT2(6,34)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			3 * 3 * 2,
		},
		//Liquid Block
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,6)),
			},
			3*3*1,
		},
		//4
		//Dungeon Heart Corner LU
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,27),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,27),XMFLOAT2(5,27)),
			},
			33,
		},
		//Dungeon Heart Middle Up
		{
			{
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Dungeon Heart Corner RU
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,27),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,27),XMFLOAT2(5,27)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			33,
		},
		//Dungeon heart Middle Left
		{
			{
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Dungeon Heart Middle
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			3*3*4,
		},
		//Dungeon Heart Middle Right
		{
			{
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Dungeon Heart Corner LD
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,27),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,27),XMFLOAT2(5,27)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			33,
		},
		//Dungeon Heart Middle Down
		{
			{
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(1,26),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27
		},
		//Dungeon Heart Corner RD
		{
			{
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(2,14),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,27),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,27),XMFLOAT2(5,27)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(true ,XMFLOAT2(4,25),XMFLOAT2(5,25)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(true ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,26)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			33,
		},
		//13
		//Portal LU
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(0,2),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,2),XMFLOAT2(7,1)),
			},
			27,
		},
		//Portal Mid U
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),
			},
			27,
		},
		//Portal RU
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(0,2),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Portal Mid L
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),
			},
			27,
		},
		//Portal Mid M
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(7,23)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(0,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(1,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(2,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(3,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(4,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(5,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(6,24)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(0,0),XMFLOAT2(7,24)),
			},
			18,
		},
		//Portal Mid R
		{
			{

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Portal LD //not sure if good
		{
			{
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(0,2),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Portal Mid D
		{
			{
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(true,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
		//Portal RD //not sure if good
		{
			{
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(0,2),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(7,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(6,1),XMFLOAT2(5,27)),
				Block(true ,XMFLOAT2(1,2),XMFLOAT2(7,1)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(4,25),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(2,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(2,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(2,0),XMFLOAT2(2,0)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),

				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(true ,XMFLOAT2(0,0),XMFLOAT2(3,14)),
				Block(false ,XMFLOAT2(1,14),XMFLOAT2(6,25)),
				Block(false ,XMFLOAT2(5,14),XMFLOAT2(6,25)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
				Block(false,XMFLOAT2(0,0),XMFLOAT2(0,0)),
			},
			27,
		},
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
			},
			{
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
				XMFLOAT2(0,0), //black
			},
			{
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
				XMFLOAT2(9,5), //normal
			},
			{
				XMFLOAT2(9,4), //lava
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
				XMFLOAT2(4,23), //black
				XMFLOAT2(5,23), //black
				XMFLOAT2(6,23), //black
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
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
			{
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
			{
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
		},
	};
	const std::vector<TileTextures> BlockTextures =
	{
		t_Black,
		t_Earth,
		t_Water,
		t_Gold,
		t_UnclaimedPath,
		t_ClaimedPath,
		t_Wall,
	};
};