#pragma once
#include <vector>
#include <d3d11.h>
#include "ThempTileArrays.h"
#include "ThempResources.h"
namespace Themp
{
	class D3D;
	class Object3D;
	class Level; 
	struct VoxelVertex;
	class VoxelObject
	{
	public:
		~VoxelObject();
		VoxelObject(Level* level);
		void Update(float dt);
		void ConstructFromLevel(int camX, int camY);
		bool CheckWallIsCorner(int x, int y);
		void DoUVs(uint16_t type, int x, int y);
		void DoWallUVs(TileNeighbours neighbour, int type, int texIndex, int x, int y);
		uint16_t HandleSpecialCases(const Tile map[85][85], int yPos, int xPos);
		int CreateFromTile(const Tile& tile, RenderTile & out);
		bool CreateVertexBuffer(VoxelVertex * vertices, size_t numVertices);
		bool EditVertexBuffer(VoxelVertex * vertices, size_t numVertices);
		bool CreateIndexBuffer(uint32_t * indices, size_t numIndices);
		bool EditIndexBuffer(uint32_t * indices, size_t numIndices);
		Object3D* m_Obj3D = nullptr;
		float m_AnimationTime = 0;
		uint8_t m_AnimationIndex = 0;

		size_t m_NumBlocks = 0;
		VoxelVertex* m_Vertices = nullptr;
		uint32_t* m_Indices = nullptr;
		Resources::Buffer m_VertexBuffer;
		Resources::Buffer m_IndexBuffer;

		//Level to construct this VoxelObject from.
		Level* m_Level = nullptr;
	};
};
