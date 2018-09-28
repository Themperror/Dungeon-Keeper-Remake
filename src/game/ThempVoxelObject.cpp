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

	for (size_t y = 0; y < MAP_SIZE_SUBTILES_RENDER; y++)
		for (size_t x = 0; x < 3; x++)
			for (size_t z = 0; z < 8; z++)
			{
				m_Map[z][y][0+x].active = true;
				m_Map[z][y][0+x].uv[0] = XMFLOAT2(2, 9);
				m_Map[z][y][0+x].uv[1] = XMFLOAT2(2, 9);
			}

	for (size_t y = 0; y < MAP_SIZE_SUBTILES_RENDER; y++)
		for (size_t x = 0; x < 3; x++)
			for (size_t z = 0; z < 8; z++)
			{
				m_Map[z][y][254-x].active = true;
				m_Map[z][y][254-x].uv[0] = XMFLOAT2(2, 9);
				m_Map[z][y][254-x].uv[1] = XMFLOAT2(2, 9);
			}
	for (size_t x = 0; x < MAP_SIZE_SUBTILES_RENDER; x++)
		for (size_t y = 0; y < 3; y++)
			for (size_t z = 0; z < 8; z++)
			{
				m_Map[z][0+y][x].active = true;
				m_Map[z][0+y][x].uv[0] = XMFLOAT2(2, 9);
				m_Map[z][0+y][x].uv[1] = XMFLOAT2(2, 9);
			}
	for (size_t x = 0; x < MAP_SIZE_SUBTILES_RENDER; x++)
		for (size_t y = 0; y < 3; y++)
			for (size_t z = 0; z < 8; z++)
			{
				m_Map[z][254-y][x].active = true;
				m_Map[z][254-y][x].uv[0] = XMFLOAT2(2, 9);
				m_Map[z][254-y][x].uv[1] = XMFLOAT2(2, 9);
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
						m_Map[z][y*3 + yy][x*3 + xx] = b;
					}
				}
			}
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
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y };

					}
					if (x - 1 <= 0 || !m_Map[z][y][x - 1].active) // left
					{
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f, z + 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x - 0.5f, z - 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY };
					}
					if (y - 1 <= 0 || !m_Map[z][y - 1][x].active) // back
					{
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
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
					uv = m_Map[z][y][x].uv[1];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (z - 1 <= 0 || !m_Map[z - 1][y][x].active) // bottom
					{
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x				,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						m_Vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y };
						m_Vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x				,uv.y };
					}
					if (z + 1 >= MAP_SIZE_HEIGHT || !m_Map[z + 1][y][x].active) // top
					{
						m_Indices[currentIndex + 0] = vIndex + 0;
						m_Indices[currentIndex + 1] = vIndex + 1;
						m_Indices[currentIndex + 2] = vIndex + 2;
						m_Indices[currentIndex + 3] = vIndex + 0;
						m_Indices[currentIndex + 4] = vIndex + 2;
						m_Indices[currentIndex + 5] = vIndex + 3;
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
	return 0;
}
void VoxelObject::DoUVs(uint16_t type, size_t x, size_t y)
{
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
	if (type != 26 && type != 14) return type;
	if (yPos > 1 && yPos < 84 && xPos > 1 && xPos < 84)
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
	case 0: //Rock
		memcpy(&out.subTile, &RenderTileMap[Tile_ROCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_ROCK].activeBlocks;
		break;
	case 1: //Gold
	case 2: //Earth
	case 3: //Earth + Torch
		memcpy(&out.subTile, &RenderTileMap[Tile_FULLBLOCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_FULLBLOCK].activeBlocks;
		break;
	case 4: //Wall 1 
	case 5: //Wall 2
	case 6: //Wall 3
	case 7: //Wall 4
	case 8: //Wall 5
	case 9: //Wall 6
		memcpy(&out.subTile, &RenderTileMap[Tile_FULLBLOCK], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_FULLBLOCK].activeBlocks;
		break;
	case 10: //Path
	case 11: //Claimed Terrain
		memcpy(&out.subTile, &RenderTileMap[Tile_GROUND], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_GROUND].activeBlocks;
		break;
	case 13: //Water
		memcpy(&out.subTile, &RenderTileMap[Tile_LIQUID], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_LIQUID].activeBlocks;
		break; 
	case 14: //Portal
		memcpy(&out.subTile, &RenderTileMap[Tile_PORTAL + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_PORTAL + usingType].activeBlocks;
		break;
	case 26: //Dungeon Heart
		memcpy(&out.subTile, &RenderTileMap[Tile_DUNGEON_HEART + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[Tile_DUNGEON_HEART + usingType].activeBlocks;
		break;
	}

	return numSolidBlocks;
}