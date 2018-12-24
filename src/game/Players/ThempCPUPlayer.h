#pragma once

#include "ThempPlayerBase.h"
namespace Themp
{
	class CPUPlayer : public PlayerBase
	{
	public:
		CPUPlayer(uint8_t playerID);
		~CPUPlayer();
		virtual void Update(float delta);
	};
};