#pragma once
#include <vector>
#include <d3d11.h>
#include "ThempTileArrays.h"
namespace Themp
{
	class D3D;
	class Object3D;
	class Level;
	struct Vertex;
	class VoxelObject
	{
	public:
		~VoxelObject();
		VoxelObject();
		void Update(float dt);
		void ConstructFromLevel(Level* level, int camX, int camY);
		void DoUVs(Level * level, uint16_t type, int x, int y);
		uint16_t HandleSpecialCases(const Tile map[85][85], int yPos, int xPos);
		int CreateFromTile(const Tile& tile, RenderTile & out);
		Object3D* m_Obj3D = nullptr;
		float m_AnimationTime = 0;
		uint8_t m_AnimationIndex = 0;

		Block m_Map[MAP_SIZE_HEIGHT][MAP_SIZE_SUBTILES_RENDER][MAP_SIZE_SUBTILES_RENDER];
		size_t m_NumBlocks = 0;
		Vertex* m_Vertices = nullptr;
		uint32_t* m_Indices = nullptr;
	};
};
