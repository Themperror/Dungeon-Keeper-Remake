#pragma once
#include "ThempPlayerBase.h"
namespace Themp
{
	class Creature;

	class Player : public PlayerBase
	{
	public:
		Player(uint8_t playerID);
		virtual ~Player();
		virtual void Update(float delta);
		int m_GoldAmount;
	};
};