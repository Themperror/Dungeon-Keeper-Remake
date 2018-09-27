#pragma once
#include <vector>
#include "ThempTileArrays.h"
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
		~Level();
		Level(int levelIndex);
		void Update(float delta);
		void LoadLevelData();
		Object2D* m_Cursor;
		VoxelObject* m_MapObject;
		int m_CurrentLevelNr;
		//clm_data m_Map[MAP_SIZE_SUBTILES][MAP_SIZE_SUBTILES]; //in subtiles,  includes the border tiles (playable area is 85x85 TILES)
		Tile m_Map[MAP_SIZE_TILES][MAP_SIZE_TILES];
	};
};
