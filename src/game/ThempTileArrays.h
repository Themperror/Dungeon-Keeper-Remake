#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#define MAP_SIZE_HEIGHT (8)
#define MAP_SIZE_SUBTILES (85 * 3 + 1)
#define MAP_SIZE_TILES (85)

namespace Themp
{
	struct Block
	{
		Block()
		{

		}
		Block(bool a, XMFLOAT2 u,XMFLOAT2 v)
		{
			active = a;
			uv[0] = u;
			uv[1] = v;
		}
		bool active = false;
		XMFLOAT2 uv[2];

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
		SubTile sub[3][3];
	};

	const RenderTile RenderTileMap[] =
	{
		//FullBlock
		{
			{
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),

				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
				Block(true,XMFLOAT2(3,0),XMFLOAT2(3,0)),
			},
			8*3*3,
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
		//3
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
		//12
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
		//following a 3x3 grid starting left up (0,0) (1,0) (2,0) (0,1) etc..
		std::vector<std::vector<XMFLOAT2>> side;
		//tiles next to liquids (normal , lava , water)
		std::vector<std::vector<XMFLOAT2>> edge; 
	};

	const TileTextures t_Black =
	{
		//top
		{
			{
				XMFLOAT2(0,0), //black
			},
		},
		//side
		{
			{
				XMFLOAT2(0,0), //black
			},
		},
		//edge
		{
			{
				XMFLOAT2(0,0), //black
			},
		},
	};
	const TileTextures t_Earth =
	{
		//top
		{
			{
				XMFLOAT2(2,0), //black
			},
			{
				XMFLOAT2(3,0), //black
			},
			{
				XMFLOAT2(4,0), //black
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
				XMFLOAT2(3,1),
				XMFLOAT2(4,1),
				XMFLOAT2(5,1),
			},
		},
		//edge
		{
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(0,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(1,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(2,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(3,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(4,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(5,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
				XMFLOAT2(6,5), //water
			},
			{
				XMFLOAT2(9,5), //normal
				XMFLOAT2(9,4), //lava
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
				XMFLOAT2(0,0),
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
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
			{
				XMFLOAT2(4,6), //black
				XMFLOAT2(5,6), //black
				XMFLOAT2(6,6), //black
				XMFLOAT2(7,6), //black
			},
			{
				XMFLOAT2(0,7), //black
				XMFLOAT2(1,7), //black
				XMFLOAT2(2,7), //black
				XMFLOAT2(3,7), //black
			},
		},
		//side
		{
			{
				XMFLOAT2(0,6), //black
				XMFLOAT2(1,6), //black
				XMFLOAT2(2,6), //black
				XMFLOAT2(3,6), //black
			},
			{
				XMFLOAT2(4,6), //black
				XMFLOAT2(5,6), //black
				XMFLOAT2(6,6), //black
				XMFLOAT2(7,6), //black
			},
			{
				XMFLOAT2(0,7), //black
				XMFLOAT2(1,7), //black
				XMFLOAT2(2,7), //black
				XMFLOAT2(3,7), //black
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
			{
				XMFLOAT2(4,6), //black
				XMFLOAT2(5,6), //black
				XMFLOAT2(6,6), //black
				XMFLOAT2(7,6), //black
			},
			{
				XMFLOAT2(0,7), //black
				XMFLOAT2(1,7), //black
				XMFLOAT2(2,7), //black
				XMFLOAT2(3,7), //black
			},
		},
	};
	const std::vector<TileTextures> BlockTextures =
	{
		t_Black,
		t_Earth,
		t_Water,
		t_Gold,

	};
	//	{
	//		XMFLOAT2(1,0), //Purple Gem?
	//	},
	//	{
	//		XMFLOAT2(2,0), //Earth 1
	//		XMFLOAT2(3,0), //Earth 2
	//		XMFLOAT2(4,0), //Earth 3
	//		XMFLOAT2(5,0), //Earth 4 Yellow?
	//	},
	//	{
	//		XMFLOAT2(6,0), //Earth 1 Top
	//		XMFLOAT2(7,0), //Earth 2 Top
	//		XMFLOAT2(8,0), //Earth 3 Top
	//		XMFLOAT2(0,1), //Earth 4 Top
	//	},
	//	{
	//		XMFLOAT2(1,1), //Earth 1 ??
	//		XMFLOAT2(2,1), //Earth 2 ??
	//		XMFLOAT2(3,1), //Earth 3 ??
	//		XMFLOAT2(4,1), //Earth 4 ??
	//		XMFLOAT2(5,1), //Earth 5 ??
	//	},
	//	{
	//		XMFLOAT2(6,1), //Portal blocks
	//		XMFLOAT2(7,1), //Portal blocks
	//		XMFLOAT2(0,2), //portal blocks
	//		XMFLOAT2(1,2), //portal blocks
	//	},
	//	{
	//		XMFLOAT2(3,2), //Lair Wall Tiles
	//		XMFLOAT2(4,2), //Lair Wall Tiles
	//		XMFLOAT2(5,2), //Lair Wall Tiles
	//		XMFLOAT2(6,2), //Lair Wall Tiles
	//		XMFLOAT2(7,2), //Lair Wall Tiles
	//		XMFLOAT2(0,3), //Lair Wall Tiles
	//		XMFLOAT2(1,3), //Lair Wall Tiles
	//		XMFLOAT2(2,3), //Lair Wall Tiles
	//	},
	//	{
	//		XMFLOAT2(3,3), //path mid
	//		XMFLOAT2(4,3), //path mid
	//		XMFLOAT2(5,3), //path mid
	//		XMFLOAT2(6,3), //path mid
	//		XMFLOAT2(7,3), //path mid
	//	},
	//	{
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3,3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	XMFLOAT2(3, 3), //path mid
	//	},
	//;
};