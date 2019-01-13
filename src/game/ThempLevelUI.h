#pragma once
#include "ThempFileManager.h"
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
namespace Themp
{
	class GUIButton;
	class Object2D;
	class Font;
	class LevelUI
	{
		class BasePanel
		{
		public:
			Object2D* BackPlateMinimap = nullptr;
			Object2D* BackPlateMinimapSide = nullptr;
			Object2D* BackPlateMinimapPlus = nullptr;
			Object2D* BackPlateMinimapMinus = nullptr;
			Object2D* BackPlateMinimapCPU = nullptr;
			Object2D* BackPlatePanelsLeft = nullptr;
			Object2D* BackPlatePanelsRight = nullptr;
			Object2D* BackPlateBaseLeft = nullptr;
			Object2D* BackPlateBaseRight = nullptr;
		};
		class Minimap
		{
		public:
			Object2D* MinimapObject = nullptr;
			GUITexture MinimapData;
		};
		class DataPanel
		{
		public:
		};
		class BuildingPanel
		{
		public:
			Object2D* ButtonSquares[16] = { nullptr };
			GUIButton* RoomButtons[16] = { nullptr };
			Font* BuildingAmount = nullptr;
			Font* BuildingCost = nullptr;
			Object2D* BuildingIcon = nullptr;
		};
		class MagicPanel
		{
		public:
		};
		class WorkshopPanel
		{
		public:
		};
		class CreaturePanel
		{
		public:
		};

		void CreateBasePanel();
		void CreateMinimap();
		void CreateDataPanel();
		void UpdateRoomIcons();
		void CreateBuildingPanel();
		void CreateMagicPanel();
		void CreateWorkshopPanel();
		void CreateCreaturePanel();
		GUIButton* m_InitialClick = nullptr;
	public:
		~LevelUI();
		LevelUI();
		
		bool Update(float delta);
		void ToggleVisibility();
		void RoomAvailable(uint16_t roomtype, bool availability);
		bool m_ShowUI = true;
		int m_GoldAmount = -1;
		std::string m_GoldText = "0";
		std::vector< Object2D*> m_GoldNumbers;
		Font* BuildingInfo = nullptr;
		BYTE* m_MiniMapScratchData = nullptr;

		BasePanel m_BasePanel;
		Minimap m_Minimap;
		DataPanel m_DataPanel;
		BuildingPanel m_BuildingPanel;
		MagicPanel m_MagicPanel;
		WorkshopPanel m_WorkshopPanel;
		CreaturePanel m_CreaturePanel;

	};
};
