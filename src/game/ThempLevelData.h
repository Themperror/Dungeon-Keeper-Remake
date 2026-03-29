#pragma once
#include <vector>
#include <unordered_map>
#include <stack>
#include "ThempTileArrays.h"
#include "ThempFileManager.h"
namespace Themp
{
	class LevelData
	{
	public:

		struct ActionPoint
		{
			uint8_t sx, tx, sy, ty;
			uint8_t rangeX, rangeY;
			uint16_t ID;
		};
		struct Thing
		{
			//0 - 5: Location
			//6 : Thing type
			//7 : Thing subtype(eg creature race, trap type)
			//8 : Owner
			//9 - 20 : Type specific data

			uint8_t sx, tx, sy, ty, sz, tz;
			uint8_t type;
			uint8_t subType;
			uint8_t owner;
			union
			{
				char typeSpecificData[11];
#pragma pack(push,1)
				struct
				{
					uint8_t field1;
					uint8_t field2;
					uint16_t field3_4;
					uint16_t field5_6;
					uint16_t field7_8;
					uint16_t field9_10;
					uint8_t field11;
				};
#pragma pack(pop)
			};
		};
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
				auto it = rhs.tiles.cbegin();
				while (it != rhs.tiles.cend())
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
		struct Dimension
		{
			int x;
			int y;
			int w;
			int h;
		};


		~LevelData();
		LevelData(int levelIndex);
		void Init();
		LevelData::HitData Raycast(XMFLOAT3 origin, XMFLOAT3 direction, float range, bool tileMode = false);
		uint8_t GetNeighbourInfo(TileType currentType, TileType nType);
		TileNeighbours CheckNeighbours(TileType type, int y, int x);
		TileNeighbourTiles GetNeighbourTiles(int y, int x);
		NeighbourSubTiles GetNeighbourSubTiles(int y, int x);
		uint8_t GetSubtileHeight(int tileY, int tileX, int subTileY, int subTileX);
		void DoUVs(TileType type, int x, int y);
		uint32_t GetFreeLightIndex(uint16_t base);
		void AddLightToTiles(const Light& light);
		void DoRoomUVs(const TileNeighbours& neighbour, TileType type, int texIndex, int x, int y);
		void DoWallUVs(const TileNeighbours& neighbour, TileType type, int texIndex, int x, int y);
		void AddExploredTileNeighboursVisibility(int y, int x, int areaCode);
		TileTypeAndVariant Handle3by3Rooms(int yPos, int xPos);
		TileTypeAndVariant HandleNon3by3RoomsPillars(int yPos, int xPos);
		void UpdateWalls(int y, int x);
		bool MineTile(int y, int x);
		bool ReinforceTile(PlayerID owner, int y, int x);
		void DestroyTile(int y, int x);
		bool MarkTile(PlayerID player, int y, int x);
		void UnMarkTile(PlayerID player, int y, int x);
		void UpdateArea(int minY, int maxY, int minX, int maxX);
		TileType GetTileType(int y, int x);
		bool IsSolidFloor(TileType type);
		bool HasWalkableNeighbour(int y, int x, int areaCode);
		XMINT2 GetWalkableNeighbour(int y, int x, int areaCode);
		void SetRoomFloodID(int ID, int startID, uint16_t type, int y, int x);
		void CreateRoomFromTile(Room & room, int ID, int startID, uint16_t type, int y, int x);
		int CreateRoomFromArea(uint16_t type, int initialRoomID, int y, int x);
		void ClaimRoomFromEnemy(PlayerID newOwner, TileType type, int y, int x);
		void UpdateSurroundingRoomsAdd(TileType type, int y, int x);
		void UpdateSurroundingRoomsRemove(TileType type, int y, int x);
		Entity * GetMapEntity();
		void AdjustRoomTile(const LevelData::Room & room, const LevelData::Room::RoomTile & roomTile);
		void UpdateAreaCode(uint32_t newCode, int ty, int tx);
		bool CollectiveClaimRoom(TileType type, int ty, int tx);
		void ClaimRoom(PlayerID newOwner, TileType type, int ty, int tx);
		bool IsClaimableCorner(int y, int x);
		bool IsOwnedRoom(PlayerID player, int y, int x);
		void ClaimTile(PlayerID player, int y, int x);
		bool BuildRoom(TileType type, PlayerID owner, int y, int x);
		bool DeleteRoom(PlayerID owner, int y, int x);
		Dimension GetRoomDimensionForTile(int x, int y, bool ownedOnly);
		static XMINT2 WorldToTile(XMFLOAT3 pos);
		static XMFLOAT3 TileToWorld(XMINT2 tPos);
		static XMFLOAT3 WorldToSubtileFloat(XMFLOAT3 pos);
		static XMINT3 WorldToSubtile(XMFLOAT3 pos);
		static XMFLOAT3 SubtileToWorld(XMINT3 pos);
		static XMINT2 TileToSubtile(XMINT2 pos);

		static std::wstring LevelIDtoString(int levelID);

		void LoadLevelFileData();
		int CreateFromTile(const Tile & tile, RenderTile & out);
		uint8_t m_MapBlockTextureID = 0;
		int m_CurrentLevelNr = 0;

		std::stack<Entity*> m_MapEntityPool;
		std::vector<Entity*> m_MapEntityUsed;
		//Map that keeps the initial state of the map (for water/lava blocks)
		TileMap m_OriginalMap{};

		//Map which the current changes to it (mined/dug out blocks, rooms etc..)
		static TileMap s_Map;
		static bool PathsInvalidated;
		static std::array<Light, MAP_SIZE_TILES*MAP_SIZE_TILES * MAX_LIGHTS_PER_TILE> s_PerTileLights;
		//Map in subtile format, used for pathfinding/picking
		Block m_BlockMap[MAP_SIZE_HEIGHT][MAP_SIZE_SUBTILES_RENDER][MAP_SIZE_SUBTILES_RENDER];
		std::vector<ActionPoint> m_ActionPoints;
		std::vector<Thing> m_HeroGates;
		std::vector<Thing> m_LevelThings;
		std::unordered_map<int32_t,Room> m_Rooms[6];

		std::unordered_map<Tile*, DirectX::XMINT2> m_UnexploredTiles;
	};
};
