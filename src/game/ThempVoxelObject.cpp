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

}

VoxelObject::VoxelObject()
{

}
void VoxelObject::Cull(int camX,int camY)
{
	
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
	case 1: //Gold
	case 2: //Earth
	case 3: //Earth + Torch
		memcpy(&out.subTile, &RenderTileMap[0], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[0].activeBlocks;
		break;
	case 4: //Wall 1 
	case 5: //Wall 2
	case 6: //Wall 3
	case 7: //Wall 4
	case 8: //Wall 5
	case 9: //Wall 6
		memcpy(&out.subTile, &RenderTileMap[0], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[0].activeBlocks;
		break;
	case 10: //Path
	case 11: //Claimed Terrain
		memcpy(&out.subTile, &RenderTileMap[1], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[1].activeBlocks;
		break;
	case 13: //Water
		memcpy(&out.subTile, &RenderTileMap[2], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[2].activeBlocks;
		break; 
	case 14: //Portal
		memcpy(&out.subTile, &RenderTileMap[12 + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[12 + usingType].activeBlocks;
		break;
	case 26: //Dungeon Heart
		memcpy(&out.subTile, &RenderTileMap[3 + usingType], sizeof(RenderTile));
		numSolidBlocks += RenderTileMap[3 + usingType].activeBlocks;
		break;
	}

	return numSolidBlocks;
}
void VoxelObject::ConstructFromLevel(Level* level)
{	
	size_t solidBlocks = 0;

	for (size_t y = 0; y < MAP_SIZE_SUBTILES; y++)
		for (size_t z = 0; z < 8; z++)
		{
			map[z][y][0].active = true;
		}
	for (size_t y = 0; y < MAP_SIZE_SUBTILES; y++)
		for (size_t z = 0; z < 8; z++)
		{
			map[z][y][255].active = true;
		}
	for (size_t x = 0; x < MAP_SIZE_SUBTILES; x++)
		for (size_t z = 0; z < 8; z++)
		{
			map[z][0][x].active = true;
		}
	for (size_t x = 0; x < MAP_SIZE_SUBTILES; x++)
		for (size_t z = 0; z < 8; z++)
		{
			map[z][255][x].active = true;
		}

	for (size_t y = 0; y < MAP_SIZE_SUBTILES-1; y+=3)
	{
		const int yPos = (y+2) / 3;
		for (size_t x = 0; x < MAP_SIZE_SUBTILES-1; x+=3)
		{
			const int xPos = (x + 2) / 3;
			Tile& tile = level->m_Map[yPos][xPos];
			RenderTile tileOut;

			//special cases
			tile.type = HandleSpecialCases(level->m_Map, yPos, xPos);
			size_t numBlocks = CreateFromTile(tile, tileOut);
			solidBlocks += numBlocks;

			for (size_t yy = 0; yy < 3; yy++)
			{
				for (size_t xx = 0; xx < 3; xx++)
				{
					for (size_t z = 0; z < 8; z++)
					{
						map[z][y + yy][x + xx] = tileOut.subTile[yy][xx][z];
					}
					if(tile.type & 0xFF != 26)
						DoUVs(tile.type & 0xFF, x + xx, y + yy);
				}
			}
		}
	}
	System::Print("Num Solid blocks: %i", solidBlocks);
	Vertex* vertices = new Vertex[solidBlocks * 6]; //worse case scenario
	uint32_t* indices = new uint32_t[solidBlocks * 6];

	size_t vIndex = 0;
	size_t currentIndex = 0;
	//9 blocks high


	const float pixelSizeX = (1.0f / 256.0f) * 31.5f;
	const float pixelSizeY = (1.0f / 2176.0f) * 31.5f;

	for (int z = 0; z < MAP_SIZE_HEIGHT; z++)
	{
		for (int y = 0; y < MAP_SIZE_SUBTILES; y++)
		{
			for (int x = 0; x < MAP_SIZE_SUBTILES; x++)
			{
				if (map[z][y][x].active)
				{
					XMFLOAT2 uv = map[z][y][x].uv[0];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (x + 1 >= MAP_SIZE_SUBTILES || !map[z][y][x+1].active) // right
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y				};
						vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY	};
						vertices[vIndex++] = { x + 0.5f, z - 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x+ pixelSizeX	,uv.y + pixelSizeY	};
						vertices[vIndex++] = { x + 0.5f, z + 0.5f  , y + 0.5f  , 1,(float)z / 9.0f,0, uv.x+ pixelSizeX	,uv.y				};

					} 
					if (x - 1 <= 0 || !map[z][y][x - 1].active) // left
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x - 0.5f, z + 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y				};
						vertices[vIndex++] = { x - 0.5f, z + 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y				};
						vertices[vIndex++] = { x - 0.5f, z - 0.5f, y + 0.5f  , 1,(float)z / 9.0f,0, uv.x + pixelSizeX	,uv.y + pixelSizeY	};
						vertices[vIndex++] = { x - 0.5f, z - 0.5f, y - 0.5f  , 1,(float)z / 9.0f,0, uv.x				,uv.y + pixelSizeY	};
					}
					if (y - 1 <= 0 || !map[z][y - 1][x].active) // back
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x + 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						vertices[vIndex++] = { x + 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y};
						vertices[vIndex++] = { x - 0.5f ,  z + 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y};
						vertices[vIndex++] = { x - 0.5f ,  z - 0.5f  , y - 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY };
					}
					if (y + 1 >= MAP_SIZE_SUBTILES || !map[z][y + 1][x].active) // front
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x + 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						vertices[vIndex++] = { x + 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x + pixelSizeX	,uv.y };
						vertices[vIndex++] = { x - 0.5f  ,   z + 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y };
						vertices[vIndex++] = { x - 0.5f  ,   z - 0.5f,  y + 0.5f , 0,0,1, uv.x				,uv.y + pixelSizeY };
					}
					uv = map[z][y][x].uv[1];
					uv.x = uv.x / 8.0;
					uv.y = uv.y / 68.0;
					if (z - 1 <= 0 || !map[z - 1][y][x].active) // bottom
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x				,uv.y + pixelSizeY };
						vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y + 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						vertices[vIndex++] = { x + 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x + pixelSizeX	,uv.y};
						vertices[vIndex++] = { x - 0.5f , z - 0.5f ,  y - 0.5f  , 0,1,0, uv.x				,uv.y};
					}
					if (z + 1 >= MAP_SIZE_HEIGHT || !map[z + 1][y][x].active) // top
					{
						indices[currentIndex + 0] = vIndex + 0;
						indices[currentIndex + 1] = vIndex + 1;
						indices[currentIndex + 2] = vIndex + 2;
						indices[currentIndex + 3] = vIndex + 0;
						indices[currentIndex + 4] = vIndex + 2;
						indices[currentIndex + 5] = vIndex + 3;
						currentIndex += 6;

						vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y};
						vertices[vIndex++] = { x + 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x + pixelSizeX	,uv.y + pixelSizeY };
						vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y + 0.5f , 0,1,0, uv.x					,uv.y + pixelSizeY };
						vertices[vIndex++] = { x - 0.5f, z + 0.5f ,  y - 0.5f , 0,1,0, uv.x					,uv.y};
					}
				}
			}
		}
	}

	System::Print("Num Vertices: %i", vIndex);
	System::Print("Num Indices: %i", currentIndex);
	m_Obj3D = new Object3D();
	Mesh* m = new Mesh();
	Themp::Resources::TRes->m_Meshes.push_back(m);
	m_Obj3D->m_Meshes.push_back(m);
	m->m_NumIndices = currentIndex;
	m->m_NumVertices = vIndex;

	m->m_Vertices = vertices;
	m->m_Indices = indices;
	m->ConstructVertexBuffer(); //requires m_Vertices/m_Indices to be set

	//unset it so it doesnt "delete[]" our vertices/indices, so we can take care of this ourselves
	m->m_Vertices = nullptr;
	m->m_Indices = nullptr; 

	m->m_Material = Resources::TRes->GetUniqueMaterial("", "default");
	if (solidBlocks * 6 * sizeof(Themp::Vertex) < vIndex * sizeof(Themp::Vertex))
	{
		System::Print("This is wrong.. Very wrong...");
		assert(false);
	}	
	if (solidBlocks * 6 * sizeof(uint32_t) < currentIndex * sizeof(uint32_t))
	{
		System::Print("This is wrong.. Very wrong...");
		assert(false);
	}

}