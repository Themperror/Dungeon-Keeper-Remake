#pragma once
#include "ThempPlayerBase.h"
#include <unordered_map>
namespace Themp
{
	class CreatureParty;
	class GoodPlayer : public PlayerBase
	{
	public:
		GoodPlayer(uint8_t playerID);
		virtual ~GoodPlayer();
		virtual void Update(float delta);
	};
};