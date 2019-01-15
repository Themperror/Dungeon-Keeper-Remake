#include "ThempSystem.h"
#include "ThempPlayerBase.h"
#include "ThempGame.h"
#include "Creature/ThempCreature.h"
#include "Creature/ThempCreatureParty.h"
#include "ThempEntity.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempLevelScript.h"
#include "ThempObject3D.h"
using namespace Themp;

std::unordered_map<CreatureData::CreatureType, std::string> CreatureTypeToString =
{
	{CreatureData::CreatureType::CREATURE_FLOATING_SPIRIT,"FLOATING_SPIRIT"},
	{CreatureData::CreatureType::CREATURE_ORC,"ORC"},
	{CreatureData::CreatureType::CREATURE_TENTACLE,"TENTACLE"},
	{CreatureData::CreatureType::CREATURE_GHOST,"GHOST"},
	{CreatureData::CreatureType::CREATURE_HELLHOUND,"HELL_HOUND"},
	{CreatureData::CreatureType::CREATURE_SPIDER,"SPIDER"},
	{CreatureData::CreatureType::CREATURE_VAMPIRE,"VAMPIRE"},
	{CreatureData::CreatureType::CREATURE_BEETLE,"BUG"},
	{CreatureData::CreatureType::CREATURE_IMP,"IMP"},
	{CreatureData::CreatureType::CREATURE_BILE_DEMON,"BILE_DEMON"},
	{CreatureData::CreatureType::CREATURE_SORCEROR,"SORCEROR"},
	{CreatureData::CreatureType::CREATURE_DARK_MISTRESS,"DARK_MISTRESS"},
	{CreatureData::CreatureType::CREATURE_FLY,"FLY"},
	{CreatureData::CreatureType::CREATURE_DEMONSPAWN,"DEMONSPAWN"},
	{CreatureData::CreatureType::CREATURE_DRAGON,"DRAGON"},
	{CreatureData::CreatureType::CREATURE_TROLL,"TROLL"},
	{CreatureData::CreatureType::CREATURE_SKELETON,"SKELETON"},
	{CreatureData::CreatureType::CREATURE_HORNY,"HORNY"},
	{CreatureData::CreatureType::CREATURE_SAMURAI,"SAMURAI"},
	{CreatureData::CreatureType::CREATURE_THIEF,"THIEF"},
	{CreatureData::CreatureType::CREATURE_FAIRY,"FAIRY"},
	{CreatureData::CreatureType::CREATURE_GIANT,"GIANT"},
	{CreatureData::CreatureType::CREATURE_WITCH,"WITCH"},
	{CreatureData::CreatureType::CREATURE_TUNNELLER,"TUNNELLER"},
	{CreatureData::CreatureType::CREATURE_AVATAR,"AVATAR"},
	{CreatureData::CreatureType::CREATURE_KNIGHT,"KNIGHT"},
	{CreatureData::CreatureType::CREATURE_DWARF,"DWARF"},
	{CreatureData::CreatureType::CREATURE_MONK,"MONK"},
	{CreatureData::CreatureType::CREATURE_ARCHER,"ARCHER"},
	{CreatureData::CreatureType::CREATURE_BARBARIAN,"BARBARIAN"},
	{CreatureData::CreatureType::CREATURE_WIZARD,"WIZARD"},
};

void PlayerBase::AddPartyToLevel(uint8_t owner, CreatureParty party, XMINT2 tilePos)
{
	Creature* leader = nullptr;
	{
		CreatureParty::Partee& p = party.m_Partees[0];
		leader = new Creature(p.type);
		leader->SetPosition(tilePos.x, 2, tilePos.y);
		leader->m_Hero_TimeTillStrike = p.timetillstrike;
		leader->m_HeroObjective = p.objective;
		leader->m_CurrentGoldHold = p.gold_hold;
		leader->m_Level = p.level;
		AddCreature(owner,leader);
	}

	for (int i = 1; i < party.m_Partees.size(); i++)
	{
		CreatureParty::Partee& p = party.m_Partees[i];
		Creature* c = new Creature(p.type);
		c->SetPosition(tilePos.x, 2, tilePos.y);
		c->m_Hero_TimeTillStrike = p.timetillstrike;
		c->m_HeroObjective = p.objective;
		c->m_CurrentGoldHold = p.gold_hold;
		c->m_Level = p.level;
		c->m_HeroLeader = leader;
		AddCreature(owner, c);
	}
}
void Themp::PlayerBase::AddTunnellerPartyToLevel(uint8_t owner, int tunnelerLevel, int tunnelerGold, CreatureParty party, XMINT2 spawnPos, std::string headfor, int targetID)
{
	Creature* leader = new Creature(CreatureData::CreatureType::CREATURE_TUNNELLER);
	leader->m_Level = tunnelerLevel;
	leader->m_CurrentGoldHold = tunnelerGold;
	leader->SetPosition(spawnPos.x, 2, spawnPos.y);

	XMINT2 subtileTarget;
	if (headfor == "ACTION_POINT")
	{
		subtileTarget.x = Level::s_CurrentLevel->m_LevelData->m_ActionPoints[targetID - 1].tx;
		subtileTarget.y = Level::s_CurrentLevel->m_LevelData->m_ActionPoints[targetID - 1].ty;
	}
	else if (headfor == "DUNGEON")
	{
		XMINT2 location = Level::s_CurrentLevel->m_Players[targetID]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}
	else if (headfor == "DUNGEON_HEART")
	{
		XMINT2 location = Level::s_CurrentLevel->m_Players[targetID]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}
	else if (headfor == "APPROPIATE_DUNGEON")
	{
		//TODO IMPLEMENT SCORE TO BASE THE TARGET PLAYER ON!

		XMINT2 location = Level::s_CurrentLevel->m_Players[0]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}


	XMINT2 tileTarget = XMINT2(subtileTarget.x / 3, subtileTarget.y / 3);
	leader->m_Activity = CreatureTaskManager::Activity(true, subtileTarget, tileTarget, CreatureTaskManager::ActivityType::Activity_Tunnel, &LevelData::m_Map.m_Tiles[tileTarget.y][tileTarget.x]);
	AddCreature(owner, leader);

	for (int i = 0; i < party.m_Partees.size(); i++)
	{
		CreatureParty::Partee& p = party.m_Partees[i];
		Creature* c = new Creature(p.type);
		c->SetPosition(spawnPos.x, 2, spawnPos.y);
		c->m_Hero_TimeTillStrike = 0;
		c->m_HeroObjective = p.objective;
		c->m_CurrentGoldHold = p.gold_hold;
		c->m_Level = p.level;
		c->m_HeroLeader = leader;
		AddCreature(owner, c);
	}
}
void  Themp::PlayerBase::AddTunnellerToLevel(uint8_t owner, int tunnelerLevel, int tunnelerGold,XMINT2 spawnPos,std::string headfor, int targetID)
{
	Creature* creature = new Creature(CreatureData::CreatureType::CREATURE_TUNNELLER);
	creature->m_Level = tunnelerLevel;
	creature->m_CurrentGoldHold = tunnelerGold;
	creature->SetPosition(spawnPos.x, 2, spawnPos.y);

	XMINT2 subtileTarget;
	if (headfor == "ACTION_POINT")
	{
		subtileTarget.x = Level::s_CurrentLevel->m_LevelData->m_ActionPoints[targetID - 1].tx;
		subtileTarget.y = Level::s_CurrentLevel->m_LevelData->m_ActionPoints[targetID - 1].ty;
	}
	else if (headfor == "DUNGEON")
	{
		XMINT2 location = Level::s_CurrentLevel->m_Players[targetID]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}
	else if (headfor == "DUNGEON_HEART")
	{
		XMINT2 location = Level::s_CurrentLevel->m_Players[targetID]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}
	else if (headfor == "APPROPIATE_DUNGEON")
	{
		//TODO IMPLEMENT SCORE TO BASE THE TARGET PLAYER ON!

		XMINT2 location = Level::s_CurrentLevel->m_Players[0]->m_DungeonHeartLocation;
		subtileTarget = XMINT2(location.x * 3, location.y * 3);
	}

	XMINT2 tileTarget = XMINT2(subtileTarget.x/3, subtileTarget.y / 3);
	creature->m_Activity = CreatureTaskManager::Activity(true, subtileTarget, tileTarget, CreatureTaskManager::ActivityType::Activity_Tunnel, &LevelData::m_Map.m_Tiles[tileTarget.y][tileTarget.x]);
	AddCreature(owner, creature);
}
void PlayerBase::AddCreature(uint8_t owner, Creature* c)
{
	if(c->m_CreatureID != CreatureData::CreatureType::CREATURE_IMP)
		LevelScript::GameValues[m_PlayerID]["TOTAL_CREATURES"]++; //Imps don't count toward total creature count

	LevelScript::GameValues[m_PlayerID][CreatureTypeToString[c->m_CreatureID]]++;
	m_CreatureCount[c->m_CreatureID]++;
	m_Creatures.push_back(c);
	c->m_Owner = owner;
	System::tSys->m_Game->AddCreature(c);
}
void PlayerBase::CreatureDied(Creature* c)
{
	for (int i = 0; i < m_Creatures.size(); i++)
	{
		if (m_Creatures[i] == c)
		{
			std::string& creatureTypeString = CreatureTypeToString[c->m_CreatureID];
			if (c->m_CreatureID != CreatureData::CreatureType::CREATURE_IMP)
				LevelScript::GameValues[m_PlayerID]["TOTAL_CREATURES"]--;
			LevelScript::GameValues[m_PlayerID][creatureTypeString]--;
			m_CreatureCount[c->m_CreatureID]--;
			m_Creatures.erase(m_Creatures.begin() + i);
			Entity* e = Level::s_CurrentLevel->m_LevelData->GetMapEntity();
			e->SetSprite(c->m_CreatureID == CreatureData::CreatureType::CREATURE_IMP ? c->m_CreatureSpriteIndex + 82 : c->m_CreatureSpriteIndex + 42);
			e->m_Renderable->SetPosition(c->m_Renderable->m_Position);
			e->ResetScale();
			System::tSys->m_Game->RemoveCreature(c);
			m_DeadCreatures.push_back(c);

			for (int j = 0; j < 5; j++)
			{
				PlayerBase* p = Level::s_CurrentLevel->m_Players[j];
				if (p == nullptr) continue;
				for (int k = 0; k < p->m_Creatures.size(); k++)
				{
					Creature* enemy = p->m_Creatures[k];
					if (enemy->m_CombatTarget == c)
					{
						enemy->m_CombatTarget = nullptr;
						enemy->m_InCombat = false;
						enemy->m_CreatureCBData._isFighting = false;
					}
				}
			}
			break;
		}
	}
}
bool PlayerBase::IsAlliedWith(uint8_t player)
{
	return m_Allies[player][m_PlayerID];
}
void PlayerBase::AllyWith(uint8_t player)
{
	m_Allies[m_PlayerID][player] = true;
}
PlayerBase::~PlayerBase()
{
	for (int i = 0; i < m_Creatures.size(); i++)
	{
		delete m_Creatures[i];
	}
}