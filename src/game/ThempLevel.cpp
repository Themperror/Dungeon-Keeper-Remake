#include "ThempSystem.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempObject2D.h"
#include "ThempGUIButton.h"
#include "ThempFont.h"
#include "ThempResources.h"
#include "ThempVoxelObject.h"
#include "ThempEntity.h"
#include "ThempLevelScript.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include "Players/ThempPlayer.h"
#include "Players/ThempCPUPlayer.h"
#include "Players/ThempGoodPlayer.h"
#include "Players/ThempNeutralPlayer.h"
#include "ThempCreature.h"
#include <DirectXMath.h>
using namespace Themp;

Level* Level::s_CurrentLevel = nullptr;
BYTE* m_MiniMapScratchData = nullptr;

Level::~Level()
{
	delete[] m_MiniMapScratchData;
	delete m_MapObject;
	delete m_MinimapData.texture;
	delete m_LevelData;
	delete m_LevelScript;
	if(m_Pather)
		delete m_Pather;
	for (int i = 0; i < 6; i++)
	{
		if (m_Players[i] != nullptr)
		{
			delete m_Players[i];
		}
	}
}
XMFLOAT2 cursorOffset = XMFLOAT2(0.03f, 0);

Object3D* tileIndicator = nullptr;


Level::Level(int levelIndex)
{
	s_CurrentLevel = this;
	m_LevelData = new LevelData(levelIndex);
	m_LevelScript = new LevelScript(LevelData::LevelIDtoString(levelIndex));
	m_MapObject = new VoxelObject(m_LevelData);
	m_MapObject->m_Obj3D->m_Meshes[0]->m_Material->SetTexture(FileManager::GetBlockTexture(0));
	Themp::System::tSys->m_Game->AddObject3D(m_MapObject->m_Obj3D);
	m_Players[Owner_PlayerRed] = new Player(Owner_PlayerRed);
	//White is the "Good" player, always exists
	m_Players[Owner_PlayerWhite] = new GoodPlayer(Owner_PlayerWhite);
	//None is the "Neutral" player, always exists
	m_Players[Owner_PlayerNone] = new NeutralPlayer(Owner_PlayerNone);

	//System::tSys->m_Game->m_Camera->SetOrtho(20, 20);
	//System::tSys->m_Game->m_Camera->SetProjection(Camera::CameraType::Orthographic)

	for (int i = 0; i < 6; i++)
	{
		Creature* creature = new Themp::Creature(CreatureData::CREATURE_IMP);
		m_Players[Owner_PlayerRed]->m_Creatures.push_back(creature);
		System::tSys->m_Game->AddCreature(creature);
		creature->m_Renderable->SetPosition(42*3 +i, 2, 43*3);
	}
	LevelScript::GameValues[Owner_PlayerRed]["TOTAL_IMPS"] = 6;


	tileIndicator = new Object3D();
	tileIndicator->CreateCube("indicator", false);
	tileIndicator->SetScale(3.05f, 5, 3.05f);
	tileIndicator->m_BackfaceCull = false;
	System::tSys->m_Game->AddObject3D(tileIndicator);


	m_MiniMapScratchData = new BYTE[MAP_SIZE_SUBTILES*MAP_SIZE_SUBTILES * 4];
	memset(m_MiniMapScratchData, 0, MAP_SIZE_SUBTILES*MAP_SIZE_SUBTILES * 4);
	m_MinimapData.texture = new Texture();
	m_MinimapData.width = MAP_SIZE_SUBTILES;
	m_MinimapData.height = MAP_SIZE_SUBTILES;
	m_MinimapData.texture->Create(m_MinimapData.width, m_MinimapData.height, DXGI_FORMAT_R8G8B8A8_UNORM, true, m_MiniMapScratchData);
	m_MinimapObject = new Object2D();
	m_MinimapObject->m_Renderable->SetPosition(-1.45f, 1.3f, 0.9f);
	m_MinimapObject->SetTexture(&m_MinimapData);
	System::tSys->m_Game->AddObject3D(m_MinimapObject->m_Renderable);
	m_MinimapObject->SetScale(0.7f, 0.7f);
	m_MinimapObject->SetVisibility(true);


	m_Cursor = new Object2D(Object2D::Source::sMENU_CURSOR, 1, true);
	m_Cursor->SetVisibility(true);
	System::tSys->m_Game->AddObject3D(m_Cursor->m_Renderable);

	m_LevelData->Init();
	m_Pather = new micropather::MicroPather(&m_LevelData->m_Map,260,8,false);

	System::tSys->m_Game->m_Camera->SetPosition(42 * 3, 12, 38 * 3);
	System::tSys->m_Game->m_Camera->SetTarget(XMFLOAT3(42 * 3, 12, 38 * 3));

	m_LevelScript->RunInitCommands();
}

int SelectedTool = 0;
int SelectedRoom = 0;
bool IsMarking = false;

struct RoomAvailableObject
{
	uint16_t type;
	std::string name;
};
std::vector<RoomAvailableObject> availableRoomsList;
void Level::AvailableRoomsChanged()
{
	availableRoomsList.clear();
	for (auto& r : LevelScript::AvailableRooms[0])
	{
		if (r.second.isAvailable)
		{
			availableRoomsList.push_back({ r.first,RoomTypeToString.at(r.first) });
		}
	}
}


void Level::Update(float delta)
{
	Game* g = Themp::System::tSys->m_Game;
	XMFLOAT3 camPos = g->m_Camera->GetPosition();

	int camTilePosX = ((int)camPos.x) / 3;
	int camTilePosY = ((int)camPos.z) / 3;

	m_MapObject->Update(delta);

	float uiMouseX = 0, uiMouseY = 0;
	Game::TranslateMousePos((int)g->m_CursorWindowedX, (int)g->m_CursorWindowedY, uiMouseX, uiMouseY);
	m_Cursor->m_Renderable->SetPosition(uiMouseX + cursorOffset.x, uiMouseY - cursorOffset.y, 0.01f);

	//Some level stuff
	ImGui::Checkbox("Wireframe", &D3D::s_D3D->m_Wireframe);

	ImGui::Text("Controls: WASD for movement.\nSelect a Room and Press Build \n");
	ImGui::Text("\n\nGold: %i", LevelScript::GameValues[Owner_PlayerRed]["MONEY"]);

	char* ToolNames[3] =
	{
		"Select",
		"Build",
		"Destroy",
	};


	std::vector<const char*> roomNameList;
	roomNameList.reserve(availableRoomsList.size()+1);
	for (int i = 0; i < availableRoomsList.size(); i++)
	{
		roomNameList.push_back(availableRoomsList[i].name.c_str());
	}
	roomNameList.push_back("None");

	ImGui::Text("Current Selected Tool: %s", ToolNames[SelectedTool]);
	if (ImGui::ListBox("Rooms", &SelectedRoom, roomNameList.data(), availableRoomsList.size()))
	{
		SelectedTool = 1;
	}
	if (ImGui::Button("Tool: Destroy"))
	{
		SelectedTool = 2;
	}

	if (g->m_Keys[257] == 2)
	{
		SelectedTool = 0;
		SelectedRoom = roomNameList.size()-1;
	}

	XMFLOAT3 mouseDir = g->m_Camera->ScreenToWorld(g->m_CursorWindowedX, g->m_CursorWindowedY);
	XMFLOAT3 dir = Normalize(mouseDir - camPos);
	LevelData::HitData hit = m_LevelData->Raycast(LevelData::WorldToSubtileFloat(camPos), dir, 50);

	//tileIndicator->SetPosition((camTilePosX-42)*3, 6, (camTilePosY-42)*3);
	if (hit.hit)
	{
		tileIndicator->isVisible = true;
		//DebugDraw::Line(camPos-XMFLOAT3(0,1,0), SubtileToWorld(XMFLOAT3(hit.posX, hit.posY, hit.posZ)), 2.0f);
		XMFLOAT3 hitPos = LevelData::SubtileToWorld(XMINT3(hit.posX, hit.posY, hit.posZ));
		XMINT2 tilePos = LevelData::WorldToTile(hitPos);

		if (tilePos.x > 0 && tilePos.x < MAP_SIZE_TILES - 1 && tilePos.y > 0 && tilePos.y < MAP_SIZE_TILES - 1)
		{
			XMFLOAT3 worldTilePos = LevelData::TileToWorld(tilePos);
			tileIndicator->SetPosition(worldTilePos + XMFLOAT3(0, 0.01f, 0));
			
			if (g->m_Keys[256] == 2)
			{
				if (SelectedTool == 0)
				{
					if (m_LevelData->m_Map.m_Tiles[tilePos.y][tilePos.x].marked[Owner_PlayerRed])
					{
						IsMarking = false;
						m_LevelData->UnMarkTile(Owner_PlayerRed, tilePos.y, tilePos.x);
					}
					else if (m_LevelData->MarkTile(Owner_PlayerRed, tilePos.y, tilePos.x))
					{
						IsMarking = true;
					}
				}
				else if (SelectedTool == 1)
				{
					m_LevelData->BuildRoom(availableRoomsList[SelectedRoom].type, Owner_PlayerRed, tilePos.y, tilePos.x);
				}
				else if (SelectedTool == 2)
				{
					m_LevelData->DeleteRoom(Owner_PlayerRed, tilePos.y, tilePos.x);
				}
			}
			else if (g->m_Keys[256])
			{
				if (SelectedTool == 0)
				{
					if (IsMarking)
					{
						m_LevelData->MarkTile(Owner_PlayerRed, tilePos.y, tilePos.x);
					}
					else
					{
						m_LevelData->UnMarkTile(Owner_PlayerRed, tilePos.y, tilePos.x);
					}
				}
			}
		}
	}
	else
	{
		tileIndicator->isVisible = false;
	}


	//Update the map
	m_MapObject->ConstructFromLevel(camPos.x,camPos.z);

	//minimap room color animation
	UnOwnedRoomColorTimer += delta;
	if (UnOwnedRoomColorTimer > (1.0 / 10.0f))
	{
		if(UnOwnedRoomColorTimer >= (1.0f / 10.0f)*2)
			UnOwnedRoomColorTimer = (1.0f / 10.0f)*2;

		UnOwnedRoomColorTimer -= (1.0f / 10.0f);
		UnownedRoomColorIndex++;
		if (UnownedRoomColorIndex > 6)
		{
			UnownedRoomColorIndex = 0;
		}
	}
	UpdateMinimap();
	
	//reset paths if need to
	if (LevelData::PathsInvalidated)
	{
		m_Pather->Reset();
	}

	m_CreatureGenerateTurnTimer += delta;
	const float turnDelta = 1.0f / GAME_TURNS_PER_SECOND;
	if (m_CreatureGenerateTurnTimer > turnDelta)
	{
		m_CreatureGenerateTurnTimer -= turnDelta;
		m_CreatureGenerateTurns++;
		int generateSpeed = LevelScript::GameValues[Owner_PlayerRed]["GENERATE_SPEED"];
		if (m_CreatureGenerateTurns >= generateSpeed)
		{
			m_CreatureGenerateTurns -= generateSpeed;
			for (int i = 0; i < 6; i++)
			{
				SpawnCreature(i);
			}
		}
		if (m_CreatureGenerateTurnTimer > turnDelta * 2)
		{
			m_CreatureGenerateTurnTimer = turnDelta * 2;
		}
	}

	for (int player = 0; player < 6; player++)
	{
		if (m_Players[player] == nullptr) continue;
		m_Players[player]->Update(delta);
	}

	for (int i = 0; i < m_LevelData->m_MapEntityUsed.size(); i++)
	{
		m_LevelData->m_MapEntityUsed[i]->Update(delta);
	}

	m_LevelScript->Update(delta);
	
}

int Level::PathFind(XMINT2 A, XMINT2 B, micropather::MPVector<void*>& outPath, float& outCost, bool AllowDoors)
{
	uint64_t APos = ((((uint64_t)A.y) << 32) | (uint64_t)A.x);
	uint64_t BPos = ((((uint64_t)B.y) << 32) | (uint64_t)B.x);
	return  m_Pather->Solve((void*)APos, (void*)BPos, &outPath, &outCost);
}

void Level::UpdateMinimap()
{
	XMFLOAT3 camPos = System::tSys->m_Game->m_Camera->GetPosition();
	camPos.z = MAP_SIZE_SUBTILES_RENDER - camPos.z;
	//minimap texturing
	const BYTE OwnerColorsPath[6][4] =
	{
		{
			0x86,0x2C,0x00,0xFF, //red
		},
		{
			0x8A,0x71,0x96,0xFF, //blue
		},
		{
			0x34,0x5D,0x04,0xFF, //green
		},
		{
			0xBE,0x9E,0x00,0xFF, //yellow
		},
		{
			0xB6,0xA2,0x7D,0xFF, //white
		},
		{
			0x34,0x24,0x04,0xFF, //none
		},
	};
	const BYTE OwnerColorsRoom[6][4] =
	{
		{
			0x9E,0x30,0x00,0xFF, //red
		},
		{
			0xA2,0x8A,0xB6,0xFF, //blue
		},
		{
			0x38,0x71,0x0C,0xFF, //green
		},
		{
			0xE7,0xD7,0x00,0xFF, //yellow
		},
		{
			0xBE,0xAA,0x86,0xFF, //white
		},
		{
			0x34,0x24,0x04,0xFF, //none
		},
	};
	for (int y = 0; y < MAP_SIZE_SUBTILES_RENDER; y++)
	{
		const int yTile = 84 - (y / 3);
		for (int x = 0; x < MAP_SIZE_SUBTILES_RENDER; x++)
		{
			const int xTile = x / 3;
			const Tile& tile = m_LevelData->m_Map.m_Tiles[yTile][xTile];
			const unsigned int texPos = (x + y * MAP_SIZE_SUBTILES) * 4;
			uint16_t tileType = tile.type & 0xFF;
			if (tile.visible)
			{
				if (tileType == Type_Claimed_Land)
				{
					m_MiniMapScratchData[texPos + 0] = OwnerColorsPath[tile.owner][0];
					m_MiniMapScratchData[texPos + 1] = OwnerColorsPath[tile.owner][1];
					m_MiniMapScratchData[texPos + 2] = OwnerColorsPath[tile.owner][2];
					m_MiniMapScratchData[texPos + 3] = OwnerColorsPath[tile.owner][3];
				}
				else if (tileType == Type_Unclaimed_Path)
				{
					m_MiniMapScratchData[texPos + 0] = OwnerColorsPath[Owner_PlayerNone][0];
					m_MiniMapScratchData[texPos + 1] = OwnerColorsPath[Owner_PlayerNone][1];
					m_MiniMapScratchData[texPos + 2] = OwnerColorsPath[Owner_PlayerNone][2];
					m_MiniMapScratchData[texPos + 3] = OwnerColorsPath[Owner_PlayerNone][3];
				}
				else if (tileType == Type_Rock)
				{
					m_MiniMapScratchData[texPos + 0] = 0x00;
					m_MiniMapScratchData[texPos + 1] = 0x00;
					m_MiniMapScratchData[texPos + 2] = 0x00;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType == Type_Lava)
				{
					m_MiniMapScratchData[texPos + 0] = 0x38;
					m_MiniMapScratchData[texPos + 1] = 0x1C;
					m_MiniMapScratchData[texPos + 2] = 0x00;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType == Type_Water)
				{
					m_MiniMapScratchData[texPos + 0] = 0x65;
					m_MiniMapScratchData[texPos + 1] = 0x4D;
					m_MiniMapScratchData[texPos + 2] = 0x49;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (IsWall(tileType))
				{
					m_MiniMapScratchData[texPos + 0] = 0x5D;
					m_MiniMapScratchData[texPos + 1] = 0x45;
					m_MiniMapScratchData[texPos + 2] = 0x1C;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType == Type_Earth || tileType == Type_Earth_Torch)
				{
					m_MiniMapScratchData[texPos + 0] = 0x30;
					m_MiniMapScratchData[texPos + 1] = 0x20;
					m_MiniMapScratchData[texPos + 2] = 0x04;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType == Type_Gold)
				{
					m_MiniMapScratchData[texPos + 0] = 0x71;
					m_MiniMapScratchData[texPos + 1] = 0x6D;
					m_MiniMapScratchData[texPos + 2] = 0x18;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType >= Type_Portal && tileType <= Type_Barracks)
				{
					if (tile.owner == Owner_PlayerNone)
					{
						m_MiniMapScratchData[texPos + 0] = OwnerColorsRoom[UnownedRoomColorIndex][0];
						m_MiniMapScratchData[texPos + 1] = OwnerColorsRoom[UnownedRoomColorIndex][1];
						m_MiniMapScratchData[texPos + 2] = OwnerColorsRoom[UnownedRoomColorIndex][2];
						m_MiniMapScratchData[texPos + 3] = OwnerColorsRoom[UnownedRoomColorIndex][3];
					}
					else
					{
						m_MiniMapScratchData[texPos + 0] = OwnerColorsRoom[tile.owner][0];
						m_MiniMapScratchData[texPos + 1] = OwnerColorsRoom[tile.owner][1];
						m_MiniMapScratchData[texPos + 2] = OwnerColorsRoom[tile.owner][2];
						m_MiniMapScratchData[texPos + 3] = OwnerColorsRoom[tile.owner][3];
					}
				}
				else if (tileType >= Type_Wooden_DoorH && tileType <= Type_Magic_DoorV)
				{
					m_MiniMapScratchData[texPos + 0] = 0x92;
					m_MiniMapScratchData[texPos + 1] = 0x9A;
					m_MiniMapScratchData[texPos + 2] = 0x51;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else if (tileType >= Type_Wooden_DoorH_Locked && tileType <= Type_Magic_DoorV_Locked)
				{
					m_MiniMapScratchData[texPos + 0] = 0xF7;
					m_MiniMapScratchData[texPos + 1] = 0x86;
					m_MiniMapScratchData[texPos + 2] = 0x5D;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else
				{
					m_MiniMapScratchData[texPos + 0] = 0xFF;
					m_MiniMapScratchData[texPos + 1] = 0x0;
					m_MiniMapScratchData[texPos + 2] = 0xFF;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
			}
			else
			{
				m_MiniMapScratchData[texPos + 0] = 0x30;
				m_MiniMapScratchData[texPos + 1] = 0x20;
				m_MiniMapScratchData[texPos + 2] = 0x04;
				m_MiniMapScratchData[texPos + 3] = 0xFF;
			}
			if (tile.marked[Owner_PlayerRed])
			{
				if (tileType == Type_Gold || tileType == Type_Gem)
				{
					m_MiniMapScratchData[texPos + 0] = 0xE2;
					m_MiniMapScratchData[texPos + 1] = 0xBC;
					m_MiniMapScratchData[texPos + 2] = 0x6A;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
				else
				{
					m_MiniMapScratchData[texPos + 0] = 0x79;
					m_MiniMapScratchData[texPos + 1] = 0x65;
					m_MiniMapScratchData[texPos + 2] = 0x38;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
			}


			if (x == (int)camPos.x - 24 || x == (int)camPos.x + 24)
			{
				if (y >= (int)camPos.z - 24 && y <= (int)camPos.z + 24)
				{
					m_MiniMapScratchData[texPos + 0] = 0xFF;
					m_MiniMapScratchData[texPos + 1] = 0xFF;
					m_MiniMapScratchData[texPos + 2] = 0xFF;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
			}
			if (y == (int)camPos.z - 24 || y == (int)camPos.z + 24)
			{
				if (x >= (int)camPos.x - 24 && x <= (int)camPos.x + 24)
				{
					m_MiniMapScratchData[texPos + 0] = 0xFF;
					m_MiniMapScratchData[texPos + 1] = 0xFF;
					m_MiniMapScratchData[texPos + 2] = 0xFF;
					m_MiniMapScratchData[texPos + 3] = 0xFF;
				}
			}
		}
	}
	//PATH DEBUG
	uint8_t cols[3][8] =
	{
		{
			0xFF,0x80,0x00, 0x40,0xA0, 0xFF,0x20,0x70,
		},
		{
			0x20,0x70,0xFF,0x80,0x00, 0x40,0xA0, 0xFF,
		},
		{
			0x40,0xA0, 0xFF,0x20,0x70, 0xFF,0x80,0x00,
		},

	};
	for (int c = 0; c < m_Players[Owner_PlayerRed]->m_Creatures.size(); c++)
	{
		for (unsigned int i = 0; i < m_Players[Owner_PlayerRed]->m_Creatures[c]->m_Path.size(); i++)
		{
			int PathX = (((uint64_t)m_Players[Owner_PlayerRed]->m_Creatures[c]->m_Path[i]) & 0xFFFFFFF);
			int PathY = ((((uint64_t)m_Players[Owner_PlayerRed]->m_Creatures[c]->m_Path[i]) >> 32) & 0xFFFFFFF);

			const unsigned int texPos = (PathX + (254 - PathY) * MAP_SIZE_SUBTILES) * 4;

			m_MiniMapScratchData[texPos + 0] = cols[0][c % 8];
			m_MiniMapScratchData[texPos + 1] = cols[1][c % 8];
			m_MiniMapScratchData[texPos + 2] = cols[2][c % 8];
			m_MiniMapScratchData[texPos + 3] = 0xFF;
		}
	}
	
	

	m_MinimapData.texture->Load(m_MiniMapScratchData, m_MinimapData.height * m_MinimapData.width * 4);
}



void Themp::Level::SpawnCreature(uint8_t player)
{
	
	std::vector<LevelData::Room*> entrances;
	for (auto& room : m_LevelData->m_Rooms[player])
	{
		if (room.second.roomType == Type_Portal)
		{
			entrances.push_back(&room.second);
		}
	}
	if (entrances.size() > 0)
	{
		int randRoom = rand() % entrances.size();
		LevelData::Room* room = entrances[randRoom];
		for (auto& tile : room->tiles)
		{
			//find the middle tile (naive method atm)
			if (tile.first->type == (tile.first->GetType() + (5 << 8)))
			{
				std::vector<int> available;
				for (int i = 0; i < m_AvailableCreatures.size(); i++)
				{
					if (m_AvailableCreatures[i].amount > 0 && LevelScript::AvailableCreatures[player][m_AvailableCreatures[i].type].isAvailable)
					{
						available.push_back(i);
					}
				}
				if (available.size() == 0)
				{
					return;
				}
				int selectedindex = rand() % available.size();
				Creature* c = new Creature(m_AvailableCreatures[available[selectedindex]].type);
				m_Players[player]->AddCreature(c);
				c->m_Renderable->SetPosition(tile.second.x*3 + 1,tile.first->pathSubTiles[1][1].height+1,tile.second.y*3 + 1);
				m_AvailableCreatures[available[selectedindex]].amount--;
				break;
			}
		}
	}
}
