#pragma once
#include <vector>
#include "ThempTileArrays.h"
#include "ThempFileManager.h"
namespace Themp
{
	class D3D;
	class VoxelObject;
	class Object3D;
	class Object2D;
	struct Sprite;
	class Level
	{
	public:
		struct clm_data
		{
			//Bit 0: Permanent - if set, the column is never overwritten
			//Bits 1 - 3 : Lintel(unknown)
			//Bits 4 - 7 : Height - in subtiles; proper values are 0..7
			uint8_t bitfield;
			//always 0
			uint8_t orientation;
			//Counter of the column uses for non-permanent columns; For permanent colums, this value has an offset.
			uint16_t use;
			//appears to be a bitmask for which of the cubes are non-zero, with the lowest cube being represented by the least significant bit.
			uint16_t solidMask;
			//determines the very lowest level (below the lowest cube). I think the block numbers here are different to the ones used by the cubes, but I'm not sure yet. This entry is mostly useful for water, lava, and hatcheries.
			uint16_t baseBlock;
			// there are 8 cubes, each having a 2 byte entry. The first cube usually forms the floor, etc, up to the ceiling.
			uint16_t cubes[8];
		};
		struct HitData
		{
			bool hit;
			int distance;
			int posX;
			int posY;
			int posZ;
		};
		~Level();
		Level(int levelIndex);
		HitData Raycast(XMFLOAT3 origin, XMFLOAT3 direction, float range);
		uint8_t GetNeighbourInfo(uint16_t currentType, uint16_t nType);
		TileNeighbours CheckNeighbours(uint16_t type, int y, int x);
		TileNeighbourTiles GetNeighbourTiles(int y, int x);
		void DoUVs(uint16_t type, int x, int y);
		void DoWallUVs(TileNeighbours neighbour, int type, int texIndex, int x, int y);
		uint16_t HandleSpecialCases(int yPos, int xPos);
		void UpdateWalls(int y, int x);
		void DestroyTile(int y, int x);
		void UpdateArea(int minY, int maxY, int minX, int maxX);
		void ClaimRoom(uint8_t newOwner, uint16_t type, int ty, int tx);
		bool IsClaimableCorner(int y, int x);
		bool IsOwnedRoom(uint8_t player, int y, int x);
		void ClaimTile(uint8_t player, int y, int x);
		void Update(float delta);
		int Level::PathFind(XMINT2 A, XMINT2 B, micropather::MPVector<void*>& outPath, float& outCost, bool AllowDoors);
		void LoadLevelData();
		int CreateFromTile(const Tile & tile, RenderTile & out);
		Object2D* m_Cursor = nullptr;
		Object2D* m_MinimapObject = nullptr;
		int UnownedRoomColorIndex = 0;
		float UnOwnedRoomColorTimer = 0;
		GUITexture m_MinimapData;
		VoxelObject* m_MapObject = nullptr;
		int m_CurrentLevelNr;
		//clm_data m_Map[MAP_SIZE_SUBTILES][MAP_SIZE_SUBTILES]; //in subtiles,  includes the border tiles (playable area is 85x85 TILES)

		//Map that keeps the initial state of the map (for water/lava blocks)
		TileMap m_OriginalMap;
		//Map which the current changes to it (mined/dug out blocks, rooms etc..)
		TileMap m_Map;
		//Map in subtile format, used for pathfinding/picking
		Block m_BlockMap[MAP_SIZE_HEIGHT][MAP_SIZE_SUBTILES_RENDER][MAP_SIZE_SUBTILES_RENDER];
	};
};
