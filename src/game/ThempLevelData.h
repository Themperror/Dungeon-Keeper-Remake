#pragma once
#include <vector>
#include <stack>
#include "ThempTileArrays.h"
#include "ThempFileManager.h"
namespace Themp
{
	class LevelData
	{
	public:
		struct Room
		{
			struct RoomTile
			{
				int tileValue;
				int x, y;
				Tile* tile;
			};
			//Actual room data
			int health;
			int biggestSquare;
			int tilecount;
			int areaCode;
			int roomID;
			int roomType;
			int roomEfficiency;
			int roomFillAmount;
			int roomFillPercentage;
			std::unordered_map<Tile*, RoomTile> tiles;
			Room& operator+=(const Room& rhs)
			{
				health += rhs.health;
				biggestSquare = biggestSquare >= rhs.biggestSquare ? biggestSquare : rhs.biggestSquare;
				tilecount += rhs.tilecount;
				roomFillAmount += rhs.roomFillAmount;
				auto& it = rhs.tiles.begin();
				while (it != rhs.tiles.end())
				{
					tiles[it->first] = it->second;
					it++;
				}
				assert(tilecount == tiles.size());
				return *this;
			}
			void RecalcRoomData()
			{
				//recalc efficiency / fill amount
				roomEfficiency = 100;
				//roomFillPercentage = tilecount / roomFillAmount * 100;
			}
		};
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
		~LevelData();
		LevelData(int levelIndex);
		void Init();
		HitData Raycast(XMFLOAT3 origin, XMFLOAT3 direction, float range);
		uint8_t GetNeighbourInfo(uint16_t currentType, uint16_t nType);
		TileNeighbours CheckNeighbours(uint16_t type, int y, int x);
		TileNeighbourTiles GetNeighbourTiles(int y, int x);
		NeighbourSubTiles GetNeighbourSubTiles(int y, int x);
		uint8_t GetSubtileHeight(int tileY, int tileX, int subTileY, int subTileX);
		void DoUVs(uint16_t type, int x, int y);
		void DoRoomUVs(const TileNeighbours& neighbour, int type, int texIndex, int x, int y);
		void DoWallUVs(const TileNeighbours& neighbour, int type, int texIndex, int x, int y);
		void AddExploredTileNeighboursVisibility(int y, int x, int areaCode);
		uint16_t Handle3by3Rooms(int yPos, int xPos);
		uint16_t HandleNon3by3RoomsPillars(int yPos, int xPos);
		void UpdateWalls(int y, int x);
		bool MineTile(int y, int x);
		bool ReinforceTile(uint8_t owner, int y, int x);
		void DestroyTile(int y, int x);
		bool MarkTile(uint8_t player, int y, int x);
		void UnMarkTile(uint8_t player, int y, int x);
		void UpdateArea(int minY, int maxY, int minX, int maxX);
		uint16_t GetTileType(int y, int x);
		bool HasWalkableNeighbour(int y, int x, int areaCode);
		XMINT2 GetWalkableNeighbour(int y, int x, int areaCode);
		void SetRoomFloodID(int ID, int startID, uint16_t type, int y, int x);
		void CreateRoomFromTile(Room & room, int ID, int startID, uint16_t type, int y, int x);
		int CreateRoomFromArea(uint16_t type, int initialRoomID, int y, int x);
		void ClaimRoomFromEnemy(uint8_t newOwner, uint16_t type, int y, int x);
		void UpdateSurroundingRoomsAdd(uint16_t type, int y, int x);
		void UpdateSurroundingRoomsRemove(uint16_t type, int y, int x);
		Entity * GetMapEntity();
		void AdjustRoomTile(const LevelData::Room & room, const LevelData::Room::RoomTile & roomTile);
		void UpdateAreaCode(uint32_t newCode, uint16_t currentType, int ty, int tx);
		bool CollectiveClaimRoom(uint16_t type, int ty, int tx);
		void ClaimRoom(uint8_t newOwner, uint16_t type, int ty, int tx);
		bool IsClaimableCorner(int y, int x);
		bool IsOwnedRoom(uint8_t player, int y, int x);
		void ClaimTile(uint8_t player, int y, int x);
		bool BuildRoom(uint16_t type, uint8_t owner, int y, int x);
		bool DeleteRoom(uint8_t owner, int y, int x);
		static XMINT2 WorldToTile(XMFLOAT3 pos);
		static XMFLOAT3 TileToWorld(XMINT2 tPos);
		static XMFLOAT3 WorldToSubtileFloat(XMFLOAT3 pos);
		static XMINT3 WorldToSubtile(XMFLOAT3 pos);
		static XMFLOAT3 SubtileToWorld(XMINT3 pos);
		static XMINT2 TileToSubtile(XMINT2 pos);

		static std::wstring LevelIDtoString(int levelID);

		void LoadLevelFileData();
		int CreateFromTile(const Tile & tile, RenderTile & out);
		int m_CurrentLevelNr;

		std::stack<Entity*> m_MapEntityPool;
		std::vector<Entity*> m_MapEntityUsed;
		//Map that keeps the initial state of the map (for water/lava blocks)
		TileMap m_OriginalMap;
		//Map which the current changes to it (mined/dug out blocks, rooms etc..)
		static TileMap m_Map;
		static bool PathsInvalidated;
		//Map in subtile format, used for pathfinding/picking
		Block m_BlockMap[MAP_SIZE_HEIGHT][MAP_SIZE_SUBTILES_RENDER][MAP_SIZE_SUBTILES_RENDER];

		std::unordered_map<int32_t,Room> m_Rooms[6];

		std::unordered_map<Tile*,XMINT2> m_UnexploredTiles;
	};
};
