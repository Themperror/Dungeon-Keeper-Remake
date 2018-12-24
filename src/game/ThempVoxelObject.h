#pragma once
#include <vector>
#include <d3d11.h>
#include "ThempTileArrays.h"
#include "ThempResources.h"
namespace Themp
{
	class D3D;
	class Object3D;
	class LevelData; 
	struct VoxelVertex;
	class VoxelObject
	{
	public:
		~VoxelObject();
		VoxelObject(LevelData* level);
		void Update(float dt);
		void DoVoxelBlockVisibleEdge(int y, int x, int yP, int xP, uint32_t & currentIndex, uint32_t & vIndex);
		void DoVoxelBlockVisibleAnimated(int y, int x, uint32_t & currentIndex, uint32_t & vIndex);
		void DoVoxelBlockVisible(int z, int y, int x, int yP, int xP, uint32_t & currentIndex, uint32_t & vIndex);
		void DoVoxelBlockInvisible(int z, int y, int x, int yP, int xP, uint32_t & currentIndex, uint32_t & vIndex);
		void ConstructFromLevel(int camX, int camY);
		bool CheckWallIsCorner(int x, int y);
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
		LevelData* m_Level = nullptr;
	};
};
