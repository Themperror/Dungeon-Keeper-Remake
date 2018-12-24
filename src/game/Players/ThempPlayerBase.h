#pragma once
#include <vector>
#include "../ThempCreatureData.h"
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
		std::vector<Creature*> m_Creatures;

		//total of 32 creature types
		int m_CreatureCount[32] = {};
		int m_CreatureAvailable[32] = {};
		bool m_IsEnabled = false;
		void AddCreature(Creature * c);

		virtual void Update(float delta) = 0;
	};
};