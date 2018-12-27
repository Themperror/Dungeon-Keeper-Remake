#include "ThempSystem.h"
#include "ThempPlayerBase.h"
#include "ThempGame.h"
#include "ThempCreature.h"
#include "ThempLevelScript.h"

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


void PlayerBase::AddCreature(Creature* c)
{
	LevelScript::GameValues[m_PlayerID]["TOTAL_CREATURES"]++;
	LevelScript::GameValues[m_PlayerID][CreatureTypeToString[c->m_CreatureID]]++;
	m_CreatureCount[c->m_CreatureID]++;
	m_Creatures.push_back(c);
	System::tSys->m_Game->AddCreature(c);
}
void PlayerBase::CreatureDied(Creature* c)
{
	for (int i = 0; i < m_Creatures.size(); i++)
	{
		if (m_Creatures[i] == c)
		{
			m_CreatureCount[c->m_CreatureID]--;
			m_Creatures.erase(m_Creatures.begin() + i);
			break;
		}
	}
}
PlayerBase::~PlayerBase()
{
	for (int i = 0; i < m_Creatures.size(); i++)
	{
		delete m_Creatures[i];
	}
}