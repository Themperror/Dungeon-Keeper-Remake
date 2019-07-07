#include "ThempSystem.h"
#include "ThempLevelUI.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempResources.h"
#include "ThempMaterial.h"
#include "ThempGUIButton.h"
#include "ThempTileArrays.h"
#include "ThempObject2D.h"
#include "ThempObject3D.h"
#include <DirectXMath.h>
#include <unordered_map>
#include <array>
#include "../Library/imgui.h"
#include "ThempFont.h"
#include "Players/ThempPlayer.h"
#include "ThempLevelConfig.h"
#include "../Engine/ThempAudio.h"
#include "ThempLevelScript.h"
using namespace Themp;
const int buttonIcons[16] =
{
	56,78,58,66,
	60,80,82,74,
	64,62,68,72,
	70,76,0,106,
};
const int bigButtonIcons[16] =
{
	28,50,30,38,
	32,52,82,46,
	36,34,40,44,
	42,48,0,106,
};
const int buttonRooms[14] =
{
	Type_Treasure_Room, Type_Lair, Type_Hatchery, Type_Training_Room,
	Type_Library, Type_Bridge, Type_Guardpost, Type_Workshop,
	Type_Prison, Type_Torture_Room,Type_Barracks, Type_Temple,
	Type_Graveyard,Type_Scavenger_Room,
};


Themp::LevelUI::~LevelUI()
{
	delete[] m_MiniMapScratchData;
	delete m_Minimap.MinimapObject;
	delete m_Minimap.MinimapData.texture;
	if (m_BasePanel.BackPlateMinimap)
	{
		delete m_BasePanel.BackPlateMinimap;
		delete m_BasePanel.BackPlateMinimapPlus;
		delete m_BasePanel.BackPlatePanelsLeft;
		delete m_BasePanel.BackPlateBaseLeft;
	}
	for (size_t i = 0; i < m_GoldNumbers.size(); i++)
	{
		delete m_GoldNumbers[i];
	}
	for (size_t i = 0; i < 16; i++)
	{
		delete m_BuildingPanel.RoomButtons[i];
	}
	delete m_BuildingPanel.BuildingAmount;
	delete m_BuildingPanel.BuildingCost;
	delete m_BuildingPanel.BuildingIcon;
}

Themp::LevelUI::LevelUI()
{
	//std::string buttonTexts[12] = {};
	//bool disabled[12] = { };
	//for (size_t i = 0; i < 12; i++)
	//{
	//	int normal[3] = { 13, 14, 15 };
	//	int hover[3] = { 1, 2, 3 };
	//	int click[3] = { 1, 2, 3 };
	//	float xOffsets[3] = { -0.75f,0,0.75f };
	//	float yOffsets[3] = { 0,0,0 };
	//	float zOffsets[3] = { 0.05f,0,0 }; //this is such a shitty hack, but I NEED the other buttons to draw over the first one..
	//	m_GUIButtons[i] = new GUIButton(Object2D::sMENU_MAIN, normal, hover, click, xOffsets, yOffsets, zOffsets, 3, true, buttonTexts[i], Font::MENU_NORMAL1, Font::MENU_NORMAL2, Font::MENU_NORMAL3);
	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		//m_GUIButtons[i]->SetSize(xSizes[j], ySizes[j], j);
	//		m_GUIButtons[i]->SetSize(1, 1, 1, 1, j);
	//	}
	//	m_GUIButtons[i]->SetPosition(0, 0.945 - (i * 0.415), 0.5);
	//	m_GUIButtons[i]->SetVisibility(true);
	//	m_GUIButtons[i]->m_IsDisabled = disabled[i];
	//}
	CreateBasePanel();
	CreateMinimap();
	CreateDataPanel();
	CreateBuildingPanel();
	CreateMagicPanel();
	CreateWorkshopPanel();
	CreateCreaturePanel();

}
int texIDMisc = 0;
int texIDPane = 0;
int texIDText = 0;

bool LevelUI::Update(float delta)
{
	ImGui::Begin("Texture Finder");
	ImGui::SliderInt("MiscGUI", &texIDMisc, 0, FileManager::GetLevelMiscAmount());
	Texture* tex = FileManager::GetLevelMiscGUITexture(texIDMisc, true)->texture;
	ImGui::Image(tex->m_View,ImVec2(tex->m_Width*2,tex->m_Height * 2));
	ImGui::SliderInt("PaneGUI", &texIDPane, 0, FileManager::GetLevelPaneAmount());
	tex = FileManager::GetLevelPaneGUITexture(texIDPane, true)->texture;
	ImGui::Image(tex->m_View, ImVec2(tex->m_Width * 2, tex->m_Height * 2));
	ImGui::SliderInt("Text", &texIDText, 0, 95);
	tex = FileManager::GetFont(1)->at(texIDText).texture;
	ImGui::Image(tex->m_View, ImVec2(tex->m_Width * 2, tex->m_Height * 2));
	ImGui::End();
	if (m_ShowUI)
	{
		Player* player = ((Player*)Level::s_CurrentLevel->m_Players[Owner_PlayerRed]);
		if (player->m_GoldAmount != m_GoldAmount) //changing gold text is expensive, only change when we need to..
		{
			m_GoldAmount = player->m_GoldAmount;
			m_GoldText = std::to_string(m_GoldAmount);
			while(m_GoldText.size() > m_GoldNumbers.size())
			{
				Object2D* obj = new Object2D(Object2D::Source::sLEVEL_MISC, 70, true);
				Themp::System::tSys->m_Game->AddObject3D(obj->m_Renderable);
				obj->SetVisibility(false);
				m_GoldNumbers.push_back(obj);
			}
			for (int i = 0; i < m_GoldNumbers.size(); i++)
			{
				m_GoldNumbers[i]->SetVisibility(false);
			}
			const float letterspacing = 0.09f;
			float totalspace = (((float)(m_GoldText.size()-1) * (letterspacing / 2.0f)));
			for (int i = 0; i < m_GoldText.size(); i++)
			{
				//ascii numbers start at 48 ('0') to 57 ('9') so char - 48 == number
				int number = m_GoldText[i] - 48;

				//The textures for each number start at 70 so 70 == '0'
				m_GoldNumbers[i]->SetTexture(FileManager::GetLevelMiscGUITexture(number + 70, true));
			
				m_GoldNumbers[i]->m_Renderable->SetPosition(-1.6f + ((float)i * letterspacing) - totalspace, 0.42f, 0.85f);
				m_GoldNumbers[i]->SetVisibility(true);
			}
		}
		UpdateRoomIcons();
	}

	bool clickedbutton = false;
	{
		BuildingPanel& bp = m_BuildingPanel;
		Game* g = System::tSys->m_Game;

		float uiMouseX = 0, uiMouseY = 0;
		Game::TranslateMousePos(g->m_CursorWindowedX, g->m_CursorWindowedY, uiMouseX, uiMouseY);

		for (size_t i = 0; i < 16; i++)
		{
			if (bp.RoomButtons[i] == nullptr) continue;
			if (bp.RoomButtons[i]->m_IsVisible)
			{
				if (bp.RoomButtons[i]->Update(delta, uiMouseX, uiMouseY, g->m_Keys[256], m_InitialClick))
				{
					clickedbutton = true;
					if (g->m_Keys[256] == 2)
					{
						m_InitialClick = bp.RoomButtons[i];
					}
					if (g->m_Keys[256] == -1) // just up
					{
						if (m_InitialClick == bp.RoomButtons[i] && !bp.RoomButtons[i]->m_IsDisabled)
						{
							if (i > 14)
							{
								Level::s_CurrentLevel->m_SelectedBuilding = 0;
							}
							else
							{
								Level::s_CurrentLevel->m_SelectedBuilding = buttonRooms[i];
							}
							Level::s_CurrentLevel->m_BuildMode = true;
							Themp::System::tSys->m_Audio->PlayOneShot(FileManager::GetSound("BUTTON1.WAV"));
						}
						else
						{
							m_InitialClick = nullptr;
						}
					}
				}
			}
		}

	}

	return clickedbutton;
}
void LevelUI::ToggleVisibility()
{
	m_ShowUI = !m_ShowUI;
	
	{
		BasePanel& bp = m_BasePanel;
		bp.BackPlateMinimap->SetVisibility(m_ShowUI);
		bp.BackPlateBaseLeft->SetVisibility(m_ShowUI);
		bp.BackPlateMinimapSide->SetVisibility(m_ShowUI);
		bp.BackPlateMinimapPlus->SetVisibility(m_ShowUI);
		bp.BackPlateMinimapMinus->SetVisibility(m_ShowUI);
		bp.BackPlateMinimapCPU->SetVisibility(m_ShowUI);
		bp.BackPlatePanelsLeft->SetVisibility(m_ShowUI);
		bp.BackPlatePanelsRight->SetVisibility(m_ShowUI);
		bp.BackPlateBaseRight->SetVisibility(m_ShowUI);
	}
	m_Minimap.MinimapObject->SetVisibility(m_ShowUI);

	for (int i = 0; i < m_GoldText.size(); i++)
	{
		m_GoldNumbers[i]->SetVisibility(m_ShowUI);
	}

	{
		BuildingPanel& bp = m_BuildingPanel;
		bp.BuildingAmount->SetVisibility(m_ShowUI);
		bp.BuildingCost->SetVisibility(m_ShowUI);
		bp.BuildingIcon->SetVisibility(m_ShowUI);
		for (int i = 0; i < 16; i++)
		{
			if (bp.ButtonSquares[i])
			{
				bp.ButtonSquares[i]->SetVisibility(m_ShowUI);
			}
			if (bp.RoomButtons[i])
			{
				AvailableObject& ao = Level::s_CurrentLevel->m_LevelScript->AvailableRooms[Owner_PlayerRed][buttonRooms[i]];
				if (ao.isAvailable)
				{
					bp.RoomButtons[i]->SetVisibility(m_ShowUI);
				}
				else
				{
					bp.RoomButtons[i]->SetVisibility(false);
				}
			}
		}
		bp.RoomButtons[15]->SetVisibility(m_ShowUI);
	}
}

void LevelUI::CreateBasePanel()
{
	BasePanel& p = m_BasePanel;

	//Minimap background
	p.BackPlateMinimap = new Object2D(Object2D::sLEVEL_PANE, 235, true);
	p.BackPlateMinimap->SetVisibility(true);
	p.BackPlateMinimap->SetScale(1, 1.3333f);
	p.BackPlateMinimap->m_Renderable->SetPosition(-1.62f, 1.3f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateMinimap->m_Renderable);

	//minimap side
	p.BackPlateMinimapSide = new Object2D(Object2D::sLEVEL_PANE, 1, true);
	p.BackPlateMinimapSide->SetVisibility(true);
	p.BackPlateMinimapSide->SetScale(1, 1.3333f);
	p.BackPlateMinimapSide->m_Renderable->SetPosition(-1.188f, 1.3f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateMinimapSide->m_Renderable);

	//minimap background plus button
	p.BackPlateMinimapPlus = new Object2D(Object2D::sLEVEL_PANE, 237, true);
	p.BackPlateMinimapPlus->SetVisibility(true);
	p.BackPlateMinimapPlus->SetScale(1, 1.3333f);
	p.BackPlateMinimapPlus->m_Renderable->SetPosition(-1.23f, 1.6f, 0.87f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateMinimapPlus->m_Renderable);
	//minimap background minus button
	p.BackPlateMinimapMinus = new Object2D(Object2D::sLEVEL_PANE, 239, true);
	p.BackPlateMinimapMinus->SetVisibility(true);
	p.BackPlateMinimapMinus->SetScale(1, 1.3333f);
	p.BackPlateMinimapMinus->m_Renderable->SetPosition(-1.23f, 0.875f, 0.87f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateMinimapMinus->m_Renderable);

	//minimap background CPU assist button
	p.BackPlateMinimapCPU = new Object2D(Object2D::sLEVEL_PANE, 492, true);
	p.BackPlateMinimapCPU->SetVisibility(true);
	p.BackPlateMinimapCPU->SetScale(1, 1.3333f);
	p.BackPlateMinimapCPU->m_Renderable->SetPosition(-1.95f, 0.875f, 0.87f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateMinimapCPU->m_Renderable);


	p.BackPlatePanelsLeft = new Object2D(Object2D::sLEVEL_PANE, 2, true);
	p.BackPlatePanelsLeft->SetVisibility(true);
	p.BackPlatePanelsLeft->SetScale(1, 1.3333f);
	p.BackPlatePanelsLeft->m_Renderable->SetPosition(-1.62f, -0.1232f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlatePanelsLeft->m_Renderable);

	//panel side
	p.BackPlatePanelsRight = new Object2D(Object2D::sLEVEL_PANE, 3, true);
	p.BackPlatePanelsRight->SetVisibility(true);
	p.BackPlatePanelsRight->SetScale(1, 1.3333f);
	p.BackPlatePanelsRight->m_Renderable->SetPosition(-1.188f, -0.1232f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlatePanelsRight->m_Renderable);

	p.BackPlateBaseLeft = new Object2D(Object2D::sLEVEL_PANE, 4, true);
	p.BackPlateBaseLeft->SetVisibility(true);
	p.BackPlateBaseLeft->SetScale(1, 1.3333f);
	p.BackPlateBaseLeft->m_Renderable->SetPosition(-1.62f, -1.5448f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateBaseLeft->m_Renderable);

	//base side
	p.BackPlateBaseRight = new Object2D(Object2D::sLEVEL_PANE, 5, true);
	p.BackPlateBaseRight->SetVisibility(true);
	p.BackPlateBaseRight->SetScale(1, 1.3333f);
	p.BackPlateBaseRight->m_Renderable->SetPosition(-1.188f, -1.5448f, 0.9f);
	Themp::System::tSys->m_Game->AddObject3D(p.BackPlateBaseRight->m_Renderable);
}
void LevelUI::CreateMinimap()
{
	m_MiniMapScratchData = new BYTE[MAP_SIZE_SUBTILES*MAP_SIZE_SUBTILES * 4];
	memset(m_MiniMapScratchData, 0, MAP_SIZE_SUBTILES*MAP_SIZE_SUBTILES * 4);
	m_Minimap.MinimapData.texture = new Texture();
	m_Minimap.MinimapData.width = MAP_SIZE_SUBTILES;
	m_Minimap.MinimapData.height = MAP_SIZE_SUBTILES;
	m_Minimap.MinimapData.texture->Create(m_Minimap.MinimapData.width, m_Minimap.MinimapData.height, DXGI_FORMAT_R8G8B8A8_UNORM, true, m_MiniMapScratchData);
	m_Minimap.MinimapObject = new Object2D();
	m_Minimap.MinimapObject->m_Renderable->SetPosition(-1.62f, 1.3f, 1.0f);
	m_Minimap.MinimapObject->SetTexture(&m_Minimap.MinimapData);
	System::tSys->m_Game->AddObject3D(m_Minimap.MinimapObject->m_Renderable);
	m_Minimap.MinimapObject->SetScale(0.5f, 0.67777f);
	m_Minimap.MinimapObject->SetVisibility(true);
}
void LevelUI::CreateDataPanel()
{

}
std::unordered_map<uint16_t, int> RoomCounts;
void LevelUI::UpdateRoomIcons()
{
	BuildingPanel& bp = m_BuildingPanel;
	for (auto& c : RoomCounts)
	{
		c.second = 0;
	}
	for (auto& room : Level::s_CurrentLevel->m_LevelData->m_Rooms[Owner_PlayerRed])
	{
		RoomCounts[room.second.roomType]++;
	}
	uint16_t selectedBuilding = Level::s_CurrentLevel->m_SelectedBuilding;
	if (Level::s_CurrentLevel->m_BuildMode && selectedBuilding > 0)
	{
		bp.BuildingAmount->SetVisibility(true);
		bp.BuildingCost->SetVisibility(true);
		bp.BuildingIcon->SetVisibility(true);
		std::string roomAmount = std::to_string(RoomCounts[selectedBuilding]);
		std::string roomCost = std::to_string(LevelConfig::roomData[LevelConfig::TypeToRoom(selectedBuilding)].Cost);
		bp.BuildingAmount->ChangeText(std::string("@") + roomAmount,20);
		bp.BuildingAmount->SetScale(1.0, 1.333, 1);
		bp.BuildingCost->ChangeText(roomCost, 20);
		bp.BuildingCost->SetScale(1.0, 1.333, 1);
		for (int i = 0; i < 14; i++)
		{
			if (selectedBuilding == buttonRooms[i])
			{
				if (m_GoldAmount < LevelConfig::roomData[LevelConfig::TypeToRoom(selectedBuilding)].Cost)
				{
					bp.BuildingIcon->SetTexture(FileManager::GetLevelPaneGUITexture(bigButtonIcons[i]+1, true));
				}
				else
				{
					bp.BuildingIcon->SetTexture(FileManager::GetLevelPaneGUITexture(bigButtonIcons[i], true));
				}
				break;
			}
		}
	}
	else
	{
		//bp.BuildingAmount->SetVisibility(false);
		//bp.BuildingCost->SetVisibility(false);
		//bp.BuildingIcon->SetVisibility(false);
	}
	for (size_t i = 0; i < 14; i++)
	{
		int roomIndex = LevelConfig::TypeToRoom(buttonRooms[i]);
		if (m_GoldAmount < LevelConfig::roomData[roomIndex].Cost)
		{
			bp.RoomButtons[i]->m_TexIDs[0].normal = buttonIcons[i] + 1;
			bp.RoomButtons[i]->m_TexIDs[0].click = buttonIcons[i] + 1;
			bp.RoomButtons[i]->m_TexIDs[0].hover = buttonIcons[i] + 1;
			bp.RoomButtons[i]->m_Objects[0]->SetTexture(FileManager::GetLevelPaneGUITexture(buttonIcons[i]+1, true));
		}
		else
		{
			bp.RoomButtons[i]->m_TexIDs[0].normal = buttonIcons[i];
			bp.RoomButtons[i]->m_TexIDs[0].click = buttonIcons[i];
			bp.RoomButtons[i]->m_TexIDs[0].hover = buttonIcons[i];
			bp.RoomButtons[i]->m_Objects[0]->SetTexture(FileManager::GetLevelPaneGUITexture(buttonIcons[i], true));
		}
		if (RoomCounts[buttonRooms[i]] > 0)
		{
			bp.ButtonSquares[i]->SetTexture(FileManager::GetLevelPaneGUITexture(26, true));
		}
		else
		{
			AvailableObject& ao = Level::s_CurrentLevel->m_LevelScript->AvailableRooms[Owner_PlayerRed][buttonRooms[i]];
			if (ao.isAvailable)
			{
				bp.ButtonSquares[i]->SetTexture(FileManager::GetLevelPaneGUITexture(23, true));
			}
			else
			{
				if (ao.canBeAvailable)
				{
					bp.ButtonSquares[i]->SetTexture(FileManager::GetLevelPaneGUITexture(24, true));
				}
				else
				{
					bp.ButtonSquares[i]->SetTexture(FileManager::GetLevelPaneGUITexture(23, true));
				}
			}
		}

	}
}
void LevelUI::RoomAvailable(uint16_t roomtype,bool availability)
{
	BuildingPanel& bp = m_BuildingPanel;
	//naive but well.. its only 14 and happens REALLY rarely..
	if (!m_ShowUI)return;
	for (int i = 0; i < 14; i++)
	{
		if (roomtype == buttonRooms[i])
		{
			bp.RoomButtons[i]->SetVisibility(availability);
			break;
		}
	}
}
void LevelUI::CreateBuildingPanel()
{
	BuildingPanel& bp = m_BuildingPanel;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int iconIndex = buttonIcons[x + y * 4];
			if (iconIndex != 106) //The Sell button comes with its own square
			{
				Object2D* obj = new Object2D(Object2D::Source::sLEVEL_PANE, 23, true);
				obj->SetScale(1.0, 1.15);
				obj->m_Renderable->SetPosition(-1.9 + ((float)x*0.2), -0.8 - ((float)y*0.35), 0.85f);
				Themp::System::tSys->m_Game->AddObject3D(obj->m_Renderable);

				bp.ButtonSquares[x + y * 4] = obj;
				if (iconIndex != 0) //we'll have to create the sell room button seperately
				{
					GUIButton* button = new GUIButton(Object2D::Source::sLEVEL_PANE, iconIndex, iconIndex, iconIndex, true);
					button->SetVisibility(false);
					button->SetPosition(obj->m_Renderable->m_Position);
					button->SetSize(1.0, 1.15,1,1);
					bp.RoomButtons[x + y * 4] = button;
				}
			}
		}
	}
	GUIButton* button = new GUIButton(Object2D::Source::sLEVEL_PANE, 106, 106, 106, true);
	button->SetVisibility(true);
	button->SetPosition(-1.9 + ((float)3*0.2), -0.8 - ((float)3*0.35), 0.85f);
	button->SetSize(1.0, 1.15, 1, 1);
	bp.RoomButtons[15] = button;

	bp.BuildingAmount = new Font("@0", Font::FontTexID::INGAME, true, XMFLOAT3(-1.65, -0.15, 0.85), 20);
	bp.BuildingCost = new Font("50", Font::FontTexID::INGAME, true, XMFLOAT3(-1.6, -0.4, 0.85), 20);
	bp.BuildingAmount->SetScale(1.0, 1.333, 1);
	bp.BuildingCost->SetScale(1.0, 1.333, 1);
	bp.BuildingIcon = new Object2D(Object2D::Source::sLEVEL_PANE, 28, true);
	bp.BuildingIcon->SetScale(1.0, 1.333);
	bp.BuildingIcon->m_Renderable->SetPosition(-1.8, -0.28, 0.85);
	Themp::System::tSys->m_Game->AddObject3D(bp.BuildingIcon->m_Renderable);
}

void LevelUI::CreateMagicPanel()
{

}
void LevelUI::CreateWorkshopPanel()
{

}
void LevelUI::CreateCreaturePanel()
{

}