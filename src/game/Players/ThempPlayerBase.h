#pragma once
#include <vector>
#include "../Creature/ThempCreatureData.h"
#include "../Creature/ThempCreatureParty.h"
#include "ThempGameTypes.h"

#include <DirectXMath.h>
using namespace DirectX;

namespace Themp
{
	class Creature;
	class PlayerBase
	{
	public:
		PlayerBase() {};
		~PlayerBase();

		PlayerID m_PlayerID = PlayerID::Red;
		XMINT2 m_DungeonHeartLocation = XMINT2(-1,-1);
		std::vector<Creature*> m_Creatures;
		std::vector<Creature*> m_DeadCreatures;

		//total of 32 creature types
		bool m_Allies[6][6] = { false };
		int m_CreatureCount[32] = {};
		int m_CreatureAvailable[32] = {};
		bool m_IsEnabled = false;
		void AddPartyToLevel(PlayerID owner, CreatureParty party, XMINT2 tilePos);
		void AddTunnellerPartyToLevel(PlayerID owner, int tunnelerLevel, int tunnelerGold, CreatureParty party, XMINT2 spawnPos, std::string headfor, int targetID);
		void AddTunnellerToLevel(PlayerID owner, int tunnelerLevel, int tunnelerGold, XMINT2 spawnPos, std::string headfor, int targetID);
		void AddCreature(PlayerID owner, Creature * c);


		void CreatureDied(Creature * c);

		bool IsAlliedWith(PlayerID player);
		void AllyWith(PlayerID player);

		virtual void Update(float delta) = 0;
	};
};