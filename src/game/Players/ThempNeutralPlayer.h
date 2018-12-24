#pragma once
#include "ThempPlayerBase.h"

namespace Themp
{

	class NeutralPlayer : public PlayerBase
	{
	public:
		NeutralPlayer(uint8_t playerID);
		virtual void Update(float delta);
	};
};