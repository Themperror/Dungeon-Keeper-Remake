#include "ThempSystem.h"
#include "ThempGoodPlayer.h"
#include "ThempCreature.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include <imgui.h>
using namespace Themp;

Themp::GoodPlayer::GoodPlayer(uint8_t playerID)
{
	m_PlayerID = playerID;
}

void Themp::GoodPlayer::Update(float delta)
{
	ImGui::Begin("Good Player Creature Info");
	for (int i = 0; i < m_Creatures.size(); i++)
	{
		m_Creatures[i]->Update(delta);
		if (LevelData::PathsInvalidated)
		{
			//one of the creatures did an action that invalidated the paths, reset it again
			Level::s_CurrentLevel->m_Pather->Reset();
			LevelData::PathsInvalidated = false;
		}
	}
	ImGui::End();
}
