#include "ThempSystem.h"
#include "ThempPlayer.h"
#include "ThempCreature.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include <imgui.h>
using namespace Themp;

Themp::Player::Player(uint8_t playerID)
{
	m_PlayerID = playerID;
}

Themp::Player::~Player()
{
	
}

void Themp::Player::Update(float delta)
{
	m_GoldAmount = LevelScript::GameValues[Owner_PlayerRed]["MONEY"];\


	ImGui::Begin("Player Creature Info");
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
