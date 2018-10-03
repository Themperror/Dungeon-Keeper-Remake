#include "ThempSystem.h"
#include "ThempVoxelObject.h"
#include "ThempLevel.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include <DirectXMath.h>


using namespace Themp;
#define L8(x) ((x)&0xFF)
VoxelObject::~VoxelObject()
{
	if(m_Vertices)
	delete[] m_Vertices;
	if(m_Indices)
	delete[] m_Indices;
}

VoxelObject::VoxelObject()
{
	m_Obj3D = new Object3D();
	m_Obj3D->SetPosition(-((MAP_SIZE_TILES*3) / 2), 0,-((MAP_SIZE_TILES*3) / 2));


	Mesh* m = new Mesh();
	Themp::Resources::TRes->m_Meshes.push_back(m);
	m_Obj3D->m_Meshes.push_back(m);

	m_NumBlocks = 0;

	m->m_NumIndices = 6;
	m->m_NumVertices = 4;
	m_Vertices = new Vertex[4]; //worse case scenario
	m_Indices = new uint32_t[6];

	m->m_Vertices = m_Vertices;
	m->m_Indices = m_Indices;
	m->ConstructVertexBuffer(); //requires m_m_Vertices/m_m_Indices to be set

	//unset it so it doesnt "delete[]" our vertices/indices, so we can take care of this ourselves
	m->m_Vertices = nullptr;
	m->m_Indices = nullptr;

	m->m_Material = Resources::TRes->GetUniqueMaterial("", "default");

	for (size_t z = 0; z < MAP_SIZE_HEIGHT; z++)
	for (size_t y = 0; y < MAP_SIZE_SUBTILES_RENDER; y++)
		for (size_t x = 0; x < MAP_SIZE_SUBTILES_RENDER; x++)
		{
			m_Map[z][y][x].active = true;
			m_Map[z][y][x].randValue = rand();
		}
	//for (size_t z = 0; z < MAP_SIZE_HEIGHT; z++)
	//{
	//	for (size_t y = 0; y < MAP_SIZE_SUBTILES; y++)
	//	{
	//		for (size_t x = 0; x < MAP_SIZE_SUBTILES; x++)
	//		{
	//			m_Map[z][y][x].active = true;
	//		}
	//	}
	//}

}
void VoxelObject::Update(float dt)
{
	m_AnimationTime += dt;
	if (m_AnimationTime >= 1.0 / 5.0)
	{
		m_AnimationTime -= 1.0 / 5.0;
		m_AnimationIndex++;
	}
}
void VoxelObject::ConstructFromLevel(Level* level, int camX,int camY)
{
	const int tileRadius = 8;
	const int subTileRadius = tileRadius * 3;

	int camTilePosX = (camX + (MAP_SIZE_SUBTILES_RENDER / 2)) / 3;
	int camTilePosY = (camY + (MAP_SIZE_SUBTILES_RENDER / 2)) / 3;

	int camSubTilePosX = camTilePosX * 3;
	int camSubTilePosY = camTilePosY * 3;


	//make sure we're within bounds of a of the 85x85 map with a 16x16 tile area
	//This is working in subtiles
	if (camTilePosX - tileRadius < 0)
	{
		camTilePosX = tileRadius;
		camSubTilePosX = subTileRadius;
	}
	else if (camTilePosX + tileRadius > MAP_SIZE_TILES)
	{
		camTilePosX = MAP_SIZE_TILES - tileRadius;
		camSubTilePosX = MAP_SIZE_SUBTILES_RENDER - subTileRadius;
	}
	if (camTilePosY - tileRadius < 0)
	{
		camTilePosY = tileRadius;
		camSubTilePosY = subTileRadius;
	}
	else if (camTilePosY + tileRadius > MAP_SIZE_TILES)
	{
		camTilePosY = MAP_SIZE_TILES - tileRadius;
		camSubTilePosY = MAP_SIZE_SUBTILES_RENDER - subTileRadius;
	}
	size_t solidBlocks = 0;

	assert(camTilePosX + 8 <= 85);
	assert(camTilePosY + 8 <= 85);
	assert(camTilePosY - 8 >= 0);
	assert(camTilePosX - 8 >= 0);
	for (int y = camTilePosY-8; y < camTilePosY+8; y++)
	{
		for (int x = camTilePosX-8; x < camTilePosX+8; x++)
		{
			Tile& tile = level->m_Map[y][x];
			RenderTile tileOut = { };

			tile.type = HandleSpecialCases(level->m_Map, y, x);
			size_t numBlocks = CreateFromTile(tile, tileOut);
			solidBlocks += numBlocks;

			for (size_t yy = 0; yy < 3; yy++)
			{
				for (size_t xx = 0; xx < 3; xx++)
				{
					for (size_t z = 0; z < 8; z++)
					{
						assert(y + yy < 256);
						assert(x + xx < 256);
						Block& b = tileOut.subTile[yy][xx][z];
						b.randValue = m_Map[z][y*3 + yy][x*3 + xx].randValue;
						m_Map[z][y*3 + yy][x*3 + xx] = b;
					}
				}
			}
			DoUVs(level, tile.type, x, y);
		}
	}
	//We'll check if we have to resize our buffers..
	if (solidBlocks > m_NumBlocks)
	{
		delete[] m_Vertices;
		delete[] m_Indices;

		m_NumBlocks = solidBlocks;
		m_Vertices = new Vertex[solidBlocks * 6 * 4]; //worse case scenario
		m_Indices = new uint32_t[solidBlocks * 6 * 6];
	}

	uint32_t vIndex = 0;
	uint32_t currentIndex = 0;
	const float pixelSizeX = (1.0f / 256.0f) * 31.5f;
	const float pixelSizeY = (1.0f / 2176.0f) * 31.5f;

	for (int z = 0; z < MAP_SIZE_HEIGHT; z++)
	{
		for (int y = camSubTilePosY - subTileRadius; y <  camSubTilePosY + subTileRadius; y++)
		{
			for (int x = camSubTilePosX - subTileRadius; x < camSubTilePosX + subTileRadius; x++)
			{
				assert(x <= 255);
				assert(y <= 255);
				assert(x >= 0);
				assert(y >= 0);
				if (m_Map[z][y][x].active)
				{
					XMFLOAT2 uv = m_Map[z][y][x].uv[0];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (x + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Map[z][y][x + 1].active) // right
					{
						m_Indices[currentIndex + 0] = vIndex + 2;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 0;
						m_Indices[currentIndex + 3] = vIndex + 3;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 0;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y };

					}
					if (x - 1 <= 0 || !m_Map[z][y][x - 1].active) // left
					{
						m_Indices[currentIndex + 0] = vIndex + 2;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 0;
						m_Indices[currentIndex + 3] = vIndex + 3;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 0;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY };
					}
					uv = m_Map[z][y][x].uv[1];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (y - 1 <= 0 || !m_Map[z][y - 1][x].active) // back
					{
						m_Indices[currentIndex + 0] = vIndex + 2;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 0;
						m_Indices[currentIndex + 3] = vIndex + 3;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 0;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x + 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY };
					}
					if (y + 1 >= MAP_SIZE_SUBTILES_RENDER || !m_Map[z][y + 1][x].active) // front
					{
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x + 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY };
					}
					uv = m_Map[z][y][x].uv[2];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (z - 1 < 0 || !m_Map[z - 1][y][x].active) // bottom
					{
						m_Indices[currentIndex + 0] = vIndex + 2;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 0;
						m_Indices[currentIndex + 3] = vIndex + 3;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 0;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x				,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x				,uv.y };
					}
					if (z + 1 >= MAP_SIZE_HEIGHT || !m_Map[z + 1][y][x].active) // top
					{
						m_Indices[currentIndex + 0] = vIndex + 2;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 0;
						m_Indices[currentIndex + 3] = vIndex + 3;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 0;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y };
					}
				}
			}
		}
	}


	Mesh* m = m_Obj3D->m_Meshes[0];
	assert(Resources::TRes->EditVertexBuffer(m->i_VertexBuffer, m_Vertices, vIndex));
	assert(Resources::TRes->EditIndexBuffer(m->i_IndexBuffer, m_Indices, currentIndex));
	m->m_NumIndices = currentIndex;
	m->m_NumVertices = vIndex;
	m->m_VertexBuffer = Resources::TRes->m_VertexBuffers[m->i_VertexBuffer].buffer;
	m->m_IndexBuffer = Resources::TRes->m_IndexBuffers[m->i_IndexBuffer].buffer;

	if ((solidBlocks * 6 * 4) * sizeof(Themp::Vertex) < vIndex * sizeof(Themp::Vertex))
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
XMFLOAT2 GetRandomTexture(std::vector<XMFLOAT2>& textures, uint8_t index)
{
	return XMFLOAT2(0, 0);
}
uint16_t TypeToTexture(uint16_t type)
{
	switch (type)
	{
	case Type_Earth: return 1;
		break;
	case Type_Water: return 2;
		break;
	case Type_Gold: return 3;
		break;
	case Type_Wall0:
	case Type_Wall1:
	case Type_Wall2:
	case Type_Wall3:
	case Type_Wall4:
	case Type_Wall5:
		return 6;
		break;
	case Type_Unclaimed_Path: return 4;
		break;
	case Type_Claimed_Land: return 5;
		break;


	default: return 0; break;
	}
}



uint8_t GetNeighbourInfo(uint16_t currentType, uint16_t nType)
{
	if (currentType == nType)
	{
		return N_SAME;
	}
	else
	{
		if (nType == Type_Lava)
		{
			return N_LAVA;
		}
		else if (nType == Type_Water)
		{
			return N_WATER;
		}
		else if (nType != Type_Rock && nType != Type_Gold && nType != Type_Gem && nType != Type_Earth && nType != Type_Earth_Torch && (nType < Type_Wall0 || nType > Type_Wall5))
		{
			return N_WALKABLE;
		}
		return N_DIFF;
	}
	return N_DIFF;
}
TileNeighbours CheckNeighbours(Level* level, uint16_t type, int x, int y)
{
	TileNeighbours nInfo = { 0 };
	uint16_t nType = 0;
	if (y + 1 < MAP_SIZE_TILES)
	{
		nType = level->m_Map[y + 1][x].type;
		nInfo.North = GetNeighbourInfo(type, nType);
		if (x + 1 < MAP_SIZE_TILES)
		{
			nType = level->m_Map[y + 1][x + 1].type;
			nInfo.NorthEast = GetNeighbourInfo(type, nType);
		}
		if (x - 1 >= 0)
		{
			nType = level->m_Map[y + 1][x - 1].type;
			nInfo.NorthWest = GetNeighbourInfo(type, nType);
		}
	}
	if (y - 1 >= 0)
	{
		nType = level->m_Map[y - 1][x].type;
		nInfo.South = GetNeighbourInfo(type, nType);
		if (x + 1 < MAP_SIZE_TILES)
		{
			nType = level->m_Map[y - 1][x + 1].type;
			nInfo.SouthEast = GetNeighbourInfo(type, nType);
		}
		if (x - 1 >= 0)
		{
			nType = level->m_Map[y - 1][x - 1].type;
			nInfo.SouthWest = GetNeighbourInfo(type, nType);
		}
	}
	if (x + 1 < MAP_SIZE_TILES)
	{
		nType = level->m_Map[y][x + 1].type;
		nInfo.East = GetNeighbourInfo(type, nType);
	}
	if (x - 1 >= 0)
	{
		nType = level->m_Map[y][x - 1].type;
		nInfo.West = GetNeighbourInfo(type, nType);
	}
	return nInfo;
}
bool CheckWallIsCorner(Level* level, int x, int y)
{
	bool north = level->m_Map[y + 1][x].type != Type_Wall0 && level->m_Map[y + 1][x].type != Type_Wall3;
	bool south = level->m_Map[y - 1][x].type != Type_Wall0 && level->m_Map[y - 1][x].type != Type_Wall3;
	bool east =  level->m_Map[y][x + 1].type != Type_Wall0 && level->m_Map[y][x + 1].type != Type_Wall3;
	bool west =  level->m_Map[y][x - 1].type != Type_Wall0 && level->m_Map[y][x - 1].type != Type_Wall3;
	int numSides = (uint8_t)north + (uint8_t)south + (uint8_t)east + (uint8_t)west;

	if (north && west || north && east || south && west || south && east)
	{
		return true;
	}
	return false;
}
void VoxelObject::DoUVs(Level* level, uint16_t type, int x, int y)
{
	const int yP = y * 3;
	const int xP = x * 3;
	TileNeighbours neighbour = CheckNeighbours(level, type, x, y);
	
	uint16_t texIndex = TypeToTexture(type);
	if (type == Type_Earth || type >= Type_Wall0 && type <= Type_Wall5)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].edge[2];
		if (neighbour.North == N_WATER)
		{
			int animIndex = m_AnimationIndex % tex0.size();
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[1][yP + 2][xP + sy].uv[1] = tex0[animIndex];
			}
		}
		if (neighbour.South == N_WATER)
		{
			int animIndex = m_AnimationIndex % tex0.size();
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[1][yP][xP + sy].uv[1] = tex0[animIndex];
			}
		}
		if (neighbour.East == N_WATER)
		{
			int animIndex = m_AnimationIndex % tex0.size();
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[1][yP + sx][xP + 2].uv[0] = tex0[animIndex];
			}
		}
		if (neighbour.West == N_WATER)
		{
			int animIndex = m_AnimationIndex % tex0.size();
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[1][yP + sx][xP].uv[0] = tex0[animIndex];
			}
		}
	}
	else if (type == Type_Water)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		int animIndex = m_AnimationIndex % tex0.size();
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[0][yP + sy][xP + sx].uv[2] = tex0[animIndex];
			}
	}
	else if(type == Type_Unclaimed_Path)
	{
		bool muddyPath = false;
		if (neighbour.North != N_SAME || neighbour.South != N_SAME)
		{
			if (neighbour.East != N_SAME || neighbour.West != N_SAME)
			{
				const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[1];
				muddyPath = true;
				for (int sy = 0; sy < 3; sy++)
					for (int sx = 0; sx < 3; sx++)
					{
						m_Map[1][yP + sy][xP + sx].uv[2] = tex0[m_Map[1][yP + sy][xP + sx].randValue % tex0.size()];
					}
			}
		}
		if (!muddyPath)
		{
			const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
			for (int sy = 0; sy < 3; sy++)
				for (int sx = 0; sx < 3; sx++)
				{
					m_Map[1][yP + sy][xP + sx].uv[2] = tex0[m_Map[1][yP + sy][xP + sx].randValue % tex0.size()];
				}
		}
	}
	else if (type == Type_Claimed_Land)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndex].top[1];
		if (neighbour.North == N_SAME)
		{
			m_Map[1][yP + 2][xP + 1].uv[2] = tex1[m_Map[1][yP + 2][xP + 1].randValue % tex1.size()];
			if (neighbour.East == N_SAME)
			{
				m_Map[1][yP + 2][xP + 2].uv[2] = tex1[m_Map[1][yP + 2][xP + 2].randValue % tex1.size()];
			}
			else
			{
				m_Map[1][yP + 2][xP + 2].uv[2] = tex0[5];
			}
			if (neighbour.West == N_SAME)
			{
				m_Map[1][yP + 2][xP].uv[2] = tex1[m_Map[1][yP + 2][xP].randValue % tex1.size()];
			}
			else
			{
				m_Map[1][yP + 2][xP].uv[2] = tex0[3];
			}
		}
		else
		{
			m_Map[1][yP + 2][xP + 1].uv[2] = tex0[7];
			if (neighbour.East == N_SAME)
			{
				m_Map[1][yP + 2][xP + 2].uv[2] = tex0[7];
			}
			else
			{
				m_Map[1][yP + 2][xP + 2].uv[2] = tex0[8];
			}
			if (neighbour.West == N_SAME)
			{
				m_Map[1][yP + 2][xP].uv[2] = tex0[7];
			}
			else
			{
				m_Map[1][yP + 2][xP].uv[2] = tex0[6];
			}
		}
		if (neighbour.South == N_SAME)
		{
			m_Map[1][yP][xP + 1].uv[2] = tex1[m_Map[1][yP][xP + 1].randValue % tex1.size()];
			if (neighbour.East == N_SAME)
			{
				m_Map[1][yP][xP + 2].uv[2] = tex1[m_Map[1][yP][xP + 2].randValue % tex1.size()];
			}
			else
			{
				m_Map[1][yP][xP + 2].uv[2] = tex0[5];
			}
			if (neighbour.West == N_SAME)
			{
				m_Map[1][yP][xP].uv[2] = tex1[m_Map[1][yP][xP].randValue % tex1.size()];
			}
			else
			{
				m_Map[1][yP][xP].uv[2] = tex0[3];
			}
		}
		else
		{
			m_Map[1][yP][xP + 1].uv[2] = tex0[1];
			if (neighbour.East == N_SAME)
			{
				m_Map[1][yP][xP + 2].uv[2] = tex0[1];
			}
			else
			{
				m_Map[1][yP][xP + 2].uv[2] = tex0[2];
			}
			if (neighbour.West == N_SAME)
			{
				m_Map[1][yP][xP].uv[2] = tex0[1];
			}
			else
			{
				m_Map[1][yP][xP].uv[2] = tex0[0];
			}
		}
		if (neighbour.East == N_SAME)
		{
			m_Map[1][yP + 1][xP + 2].uv[2] = tex1[m_Map[1][yP+1][xP + 2].randValue % tex1.size()];
		}
		else
		{
			m_Map[1][yP + 1][xP + 2].uv[2] = tex0[5];
		}
		if (neighbour.West == N_SAME)
		{
			m_Map[1][yP + 1][xP].uv[2] = tex1[m_Map[1][yP + 1][xP].randValue % tex1.size()];
		}
		else
		{
			m_Map[1][yP + 1][xP].uv[2] = tex0[3];
		}
	}
	else if (type == Type_Gold) //corner
	{
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[m_Map[1][yP + sy][xP + sx].randValue % BlockTextures[texIndex].top.size()];
				m_Map[7][yP + sy][xP + sx].uv[2] = tex0[m_AnimationIndex % tex0.size()];
			}
		for (int sz = 0; sz < 8; sz++)
			for (int sy = 0; sy < 3; sy++)
				for (int sx = 0; sx < 3; sx++)
				{
					const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[m_Map[sz][yP + sy][xP + sx].randValue % BlockTextures[texIndex].top.size()];
					m_Map[sz][yP + sy][xP + sx].uv[0] = tex0[m_AnimationIndex % tex0.size()];
					m_Map[sz][yP + sy][xP + sx].uv[1] = tex0[m_AnimationIndex % tex0.size()];
				}
		
	}

	//Previous check on Type_Wall0 was so that we add the edge water animation, this one is for the top and sides
	if (type == Type_Wall0)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + 2][xP + sy].uv[2] = tex0[2];
			}
		}
		m_Map[7][yP + 1][xP + 1].uv[2] = XMFLOAT2(3 + level->m_Map[y][x].owner, 13);
		if (neighbour.North == N_WALKABLE)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[7][yP + 2][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.South == N_WALKABLE)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[7][yP][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.East == N_WALKABLE)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + sx][xP + 2].uv[2] = tex0[1];
			}
		}
		if (neighbour.West == N_WALKABLE)
		{
			int randValue = m_Map[7][yP][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndex].side[randValue & BlockTextures[texIndex].side.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 1; z < MAP_SIZE_HEIGHT; z++)
				{
					m_Map[z][yP + sx][xP].uv[2] = tex1[0];
				}
				m_Map[7][yP + sx][xP].uv[2] = tex0[1];
			}

			
		}
		
	}
	if (type == Type_Wall2) //wall+corner, this tile can't ever exist on the edges so checking for X/Y -1 / +1 is totally fine
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + sy][xP + sx].uv[2] = tex0[2];
			}

		m_Map[7][yP + 1][xP + 1].uv[2] = XMFLOAT2(3 + level->m_Map[y][x].owner, 13);
		bool northWall = level->m_Map[y + 1][x].type == Type_Wall0;// && level->m_Map[y + 1][x].type <= Type_Wall5;
		bool southWall = level->m_Map[y - 1][x].type == Type_Wall0;// && level->m_Map[y - 1][x].type <= Type_Wall5;
		bool eastWall = level->m_Map[y][x + 1].type == Type_Wall0 ;//&& level->m_Map[y][x + 1].type <= Type_Wall5;
		bool westWall = level->m_Map[y][x - 1].type == Type_Wall0 ;//&& level->m_Map[y][x - 1].type <= Type_Wall5;

		//corner pieces
		if (northWall && eastWall)
		{
			m_Map[7][yP + 2][xP+ 2].uv[2] = tex0[0];
		}
		if (northWall && westWall)
		{
			m_Map[7][yP + 2][xP].uv[2] = tex0[0];
		}
		if (southWall && eastWall)
		{
			m_Map[7][yP][xP + 2].uv[2] = tex0[0];
		}
		if (southWall && westWall)
		{
			m_Map[7][yP][xP].uv[2] = tex0[0];
		}

		bool northWalkable = GetNeighbourInfo(type, level->m_Map[y + 1][x].type) == N_WALKABLE;
		bool southhWalkable = GetNeighbourInfo(type, level->m_Map[y - 1][x].type) == N_WALKABLE;
		bool eastWalkable = GetNeighbourInfo(type, level->m_Map[y][x + 1].type) == N_WALKABLE;
		bool westWalkable = GetNeighbourInfo(type, level->m_Map[y][x - 1].type) == N_WALKABLE;
		if (northWalkable)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[7][yP + 2][xP + sy].uv[2] = tex0[1];
			}
		}
		if (southhWalkable)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_Map[7][yP][xP + sy].uv[2] = tex0[1];
			}
		}
		if (eastWalkable)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + sx][xP + 2].uv[2] = tex0[1];
			}
		}
		if (westWalkable)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + sx][xP].uv[2] = tex0[1];
			}
		}
	}
	if (type == Type_Wall3) //corner
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				m_Map[7][yP + sy][xP + sx].uv[2] = tex0[2];
			}
		m_Map[7][yP + 1][xP + 1].uv[2] = XMFLOAT2(3+level->m_Map[y][x].owner,13);
		bool northWall = level->m_Map[y + 1][x].type >= Type_Wall0 && level->m_Map[y + 1][x].type <= Type_Wall5;
		bool southWall = level->m_Map[y - 1][x].type >= Type_Wall0 && level->m_Map[y - 1][x].type <= Type_Wall5;
		bool eastWall  = level->m_Map[y][x + 1].type >= Type_Wall0 && level->m_Map[y][x + 1].type <= Type_Wall5;
		bool westWall  = level->m_Map[y][x - 1].type >= Type_Wall0 && level->m_Map[y][x - 1].type <= Type_Wall5;
		if (northWall)
		{
			if (eastWall)
			{
				m_Map[7][yP + 2][xP + 2].uv[2] = tex0[0];
			}
			if (westWall)
			{
				m_Map[7][yP + 2][xP].uv[2] = tex0[0];
			}
		}
		if (southWall)
		{
			if (eastWall)
			{
				m_Map[7][yP][xP + 2].uv[2] = tex0[0];
			}
			if (westWall)
			{
				m_Map[7][yP][xP].uv[2] = tex0[0];
			}
		}
	}


	//const float pixelSizeX = 1.0f / 256.0f;
	//const float pixelSizeY = 1.0f / 2176.0f;
	//const std::vector<XMFLOAT2>& tex0 = BlockTextures[TypeToTexture(type)].side;
	//const std::vector<XMFLOAT2>& tex1 = BlockTextures[TypeToTexture(type)].side;
	//tex.x = tex.x/8.0;
	//tex.y = tex.y/68.0;
	//if (type == 12 || type == 13) tex.x *= m_AnimationIndex;
	//map[0][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[1][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[2][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[3][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[4][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[5][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[6][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);
	//map[7][y][x].uv[0] = GetRandomTexture(BlockTextures[TypeToTexture(type)].side, 0);

	//typeX = map[8][y][x].type % 8;
	//typeY = map[8][y][x].type % 68;
	//map[8][y][x].uv = XMFLOAT2((float)typeX * pixelSizeX, (float)typeY *pixelSizeY);
}
uint16_t VoxelObject::HandleSpecialCases(const Tile map[85][85], int yPos, int xPos)
{
	uint16_t type = map[yPos][xPos].type & 0xFF;
	if (type != Type_Dungeon_Heart && type != Type_Portal) return type;
	if (yPos >= 1 && yPos < 84 && xPos >= 1 && xPos < 84)
	{
		if (L8(map[yPos - 1][xPos].type) != type && L8(map[yPos + 1][xPos].type) == type) // U
		{
			if (L8(map[yPos][xPos - 1].type) == type && L8(map[yPos][xPos + 1].type) != type) // LU
			{
				return type + (3 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) == type) // MU
			{
				return type + (2 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) != type) // RU
			{
				return type + (1 << 8);
			}
		}
		if (L8(map[yPos + 1][xPos].type) != type && L8(map[yPos - 1][xPos].type) == type) // L
		{
			if (L8(map[yPos][xPos - 1].type) == type && L8(map[yPos][xPos + 1].type) != type) // LL
			{
				return type + (9 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) == type) // ML
			{
				return type + (8 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) != type) // RL
			{
				return type + (7 << 8);
			}
		}
		if (L8(map[yPos + 1][xPos].type) == type && L8(map[yPos - 1][xPos].type) == type) // M
		{
			if (L8(map[yPos][xPos - 1].type) == type && L8(map[yPos][xPos + 1].type) != type) // LM
			{
				return type + (6 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) == type) // MM
			{
				if (L8(map[yPos + 1][xPos + 1].type) == type && L8(map[yPos - 1][xPos + 1].type) == type && L8(map[yPos + 1][xPos - 1].type) == type && L8(map[yPos - 1][xPos - 1].type) == type)
				{
					return type + (10 << 8);
				}
				return type + (5 << 8);
			}
			if (L8(map[yPos][xPos + 1].type) == type && L8(map[yPos][xPos - 1].type) != type) // RM
			{
				return type + (4 << 8);
			}
		}
	}
	return type + (5 << 8); //Middle tile
}
int VoxelObject::CreateFromTile(const Tile& tile, RenderTile& out)
{
	int numSolidBlocks = 0;
	uint8_t usingType = (tile.type >> 8) == 10 ? 5 : (tile.type >> 8);
	if (usingType > 0)usingType--;
	switch (tile.type & 0xFF)
	{
	case Type_Rock:
		memcpy(&out.subTile, &RenderTileMap[Tile_ROCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_ROCK].activeBlocks;
		break;
	case Type_Gold:
	case Type_Earth:
	case Type_Earth_Torch:
		memcpy(&out.subTile, &RenderTileMap[Tile_FULLBLOCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_FULLBLOCK].activeBlocks;
		break;
	case Type_Wall0:
	case Type_Wall1:
	case Type_Wall2:
	case Type_Wall3:
	case Type_Wall4:
	case Type_Wall5:
		memcpy(&out.subTile, &RenderTileMap[Tile_FULLBLOCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_FULLBLOCK].activeBlocks;
		break;
	case Type_Unclaimed_Path:
	case Type_Claimed_Land:
		memcpy(&out.subTile, &RenderTileMap[Tile_GROUND], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_GROUND].activeBlocks;
		break;
	case Type_Water:
	case Type_Lava:
		memcpy(&out.subTile, &RenderTileMap[Tile_LIQUID], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_LIQUID].activeBlocks;
		break;
	case Type_Portal:
		memcpy(&out.subTile, &RenderTileMap[Tile_PORTAL + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_PORTAL + usingType].activeBlocks;
		break;
	case Type_Dungeon_Heart:
		memcpy(&out.subTile, &RenderTileMap[Tile_DUNGEON_HEART + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_DUNGEON_HEART + usingType].activeBlocks;
		break;
	}

	return numSolidBlocks;
}