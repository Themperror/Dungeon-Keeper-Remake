#pragma once
#include "ThempPlayerBase.h"
#include <unordered_map>
namespace Themp
{
	class CreatureParty;
	class GoodPlayer : public PlayerBase
	{
	public:
		GoodPlayer(PlayerID playerID);
		virtual ~GoodPlayer();
		virtual void Update(float delta);
	};
};