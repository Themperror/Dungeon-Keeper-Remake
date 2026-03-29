#pragma once
#include "ThempPlayerBase.h"

namespace Themp
{

	class NeutralPlayer : public PlayerBase
	{
	public:
		NeutralPlayer(PlayerID playerID);
		virtual void Update(float delta);
	};
};