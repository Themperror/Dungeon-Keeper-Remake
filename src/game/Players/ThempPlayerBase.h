#pragma once
#include <vector>
#include "../Creature/ThempCreatureData.h"
#include "../Creature/ThempCreatureParty.h"
#include "ThempTileArrays.h"
namespace Themp
{
	class Creature;
	class PlayerBase
	{
	public:
		PlayerBase() {};
		~PlayerBase();

		uint8_t m_PlayerID = Owner_PlayerRed;
		XMINT2 m_DungeonHeartLocation = XMINT2(-1,-1);
		std::vector<Creature*> m_Creatures;
		std::vector<Creature*> m_DeadCreatures;

		//total of 32 creature types
		bool m_Allies[6][6] = { false };
		int m_CreatureCount[32] = {};
		int m_CreatureAvailable[32] = {};
		bool m_IsEnabled = false;
		void AddPartyToLevel(uint8_t owner, CreatureParty party, XMINT2 tilePos);
		void AddTunnellerPartyToLevel(uint8_t owner, int tunnelerLevel, int tunnelerGold, CreatureParty party, XMINT2 spawnPos, std::string headfor, int targetID);
		void AddTunnellerToLevel(uint8_t owner, int tunnelerLevel, int tunnelerGold, XMINT2 spawnPos, std::string headfor, int targetID);
		void AddCreature(uint8_t owner, Creature * c);


		void CreatureDied(Creature * c);

		bool IsAlliedWith(uint8_t player);
		void AllyWith(uint8_t player);

		virtual void Update(float delta) = 0;
	};
};