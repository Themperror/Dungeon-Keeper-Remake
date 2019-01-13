#include "ThempSystem.h"
#include "ThempGoodPlayer.h"
#include "Creature/ThempCreature.h"
#include "Creature/ThempCreatureParty.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include <imgui.h>
using namespace Themp;

Themp::GoodPlayer::GoodPlayer(uint8_t playerID)
{
	m_PlayerID = playerID;
}

Themp::GoodPlayer::~GoodPlayer()
{

}
void Themp::GoodPlayer::Update(float delta)
{
#ifdef _DEBUG
	ImGui::Begin("Good Player Creature Info");
#endif
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
#ifdef _DEBUG
	ImGui::End();
#endif
}
