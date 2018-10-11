#include "ThempSystem.h"
#include "ThempLevel.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempObject2D.h"
#include "ThempGUIButton.h"
#include "ThempFont.h"
#include "ThempResources.h"
#include "ThempVoxelObject.h"
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

Level::~Level()
{
	delete m_MapObject;
}
XMFLOAT2 cursorOffset = XMFLOAT2(0.03, 0);

Object3D* tileIndicator = nullptr;
Level::Level(int levelIndex)
{
	m_CurrentLevelNr = levelIndex;
	m_Cursor = new Object2D(Object2D::Source::sMENU_CURSOR, 1, true);
	m_Cursor->SetVisibility(true);
	System::tSys->m_Game->AddObject3D(m_Cursor->m_Renderable);

	tileIndicator = new Object3D();
	tileIndicator->CreateCube("default", false);
	tileIndicator->SetScale(3, 0.5, 3);
	System::tSys->m_Game->AddObject3D(tileIndicator);

	LoadLevelData();
	m_MapObject = new VoxelObject(this);
	m_MapObject->m_Obj3D->m_Meshes[0]->m_Material->SetTexture(FileManager::GetBlockTexture(0));
	Themp::System::tSys->m_Game->AddObject3D(m_MapObject->m_Obj3D);
	//System::tSys->m_Game->m_Camera->SetOrtho(20, 20);
	//System::tSys->m_Game->m_Camera->SetProjection(Camera::CameraType::Orthographic);
	UpdateArea(1, 84, 1, 84);
}


uint8_t Level::GetNeighbourInfo(uint16_t currentType, uint16_t nType)
{
	if (currentType == nType || currentType >= Type_Wall0 && currentType <= Type_Wall5 && nType >= Type_Wall0 && nType <= Type_Wall5)
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
TileNeighbours Level::CheckNeighbours(uint16_t type, int y, int x)
{
	TileNeighbours nInfo = { 0 };
	uint16_t nType = 0;
	if (y + 1 < MAP_SIZE_TILES)
	{
		nType = m_Map[y + 1][x].type;
		nInfo.North = GetNeighbourInfo(type, nType);
		if (x + 1 < MAP_SIZE_TILES)
		{
			nType = m_Map[y + 1][x + 1].type;
			nInfo.NorthEast = GetNeighbourInfo(type, nType);
		}
		if (x - 1 >= 0)
		{
			nType = m_Map[y + 1][x - 1].type;
			nInfo.NorthWest = GetNeighbourInfo(type, nType);
		}
	}
	if (y - 1 >= 0)
	{
		nType = m_Map[y - 1][x].type;
		nInfo.South = GetNeighbourInfo(type, nType);
		if (x + 1 < MAP_SIZE_TILES)
		{
			nType = m_Map[y - 1][x + 1].type;
			nInfo.SouthEast = GetNeighbourInfo(type, nType);
		}
		if (x - 1 >= 0)
		{
			nType = m_Map[y - 1][x - 1].type;
			nInfo.SouthWest = GetNeighbourInfo(type, nType);
		}
	}
	if (x + 1 < MAP_SIZE_TILES)
	{
		nType = m_Map[y][x + 1].type;
		nInfo.East = GetNeighbourInfo(type, nType);
	}
	if (x - 1 >= 0)
	{
		nType = m_Map[y][x - 1].type;
		nInfo.West = GetNeighbourInfo(type, nType);
	}
	return nInfo;
}
TileNeighbourTiles Level::GetNeighbourTiles(int y, int x)
{
	TileNeighbourTiles nInfo = { 0 };
	nInfo.NorthWest.owner = m_Map[y + 1][x - 1].owner;
	nInfo.NorthWest.type  = m_Map[y + 1][x - 1].type;
	nInfo.North.owner = m_Map[y + 1][x].owner;
	nInfo.North.type = m_Map[y + 1][x].type;
	nInfo.NorthEast.owner = m_Map[y + 1][x + 1].owner;
	nInfo.NorthEast.type = m_Map[y + 1][x + 1].type;
	nInfo.SouthWest.owner = m_Map[y - 1][x - 1].owner;
	nInfo.SouthWest.type = m_Map[y - 1][x - 1].type;
	nInfo.South.owner = m_Map[y - 1][x].owner;
	nInfo.South.type = m_Map[y - 1][x].type;
	nInfo.SouthEast.owner = m_Map[y - 1][x + 1].owner;
	nInfo.SouthEast.type = m_Map[y - 1][x + 1].type;
	nInfo.East.owner = m_Map[y][x + 1].owner;
	nInfo.East.type = m_Map[y][x + 1].type;
	nInfo.West.owner = m_Map[y][x - 1].owner;
	nInfo.West.type = m_Map[y][x - 1].type;
	return nInfo;
}
void Level::DoUVs(uint16_t type, int y, int x)
{
	const int yP = y * 3;
	const int xP = x * 3;
	TileNeighbours neighbour = CheckNeighbours(type, y, x);

	uint16_t texIndex = TypeToTexture(type);
	if (type == Type_Earth)
	{
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndex].top[0];
				m_BlockMap[5][yP + sy][xP + sx].uv[2] = tex1[m_BlockMap[5][yP + sy][xP + sx].randValue % tex1.size()];
			}
	}
	else if (type == Type_Unclaimed_Path)
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
						m_BlockMap[1][yP + sy][xP + sx].uv[2] = tex0[m_BlockMap[1][yP + sy][xP + sx].randValue % tex0.size()];
					}
			}
		}
		if (!muddyPath)
		{
			const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
			for (int sy = 0; sy < 3; sy++)
				for (int sx = 0; sx < 3; sx++)
				{
					m_BlockMap[1][yP + sy][xP + sx].uv[2] = tex0[m_BlockMap[1][yP + sy][xP + sx].randValue % tex0.size()];
				}
		}
	}
	else if (type == Type_Claimed_Land)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndex].top[1];
		if (neighbour.North == N_SAME)
		{
			m_BlockMap[1][yP + 2][xP + 1].uv[2] = tex1[m_BlockMap[1][yP + 2][xP + 1].randValue % tex1.size()];
			if (neighbour.East == N_SAME)
			{
				m_BlockMap[1][yP + 2][xP + 2].uv[2] = tex1[m_BlockMap[1][yP + 2][xP + 2].randValue % tex1.size()];
			}
			else
			{
				m_BlockMap[1][yP + 2][xP + 2].uv[2] = tex0[5];
			}
			if (neighbour.West == N_SAME)
			{
				m_BlockMap[1][yP + 2][xP].uv[2] = tex1[m_BlockMap[1][yP + 2][xP].randValue % tex1.size()];
			}
			else
			{
				m_BlockMap[1][yP + 2][xP].uv[2] = tex0[3];
			}
		}
		else
		{
			m_BlockMap[1][yP + 2][xP + 1].uv[2] = tex0[7];
			if (neighbour.East == N_SAME)
			{
				m_BlockMap[1][yP + 2][xP + 2].uv[2] = tex0[7];
			}
			else
			{
				m_BlockMap[1][yP + 2][xP + 2].uv[2] = tex0[8];
			}
			if (neighbour.West == N_SAME)
			{
				m_BlockMap[1][yP + 2][xP].uv[2] = tex0[7];
			}
			else
			{
				m_BlockMap[1][yP + 2][xP].uv[2] = tex0[6];
			}
		}
		if (neighbour.South == N_SAME)
		{
			m_BlockMap[1][yP][xP + 1].uv[2] = tex1[m_BlockMap[1][yP][xP + 1].randValue % tex1.size()];
			if (neighbour.East == N_SAME)
			{
				m_BlockMap[1][yP][xP + 2].uv[2] = tex1[m_BlockMap[1][yP][xP + 2].randValue % tex1.size()];
			}
			else
			{
				m_BlockMap[1][yP][xP + 2].uv[2] = tex0[5];
			}
			if (neighbour.West == N_SAME)
			{
				m_BlockMap[1][yP][xP].uv[2] = tex1[m_BlockMap[1][yP][xP].randValue % tex1.size()];
			}
			else
			{
				m_BlockMap[1][yP][xP].uv[2] = tex0[3];
			}
		}
		else
		{
			m_BlockMap[1][yP][xP + 1].uv[2] = tex0[1];
			if (neighbour.East == N_SAME)
			{
				m_BlockMap[1][yP][xP + 2].uv[2] = tex0[1];
			}
			else
			{
				m_BlockMap[1][yP][xP + 2].uv[2] = tex0[2];
			}
			if (neighbour.West == N_SAME)
			{
				m_BlockMap[1][yP][xP].uv[2] = tex0[1];
			}
			else
			{
				m_BlockMap[1][yP][xP].uv[2] = tex0[0];
			}
		}
		if (neighbour.East == N_SAME)
		{
			m_BlockMap[1][yP + 1][xP + 2].uv[2] = tex1[m_BlockMap[1][yP + 1][xP + 2].randValue % tex1.size()];
		}
		else
		{
			m_BlockMap[1][yP + 1][xP + 2].uv[2] = tex0[5];
		}
		if (neighbour.West == N_SAME)
		{
			m_BlockMap[1][yP + 1][xP].uv[2] = tex1[m_BlockMap[1][yP + 1][xP].randValue % tex1.size()];
		}
		else
		{
			m_BlockMap[1][yP + 1][xP].uv[2] = tex0[3];
		}
		m_BlockMap[1][yP + 1][xP + 1].uv[2] = t_FloorOwners[m_Map[y][x].owner];
	}
	DoWallUVs(neighbour, type, texIndex, x, y);

	//debugging ownership
	//m_BlockMap[1][yP + 1][xP + 1].uv[2] = t_FloorOwners[m_Map[y][x].owner];
	//m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_FloorOwners[m_Map[y][x].owner];
}
void Level::DoWallUVs(TileNeighbours neighbour, int type, int texIndex, int x, int y)
{
	const int yP = y * 3;
	const int xP = x * 3;
	if (type == Type_Wall0)
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + 2][xP + sy].uv[2] = tex0[2];
			}
		}
		m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[m_Map[y][x].owner];
		//for debugging
		//m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[Owner_PlayerWhite];
		if (neighbour.North != N_SAME && neighbour.North != N_DIFF)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP + 2][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.South != N_SAME && neighbour.South != N_DIFF)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.East != N_SAME && neighbour.East != N_DIFF)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP + 2].uv[2] = tex0[1];
			}
		}
		if (neighbour.West != N_SAME && neighbour.West != N_DIFF)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP].uv[2] = tex0[1];
			}
		}
		if (neighbour.West == N_WALKABLE || neighbour.West == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x - 1].type);
			int randValue = m_BlockMap[7][yP][xP + 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.East == N_WALKABLE || neighbour.East == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x + 1].type);
			int randValue = m_BlockMap[7][yP][xP - 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP + 2].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.North == N_WALKABLE || neighbour.North == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y + 1][x].type);
			int randValue = m_BlockMap[7][yP + 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + 2][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.South == N_WALKABLE || neighbour.South == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y - 1][x].type);
			int randValue = m_BlockMap[7][yP - 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}

		bool northWalkable = (neighbour.North != N_DIFF && neighbour.North != N_SAME);
		bool northEastWalkable = (neighbour.NorthEast != N_DIFF && neighbour.NorthEast != N_SAME);
		bool northWestWalkable = (neighbour.NorthWest != N_DIFF && neighbour.NorthWest != N_SAME);
		bool southWalkable = (neighbour.South != N_DIFF && neighbour.South != N_SAME);
		bool southEastWalkable = (neighbour.SouthEast != N_DIFF && neighbour.SouthEast != N_SAME);
		bool southWestWalkable = (neighbour.SouthWest != N_DIFF && neighbour.SouthWest != N_SAME);
		bool eastWalkable = (neighbour.East != N_DIFF && neighbour.East != N_SAME);
		bool westWalkable = (neighbour.West != N_DIFF && neighbour.West != N_SAME);

		//corner pieces
		if (northWalkable && eastWalkable || !northWalkable && !eastWalkable && northEastWalkable)
		{
			m_BlockMap[5][yP + 2][xP + 2].uv[2] = tex0[0];
		}
		if (northWalkable && westWalkable || !northWalkable && !westWalkable && northWestWalkable)
		{
			m_BlockMap[5][yP + 2][xP].uv[2] = tex0[0];
		}
		if (southWalkable && eastWalkable || !southWalkable && !eastWalkable && southEastWalkable)
		{
			m_BlockMap[5][yP][xP + 2].uv[2] = tex0[0];
		}
		if (southWalkable && westWalkable || !southWalkable && !westWalkable && southWestWalkable)
		{
			m_BlockMap[5][yP][xP].uv[2] = tex0[0];
		}

	}
	else if (type == Type_Wall2) //wall+corner, this tile can't ever exist on the edges so checking for X/Y -1 / +1 is totally fine
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sy][xP + sx].uv[2] = tex0[2];
			}

		m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[m_Map[y][x].owner];
		//for debugging
		//m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[Owner_PlayerBlue];
		bool northWall = m_Map[y + 1][x].type == Type_Wall0;// && m_Map[y + 1][x].type <= Type_Wall5;
		bool southWall = m_Map[y - 1][x].type == Type_Wall0;// && m_Map[y - 1][x].type <= Type_Wall5;
		bool eastWall = m_Map[y][x + 1].type == Type_Wall0;//&& m_Map[y][x + 1].type <= Type_Wall5;
		bool westWall = m_Map[y][x - 1].type == Type_Wall0;//&& m_Map[y][x - 1].type <= Type_Wall5;

																	//bool northWall = IsWall(m_Map[y + 1][x].type);// && m_Map[y + 1][x].type <= Type_Wall5;
																	//bool southWall = IsWall(m_Map[y - 1][x].type);// && m_Map[y - 1][x].type <= Type_Wall5;
																	//bool eastWall = IsWall(m_Map[y][x + 1].type);//&& m_Map[y][x + 1].type <= Type_Wall5;
																	//bool westWall = IsWall(m_Map[y][x - 1].type);//&& m_Map[y][x - 1].type <= Type_Wall5;
																	//corner pieces
		if (northWall && eastWall)
		{
			m_BlockMap[5][yP + 2][xP + 2].uv[2] = tex0[0];
		}
		if (northWall && westWall)
		{
			m_BlockMap[5][yP + 2][xP].uv[2] = tex0[0];
		}
		if (southWall && eastWall)
		{
			m_BlockMap[5][yP][xP + 2].uv[2] = tex0[0];
		}
		if (southWall && westWall)
		{
			m_BlockMap[5][yP][xP].uv[2] = tex0[0];
		}

		//uint8_t northInfo = GetNeighbourInfo(type, m_Map[y + 1][x].type);
		//bool northWalkable = (northInfo != N_DIFF && northInfo != N_SAME);
		//uint8_t southInfo = GetNeighbourInfo(type, m_Map[y - 1][x].type);
		//bool southWalkable = (southInfo != N_DIFF && southInfo != N_SAME);
		//uint8_t eastInfo = GetNeighbourInfo(type, m_Map[y][x + 1].type);
		//bool eastWalkable = (eastInfo != N_DIFF && eastInfo != N_SAME);
		//uint8_t westInfo = GetNeighbourInfo(type, m_Map[y][x - 1].type);
		//bool westWalkable = (westInfo != N_DIFF && westInfo != N_SAME);

		uint8_t northInfo = neighbour.North;
		bool northWalkable = (neighbour.North != N_DIFF && neighbour.North != N_SAME);
		uint8_t southInfo = neighbour.South;
		bool southWalkable = (neighbour.South != N_DIFF && neighbour.South != N_SAME);
		uint8_t eastInfo = neighbour.East;
		bool eastWalkable = (neighbour.East != N_DIFF && neighbour.East != N_SAME);
		uint8_t westInfo = neighbour.West;
		bool westWalkable = (neighbour.West != N_DIFF && neighbour.West != N_SAME);

		if (northWalkable)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP + 2][xP + sy].uv[2] = tex0[1];
			}
			uint16_t texIndexRoom = TypeToTexture(m_Map[y + 1][x].type);
			int randValue = m_BlockMap[7][yP + 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + 2][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (southWalkable)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP][xP + sy].uv[2] = tex0[1];
			}
			uint16_t texIndexRoom = TypeToTexture(m_Map[y - 1][x].type);
			int randValue = m_BlockMap[7][yP - 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (eastWalkable)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP + 2].uv[2] = tex0[1];
			}
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x + 1].type);
			int randValue = m_BlockMap[7][yP][xP - 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP + 2].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (westWalkable)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP].uv[2] = tex0[1];
			}
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x - 1].type);
			int randValue = m_BlockMap[7][yP][xP + 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}

		if (neighbour.West == N_WALKABLE || neighbour.West == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x - 1].type);
			int randValue = m_BlockMap[7][yP][xP + 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.East == N_WALKABLE || neighbour.East == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x + 1].type);
			int randValue = m_BlockMap[7][yP][xP - 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP + 2].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.North == N_WALKABLE || neighbour.North == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y + 1][x].type);
			int randValue = m_BlockMap[7][yP + 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + 2][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.South == N_WALKABLE || neighbour.South == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y - 1][x].type);
			int randValue = m_BlockMap[7][yP - 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}

		if ((neighbour.NorthEast != N_SAME && neighbour.NorthEast != N_DIFF) && ((northWalkable && eastWalkable) || (northInfo == N_SAME && eastInfo == N_SAME)))
		{
			m_BlockMap[5][yP + 2][xP + 2].uv[2] = tex0[0];
		}
		if (neighbour.NorthWest != N_SAME && neighbour.NorthWest != N_DIFF && ((northWalkable && westWalkable) || (northInfo == N_SAME && westInfo == N_SAME)))
		{
			m_BlockMap[5][yP + 2][xP].uv[2] = tex0[0];
		}
		if (neighbour.SouthEast != N_SAME && neighbour.SouthEast != N_DIFF && ((southWalkable && eastWalkable) || (southInfo == N_SAME && eastInfo == N_SAME)))
		{
			m_BlockMap[5][yP][xP + 2].uv[2] = tex0[0];
		}
		if (neighbour.SouthWest != N_SAME && neighbour.SouthWest != N_DIFF && ((southWalkable && westWalkable) || (southInfo == N_SAME && westInfo == N_SAME)))
		{
			m_BlockMap[5][yP][xP].uv[2] = tex0[0];
		}
	}
	else if (type == Type_Wall3) //corner
	{
		const std::vector<XMFLOAT2>& tex0 = BlockTextures[texIndex].top[0];
		for (int sy = 0; sy < 3; sy++)
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sy][xP + sx].uv[2] = tex0[2];
			}
		m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[m_Map[y][x].owner];
		//for debugging
		//m_BlockMap[5][yP + 1][xP + 1].uv[2] = t_WallOwners[Owner_PlayerRed];
		bool northWall = neighbour.North == N_SAME;
		bool southWall = neighbour.South == N_SAME;
		bool eastWall = neighbour.East == N_SAME;
		bool westWall = neighbour.West == N_SAME;

		//mark corner pieces
		if (northWall)
		{
			if (eastWall && neighbour.NorthEast == N_WALKABLE)
			{
				m_BlockMap[5][yP + 2][xP + 2].uv[2] = tex0[0];
			}
			if (westWall && neighbour.NorthWest == N_WALKABLE)
			{
				m_BlockMap[5][yP + 2][xP].uv[2] = tex0[0];
			}
		}
		if (southWall)
		{
			if (eastWall && neighbour.SouthEast == N_WALKABLE)
			{
				m_BlockMap[5][yP][xP + 2].uv[2] = tex0[0];
			}
			if (westWall && neighbour.SouthWest == N_WALKABLE)
			{
				m_BlockMap[5][yP][xP].uv[2] = tex0[0];
			}
		}

		//top edges mark a walkable area next to it
		if (neighbour.North != N_SAME && neighbour.North != N_DIFF)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP + 2][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.South != N_SAME && neighbour.South != N_DIFF)
		{
			for (int sy = 0; sy < 3; sy++)
			{
				m_BlockMap[5][yP][xP + sy].uv[2] = tex0[1];
			}
		}
		if (neighbour.East != N_SAME && neighbour.East != N_DIFF)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP + 2].uv[2] = tex0[1];
			}
		}
		if (neighbour.West != N_SAME && neighbour.West != N_DIFF)
		{
			for (int sx = 0; sx < 3; sx++)
			{
				m_BlockMap[5][yP + sx][xP].uv[2] = tex0[1];
			}
		}

		

		//Set the wall side to their respective texture
		if (neighbour.West == N_WALKABLE || neighbour.West == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x - 1].type);
			int randValue = m_BlockMap[7][yP][xP + 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					//maps | sx + (3 * (3 - (z - 2))) |  to a value within 3*4 area, upper left being 0, lower right being 8
					m_BlockMap[z][yP + sx][xP].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.East == N_WALKABLE || neighbour.East == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y][x + 1].type);
			int randValue = m_BlockMap[7][yP][xP - 3].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + sx][xP + 2].uv[0] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.North == N_WALKABLE || neighbour.North == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y + 1][x].type);
			int randValue = m_BlockMap[7][yP + 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP + 2][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
		if (neighbour.South == N_WALKABLE || neighbour.South == N_WATER)
		{
			uint16_t texIndexRoom = TypeToTexture(m_Map[y - 1][x].type);
			int randValue = m_BlockMap[7][yP - 3][xP].randValue;//we use the random value of one block as the entire side will need to match
			const std::vector<XMFLOAT2>& tex1 = BlockTextures[texIndexRoom].wall[randValue % BlockTextures[texIndexRoom].wall.size()];
			for (int sx = 0; sx < 3; sx++)
			{
				for (int z = 2; z < MAP_SIZE_HEIGHT - 2; z++)
				{
					m_BlockMap[z][yP][xP + sx].uv[1] = tex1[sx + (3 * (3 - (z - 2)))];
				}
			}
		}
	}
}
uint16_t Level::HandleSpecialCases(int yPos, int xPos)
{
	uint16_t type = m_Map[yPos][xPos].type & 0xFF;
	if (type != Type_Dungeon_Heart && type != Type_Portal) return type;
	if (yPos >= 1 && yPos < 84 && xPos >= 1 && xPos < 84)
	{
		if (L8(m_Map[yPos - 1][xPos].type) != type && L8(m_Map[yPos + 1][xPos].type) == type) // U
		{
			if (L8(m_Map[yPos][xPos - 1].type) == type && L8(m_Map[yPos][xPos + 1].type) != type) // LU
			{
				return type + (3 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) == type) // MU
			{
				return type + (2 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) != type) // RU
			{
				return type + (1 << 8);
			}
		}
		if (L8(m_Map[yPos + 1][xPos].type) != type && L8(m_Map[yPos - 1][xPos].type) == type) // L
		{
			if (L8(m_Map[yPos][xPos - 1].type) == type && L8(m_Map[yPos][xPos + 1].type) != type) // LL
			{
				return type + (9 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) == type) // ML
			{
				return type + (8 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) != type) // RL
			{
				return type + (7 << 8);
			}
		}
		if (L8(m_Map[yPos + 1][xPos].type) == type && L8(m_Map[yPos - 1][xPos].type) == type) // M
		{
			if (L8(m_Map[yPos][xPos - 1].type) == type && L8(m_Map[yPos][xPos + 1].type) != type) // LM
			{
				return type + (6 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) == type) // MM
			{
				if (L8(m_Map[yPos + 1][xPos + 1].type) == type && L8(m_Map[yPos - 1][xPos + 1].type) == type && L8(m_Map[yPos + 1][xPos - 1].type) == type && L8(m_Map[yPos - 1][xPos - 1].type) == type)
				{
					return type + (10 << 8);
				}
				return type + (5 << 8);
			}
			if (L8(m_Map[yPos][xPos + 1].type) == type && L8(m_Map[yPos][xPos - 1].type) != type) // RM
			{
				return type + (4 << 8);
			}
		}
	}
	return type + (5 << 8); //Middle tile
}

void Level::UpdateWalls(int y, int x)
{
	uint16_t type = m_Map[y][x].type;
	TileNeighbours neighbour = CheckNeighbours(type, y, x);

	bool northWalkable = m_Map[y + 1][x].type > Type_Wall5;
	bool southWalkable = m_Map[y - 1][x].type > Type_Wall5;
	bool eastWalkable = m_Map[y][x + 1].type > Type_Wall5;
	bool westWalkable = m_Map[y][x - 1].type > Type_Wall5;
	uint8_t numWalkable = northWalkable + southWalkable + eastWalkable + westWalkable;

	if (type == Type_Wall3)
	{
		if (northWalkable && southWalkable || eastWalkable && westWalkable)
		{
			m_Map[y][x].type = Type_Wall0;
		}
		else if(numWalkable >= 1)
		{
			m_Map[y][x].type = Type_Wall2;
		}
	}
	else if (type == Type_Wall2)
	{
		if (northWalkable && southWalkable || eastWalkable && westWalkable)
		{
			m_Map[y][x].type = Type_Wall0;
		}
	}
}
void Level::DestroyTile(int y, int x)
{
	uint16_t type = m_Map[y][x].type;
	if (type == Type_Earth || type == Type_Earth_Torch || type >= Type_Wall0 && type <= Type_Wall5 || type == Type_Gold)
	{
		m_Map[y][x].type = Type_Unclaimed_Path;
		m_Map[y][x].owner = Owner_PlayerNone;

		//first we loop to update all the walls
		for (int sy = -1; sy <= 1; sy++)
		{
			for (int sx = -1; sx <= 1; sx++)
			{
				UpdateWalls(y + sy, x + sx);
			}
		}

		//then update the area surrounding this
		UpdateArea(y - 2, y + 2, x - 2, x + 2);
	}
}
//in Tile Positions
void Level::UpdateArea(int minY, int maxY, int minX, int maxX)
{
	RenderTile tileOut = {};
	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			m_Map[y][x].type = HandleSpecialCases(y, x);
			size_t numBlocks = CreateFromTile(m_Map[y][x], tileOut);
			m_Map[y][x].numBlocks = numBlocks;
			//Update area surrounding this
			for (size_t yy = 0; yy < 3; yy++)
			{
				for (size_t xx = 0; xx < 3; xx++)
				{
					for (size_t z = 0; z < 8; z++)
					{
						assert(y + yy < 256);
						assert(x + xx < 256);
						Block& b = tileOut.subTile[yy][xx][z];
						b.randValue = m_BlockMap[z][y * 3 + yy][x * 3 + xx].randValue;
						m_BlockMap[z][y * 3 + yy][x * 3 + xx] = b;
					}
				}
			}
			DoUVs(m_Map[y][x].type, y, x);
		}
	}
}
bool HasLandNeighbour(Level* l, const int pos[4], int position, int y, int x)
{
	//for (size_t i = 0; i < 4; i++)
	{
		//if (abs(pos[i] - position) < 2)
		{
			TileNeighbourTiles n = l->GetNeighbourTiles(y, x);
			for (size_t j = 0; j < 4; j++)
			{
				if (n.Axii[j].type == Type_Claimed_Land || n.Axii[j].type == Type_Unclaimed_Path)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void Level::ClaimRoom(uint8_t newOwner, uint16_t type , int ty, int tx)
{
	//if this tile is already ours, return
	if (newOwner == m_Map[ty][tx].owner) return;

	//tile is of a different type, return
	uint16_t nType = m_Map[ty][tx].type & 0xFF;
	if (type != nType) return;

	m_Map[ty][tx].owner = newOwner;

	ClaimRoom(newOwner, type, ty - 1, tx);
	ClaimRoom(newOwner, type, ty + 1, tx);
	ClaimRoom(newOwner, type, ty , tx - 1);
	ClaimRoom(newOwner, type, ty , tx + 1);

	UpdateArea(ty - 1, ty + 1, tx - 1, tx + 1);
}
bool Level::IsClaimableCorner(int y, int x)
{
	TileNeighbourTiles n = GetNeighbourTiles(y, x);
	if ((   n.SouthEast.type == Type_Claimed_Land || IsOwnedRoom(n.SouthEast.owner,y-1,x+1)) && (IsWall(n.South.type) && IsWall(n.East.type))
		|| (n.SouthWest.type == Type_Claimed_Land || IsOwnedRoom(n.SouthWest.owner, y - 1, x - 1)) && (IsWall(n.South.type) && IsWall(n.West.type))
		|| (n.NorthEast.type == Type_Claimed_Land || IsOwnedRoom(n.NorthEast.owner, y + 1, x + 1)) && (IsWall(n.North.type) && IsWall(n.East.type))
		|| (n.NorthWest.type == Type_Claimed_Land || IsOwnedRoom(n.NorthWest.owner, y + 1, x - 1)) && (IsWall(n.North.type) && IsWall(n.West.type)))
	{
		return true;
	}
	return false;
}
bool IsClaimableRoom(uint16_t type)
{
 	return ((type >= Type_Portal && type <= Type_Barracks || type == Type_Bridge || type == Type_Guardpost) && type != Type_Dungeon_Heart);
}
bool Level::IsOwnedRoom(uint8_t player, int y, int x)
{
	uint16_t type = m_Map[y][x].type & 0xFF;
	uint8_t owner = m_Map[y][x].owner;

	bool isRoom = type >= Type_Portal && type <= Type_Barracks || type == Type_Bridge || type == Type_Guardpost || type == Type_Dungeon_Heart;
	
	return (isRoom && owner == player);
}
//This function should never be able to be called on y/x == 0 or y/x == MAP_SIZE_TILES
void Level::ClaimTile(uint8_t player, int y, int x)
{
	uint16_t type = m_Map[y][x].type & 0xFF;
	TileNeighbourTiles neighbourTiles = GetNeighbourTiles(y, x);

	if ((neighbourTiles.North.type == Type_Claimed_Land && neighbourTiles.North.owner == player) || IsOwnedRoom(player,y+1,x)
		|| (neighbourTiles.South.type == Type_Claimed_Land && neighbourTiles.South.owner == player) || IsOwnedRoom(player, y - 1, x)
		|| (neighbourTiles.East.type == Type_Claimed_Land && neighbourTiles.East.owner == player) || IsOwnedRoom(player, y, x + 1)
		|| (neighbourTiles.West.type == Type_Claimed_Land && neighbourTiles.West.owner == player) || IsOwnedRoom(player, y, x - 1))
	{
		if (type == Type_Unclaimed_Path)
		{
			m_Map[y][x].type = Type_Claimed_Land;
			m_Map[y][x].owner = player;
			UpdateArea(y - 1, y + 1, x - 1, x + 1);
		}
		if (type == Type_Earth || type == Type_Earth_Torch)
		{
			m_Map[y][x].type = Type_Wall2;
			m_Map[y][x].owner = player;
			TileNeighbours neighbour = CheckNeighbours(Type_Wall2, y, x);

			int xPos[4] = { -100 }; //some tile far enough to never have a neigbour
			int yPos[4] = { -100 }; //some tile far enough to never have a neigbour
			if (neighbourTiles.North.owner == player && neighbour.North == N_WALKABLE)
			{
				yPos[0] = y + 1;
				xPos[0] = x;
			}
			if (neighbourTiles.South.owner == player && neighbour.South == N_WALKABLE)
			{
				yPos[2] = y - 1;
				xPos[2] = x;
			}
			if (neighbourTiles.East.owner == player && neighbour.East == N_WALKABLE)
			{
				yPos[1] = y;
				xPos[1] = x + 1;
			}
			if (neighbourTiles.West.owner == player && neighbour.West == N_WALKABLE)
			{
				yPos[3] = y;
				xPos[3] = x - 1;
			}
			if ((neighbourTiles.North.type == Type_Earth || neighbourTiles.North.type == Type_Earth_Torch) && (neighbour.NorthEast == N_SAME || neighbour.NorthWest == N_SAME))
			{
				if (IsClaimableCorner(y+1,x))
				{
					m_Map[y + 1][x].type = Type_Wall3;
					m_Map[y + 1][x].owner = player;
				}
			}
			if ((neighbourTiles.South.type == Type_Earth || neighbourTiles.South.type == Type_Earth_Torch) && (neighbour.SouthEast == N_SAME || neighbour.SouthWest == N_SAME))
			{
				if (IsClaimableCorner(y - 1, x))
				{
					m_Map[y - 1][x].type = Type_Wall3;
					m_Map[y - 1][x].owner = player;
				}
			}
			if ((neighbourTiles.East.type == Type_Earth || neighbourTiles.East.type == Type_Earth_Torch) && (neighbour.NorthEast == N_SAME || neighbour.SouthEast == N_SAME))
			{
				if (IsClaimableCorner(y, x + 1))
				{
					m_Map[y][x + 1].type = Type_Wall3;
					m_Map[y][x + 1].owner = player;
				}
			}
			if ((neighbourTiles.West.type == Type_Earth || neighbourTiles.West.type == Type_Earth_Torch) && (neighbour.NorthWest == N_SAME || neighbour.SouthWest == N_SAME))
			{
				if (IsClaimableCorner(y, x - 1))
				{
					m_Map[y][x - 1].type = Type_Wall3;
					m_Map[y][x - 1].owner = player;
				}
			}
			UpdateWalls(y, x);

			UpdateArea(y - 1, y + 1, x - 1, x + 1);
		}
		if (IsClaimableRoom(type))
		{
			ClaimRoom(player, type, y, x);
		}
	}
}
void Level::Update(float delta)
{
	Game* g = Themp::System::tSys->m_Game;
	XMFLOAT3 camPos = g->m_Camera->GetPosition();

	int camTilePosX = (camPos.x + (MAP_SIZE_SUBTILES_RENDER / 2)) / 3;
	int camTilePosY = (camPos.z + (MAP_SIZE_SUBTILES_RENDER / 2)) / 3 + 1;

	m_MapObject->Update(delta);

	float uiMouseX = 0, uiMouseY = 0;
	Game::TranslateMousePos(g->m_CursorWindowedX, g->m_CursorWindowedY, uiMouseX, uiMouseY);
	m_Cursor->m_Renderable->SetPosition(uiMouseX + cursorOffset.x, uiMouseY - cursorOffset.y, 0.01);

	//Some level stuff
	ImGui::Checkbox("Wireframe", &D3D::s_D3D->m_Wireframe);
	if (ImGui::Button("Destroy Tile"))
	{
		DestroyTile(camTilePosY, camTilePosX);
	}
	if (ImGui::Button("Claim Tile Red"))
	{
		ClaimTile(Owner_PlayerRed, camTilePosY, camTilePosX);
	}
	//if (ImGui::Button("Claim Tile Blue"))
	//{
	//	ClaimTile(Owner_PlayerBlue, camTilePosY, camTilePosX);
	//}
	//if (ImGui::Button("Claim Tile Green"))
	//{
	//	ClaimTile(Owner_PlayerGreen, camTilePosY, camTilePosX);
	//}
	//if (ImGui::Button("Claim Tile Yellow"))
	//{
	//	ClaimTile(Owner_PlayerYellow, camTilePosY, camTilePosX);
	//}
	//if (ImGui::Button("Claim Tile White"))
	//{
	//	ClaimTile(Owner_PlayerWhite, camTilePosY, camTilePosX);
	//}
	//if (ImGui::Button("Claim Tile None"))
	//{
	//	ClaimTile(Owner_PlayerNone, camTilePosY, camTilePosX);
	//}



	tileIndicator->SetPosition((camTilePosX-42)*3, 6, (camTilePosY-42)*3);


	//Update the map
	m_MapObject->ConstructFromLevel(camPos.x,camPos.z);
}
void Level::LoadLevelData()
{
	//"MAP00001"
	std::wstring LevelPath = L"LEVELS\\MAP0";

	//local scope so we don't waste the variable names
	{
		wchar_t buf[32]; //should only ever have 4 or less chars, but safety first..
		memset(buf, 0, 32);
		_itow(m_CurrentLevelNr, buf, 10); //Though I guess I shouldn't be using such a unsafe function then heh...
		std::wstring levelNr = L"";

		//Now we add the pre-trailing 0's since itoa 1 == "1" and we need "0001" or 10 being "0010"
		if (m_CurrentLevelNr < 10)
		{
			levelNr.append(3, L'0'); //000
		}
		else if (m_CurrentLevelNr < 100)
		{
			levelNr.append(2, L'0'); //00
		}
		else if (m_CurrentLevelNr < 1000)
		{
			levelNr.append(1, L'0'); //0
		}
		levelNr.append(buf); //append the resulting number to the zero's resulting in XXXN (X being a 0 or number)
		LevelPath.append(levelNr); //append it to the complete path "LEVELS\\MAP0NNNN" 
	}

	//All these files are needed to completely load a single level, so I guess we'll just grab em already.
	//Source: http://keeper.lubiki.pl/dk1_docs/dk_mapfiles_ref.htm


	//Description: Gives location of action points for reference in script (.txt) files.
	//Format:
	//4 bytes : Number of 8 byte entries.
	//Each entry :
	//	4 bytes : XY Location
	//	2 bytes : Range
	//	2 bytes : Action point number(points are usually in order, but they don't have to be.
	FileData map_apt = FileManager::GetFileData(LevelPath + L".apt");

	//Description:Along with the.dat file, this determines the graphics for the level.Each entry gives the data for a subtile.
	//Format :
	//8 bytes : Number of 24 byte entries(always 2048).
	//Each entry :
	//24 bytes :
	//	0 - 1 : Use - Counter of the column uses for non - permanent columns; For permanent colums, this value has an offset.
	//	2: Byte with bitfields :
	//Bit 0 : Permanent - if set, the column is never overwritten
	//	Bits 1 - 3 : Lintel(unknown)
	//	Bits 4 - 7 : Height - in subtiles; proper values are 0..7
	//	3 - 4: Solidmask - appears to be a bitmask for which of the cubes are non - zero, with the lowest cube being represented by the least significant bit.
	//	5 - 6 : Baseblock - determines the very lowest level(below the lowest cube).I think the block numbers here are different to the ones used by the cubes, but I'm not sure yet. This entry is mostly useful for water, lava, and hatcheries.
	//	7 : Orientation(always 0)
	//	8 - 23 : Cubes - there are 8 cubes, each having a 2 byte entry.The first cube usually forms the floor, etc, up to the ceiling.
	FileData map_clm = FileManager::GetFileData(LevelPath + L".clm");

	//Description: Determines the graphics at the start of the game.When new areas are dug out or rooms are changed, computer automatically generates different graphics for these areas, but otherwise everything is in this file.
	//Format : Subtile map format, with 2 bytes per entry.
	FileData map_dat = FileManager::GetFileData(LevelPath + L".dat");

	//Description: Specifies the landscape style, or in other words, texture file number(making the dungeon icy, for instance).
	//Format : Always 1 byte, which is a 0 for all but the following maps :
	//	 01 for maps 13, 14, 15, 16, 17, 104
	//	 02 for maps 9, 10
	//	 03 for maps 19, 20
	FileData map_inf = FileManager::GetFileData(LevelPath + L".inf");

	//Description: Provides extra lighting.
	//Format :
	//4 bytes : Number of 20 byte entries.
	//Each entry :
	//	 2 bytes : Range
	//	 1 byte : Light intensivity
	//	 7 bytes : unknown
	//	 6 bytes : XYZ Location
	//	 2 bytes : unknown.
	//	 2 bytes : always - 1 (0xffff).
	FileData map_lgt = FileManager::GetFileData(LevelPath + L".lgt");

	//Description: Determine ownership of tiles.
	//Notes: in subtile format. (each tile = 3x3 subtiles)
	FileData map_own = FileManager::GetFileData(LevelPath + L".own");

	//Description: Determines type of "room" each tile is.
	//Format: 85 lines of 85 tiles, each having a two byte entry.Each entry only uses the first byte, the second byte being 0.
	FileData map_slb = FileManager::GetFileData(LevelPath + L".slb");

	//Description: List of "things" in the dungeon.
	//Format:
	//2 bytes : Number of 21 byte entries
	//Each entry :
	//	0 - 5 : Location
	//	6 : Thing type
	//	7 : Thing subtype(eg creature race, trap type)
	//	8 : Owner
	//	9 - 20 : Type specific data
	FileData map_tng = FileManager::GetFileData(LevelPath + L".tng");

	//Description: Level script which determines victory conditions, enemy keepers actions, training mode, etc.
	//Format: Text file.
	//This is a pretty simple scripting language. There are no loops or jumps, and condition testing is very basic. There are only three statement forms - conditions, commands, and condition terminators. Conditions are naturally nested, but that's about as complicated as it gets.
	//Each line contains a single instruction.Whitespace in the form of spaces and tabs is ignored.Comments begin with REM.All unconditional commands(ie ones outside any condition statements) are executed at the beginning of the game, then ignored.All conditions are checked at regular intervals.
	FileData map_txt = FileManager::GetFileData(LevelPath + L".txt");

	//Description: Determines which subtiles are animated.
	//Format : Subtile map with single byte entries.Each byte is 00, 01, or 02 :
	//00 : Display the subtile without any change
	//01 : Skew the subtile slightly(default)
	//02 : Animate the subtile
	FileData map_wib = FileManager::GetFileData(LevelPath + L".wib");

	//Description: Determines type of liquid each tile is. (Water/Lava)
	//Format: 85 lines of 85 tiles, each having one byte entry.
	FileData map_wlb = FileManager::GetFileData(LevelPath + L".wlb");


	int slb_index = 0;
	for (int y = MAP_SIZE_TILES - 1; y >= 0; y--)
	{
		for (int x = 0; x < MAP_SIZE_TILES; x++)
		{
			BYTE tile = map_slb.data[slb_index++];
			slb_index++; //second byte is always 0 so we skip it
			if (tile >= Type_Wall0 && tile <= Type_Wall5)
			{
				tile = Type_Wall0;
			}
			Tile& mapTile = m_Map[y][x];
			mapTile.type = tile;
		}
	}
	Tile m_AdjustedMap[85][85];
	memcpy(m_AdjustedMap, m_Map, sizeof(m_Map));

	int own_index = 0; //skip the first rown of subtiles
	int yIndex = 0;
	int xIndex = 0;
	for (int y = MAP_SIZE_SUBTILES - 1; y >= 0;)
	{
		if (y == 255)
		{
			//skip this line of subtiles
			y--;
			continue;
		}
		for (int x = 0; x < MAP_SIZE_SUBTILES;)
		{
			if (x == 255)
			{
				//skip this subtile
				own_index++;
				x++;
				continue;
			}
			assert(own_index < map_own.size);
			uint8_t owner = map_own.data[own_index];
			m_AdjustedMap[yIndex][xIndex].owner = owner;
			own_index += 3; //skip a whole tile
			x += 3;
			xIndex++;
		}
		y -= 3;
		own_index = MAP_SIZE_SUBTILES * y;
		xIndex = 0;
		yIndex++;
	}

	for (int y = 0; y < MAP_SIZE_TILES; y++)
	{
		for (int x = 0; x < MAP_SIZE_TILES; x++)
		{
			Tile& mapTile = m_Map[y][x];
			if (mapTile.type == Type_Wall0)
			{
				m_AdjustedMap[y][x].type = Type_Wall0;
				bool northWall = m_Map[y + 1][x].type == Type_Wall0;
				bool southWall = m_Map[y - 1][x].type == Type_Wall0;
				bool westWall = m_Map[y][x - 1].type == Type_Wall0;
				bool eastWall = m_Map[y][x + 1].type == Type_Wall0;
				bool northRock = m_Map[y + 1][x].type == Type_Rock || m_Map[y + 1][x].type == Type_Earth;
				bool southRock = m_Map[y - 1][x].type == Type_Rock || m_Map[y - 1][x].type == Type_Earth;
				bool westRock = m_Map[y][x - 1].type == Type_Rock || m_Map[y][x - 1].type == Type_Earth;
				bool eastRock = m_Map[y][x + 1].type == Type_Rock || m_Map[y][x + 1].type == Type_Earth;

				if (southWall)
				{//southern piece is a walltype
					if (eastWall)
					{//eastern piece is a walltype
						if (northRock && westRock)
						{ //both other sides are not walls
							m_AdjustedMap[y][x].type = Type_Wall3; //This is a corner piece
						}
						else if (!northWall || !westWall)
						{//only 1 side is not a wall
							m_AdjustedMap[y][x].type = Type_Wall2; //this is a wall AND a cornerpiece
						}
						else
						{//otherwise this is a wall
							m_AdjustedMap[y][x].type = Type_Wall0;
						}
					}
					else if (westWall)
					{//Western piece is a walltype
						if (northRock && eastRock)
						{ //both other sides are not walls
							m_AdjustedMap[y][x].type = Type_Wall3; //This is a corner piece
						}
						else if (!northWall || !eastWall)
						{//only 1 northRock is not a wall
							m_AdjustedMap[y][x].type = Type_Wall2; //this is a wall AND a cornerpiece
						}
						else
						{//otherwise this is a wall
							m_AdjustedMap[y][x].type = Type_Wall0;
						}
					}
				}
				if (northWall)
				{//northern piece is a walltype
					if (eastWall)
					{//eastern piece is a walltype
						if (southRock && westRock)
						{ //both other sides are not walls
							m_AdjustedMap[y][x].type = Type_Wall3; //This is a corner piece
						}
						else if (!southWall || !eastWall)
						{//only 1 side is not a wall
							m_AdjustedMap[y][x].type = Type_Wall2; //this is a wall AND a cornerpiece
						}
						else
						{//otherwise this is a wall
							m_AdjustedMap[y][x].type = Type_Wall0;
						}
					}
					else if (westWall)
					{//Western piece is a walltype
						if (southRock && eastRock)
						{ //both other sides are not walls
							m_AdjustedMap[y][x].type = Type_Wall3; //This is a corner piece
							continue;
						}
						else if (!southWall || !eastWall)
						{//only 1 side is not a wall
							m_AdjustedMap[y][x].type = Type_Wall2; //this is a wall AND a cornerpiece
							continue;
						}
						else
						{//otherwise this is a wall
							m_AdjustedMap[y][x].type = Type_Wall0;
							continue;

						}
					}
				}
				
			}
		}
	}

	memcpy(m_Map, m_AdjustedMap, sizeof(m_Map));
	memcpy(m_OriginalMap, m_Map, sizeof(m_Map));
}
//TODO: Move this to the Level file, update that data instead of recreating the visible map every frame, this will result in an up-to-date map even for non visble areas, when combined with a "Place Room /Destroy Room" function, which updates the surrounding tiles
int Level::CreateFromTile(const Tile& tile, RenderTile& out)
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
