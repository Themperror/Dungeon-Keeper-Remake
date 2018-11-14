#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../Library/micropather.h"
#include "ThempFileManager.h"
namespace Themp
{
	class D3D;
	class VoxelObject;
	class Object3D;
	class Object2D;
	class Creature;
	struct Sprite;
	class LevelData;

	class Level
	{
	public:
		
		~Level();
		Level(int levelIndex);
		void Update(float delta);
		int PathFind(DirectX::XMINT2 A, DirectX::XMINT2 B, micropather::MPVector<void*>& outPath, float & outCost, bool AllowDoors);
		void UpdateMinimap();
		Object2D* m_Cursor = nullptr;
		Object2D* m_MinimapObject = nullptr;
		int UnownedRoomColorIndex = 0;
		float UnOwnedRoomColorTimer = 0;
		GUITexture m_MinimapData;
		VoxelObject* m_MapObject = nullptr;
		LevelData* m_LevelData = nullptr;
		std::vector<Creature*> m_Creatures;
		
		static Level* s_CurrentLevel;
	};
};
