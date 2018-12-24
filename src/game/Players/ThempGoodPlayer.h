#pragma once

#include "ThempPlayerBase.h"
namespace Themp
{

	class GoodPlayer : public PlayerBase
	{
	public:
		GoodPlayer(uint8_t playerID);
		virtual void Update(float delta);
	};
};