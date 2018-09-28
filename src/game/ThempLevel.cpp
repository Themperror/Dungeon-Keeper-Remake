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

Level::Level(int levelIndex)
{
	m_CurrentLevelNr = levelIndex;
	m_Cursor = new Object2D(Object2D::Source::sMENU_CURSOR, 1, true);
	m_Cursor->SetVisibility(true);
	System::tSys->m_Game->AddObject3D(m_Cursor->m_Renderable);
	LoadLevelData();
	m_MapObject = new VoxelObject();
	m_MapObject->m_Obj3D->m_Meshes[0]->m_Material->SetTexture(FileManager::GetBlockTexture(0));
	Themp::System::tSys->m_Game->AddObject3D(m_MapObject->m_Obj3D);
}
//void Level::Cull()
//{
//	Game* g = Themp::System::tSys->m_Game;
//	const XMFLOAT3& p = g->m_Camera->GetPosition();
//	for (size_t y = 0; y < 86; y++)
//	{
//		for (size_t x = 0; x < 86; x++)
//		{
//			const size_t coord = x + y * 86;
//			if (p.x + 8 > x || p.x - 8 < x)
//			{
//				for (size_t i = 0; i < 9; i++)
//				{
//					for (size_t h = 0; h < 8; h++)
//					{
//						if(m_Map[coord].subTiles[i].column[h].obj)
//							m_Map[coord].subTiles[i].column[h].obj->isVisible = false;
//					}
//				}
//				continue;
//			}
//			if (p.z + 6 > y || p.z - 8 < y)
//			{
//				for (size_t i = 0; i < 9; i++)
//				{
//					for (size_t h = 0; h < 8; h++)
//					{
//						if (m_Map[coord].subTiles[i].column[h].obj)
//							m_Map[coord].subTiles[i].column[h].obj->isVisible = false;
//					}
//				}
//				continue;
//			}
//			for (size_t i = 0; i < 9; i++)
//			{
//				for (size_t h = 0; h < 8; h++)
//				{
//					if (m_Map[coord].subTiles[i].column[h].obj)
//						m_Map[coord].subTiles[i].column[h].obj->isVisible = true;
//				}
//			}
//		}
//	}
//}
void Level::Update(float delta)
{
	Game* g = Themp::System::tSys->m_Game;

	float uiMouseX = 0, uiMouseY = 0;
	Game::TranslateMousePos(g->m_CursorWindowedX, g->m_CursorWindowedY, uiMouseX, uiMouseY);
	m_Cursor->m_Renderable->SetPosition(uiMouseX + cursorOffset.x, uiMouseY - cursorOffset.y, 0.01);

	XMFLOAT3 camPos = g->m_Camera->GetPosition();
	m_MapObject->ConstructFromLevel(this,camPos.x,camPos.z);
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
	for (int y = MAP_SIZE_TILES-1; y >= 0; y--)
	{
		for (size_t x = 0; x < MAP_SIZE_TILES; x++)
		{
			BYTE tile = map_slb.data[slb_index++];
			slb_index++; //second byte is always 0 so we skip it
			Tile& mapTile = m_Map[y][x];
			mapTile.type = tile;
		}
	}
}