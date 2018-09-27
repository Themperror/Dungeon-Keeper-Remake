#pragma once
#include <vector>
#include <d3d11.h>
#include "ThempTileArrays.h"
namespace Themp
{
	class D3D;
	class Object3D;
	class Level;
	class VoxelObject
	{
	public:
		~VoxelObject();
		VoxelObject();
		void Cull(int camX, int camY);
		void DoUVs(uint16_t type, size_t x, size_t y);
		uint16_t HandleSpecialCases(const Tile map[85][85], int yPos, int xPos);
		int CreateFromTile(const Tile& tile, RenderTile & out);
		void ConstructFromLevel(Level* level);
		Object3D* m_Obj3D = nullptr;
		uint8_t m_AnimationIndex = 0;
		Block map[MAP_SIZE_HEIGHT][MAP_SIZE_SUBTILES][MAP_SIZE_SUBTILES]; //including the base block;
	};
};
