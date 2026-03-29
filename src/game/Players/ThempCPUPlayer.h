#pragma once

#include "ThempPlayerBase.h"
namespace Themp
{
	class CPUPlayer : public PlayerBase
	{
	public:
		CPUPlayer(PlayerID playerID);
		~CPUPlayer();
		virtual void Update(float delta);
	};
};