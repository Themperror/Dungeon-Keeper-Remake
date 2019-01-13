#pragma once
#include <string>
#include <unordered_map>
#include "ThempCreatureData.h"
namespace Themp
{
	class Creature;
	class CreatureParty
	{
	public:
		enum class HeroObjective { NONE, STEAL_GOLD, STEAL_SPELLS, ATTACK_ENEMIES, ATTACK_DUNGEON_HEART, ATTACK_ROOMS, DEFEND_PARTY };
		struct Partee
		{
			Partee(CreatureData::CreatureType a_type, uint8_t a_level, uint32_t a_gold_hold, HeroObjective a_objective, int a_timetillstrike)
			{
				type = a_type;
				level = a_level;
				gold_hold = a_gold_hold;
				objective = a_objective;
				timetillstrike = a_timetillstrike;
			}
			CreatureData::CreatureType type;
			uint8_t level;
			uint32_t gold_hold;
			//number of game turns till they start their attack
			int timetillstrike;
			HeroObjective objective;
		};
		CreatureParty() { }
		CreatureParty(std::string name);
		void AddToParty(CreatureData::CreatureType type, uint8_t level, uint32_t gold_hold, HeroObjective objective, int timetillstrike);

		std::string m_PartyName;
		std::vector<Partee> m_Partees;
		static const std::unordered_map<std::string, HeroObjective> StringToObjectives;
	};
};