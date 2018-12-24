#include "ThempSystem.h"
#include "ThempVoxelObject.h"
#include "ThempLevelData.h"
#include "ThempFileManager.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"

D3D11_INPUT_ELEMENT_DESC VoxelInputLayoutDesc[] =
{
	{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "VISIBLE", 0 ,DXGI_FORMAT_R32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
	{ "ANIMATE", 0 ,DXGI_FORMAT_R32_UINT,0,36,D3D11_INPUT_PER_VERTEX_DATA,0},
};

using namespace Themp;
VoxelObject::~VoxelObject()
{
	if(m_Vertices)
	delete[] m_Vertices;
	if(m_Indices)
	delete[] m_Indices;
	if (m_IndexBuffer.buffer)
	{
		m_IndexBuffer.buffer->Release();
	}
	if (m_VertexBuffer.buffer)
	{
		m_VertexBuffer.buffer->Release();
	}
}

VoxelObject::VoxelObject(LevelData* leveldata)
{
	m_Level = leveldata;
	m_Obj3D = new Object3D();
	m_Obj3D->SetPosition(0,0,0);


	Mesh* m = new Mesh();
	Themp::Resources::TRes->m_Meshes.push_back(m);
	m_Obj3D->m_Meshes.push_back(m);

	m_NumBlocks = 0;

	m->m_NumIndices = 6;
	m->m_NumVertices = 4;

	m_Vertices = new VoxelVertex[4]; 
	m_Indices = new uint32_t[6];

	m->m_Vertices = nullptr;
	m->m_Indices = nullptr;

	CreateVertexBuffer(m_Vertices, 4);
	CreateIndexBuffer(m_Indices, 6);

	m->m_VertexBuffer = m_VertexBuffer.buffer;
	m->m_IndexBuffer = m_IndexBuffer.buffer;
	m->m_VertexSize = sizeof(VoxelVertex);

	m->m_Material = Resources::TRes->GetUniqueMaterial("", "voxel", VoxelInputLayoutDesc,5);

	for (size_t z = 0; z < MAP_SIZE_HEIGHT; z++)
	for (size_t y = 0; y < MAP_SIZE_SUBTILES_RENDER; y++)
		for (size_t x = 0; x < MAP_SIZE_SUBTILES_RENDER; x++)
		{
			m_Level->m_BlockMap[z][y][x].active = true;
			m_Level->m_BlockMap[z][y][x].randValue = rand();
		}

}
void VoxelObject::Update(float dt)
{
	m_AnimationTime += dt;
	if (m_AnimationTime >= 1.0 / 5.0)
	{
		if (m_AnimationTime > (1.0 / 2.5)) m_AnimationTime = 1.0 / 2.5;
		m_AnimationTime -= 1.0 / 5.0;
		m_AnimationIndex++;
	}
}

const float pixelSizeX = (1.0f / 256.0f) * 31.5f;
const float pixelSizeY = (1.0f / 2176.0f) * 31.5f;

//Z = 1
void VoxelObject::DoVoxelBlockVisibleEdge(int y, int x,int yP, int xP, uint32_t& currentIndex, uint32_t& vIndex)
{
	const int tileType = m_Level->m_Map.m_Tiles[yP][xP].type;
	if (tileType == Type_Earth || IsWall(tileType))
	{
		TileNeighbours neighbour = m_Level->CheckNeighbours(tileType, yP, xP);
		uint16_t texIndex = TypeToTexture(tileType);
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].edge[2];
		int animIndex = m_AnimationIndex % tex0.size();
		if (neighbour.North == N_WATER)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[animIndex];
		}
		else if (neighbour.North == N_LAVA)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[m_Level->m_BlockMap[1][y][x].randValue & 1];
		}
		if (neighbour.South == N_WATER)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[animIndex];
		}
		else if (neighbour.South == N_LAVA)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[m_Level->m_BlockMap[1][y][x].randValue & 1];
		}
		if (neighbour.East == N_WATER)
		{
			m_Level->m_BlockMap[1][y][x].uv[0] = tex0[animIndex];
		}
		else if (neighbour.East == N_LAVA)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[m_Level->m_BlockMap[1][y][x].randValue & 1];
		}
		if (neighbour.West == N_WATER)
		{
			m_Level->m_BlockMap[1][y][x].uv[0] = tex0[animIndex];
		}
		else if (neighbour.West == N_LAVA)
		{
			m_Level->m_BlockMap[1][y][x].uv[1] = tex0[m_Level->m_BlockMap[1][y][x].randValue & 1];
		}

		XMFLOAT2 uv = m_Level->m_BlockMap[1][y][x].uv[0];
		uv.x = uv.x / 8.0;
		uv.y = uv.y / 68.0;
		if (!m_Level->m_BlockMap[1][y][x + 1].active) // right
		{
			m_Indices[currentIndex + 0] = vIndex + 2;
			m_Indices[currentIndex + 1] = vIndex + 1;
			m_Indices[currentIndex + 2] = vIndex + 0;
			m_Indices[currentIndex + 3] = vIndex + 3;
			m_Indices[currentIndex + 4] = vIndex + 2;
			m_Indices[currentIndex + 5] = vIndex + 0;
			currentIndex += 6;

			m_Vertices[vIndex++] = { x + 0.5f, 1.0f + 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y,  1,0 };
			m_Vertices[vIndex++] = { x + 0.5f, 1.0f - 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x + 0.5f, 1.0f - 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
			m_Vertices[vIndex++] = { x + 0.5f, 1.0f + 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y,  1,0 };

		}
		if (!m_Level->m_BlockMap[1][y][x - 1].active) // left
		{
			m_Indices[currentIndex + 0] = vIndex + 2;
			m_Indices[currentIndex + 1] = vIndex + 1;
			m_Indices[currentIndex + 2] = vIndex + 0;
			m_Indices[currentIndex + 3] = vIndex + 3;
			m_Indices[currentIndex + 4] = vIndex + 2;
			m_Indices[currentIndex + 5] = vIndex + 0;
			currentIndex += 6;

			m_Vertices[vIndex++] = { x - 0.5f, 1.0f + 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, 1.0f + 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y,  1,0 };
			m_Vertices[vIndex++] = { x - 0.5f, 1.0f - 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, 1.0f - 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY, 1, 0 };
		}
		uv = m_Level->m_BlockMap[1][y][x].uv[1];
		uv.x = uv.x / 8.0;
		uv.y = uv.y / 68.0;
		if (!m_Level->m_BlockMap[1][y - 1][x].active) // back
		{
			m_Indices[currentIndex + 0] = vIndex + 2;
			m_Indices[currentIndex + 1] = vIndex + 1;
			m_Indices[currentIndex + 2] = vIndex + 0;
			m_Indices[currentIndex + 3] = vIndex + 3;
			m_Indices[currentIndex + 4] = vIndex + 2;
			m_Indices[currentIndex + 5] = vIndex + 0;
			currentIndex += 6;

			m_Vertices[vIndex++] = { x + 0.5f ,  1.0f - 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x + 0.5f ,  1.0f + 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y,  1,0 };
			m_Vertices[vIndex++] = { x - 0.5f ,  1.0f + 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y,  1,0 };
			m_Vertices[vIndex++] = { x - 0.5f ,  1.0f - 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY,  1,0 };
		}
		if (!m_Level->m_BlockMap[1][y + 1][x].active) // front
		{
			m_Indices[currentIndex + 0] = vIndex + 0;
			m_Indices[currentIndex + 1] = vIndex + 1;
			m_Indices[currentIndex + 2] = vIndex + 2;
			m_Indices[currentIndex + 3] = vIndex + 0;
			m_Indices[currentIndex + 4] = vIndex + 2;
			m_Indices[currentIndex + 5] = vIndex + 3;
			currentIndex += 6;

			m_Vertices[vIndex++] = { x + 0.5f  , 1.0f - 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x + 0.5f  , 1.0f + 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f  , 1.0f + 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f  , 1.0f - 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
		}
	}
	else
	{
		assert(x <= 254);
		assert(y <= 254);
		assert(x >= 0);
		assert(y >= 0);
		if (m_Level->m_BlockMap[1][y][x].active)
		{
			XMFLOAT2 uv = m_Level->m_BlockMap[1][y][x].uv[0];
			uv.x = uv.x / 8.0;
			uv.y = uv.y / 68.0;
			if (x + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[1][y][x + 1].active) // right
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x + 0.5f, 1 + 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x + 0.5f, 1 - 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY, 1, 0 };
				m_Vertices[vIndex++] = { x + 0.5f, 1 - 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
				m_Vertices[vIndex++] = { x + 0.5f, 1 + 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y, 1, 0 };

			}
			if (x - 1 <= 0 || !m_Level->m_BlockMap[1][y][x - 1].active) // left
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x - 0.5f, 1 + 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f, 1 + 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f, 1 - 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f, 1 - 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY,  1,0 };
			}
			uv = m_Level->m_BlockMap[1][y][x].uv[1];
			uv.x = uv.x / 8.0;
			uv.y = uv.y / 68.0;
			if (y - 1 <= 0 || !m_Level->m_BlockMap[1][y - 1][x].active) // back
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x + 0.5f ,  1 - 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
				m_Vertices[vIndex++] = { x + 0.5f ,  1 + 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y,  1,0 };
				m_Vertices[vIndex++] = { x - 0.5f ,  1 + 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f ,  1 - 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
			}
			if (y + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[1][y + 1][x].active) // front
			{
				m_Indices[currentIndex + 0] = vIndex + 0;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 2;
				m_Indices[currentIndex + 3] = vIndex + 0;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 3;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x + 0.5f  , 1 - 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
				m_Vertices[vIndex++] = { x + 0.5f  , 1 + 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f  , 1 + 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f  , 1 - 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
			}
			uv = m_Level->m_BlockMap[1][y][x].uv[2];
			uv.x = uv.x / 8.0;
			uv.y = uv.y / 68.0;
			if (!m_Level->m_BlockMap[0][y][x].active) // bottom
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x - 0.5f , 1 - 0.5f ,  y + 0.5f  , 0,1,0, uv.x				,uv.y + pixelSizeY,  1,0 };
				m_Vertices[vIndex++] = { x + 0.5f , 1 - 0.5f ,  y + 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
				m_Vertices[vIndex++] = { x + 0.5f , 1 - 0.5f ,  y - 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y, 1, 0 };
				m_Vertices[vIndex++] = { x - 0.5f , 1 - 0.5f ,  y - 0.5f  , 0,1,0, uv.x				,uv.y, 1, 0 };
			}
			if (!m_Level->m_BlockMap[2][y][x].active) // top
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x + 0.5f, 1 + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y,  1,0 };
				m_Vertices[vIndex++] = { x + 0.5f, 1 + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
				m_Vertices[vIndex++] = { x - 0.5f, 1 + 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY,  1,0 };
				m_Vertices[vIndex++] = { x - 0.5f, 1 + 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y, 1, 0 };
			}
		}
	}
}
//Z = 0
void VoxelObject::DoVoxelBlockVisibleAnimated(int y, int x,uint32_t& currentIndex, uint32_t& vIndex)
{
	XMFLOAT2 uv = m_Level->m_BlockMap[0][y][x].uv[0];
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (x + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[0][y][x + 1].active) // right
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f,+0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y, 1,0 };
		m_Vertices[vIndex++] = { x + 0.5f,-0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f,-0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f,+0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y, 1, 0 };

	}
	if (x - 1 <= 0 || !m_Level->m_BlockMap[0][y][x - 1].active) // left
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x - 0.5f,+0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f,+0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y,  1,0 };
		m_Vertices[vIndex++] = { x - 0.5f,-0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f,-0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY , 1, 0 };
	}
	uv = m_Level->m_BlockMap[0][y][x].uv[1];
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (y - 1 <= 0 || !m_Level->m_BlockMap[0][y - 1][x].active) // back
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f ,-0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY ,  1,0 };
		m_Vertices[vIndex++] = { x + 0.5f ,+0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,+0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,-0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
	}
	if (y + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[0][y + 1][x].active) // front
	{
		m_Indices[currentIndex + 0] = vIndex + 0;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 2;
		m_Indices[currentIndex + 3] = vIndex + 0;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 3;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f  ,-0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
		m_Vertices[vIndex++] = { x + 0.5f  ,+0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,+0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y,  1,0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,-0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
	}
	uv = m_Level->m_BlockMap[0][y][x].uv[2];
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (!m_Level->m_BlockMap[1][y][x].active) // top
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		const int yP = floor((float)y / 3.0f);
		const int xP = floor((float)x / 3.0f);
		const int tileType = m_Level->m_Map.m_Tiles[yP][xP].type;
		if (tileType == Type_Water || tileType == Type_Lava)
		{
			uint16_t texIndex = TypeToTexture(tileType);
			const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
			int animIndex = m_AnimationIndex % tex0.size();
			uv = tex0[animIndex];
			uv.x = uv.x / 8.0;
			uv.y = uv.y / 68.0;

			bool cornerQuad = false;
			uint32_t useAnimNorth = 1;
			uint32_t useAnimWest = 1;
			uint32_t useAnimSouth = 1;
			uint32_t useAnimEast = 1;
			const int xMod = x % 3;
			const int yMod = y % 3;
			TileNeighbours neighbour = m_Level->CheckNeighbours(tileType, yP, xP);
			if (neighbour.North != N_SAME && yMod == 2)
			{
				useAnimNorth = 0;
			}
			else if (neighbour.South != N_SAME && yMod == 0)
			{
				useAnimSouth = 0;
			}
			if (neighbour.East != N_SAME && xMod == 2)
			{
				useAnimEast = 0;
			}
			else if (neighbour.West != N_SAME && xMod == 0)
			{
				useAnimWest = 0;
			}
			if (xMod == 0 && yMod == 2 && neighbour.North == N_SAME && neighbour.West == N_SAME)
			{
				if (neighbour.NorthWest != N_SAME)
				{
					useAnimNorth = 0;
					useAnimWest = 0;
					cornerQuad = true;
				}
			}
			else if (xMod == 2 && yMod == 2 && neighbour.North == N_SAME && neighbour.East == N_SAME)
			{
				if (neighbour.NorthEast != N_SAME)
				{
					useAnimNorth = 0;
					useAnimEast = 0;
					cornerQuad = true;
				}
			}
			else if (xMod == 0 && yMod == 0 && neighbour.South == N_SAME && neighbour.West == N_SAME)
			{
				if (neighbour.SouthWest != N_SAME)
				{
					useAnimSouth = 0;
					useAnimWest = 0;
					cornerQuad = true;
				}
			}
			else if (xMod == 2 && yMod == 0 && neighbour.South == N_SAME && neighbour.East == N_SAME)
			{
				if (neighbour.SouthEast != N_SAME)
				{
					useAnimSouth = 0;
					useAnimEast = 0;
					cornerQuad = true;
				}
			}

			if (cornerQuad)
			{
				m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y,  1,(useAnimEast || useAnimSouth) };
				m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1,(useAnimEast || useAnimNorth) };
				m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY, 1,(useAnimWest || useAnimNorth) };
				m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y,  1,(useAnimWest || useAnimSouth) };
			}
			else
			{
				m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y, 1, (useAnimEast && useAnimSouth) };
				m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1,(useAnimEast && useAnimNorth) };
				m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY, 1,(useAnimWest && useAnimNorth) };
				m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y, 1, (useAnimWest && useAnimSouth) };
			}
		}
		else
		{
			m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y, 1, 0 };
			m_Vertices[vIndex++] = { x + 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1,0 };
			m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY,  1,0 };
			m_Vertices[vIndex++] = { x - 0.5f, 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y, 1, 0 };
		}
	}
}
//Z = 2 to 8
void VoxelObject::DoVoxelBlockVisible(int z, int y, int x, int yP, int xP, uint32_t& currentIndex, uint32_t& vIndex)
{

	const int tileType = m_Level->m_Map.m_Tiles[yP][xP].type;

	if (tileType == Type_Gold || tileType == Type_Gem)
	{
		uint16_t texIndex = TypeToTexture(tileType);

		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[m_Level->m_BlockMap[z][y][x].randValue % BlockTextures[texIndex].top.size()];
		m_Level->m_BlockMap[z][y][x].uv[0] = tex0[m_AnimationIndex % tex0.size()];
		m_Level->m_BlockMap[z][y][x].uv[1] = tex0[m_AnimationIndex % tex0.size()];

		if (z == 5)
		{
			if (m_Level->m_Map.m_Tiles[yP][xP].marked[Owner_PlayerRed])
			{
				const std::vector<XMFLOAT2>& tex0 = BlockTextures[7].top[1];
				m_Level->m_BlockMap[5][y][x].uv[2] = tex0[m_AnimationIndex % tex0.size()];
			}
			else
			{
				const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[m_Level->m_BlockMap[1][y][x].randValue % BlockTextures[texIndex].top.size()];
				m_Level->m_BlockMap[5][y][x].uv[2] = tex0[m_AnimationIndex % tex0.size()];
			}
		}
	}

	XMFLOAT2 uv = m_Level->m_BlockMap[z][y][x].uv[0];
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (x + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[z][y][x + 1].active) // right
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y, 1, 0 };

	}
	if (x - 1 <= 0 || !m_Level->m_BlockMap[z][y][x - 1].active) // left
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y + 0.5f  , 1,0,0, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y - 0.5f  , 1,0,0, uv.x				,uv.y + pixelSizeY,  1, 0 };
	}
	uv = m_Level->m_BlockMap[z][y][x].uv[1];
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (y - 1 <= 0 || !m_Level->m_BlockMap[z][y - 1][x].active) // back
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x					,uv.y, 1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x					,uv.y + pixelSizeY,  1, 0 };
	}
	if (y + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Level->m_BlockMap[z][y + 1][x].active) // front
	{
		m_Indices[currentIndex + 0] = vIndex + 0;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 2;
		m_Indices[currentIndex + 3] = vIndex + 0;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 3;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY,  1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY, 1, 0 };
	}

	uv = m_Level->m_BlockMap[z][y][x].uv[2];
	if (tileType != Type_Gold && tileType != Type_Gem && m_Level->m_Map.m_Tiles[yP][xP].marked[Owner_PlayerRed])
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[7].top[0];
		uv = tex0[m_AnimationIndex % tex0.size()];
	}
	uv.x = uv.x / 8.0;
	uv.y = uv.y / 68.0;
	if (!m_Level->m_BlockMap[z - 1][y][x].active) // bottom
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x				,uv.y + pixelSizeY,  1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x + pixelSizeX,uv.y + pixelSizeY, 1, 0 };
		m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x + pixelSizeX,uv.y,  1, 0 };
		m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x				,uv.y,  1, 0 };
	}
	if (z + 1 >= MAP_SIZE_HEIGHT || !m_Level->m_BlockMap[z + 1][y][x].active) //top
	{
		
		bool visible[4] = {true,true,true,true};
		
		if (z + 1 == 6)
		{
			bool isCorner = false;
			bool edge[4] = {};
			if ((x + 1) / 3 == xP + 1) //neighbours the east tile;
			{
				visible[0] = m_Level->m_Map.m_Tiles[yP][xP + 1].visible;
				edge[0] = true;
			}
			else if ((x - 1) / 3 == xP - 1) //neighbours the west tile;
			{
				visible[1] = m_Level->m_Map.m_Tiles[yP][xP - 1].visible;
				edge[1] = true;
			}
			if ((y + 1) / 3 == yP + 1) //neighbours the north tile;
			{
				visible[2] = m_Level->m_Map.m_Tiles[yP + 1][xP].visible;
				edge[2] = true;
			}
			else if ((y - 1) / 3 == yP - 1) //neighbours the south tile;
			{
				visible[3] = m_Level->m_Map.m_Tiles[yP - 1][xP].visible;
				edge[3] = true;
			}
			//fix diagonal cases
			if (edge[1] && edge[2] && (visible[1] && visible[2]))
			{
				if (!m_Level->m_Map.m_Tiles[yP + 1][xP - 1].visible)
				{
					visible[0] = false;
				}
				isCorner = true;
				
			}
			else if (edge[2] && edge[0] && (visible[2] && visible[0]))
			{
				if (!m_Level->m_Map.m_Tiles[yP + 1][xP + 1].visible)
				{
					visible[1] = false;
				}
				isCorner = true;
			}
			else if (edge[1] && edge[3] && (visible[1] && visible[3]))
			{
				if (!m_Level->m_Map.m_Tiles[yP - 1][xP - 1].visible)
				{
					visible[2] = false;
				}
				isCorner = true;
			}
			else if (edge[0] && edge[3] && (visible[0] && visible[3]))
			{
				if (!m_Level->m_Map.m_Tiles[yP - 1][xP + 1].visible)
				{
					visible[3] = false;
				}
				isCorner = true;
			}
			if (m_Level->m_Map.m_Tiles[yP][xP].marked[Owner_PlayerRed])
			{
				visible[0] = true;
				visible[1] = true;
				visible[2] = true;
				visible[3] = true;
			}
			if (isCorner)
			{
				m_Indices[currentIndex + 0] = vIndex + 2;
				m_Indices[currentIndex + 1] = vIndex + 1;
				m_Indices[currentIndex + 2] = vIndex + 0;
				m_Indices[currentIndex + 3] = vIndex + 3;
				m_Indices[currentIndex + 4] = vIndex + 2;
				m_Indices[currentIndex + 5] = vIndex + 0;
				currentIndex += 6;

				m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y,				(float)(visible[3]) , 0 };
				m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, (float)(visible[1]) , 0 };
				m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x				,uv.y + pixelSizeY, (float)(visible[0]) , 0 };
				m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x				,uv.y,				(float)(visible[2]) , 0 };
			}
			else
			{

				if ((!visible[0] && !visible[2]) || (!visible[1] && !visible[3]))
				{
					m_Indices[currentIndex + 0] = vIndex + 0;
					m_Indices[currentIndex + 1] = vIndex + 2;
					m_Indices[currentIndex + 2] = vIndex + 1;
					m_Indices[currentIndex + 3] = vIndex + 0;
					m_Indices[currentIndex + 4] = vIndex + 3;
					m_Indices[currentIndex + 5] = vIndex + 2;
					currentIndex += 6;
					m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x				,uv.y,				(float)(visible[1] && visible[3]) , 0 };
					m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y,				(float)(visible[0] && visible[3]) , 0 };
					m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, (float)(visible[0] && visible[2]) , 0 };
					m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x				,uv.y + pixelSizeY, (float)(visible[1] && visible[2]) , 0 };
				}
				else
				{
					m_Indices[currentIndex + 0] = vIndex + 2;
					m_Indices[currentIndex + 1] = vIndex + 1;
					m_Indices[currentIndex + 2] = vIndex + 0;
					m_Indices[currentIndex + 3] = vIndex + 3;
					m_Indices[currentIndex + 4] = vIndex + 2;
					m_Indices[currentIndex + 5] = vIndex + 0;
					currentIndex += 6;
					m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y,				(float)(visible[0] && visible[3]) , 0 };
					m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, (float)(visible[0] && visible[2]) , 0 };
					m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x				,uv.y + pixelSizeY, (float)(visible[1] && visible[2]) , 0 };
					m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x				,uv.y,				(float)(visible[1] && visible[3]) , 0 };
				}
			}
		}
		else
		{
			m_Indices[currentIndex + 0] = vIndex + 2;
			m_Indices[currentIndex + 1] = vIndex + 1;
			m_Indices[currentIndex + 2] = vIndex + 0;
			m_Indices[currentIndex + 3] = vIndex + 3;
			m_Indices[currentIndex + 4] = vIndex + 2;
			m_Indices[currentIndex + 5] = vIndex + 0;
			currentIndex += 6;

			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y, 1 , 0 };
			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x				,uv.y + pixelSizeY, 1 , 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x				,uv.y, 1 , 0 };
		}
	}
	
}
//All Z's 
void VoxelObject::DoVoxelBlockInvisible(int z, int y, int x, int yP,int xP, uint32_t& currentIndex, uint32_t& vIndex)
{
	if (z + 1 > 6) return;

	const XMFLOAT2 uv = XMFLOAT2(0, 0);
	if (((x+1) / 3) == (xP+1) && m_Level->m_Map.m_Tiles[yP][xP+1].visible) // right
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y - 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y - 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y + 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y + 0.5f  , 1,0,0, 0,0,0, 0 };

	}
	if (((x - 1) / 3) == (xP - 1) && m_Level->m_Map.m_Tiles[yP][xP - 1].visible) // left
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y - 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y + 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y + 0.5f  , 1,0,0, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y - 0.5f  , 1,0,0, 0,0,0, 0 };
	}
	if (((y - 1) / 3) == (yP - 1) && m_Level->m_Map.m_Tiles[yP - 1][xP].visible) // back
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x + 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, 0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, 0,0,0, 0 };
	}
	if (((y + 1) / 3) == (yP + 1) && m_Level->m_Map.m_Tiles[yP + 1][xP].visible) // front
	{
		m_Indices[currentIndex + 0] = vIndex + 0;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 2;
		m_Indices[currentIndex + 3] = vIndex + 0;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 3;
		currentIndex += 6;

		m_Vertices[vIndex++] = { x + 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1,  0,0,0, 0 };
		m_Vertices[vIndex++] = { x + 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1,  0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1,  0,0,0, 0 };
		m_Vertices[vIndex++] = { x - 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1,  0,0,0, 0 };
	}

	if (z + 1 == 6) // top
	{
		m_Indices[currentIndex + 0] = vIndex + 2;
		m_Indices[currentIndex + 1] = vIndex + 1;
		m_Indices[currentIndex + 2] = vIndex + 0;
		m_Indices[currentIndex + 3] = vIndex + 3;
		m_Indices[currentIndex + 4] = vIndex + 2;
		m_Indices[currentIndex + 5] = vIndex + 0;
		currentIndex += 6;

		if (m_Level->m_Map.m_Tiles[yP][xP].marked[Owner_PlayerRed])
		{
			const std::vector<XMFLOAT2>& tex0 = BlockTextures[7].top[0];
			XMFLOAT2 uv = tex0[m_AnimationIndex % tex0.size()];
			uv.x = uv.x / 8.0;
			uv.y = uv.y / 68.0;
			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y, 1 , 0 };
			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY, 1, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x				,uv.y + pixelSizeY, 1 , 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x				,uv.y, 1 , 0 };
		}
		else
		{
			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, 0,0,0, 0 };
			m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, 0,0,0, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, 0,0,0, 0 };
			m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, 0,0,0, 0 };
		}
	}
}

void VoxelObject::ConstructFromLevel(int camX,int camY)
{
	const int tileRadius = 8;
	const int subTileRadius = tileRadius * 3;

	XMINT2 camTilePos = LevelData::WorldToTile(XMFLOAT3(camX, 2, camY));

	int camSubTilePosX = camTilePos.x * 3;
	int camSubTilePosY = camTilePos.y * 3;


	//make sure we're within bounds of a of the 85x85 map with a 16x16 tile area
	//This is working in subtiles
	if (camTilePos.x - tileRadius < 0)
	{
		camTilePos.x = tileRadius;
		camSubTilePosX = subTileRadius;
	}
	else if (camTilePos.x + tileRadius >= MAP_SIZE_TILES)
	{
		camTilePos.x = MAP_SIZE_TILES - tileRadius - 1;
		camSubTilePosX = MAP_SIZE_SUBTILES_RENDER - subTileRadius - 1;
	}
	if (camTilePos.y - tileRadius < 0)
	{
		camTilePos.y = tileRadius;
		camSubTilePosY = subTileRadius;
	}
	else if (camTilePos.y + tileRadius >= MAP_SIZE_TILES)
	{
		camTilePos.y = MAP_SIZE_TILES - tileRadius - 1;
		camSubTilePosY = MAP_SIZE_SUBTILES_RENDER - subTileRadius - 1;
	}
	size_t solidBlocks = 0;

	assert(camTilePos.x + 8 <= 84);
	assert(camTilePos.y + 8 <= 84);
	assert(camTilePos.y - 8 >= 0);
	assert(camTilePos.x - 8 >= 0);
	for (int y = camTilePos.y -8; y < camTilePos.y +8; y++)
	{
		for (int x = camTilePos.x -8; x < camTilePos.x+8; x++)
		{
			solidBlocks += m_Level->m_Map.m_Tiles[y][x].numBlocks;
		}
	}
	//We'll check if we have to resize our buffers..
	if (solidBlocks > m_NumBlocks)
	{
		delete[] m_Vertices;
		delete[] m_Indices;

		m_NumBlocks = solidBlocks;
		m_Vertices = new VoxelVertex[solidBlocks * 6 * 4]; //worse case scenario
		m_Indices = new uint32_t[solidBlocks * 6 * 6];
	}

	uint32_t vIndex = 0;
	uint32_t currentIndex = 0; 

	//Z == 0 //For water and Lava animation (vertex displacement)
	for (int y = camSubTilePosY - subTileRadius; y < camSubTilePosY + subTileRadius; y++)
	{
		const int yTile = y / 3;
		for (int x = camSubTilePosX - subTileRadius; x < camSubTilePosX + subTileRadius; x++)
		{
			const int xTile = x / 3;
			assert(x <= 254);
			assert(y <= 254);
			assert(x >= 0);
			assert(y >= 0);
			if (m_Level->m_Map.m_Tiles[yTile][xTile].visible)
			{
				if (!m_Level->m_BlockMap[0][y][x].active) continue;
				DoVoxelBlockVisibleAnimated(y, x,currentIndex, vIndex);
			}
			else
			{
				DoVoxelBlockInvisible(0, y, x,yTile,xTile, currentIndex, vIndex);
			}
		}
	}
	//Z == 1
	for (int y = camSubTilePosY - subTileRadius; y < camSubTilePosY + subTileRadius; y++)
	{
		const int yP = y / 3;
		for (int x = camSubTilePosX - subTileRadius; x < camSubTilePosX + subTileRadius; x++)
		{
			const int xP = x / 3;
			if (m_Level->m_Map.m_Tiles[yP][xP].visible)
			{
				if (!m_Level->m_BlockMap[1][y][x].active) continue;
				DoVoxelBlockVisibleEdge(y, x,yP,xP, currentIndex, vIndex);
			}
			else
			{
				DoVoxelBlockInvisible(1, y, x,yP,xP, currentIndex, vIndex);
			}
		}
	}
	for (int z = 2; z < MAP_SIZE_HEIGHT; z++)
	{
		for (int y = camSubTilePosY - subTileRadius; y < camSubTilePosY + subTileRadius; y++)
		{
			const int yP = y / 3;
			for (int x = camSubTilePosX - subTileRadius; x < camSubTilePosX + subTileRadius; x++)
			{
				assert(x <= 254);
				assert(y <= 254);
				assert(x >= 0);
				assert(y >= 0);

				const int xP = x / 3;
				if (m_Level->m_Map.m_Tiles[yP][xP].visible)
				{
					if (!m_Level->m_BlockMap[z][y][x].active) continue;
					DoVoxelBlockVisible(z, y, x, yP, xP, currentIndex, vIndex);
				}
				else
				{
					DoVoxelBlockInvisible(z, y, x,yP,xP, currentIndex, vIndex);
				}
			}
		}
	}
	

	Mesh* m = m_Obj3D->m_Meshes[0];
	assert(EditVertexBuffer(m_Vertices, vIndex));
	assert(EditIndexBuffer(m_Indices, currentIndex));
	m->m_NumIndices = currentIndex;
	m->m_NumVertices = vIndex;
	m->m_VertexBuffer = m_VertexBuffer.buffer;
	m->m_IndexBuffer = m_IndexBuffer.buffer;

	if ((solidBlocks * 6 * 4) * sizeof(Themp::VoxelVertex) < vIndex * sizeof(Themp::VoxelVertex))
	{
		System::Print("This is wrong.. Very wrong...");
		assert(false);
	}
	if ((solidBlocks * 6 * 6) * sizeof(uint32_t) < currentIndex * sizeof(uint32_t))
	{
		System::Print("This is wrong.. Very wrong...");
		assert(false);
	}
}


bool VoxelObject::CheckWallIsCorner(int x, int y)
{
	bool north = m_Level->m_Map.m_Tiles[y + 1][x].type != Type_Wall0 && m_Level->m_Map.m_Tiles[y + 1][x].type != Type_Wall3;
	bool south = m_Level->m_Map.m_Tiles[y - 1][x].type != Type_Wall0 && m_Level->m_Map.m_Tiles[y - 1][x].type != Type_Wall3;
	bool east =  m_Level->m_Map.m_Tiles[y][x + 1].type != Type_Wall0 && m_Level->m_Map.m_Tiles[y][x + 1].type != Type_Wall3;
	bool west =  m_Level->m_Map.m_Tiles[y][x - 1].type != Type_Wall0 && m_Level->m_Map.m_Tiles[y][x - 1].type != Type_Wall3;
	int numSides = (uint8_t)north + (uint8_t)south + (uint8_t)east + (uint8_t)west;

	if (north && west || north && east || south && west || south && east)
	{
		return true;
	}
	return false;
}


bool VoxelObject::CreateVertexBuffer(VoxelVertex* vertices, size_t numVertices)
{
	Themp::D3D* d = Themp::System::tSys->m_D3D;
	D3D11_BUFFER_DESC bd;
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//set up for vertices
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VoxelVertex) * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer* vertexBuffer;
	Resources::Buffer buf;
	HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &vertexBuffer);
	if (res == S_OK)
	{
		buf.buffer = vertexBuffer;
		buf.numElements = numVertices;
		d->m_DevCon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(VoxelVertex)*numVertices);
		d->m_DevCon->Unmap(vertexBuffer, NULL);
		m_VertexBuffer = buf;
		return true;
	}
	System::Print("Could not create vertex buffer!");
	return false;
}
bool VoxelObject::EditVertexBuffer(VoxelVertex* vertices, size_t numVertices)
{
	Themp::D3D* d = Themp::System::tSys->m_D3D;
	Resources::Buffer& buf = m_VertexBuffer;
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	if (buf.numElements >= numVertices) //re-use our buffer
	{
		if (d->m_DevCon->Map(buf.buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
		{
			memcpy(ms.pData, vertices, sizeof(VoxelVertex)*numVertices);
			d->m_DevCon->Unmap(buf.buffer, NULL);
			return true;
		}
	}
	else
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		//set up for vertices
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VoxelVertex) * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* vBuffer;
		HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &vBuffer);
		if (res == S_OK)
		{
			if (d->m_DevCon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
			{
				memcpy(ms.pData, vertices, sizeof(VoxelVertex)*numVertices);
				d->m_DevCon->Unmap(vBuffer, NULL);
				buf.buffer->Release(); //release old buffer
				buf.buffer = vBuffer;
				buf.numElements = numVertices;
				return true;
			}
		}
	}
	return false;
}
bool VoxelObject::CreateIndexBuffer(uint32_t* indices, size_t numIndices)
{
	Themp::D3D* d = Themp::System::tSys->m_D3D;
	HRESULT res;

	D3D11_BUFFER_DESC bd;
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//set up for indices
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(uint32_t) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ID3D11Buffer* indexBuffer;
	Resources::Buffer buf;
	res = d->m_Device->CreateBuffer(&bd, NULL, &indexBuffer);
	if (res == S_OK)
	{
		buf.buffer = indexBuffer;
		buf.numElements = numIndices;
		d->m_DevCon->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
		d->m_DevCon->Unmap(indexBuffer, NULL);
		m_IndexBuffer = buf;
		return true;
	}
	System::Print("Could not create index buffer!");
	return false;
}
bool VoxelObject::EditIndexBuffer(uint32_t* indices, size_t numIndices)
{
	Themp::D3D* d = Themp::System::tSys->m_D3D;
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Resources::Buffer& buf = m_IndexBuffer;
	if (buf.numElements >= numIndices) //re-use our buffer
	{
		if (d->m_DevCon->Map(buf.buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
		{
			memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
			d->m_DevCon->Unmap(buf.buffer, NULL);
			return true;
		}
	}
	else
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

		//set up for vertices
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(uint32_t) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* iBuffer;
		HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &iBuffer);
		if (res == S_OK)
		{
			if (d->m_DevCon->Map(iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
			{
				memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
				d->m_DevCon->Unmap(iBuffer, NULL);
				buf.buffer->Release(); //release old buffer
				buf.buffer = iBuffer;
				buf.numElements = numIndices;
				return true;
			}
		}
	}
	return false;
}