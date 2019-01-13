#include "ThempSystem.h"
#include "ThempCreatureParty.h"
#include "ThempCreature.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"

using namespace Themp;

const std::unordered_map<std::string,CreatureParty::HeroObjective> CreatureParty::StringToObjectives =
{
	{"STEAL_GOLD", HeroObjective::STEAL_GOLD },
	{"STEAL_SPELLS", HeroObjective::STEAL_SPELLS },
	{"ATTACK_ENEMIES", HeroObjective::ATTACK_ENEMIES },
	{"ATTACK_DUNGEON_HEART", HeroObjective::ATTACK_DUNGEON_HEART },
	{"ATTACK_ROOMS", HeroObjective::ATTACK_ROOMS },
	{"DEFEND_PARTY", HeroObjective::DEFEND_PARTY },
};

Themp::CreatureParty::CreatureParty(std::string name)
{
	m_PartyName = name;
}
void Themp::CreatureParty::AddToParty(CreatureData::CreatureType type, uint8_t level, uint32_t gold_hold, HeroObjective objective, int timetillstrike)
{
	m_Partees.push_back(Partee(type, level, gold_hold, objective, timetillstrike));
}
