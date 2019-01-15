#include "ThempSystem.h"
#include "ThempNeutralPlayer.h"
#include "Creature/ThempCreature.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"

using namespace Themp;

Themp::NeutralPlayer::NeutralPlayer(uint8_t playerID)
{
	m_PlayerID = playerID;
}

void Themp::NeutralPlayer::Update(float delta)
{
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
	for (int i = 0; i < m_DeadCreatures.size(); i++)
	{
		delete m_DeadCreatures[i];
	}
	m_DeadCreatures.clear();
}
