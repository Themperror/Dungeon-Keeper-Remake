#include "ThempSystem.h"
#include "ThempCPUPlayer.h"
#include "ThempCreature.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"

using namespace Themp;

CPUPlayer::CPUPlayer(uint8_t playerID)
{
	m_PlayerID = playerID;
}

CPUPlayer::~CPUPlayer()
{
	
}

void Themp::CPUPlayer::Update(float delta)
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
}
