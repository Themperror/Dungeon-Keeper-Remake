#include "ThempSystem.h"
#include "ThempCreature.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempLevel.h"
#include "ThempLevelData.h"
#include "ThempResources.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include "ThempTileArrays.h"
#include "ThempEntity.h"
#include "ThempAudio.h"
#include "ThempLevelConfig.h"
#include "ThempLevelScript.h"
#include "Players/ThempPlayerBase.h"
#include <DirectXMath.h>
#include <unordered_map>
#include <array>
#include <imgui.h>
using namespace Themp;

//2 types (FP or World), 5 directions, 12 states
const std::array<std::array<std::array<CreatureData::ImpAnimations, 12>, 5>, 2> ImpAnimState =
{
	//5 directional sprites
	std::array<std::array<CreatureData::ImpAnimations, 12>, 5>
	{
		std::array<CreatureData::ImpAnimations, 12>
		{
			CreatureData::IMP_FP_WALK_Forward,
			CreatureData::IMP_FP_DROPPING_Forward,
			CreatureData::IMP_FP_MINE_Forward,
			CreatureData::IMP_FP_HIT_Forward,
			CreatureData::IMP_FP_IDLE_Forward,
			CreatureData::IMP_FP_CLAIM_Forward,
			CreatureData::IMP_FP_DRAG_Forward,
			CreatureData::IMP_FP_IDLE2_Forward,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Forward,
		},
		{
			CreatureData::IMP_FP_WALK_DiagForwardRight,
			CreatureData::IMP_FP_DROPPING_DiagForwardRight,
			CreatureData::IMP_FP_MINE_DiagForwardRight,
			CreatureData::IMP_FP_HIT_DiagForwardRight,
			CreatureData::IMP_FP_IDLE_DiagForwardRight,
			CreatureData::IMP_FP_CLAIM_DiagForwardRight,
			CreatureData::IMP_FP_DRAG_DiagForwardRight,
			CreatureData::IMP_FP_IDLE2_DiagForwardRight,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_DiagForwardRight,
		},
		{
			CreatureData::IMP_FP_WALK_Right,
			CreatureData::IMP_FP_DROPPING_Right,
			CreatureData::IMP_FP_MINE_Right,
			CreatureData::IMP_FP_HIT_Right,
			CreatureData::IMP_FP_IDLE_Right,
			CreatureData::IMP_FP_CLAIM_Right,
			CreatureData::IMP_FP_DRAG_Right,
			CreatureData::IMP_FP_IDLE2_Right,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Right,
		},
		{
			CreatureData::IMP_FP_WALK_DiagBackRight,
			CreatureData::IMP_FP_DROPPING_DiagBackRight,
			CreatureData::IMP_FP_MINE_DiagBackRight,
			CreatureData::IMP_FP_HIT_DiagBackRight,
			CreatureData::IMP_FP_IDLE_DiagBackRight,
			CreatureData::IMP_FP_CLAIM_DiagBackRight,
			CreatureData::IMP_FP_DRAG_DiagBackRight,
			CreatureData::IMP_FP_IDLE2_DiagBackRight,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_DiagBackRight,
		},
		{
			CreatureData::IMP_FP_WALK_Back,
			CreatureData::IMP_FP_DROPPING_Back,
			CreatureData::IMP_FP_MINE_Back,
			CreatureData::IMP_FP_HIT_Back,
			CreatureData::IMP_FP_IDLE_Back,
			CreatureData::IMP_FP_CLAIM_Back,
			CreatureData::IMP_FP_DRAG_Back,
			CreatureData::IMP_FP_IDLE2_Back,
			CreatureData::IMP_FP_DAZED,
			CreatureData::IMP_FP_DIE,
			CreatureData::IMP_FP_HAND,
			CreatureData::IMP_FP_SLAPPED_Back,
		},
	},
	//5 directional sprites
	std::array<std::array<CreatureData::ImpAnimations, 12>, 5>
	{
		std::array<CreatureData::ImpAnimations, 12>
		{
			CreatureData::IMP_W_WALK_Forward,
			CreatureData::IMP_W_DROPPING_Forward,
			CreatureData::IMP_W_MINE_Forward,
			CreatureData::IMP_W_HIT_Forward,
			CreatureData::IMP_W_IDLE_Forward,
			CreatureData::IMP_W_CLAIM_Forward,
			CreatureData::IMP_W_DRAG_Forward,
			CreatureData::IMP_W_IDLE2_Forward,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Forward,
		},
		{
			CreatureData::IMP_W_WALK_DiagForwardRight,
			CreatureData::IMP_W_DROPPING_DiagForwardRight,
			CreatureData::IMP_W_MINE_DiagForwardRight,
			CreatureData::IMP_W_HIT_DiagForwardRight,
			CreatureData::IMP_W_IDLE_DiagForwardRight,
			CreatureData::IMP_W_CLAIM_DiagForwardRight,
			CreatureData::IMP_W_DRAG_DiagForwardRight,
			CreatureData::IMP_W_IDLE2_DiagForwardRight,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_DiagForwardRight,
		},
		{
			CreatureData::IMP_W_WALK_Right,
			CreatureData::IMP_W_DROPPING_Right,
			CreatureData::IMP_W_MINE_Right,
			CreatureData::IMP_W_HIT_Right,
			CreatureData::IMP_W_IDLE_Right,
			CreatureData::IMP_W_CLAIM_Right,
			CreatureData::IMP_W_DRAG_Right,
			CreatureData::IMP_W_IDLE2_Right,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Right,
		},
		{
			CreatureData::IMP_W_WALK_DiagBackRight,
			CreatureData::IMP_W_DROPPING_DiagBackRight,
			CreatureData::IMP_W_MINE_DiagBackRight,
			CreatureData::IMP_W_HIT_DiagBackRight,
			CreatureData::IMP_W_IDLE_DiagBackRight,
			CreatureData::IMP_W_CLAIM_DiagBackRight,
			CreatureData::IMP_W_DRAG_DiagBackRight,
			CreatureData::IMP_W_IDLE2_DiagBackRight,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_DiagBackRight,
		},
		{
			CreatureData::IMP_W_WALK_Back,
			CreatureData::IMP_W_DROPPING_Back,
			CreatureData::IMP_W_MINE_Back,
			CreatureData::IMP_W_HIT_Back,
			CreatureData::IMP_W_IDLE_Back,
			CreatureData::IMP_W_CLAIM_Back,
			CreatureData::IMP_W_DRAG_Back,
			CreatureData::IMP_W_IDLE2_Back,
			CreatureData::IMP_W_DAZED,
			CreatureData::IMP_W_DIE,
			CreatureData::IMP_W_HAND,
			CreatureData::IMP_W_SLAPPED_Back,
		},
	},
};

//Hellhound uses this
std::array<std::array<int, 12>, 5> FPAnimData72 =
{
	std::array<int, 12>
	{
		0 ,//  CreatureData::FP_WALK_Forward,
		10,//  CreatureData::FP_DROPPING_Forward,
		20,//  CreatureData::FP_MELEE_Forward,
		30,//  CreatureData::FP_HIT_Forward,
		40,//  CreatureData::FP_BEG_Forward,
		42,//  CreatureData::FP_DIE,
		44,//  CreatureData::FP_HAND,
		46,//  CreatureData::FP_HAPPY,
		48,//  CreatureData::FP_SLEEP,
		30,//  CreatureData::FP_SLAPPED_Forward,
		60,//  CreatureData::FP_EAT,
		62,//  CreatureData::FP_PISS_Forward,
	},
	{
		1 ,//  CreatureData::FP_WALK_DiagForwardRight,
		11,//  CreatureData::FP_DROPPING_DiagForwardRight,
		21,//  CreatureData::FP_MELEE_DiagForwardRight,
		31,//  CreatureData::FP_HIT_DiagForwardRight,
		40,//  CreatureData::FP_BEG_DiagForwardRight,
		42,//  CreatureData::FP_DIE,
		44,//  CreatureData::FP_HAND,
		46,//  CreatureData::FP_HAPPY,
		48,//  CreatureData::FP_SLEEP,
		31,//  CreatureData::FP_SLAPPED_DiagForwardRight,
		60,//  CreatureData::FP_EAT,
		63,//  CreatureData::FP_PISS_DiagForwardRight,
	},
	{
		2 ,//  CreatureData::FP_WALK_Right,
		12,//  CreatureData::FP_DROPPING_Right,
		22,//  CreatureData::FP_MELEE_Right,
		32,//  CreatureData::FP_HIT_Right,
		40,//  CreatureData::FP_BEG_Right,
		42,//  CreatureData::FP_DIE,
		44,//  CreatureData::FP_HAND,
		46,//  CreatureData::FP_HAPPY,
		48,//  CreatureData::FP_SLEEP,
		32,//  CreatureData::FP_SLAPPED_Right,
		60,//  CreatureData::FP_EAT,
		64,//  CreatureData::FP_PISS_Right,
	},
	{
		3 ,//  CreatureData::FP_WALK_DiagBackRight,
		13,//  CreatureData::FP_DROPPING_DiagBackRight,
		23,//  CreatureData::FP_MELEE_DiagBackRight,
		33,//  CreatureData::FP_HIT_DiagBackRight,
		40,//  CreatureData::FP_BEG_DiagBackRight,
		42,//  CreatureData::FP_DIE,
		44,//  CreatureData::FP_HAND,
		46,//  CreatureData::FP_HAPPY,
		48,//  CreatureData::FP_SLEEP,
		33,//  CreatureData::FP_SLAPPED_DiagBackRight,
		60,//  CreatureData::FP_EAT,
		65,//  CreatureData::FP_PISS_DiagBackRight,
	},
	{
		4 ,//  CreatureData::FP_WALK_Back,
		14,//  CreatureData::FP_DROPPING_Back,
		24,//  CreatureData::FP_MELEE_Back,
		34,//  CreatureData::FP_HIT_Back,
		40,//  CreatureData::FP_BEG_Back,
		42,//  CreatureData::FP_DIE,
		44,//  CreatureData::FP_HAND,
		46,//  CreatureData::FP_HAPPY,
		48,//  CreatureData::FP_SLEEP,
		34,//  CreatureData::FP_SLAPPED_Back,
		60,//  CreatureData::FP_EAT,
		66,//  CreatureData::FP_PISS_Back,
	},
};
std::array<std::array<int, 12>, 5> WAnimData72 =
{
	std::array<int, 12>
	{
		5 ,//  CreatureData::FP_WALK_Forward,
		15,//  CreatureData::FP_DROPPING_Forward,
		25,//  CreatureData::FP_MELEE_Forward,
		35,//  CreatureData::FP_HIT_Forward,
		41,//  CreatureData::FP_BEG_Forward,
		43,//  CreatureData::FP_DIE,
		45,//  CreatureData::FP_HAND,
		47,//  CreatureData::FP_HAPPY,
		49,//  CreatureData::FP_SLEEP,
		35,//  CreatureData::FP_SLAPPED_Forward,
		61,//  CreatureData::FP_EAT,
		67,//  CreatureData::FP_PISS_Forward,
	},
	{
		6 ,//  CreatureData::FP_WALK_Forward,
		16,//  CreatureData::FP_DROPPING_Forward,
		26,//  CreatureData::FP_MELEE_Forward,
		36,//  CreatureData::FP_HIT_Forward,
		41,//  CreatureData::FP_BEG_Forward,
		43,//  CreatureData::FP_DIE,
		45,//  CreatureData::FP_HAND,
		47,//  CreatureData::FP_HAPPY,
		49,//  CreatureData::FP_SLEEP,
		36,//  CreatureData::FP_SLAPPED_Forward,
		61,//  CreatureData::FP_EAT,
		68,//  CreatureData::FP_PISS_Forward,
	},
	{
		7 ,//  CreatureData::FP_WALK_Forward,
		17,//  CreatureData::FP_DROPPING_Forward,
		27,//  CreatureData::FP_MELEE_Forward,
		37,//  CreatureData::FP_HIT_Forward,
		41,//  CreatureData::FP_BEG_Forward,
		43,//  CreatureData::FP_DIE,
		45,//  CreatureData::FP_HAND,
		47,//  CreatureData::FP_HAPPY,
		49,//  CreatureData::FP_SLEEP,
		37,//  CreatureData::FP_SLAPPED_Forward,
		61,//  CreatureData::FP_EAT,
		69,//  CreatureData::FP_PISS_Forward,
	},
	{
		8 ,//  CreatureData::FP_WALK_Forward,
		18,//  CreatureData::FP_DROPPING_Forward,
		28,//  CreatureData::FP_MELEE_Forward,
		38,//  CreatureData::FP_HIT_Forward,
		41,//  CreatureData::FP_BEG_Forward,
		43,//  CreatureData::FP_DIE,
		45,//  CreatureData::FP_HAND,
		47,//  CreatureData::FP_HAPPY,
		49,//  CreatureData::FP_SLEEP,
		38,//  CreatureData::FP_SLAPPED_Forward,
		61,//  CreatureData::FP_EAT,
		70,//  CreatureData::FP_PISS_Forward,
	},
	{
		9 ,//  CreatureData::FP_WALK_Forward,
		19,//  CreatureData::FP_DROPPING_Forward,
		29,//  CreatureData::FP_MELEE_Forward,
		39,//  CreatureData::FP_HIT_Forward,
		41,//  CreatureData::FP_BEG_Forward,
		43,//  CreatureData::FP_DIE,
		45,//  CreatureData::FP_HAND,
		47,//  CreatureData::FP_HAPPY,
		49,//  CreatureData::FP_SLEEP,
		39,//  CreatureData::FP_SLAPPED_Forward,
		61,//  CreatureData::FP_EAT,
		71,//  CreatureData::FP_PISS_Forward,
	},
};

std::array<std::array<int, 12>, 5> FPAnimData62 =
{
	//Forward
	std::array<int,12>
	{
		0,  //{CreatureData::Walking	,	},
		10, //{CreatureData::Dropping	,	},
		20, //{CreatureData::Attacking,	},
		30, //{CreatureData::Hit		,	},
		40, //{CreatureData::Begging	,	},
		42, //{CreatureData::Dying	,	},
		44, //{CreatureData::Held		,	},
		46, //{CreatureData::Happy	,	},
		48, //{CreatureData::Sleeping	,	},
		50, //{CreatureData::Slapped	,	},
		60, //{CreatureData::Eating	,	},
		0, // {CreatureData::Pissing	,	,
	},
	//DiagForwardRight
	{
		1,
		11,
		21,
		31,
		40,
		42,
		44,
		46,
		48,
		51,
		60,
		1,
	},
	//Right
	{
		2,
		12,
		22,
		32,
		40,
		42,
		44,
		46,
		48,
		52,
		60,
		2,
	},
	//DiagBackRight
	{
		3,
		13,
		23,
		33,
		40,
		42,
		44,
		46,
		48,
		53,
		60,
		3,
	},
	//Back
	{
		4,
		14,
		24,
		34,
		40,
		42,
		44,
		46,
		48,
		54,
		60,
		4,
	},
};
std::array<std::array<int, 12>, 5> WAnimData62 =
{
	//Forward
	std::array<int,12>
	{
		5,  //{CreatureData::Walking	,	},
		15, //{CreatureData::Dropping	,	},
		25, //{CreatureData::Attacking,	},
		35, //{CreatureData::Hit		,	},
		41, //{CreatureData::Begging	,	},
		43, //{CreatureData::Dying	,	},
		45, //{CreatureData::Held		,	},
		47, //{CreatureData::Happy	,	},
		49, //{CreatureData::Sleeping	,	},
		55, //{CreatureData::Slapped	,	},
		61, //{CreatureData::Eating	,	},
		5, // {CreatureData::Pissing	,	,
	},
	//DiagForwardRight
	{
		6,
		16,
		26,
		36,
		41,
		43,
		44,
		47,
		49,
		56,
		61,
		6,
	},
	//Right
	{
		7,
		17,
		27,
		37,
		41,
		43,
		44,
		47,
		49,
		57,
		61,
		7,
	},
	//DiagBackRight
	{
		8,
		18,
		28,
		38,
		41,
		43,
		44,
		47,
		49,
		58,
		61,
		8,
	},
	//Back
	{
		9,
		19,
		29,
		39,
		41,
		43,
		44,
		47,
		49,
		59,
		61,
		9,
	},
};
std::array<std::array<int, 12>, 5> FPAnimData52 =
{
	std::array<int, 12>
	{
		0  ,//CreatureData::FP_WALK_Forward,   
		0  ,//CreatureData::FP_NULL, 
		10 ,//CreatureData::FP_MELEE_Forward, 
		20 ,//CreatureData::FP_HIT_Forward, 
		30 ,//CreatureData::FP_BEG, 
		32 ,//CreatureData::FP_DIE, 
		34 ,//CreatureData::FP_HAND, 
		36 ,//CreatureData::FP_HAPPY, 
		38 ,//CreatureData::FP_SLEEP, 
		40 ,//CreatureData::FP_SLAPPED_Forward, 
		50 ,//CreatureData::FP_EAT, 
		0  ,//CreatureData::FP_NULL, 0
	},
	{

		1  ,
		1  ,
		11 ,
		21 ,
		30 ,
		32 ,
		34 ,
		36 ,
		38 ,
		41 ,
		50 ,
		0  ,
	},
	{
		2  ,
		2  ,
		12 ,
		22 ,
		30 ,
		32 ,
		34 ,
		36 ,
		38 ,
		42 ,
		50 ,
		0  ,
	},
	{
		3  ,
		3  ,
		13 ,
		23 ,
		30 ,
		32 ,
		34 ,
		36 ,
		38 ,
		44 ,
		50 ,
		0  ,
	},
	{
		4  ,
		4  ,
		14 ,
		24 ,
		30 ,
		32 ,
		34 ,
		36 ,
		38 ,
		44 ,
		50 ,
		0  ,
	},
};
std::array<std::array<int, 12>, 5> WAnimData52 =
{
	std::array<int, 12>
	{
		5  ,//CreatureData::FP_WALK_Forward,   
		5  ,//CreatureData::FP_NULL, 
		15 ,//CreatureData::FP_MELEE_Forward, 
		25 ,//CreatureData::FP_HIT_Forward, 
		31 ,//CreatureData::FP_BEG, 
		33 ,//CreatureData::FP_DIE, 
		35 ,//CreatureData::FP_HAND, 
		37 ,//CreatureData::FP_HAPPY, 
		39 ,//CreatureData::FP_SLEEP, 
		45 ,//CreatureData::FP_SLAPPED_Forward, 
		51 ,//CreatureData::FP_EAT, 
		5  ,//CreatureData::FP_NULL, 0
	},
	{

		6  ,
		6  ,
		16 ,
		26 ,
		31 ,
		33 ,
		35 ,
		37 ,
		39 ,
		46 ,
		51 ,
		6  ,
	},
	{
		7  ,
		7  ,
		17 ,
		27 ,
		31 ,
		33 ,
		35 ,
		37 ,
		39 ,
		47 ,
		51 ,
		7  ,
	},
	{
		8  ,
		8  ,
		18 ,
		28 ,
		31 ,
		33 ,
		35 ,
		37 ,
		39 ,
		48 ,
		51 ,
		8  ,
	},
	{
		9  ,
		9  ,
		19 ,
		29 ,
		31 ,
		33 ,
		35 ,
		37 ,
		39 ,
		49 ,
		51 ,
		9  ,
	},
};
//2 types (FP or World), for each creature, 5 directions, 12 states
const std::array<std::unordered_map<CreatureData::CreatureSpriteIndex,std::array<std::array<int,12>,5>>,2>	CreatureAnimState =
{
	std::unordered_map<CreatureData::CreatureSpriteIndex,std::array<std::array<int, 12>, 5>>
	{
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Hellhound , FPAnimData72},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Samurai , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Avatar , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Orc , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Monk , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Knight , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Thief , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Fairy , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Giant , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Witch , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Tunneler , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Archer , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Barbarian , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Wizard , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Tentacle , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Ghost , FPAnimData52},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Spider , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Vampire , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Beetle , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_BileDemon , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Warlock , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_DarkMistress , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Fly , FPAnimData52},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_DemonSpawn , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Dragon , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Troll , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Skeleton , FPAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Horny , FPAnimData62},
	},
	std::unordered_map<CreatureData::CreatureSpriteIndex,std::array<std::array<int, 12>, 5>>
	{
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Hellhound , WAnimData72},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Samurai , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Avatar , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Orc , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Monk , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Knight , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Thief , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Fairy , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Giant , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Witch , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Tunneler , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Archer , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Barbarian , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Wizard , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Tentacle , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Ghost , WAnimData52},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Spider , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Vampire , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Beetle , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_BileDemon , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Warlock , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_DarkMistress , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Fly , WAnimData52},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_DemonSpawn , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Dragon , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Troll , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Skeleton , WAnimData62},
		{CreatureData::CreatureSpriteIndex::CreatureSprite_Horny , WAnimData62},
	},
};

const std::unordered_map<CreatureData::CreatureType, CreatureData::CreatureSpriteIndex> TypeToSprite =
{
	{CreatureData::CREATURE_NULL, CreatureData::CreatureSprite_Ghost },
	{CreatureData::CREATURE_WIZARD, CreatureData::CreatureSprite_Wizard },
	{CreatureData::CREATURE_BARBARIAN, CreatureData::CreatureSprite_Barbarian },
	{CreatureData::CREATURE_ARCHER, CreatureData::CreatureSprite_Archer },
	{CreatureData::CREATURE_MONK, CreatureData::CreatureSprite_Monk },
	{CreatureData::CREATURE_DWARF, CreatureData::CreatureSprite_Dwarf },
	{CreatureData::CREATURE_KNIGHT, CreatureData::CreatureSprite_Knight },
	{CreatureData::CREATURE_AVATAR, CreatureData::CreatureSprite_Avatar },
	{CreatureData::CREATURE_TUNNELLER, CreatureData::CreatureSprite_Tunneler },
	{CreatureData::CREATURE_WITCH, CreatureData::CreatureSprite_Witch },
	{CreatureData::CREATURE_GIANT, CreatureData::CreatureSprite_Giant },
	{CreatureData::CREATURE_FAIRY, CreatureData::CreatureSprite_Fairy },
	{CreatureData::CREATURE_THIEF, CreatureData::CreatureSprite_Thief },
	{CreatureData::CREATURE_SAMURAI, CreatureData::CreatureSprite_Samurai },
	{CreatureData::CREATURE_HORNY, CreatureData::CreatureSprite_Horny },
	{CreatureData::CREATURE_SKELETON, CreatureData::CreatureSprite_Skeleton },
	{CreatureData::CREATURE_TROLL, CreatureData::CreatureSprite_Troll },
	{CreatureData::CREATURE_DRAGON, CreatureData::CreatureSprite_Dragon },
	{CreatureData::CREATURE_DEMONSPAWN, CreatureData::CreatureSprite_DemonSpawn },
	{CreatureData::CREATURE_FLY, CreatureData::CreatureSprite_Fly },
	{CreatureData::CREATURE_DARK_MISTRESS, CreatureData::CreatureSprite_DarkMistress },
	{CreatureData::CREATURE_SORCEROR, CreatureData::CreatureSprite_Warlock },
	{CreatureData::CREATURE_BILE_DEMON, CreatureData::CreatureSprite_BileDemon},
	{CreatureData::CREATURE_IMP, CreatureData::CreatureSprite_Imp },
	{CreatureData::CREATURE_BEETLE, CreatureData::CreatureSprite_Beetle },
	{CreatureData::CREATURE_VAMPIRE, CreatureData::CreatureSprite_Vampire },
	{CreatureData::CREATURE_SPIDER, CreatureData::CreatureSprite_Spider },
	{CreatureData::CREATURE_HELLHOUND, CreatureData::CreatureSprite_Hellhound },
	{CreatureData::CREATURE_GHOST, CreatureData::CreatureSprite_Ghost },
	{CreatureData::CREATURE_TENTACLE, CreatureData::CreatureSprite_Tentacle },
	{CreatureData::CREATURE_ORC, CreatureData::CreatureSprite_Orc },
	{CreatureData::CREATURE_FLOATING_SPIRIT, CreatureData::CreatureSprite_Ghost },
};


Themp::Creature::~Creature()
{
	if (m_CreatureCB)
	{
		m_CreatureCB->Release();
		m_CreatureCB = nullptr;
	}
}

Themp::Creature::Creature(CreatureData::CreatureType creatureIndex)
{
	m_CreatureID = creatureIndex;
	m_CreatureData = LevelConfig::creatureData[m_CreatureID];
	m_CreatureSpriteIndex = TypeToSprite.find(creatureIndex)->second;
	m_AnimState = CreatureData::AnimationState::Walking;
	m_Renderable = new Object3D();
	m_Renderable->CreateQuad("creature", false);
	Material* material = Resources::TRes->GetUniqueMaterial("", "creature");
	m_Renderable->SetMaterial(material);

	SetSprite(m_CreatureSpriteIndex);
	m_Speed = BASESPEED_TO_DELTA(m_CreatureData.BaseSpeed);
	//const float creatureYSize = (float)m_CreatureData.SizeYZ / 256.0f;
	//m_Renderable->m_Scale = XMFLOAT3(creatureYSize, creatureYSize, creatureYSize);
	m_Direction = XMFLOAT3(1, 0, 0);

	m_CreatureCBData._AnimIndex = 0;
	m_CreatureCBData._NumAnim = (float)m_Sprite->numAnim;
	m_CreatureCBData._SpriteWidth = (float)m_Sprite->texture->m_Width;
	m_CreatureCBData._isFrozen = false;
	m_CreatureCBData._isFlipped = false;
	m_CreatureCBData._isHovered = false;
	if (!m_CreatureCB)
	{
		// Fill in a buffer description.
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CreatureConstantBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_CreatureCBData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the buffer.
		Themp::System::tSys->m_D3D->m_Device->CreateBuffer(&cbDesc, &InitData, &m_CreatureCB);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		Themp::System::tSys->m_D3D->m_DevCon->Map(m_CreatureCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_CreatureCBData, sizeof(CreatureConstantBuffer));
		Themp::System::tSys->m_D3D->m_DevCon->Unmap(m_CreatureCB, NULL);
	}
	for (int i = 0; i < 10; i++)
	{
		m_PowerCooldownTimer[i] = 0.0f;
	}
	m_Renderable->m_Meshes[0]->m_ConstantBuffer = m_CreatureCB;
}
void Creature::SetPosition(int subTileX,int height, int subTileY)
{
	m_Renderable->SetPosition(XMFLOAT3(subTileX, height, subTileY));
}
void Creature::SetSprite(int SpriteID)
{
	m_Sprite = FileManager::GetCreatureSprite(SpriteID);
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Sprite->texture);
	m_CreatureCBData._NumAnim = (float)m_Sprite->numAnim;

	//const float creatureYSize = (float)m_CreatureData.SizeYZ / 256.0f;
	//m_Renderable->m_Scale = XMFLOAT3(creatureYSize, creatureYSize, creatureYSize);
}
void Creature::SetToFreshAnimation(CreatureData::AnimationState anim)
{
	m_AnimState = anim;
	m_AnimationIndex = 0;
	m_AnimationTime = 0;
}
void Creature::Update(float delta)
{
	for (int i = 0; i < 10; i++)
	{
		if(m_PowerCooldownTimer[i] > 0.0f)
			m_PowerCooldownTimer[i] -= delta;
	}

	m_AnimationTime += delta;
	if (m_AnimationTime > 1.0f / 15.0f)
	{
		if (m_AnimationTime > 2 * (1.0f / 15.0f))
		{
			m_AnimationTime = 2 * (1.0f / 15.0f);
		}
		m_AnimationTime -= 1.0f / 15.0f;
		m_AnimationIndex++;
		if (m_AnimationIndex >= m_CreatureCBData._NumAnim)
		{
			AnimationDoneEvent();
		}
		m_AnimationIndex = m_AnimationIndex % (int)m_CreatureCBData._NumAnim;
		m_CreatureCBData._AnimIndex = (float)m_AnimationIndex;
	}

	const XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
	const XMINT2 subTilePos = XMINT2((int)round(m_Renderable->m_Position.x), (int)round(m_Renderable->m_Position.z));
	m_Renderable->m_Position.y = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].pathSubTiles[subTilePos.y % 3][subTilePos.x % 3].height;
	if (m_CreatureID == CreatureData::CREATURE_IMP)
	{
		ImpUpdate(delta);
		DoAnimationDirectionsImp();
	}
	else
	{
		CreatureUpdate(delta);
		DoAnimationDirections();
	}
	m_Renderable->isDirty = true;

	CheckVisibility();
	//DebugDraw::Line(m_Renderable->m_Position, m_Renderable->m_Position + m_Direction * 3);
}

void Creature::GetTask()
{
	if (m_Order.valid) return;
	XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
	int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;

	//priority - top to bottom:
	//If full on gold -> bring to treasury
	//Drag unconcious creatures to prison 
	//Drag dead bodies to graveyard
	//Mine Gold/Gem
	//Mine Rock/Walls
	//Claim tiles
	//Collect Special / Traps / Spells
	//Reinforce walls
	//Build traps
	//Deliver ANY gold
	if (m_CurrentGoldHold >= m_CreatureData.GoldHold && CreatureTaskManager::IsTreasuryAvailable(this, areaCode))
	{
		m_Order = CreatureTaskManager::GetAvailableTreasury(this, areaCode);
		if (m_Order.valid) return;
	}

	m_Order = CreatureTaskManager::GetSoloMiningTask(this, areaCode);
	if (m_Order.valid)return;

	m_Order = CreatureTaskManager::GetMiningTask(this, areaCode);
	if (m_Order.valid) return;

	m_Order = CreatureTaskManager::GetClaimingTask(this, areaCode);
	if (m_Order.valid) return;

	m_Order = CreatureTaskManager::GetReinforcingTask(this, areaCode);
	if (m_Order.valid) return;

	if (m_CurrentGoldHold > 0)
	{
		m_Order = CreatureTaskManager::GetAvailableTreasury(this, areaCode);
		if (m_Order.valid) return;
	}
	m_Order = CreatureTaskManager::GetRandomMovementOrder(this, areaCode);
}
bool Creature::PathTo(float deltaTime, XMINT2 targetSubTile, bool ignoreWalls)
{
	int pathingResult = micropather::MicroPather::SOLVED;
	if (LevelData::PathsInvalidated || m_Path.size() == 0 || m_JustSlapped)
	{
		m_JustSlapped = false;
		const XMINT3 subTilePos = LevelData::WorldToSubtile(m_Renderable->m_Position);
		float PathCost = 0;
		m_CurrentPathIndex = 0;
		m_PathLerpTime = 0;

		XMINT2 targetTilePos = LevelData::WorldToTile(XMFLOAT3(targetSubTile.x, 2, targetSubTile.y));
		const int targetAreaCode = LevelData::m_Map.m_Tiles[targetTilePos.y][targetTilePos.x].areaCode;

		const XMINT2 currentTilePos = LevelData::WorldToTile(m_Renderable->m_Position);
		const int areaCode = LevelData::m_Map.m_Tiles[currentTilePos.y][currentTilePos.x].areaCode;

		if (targetAreaCode != areaCode)
		{
			if (ignoreWalls)
			{
				pathingResult = System::tSys->m_Game->m_CurrentLevel->PathFindThroughWalls(XMINT2(subTilePos.x, subTilePos.z), targetSubTile, m_Path, PathCost, true);
			}
			else
			{
				pathingResult = micropather::MicroPather::NO_SOLUTION;
			}
		}
		else
		{
			pathingResult = System::tSys->m_Game->m_CurrentLevel->PathFind(XMINT2(subTilePos.x, subTilePos.z), targetSubTile, m_Path, PathCost, true);
		}

		if (pathingResult != micropather::MicroPather::SOLVED && pathingResult != micropather::MicroPather::START_END_SAME)
		{
			taskString = "Invalid Path!";
			LevelData::PathsInvalidated = true;
			if (m_CreatureID == CreatureData::CREATURE_IMP)
			{
				System::Print("Imp could not find path, resetting world pathing");
				CreatureTaskManager::UnlistImpFromTask(this);
			}
			else
			{
				System::Print("Creature could not find path, resetting world pathing");
			}
		}
	}

	m_PathLerpTime += deltaTime * m_Speed;
	if (pathingResult == micropather::MicroPather::SOLVED || pathingResult == micropather::MicroPather::START_END_SAME)
	{
		if (m_PathLerpTime >= 1.0f)
		{
			m_PathLerpTime = 0.0f;
			m_CurrentPathIndex++;
		}
		XMFLOAT3 currentPos = m_Renderable->m_Position;
		const XMINT3 currentSubTilePos = LevelData::WorldToSubtile(currentPos);
		currentPos.y = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight(currentSubTilePos.z / 3, currentSubTilePos.x / 3, currentSubTilePos.z % 3, currentSubTilePos.x % 3);
		float OldPathX = m_Renderable->m_Position.x;
		float OldPathY = m_Renderable->m_Position.z;

		if (m_CurrentPathIndex < m_Path.size())
		{
			taskString = "Pathing to task!";
			if (m_CurrentPathIndex > 0)
			{
				OldPathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex - 1]) & 0xFFFFFFFF);
				OldPathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex - 1]) >> 32) & 0xFFFFFFFF);
			}
			const float PathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex]) & 0xFFFFFFFF);
			const float PathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex]) >> 32) & 0xFFFFFFFF);


			const XMFLOAT2 nPos = Lerp(XMFLOAT2(OldPathX, OldPathY), XMFLOAT2(PathX, PathY), m_PathLerpTime);
			XMFLOAT2 dir = XMFLOAT2(OldPathX, OldPathY) - nPos;
			if (dir.x != 0 || dir.y != 0)
			{
				dir = Normalize(dir);
				m_Direction.x = dir.x;
				m_Direction.z = dir.y;
			}
			const int8_t height = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight((int)round(nPos.y) / 3, (int)round(nPos.x) / 3, (int)round(nPos.y) % 3, (int)round(nPos.x) % 3);
			if (height <= 5)
				m_Renderable->SetPosition(nPos.x, height, nPos.y);

			m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Walking;
			m_AnimState = CreatureData::AnimationState::Walking;
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
bool Creature::TunnelPathTo(float deltaTime, XMINT2 targetSubTile, bool ignoreWalls)
{
	int pathingResult = micropather::MicroPather::SOLVED;
	if (LevelData::PathsInvalidated || m_Path.size() == 0 || m_JustSlapped)
	{
		m_JustSlapped = false;
		const XMINT3 subTilePos = LevelData::WorldToSubtile(m_Renderable->m_Position);
		float PathCost = 0;
		m_CurrentPathIndex = 0;
		m_PathLerpTime = 0;

		XMINT2 targetTilePos = LevelData::WorldToTile(XMFLOAT3(targetSubTile.x, 2, targetSubTile.y));
		const int targetAreaCode = LevelData::m_Map.m_Tiles[targetTilePos.y][targetTilePos.x].areaCode;

		const XMINT2 currentTilePos = LevelData::WorldToTile(m_Renderable->m_Position);
		const int areaCode = LevelData::m_Map.m_Tiles[currentTilePos.y][currentTilePos.x].areaCode;

		if (targetAreaCode != areaCode)
		{
			if (ignoreWalls)
			{
				pathingResult = System::tSys->m_Game->m_CurrentLevel->PathFindThroughWalls(XMINT2(subTilePos.x, subTilePos.z), targetSubTile, m_Path, PathCost, true);
			}
			else
			{
				pathingResult = micropather::MicroPather::NO_SOLUTION;
			}
		}
		else
		{
			pathingResult = System::tSys->m_Game->m_CurrentLevel->PathFind(XMINT2(subTilePos.x, subTilePos.z), targetSubTile, m_Path, PathCost, true);
		}

		if (pathingResult != micropather::MicroPather::SOLVED && pathingResult != micropather::MicroPather::START_END_SAME)
		{
			taskString = "Invalid Path!";
			LevelData::PathsInvalidated = true;
			if (m_CreatureID == CreatureData::CREATURE_IMP)
			{
				System::Print("Imp could not find path, resetting world pathing");
				CreatureTaskManager::UnlistImpFromTask(this);
			}
			else
			{
				System::Print("Creature could not find path, resetting world pathing");
			}
		}
	}

	m_PathLerpTime += deltaTime * m_Speed;
	if (pathingResult == micropather::MicroPather::SOLVED || pathingResult == micropather::MicroPather::START_END_SAME)
	{
		if (m_PathLerpTime >= 1.0f)
		{
			m_PathLerpTime = 0.0f;
			m_CurrentPathIndex++;
		}
		XMFLOAT3 currentPos = m_Renderable->m_Position;
		const XMINT3 currentSubTilePos = LevelData::WorldToSubtile(currentPos);
		currentPos.y = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight(currentSubTilePos.z / 3, currentSubTilePos.x / 3, currentSubTilePos.z % 3, currentSubTilePos.x % 3);
		float OldPathX = m_Renderable->m_Position.x;
		float OldPathY = m_Renderable->m_Position.z;

		if (m_CurrentPathIndex < m_Path.size())
		{
			taskString = "Pathing to task!";
			if (m_CurrentPathIndex > 0)
			{
				OldPathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex - 1]) & 0xFFFFFFFF);
				OldPathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex - 1]) >> 32) & 0xFFFFFFFF);
			}
			const float PathX = (float)(((uint64_t)m_Path[m_CurrentPathIndex]) & 0xFFFFFFFF);
			const float PathY = (float)((((uint64_t)m_Path[m_CurrentPathIndex]) >> 32) & 0xFFFFFFFF);

			XMINT2 targetTilePos = LevelData::WorldToTile(XMFLOAT3(PathX, 2, PathY));
			Tile* targetTile = &LevelData::m_Map.m_Tiles[targetTilePos.y][targetTilePos.x];
			//if (IsMineableForPlayer(targetTile->GetType(), targetTile->owner, m_Owner))
			if (IsMineable(targetTile->GetType()))
			{
				m_PathLerpTime = 0.0f;
				m_AnimState = CreatureData::AnimationState::Attacking;
				if (m_ImpSpecialTimer <= 0.0f)
				{
					const InstanceData& digInstance = LevelConfig::instanceData[INSTANCE_DIG];
					m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(digInstance.Time + digInstance.ActionTime + digInstance.ResetTime);
					if (Level::s_CurrentLevel->m_LevelData->MineTile(targetTilePos.y, targetTilePos.x))
					{
						Level::s_CurrentLevel->m_LevelData->m_UnexploredTiles[targetTile] = targetTilePos;
						TileNeighbourTiles n = Level::s_CurrentLevel->m_LevelData->GetNeighbourTiles(targetTilePos.y, targetTilePos.x);
						for (int i = 0; i < 4; i++)
						{
							if (!n.Axii[i]->visible)
							{
								Level::s_CurrentLevel->m_LevelData->m_UnexploredTiles[n.Axii[i]] = targetTilePos + AxiiDirections[i];
							}
						}
					}
				}
				return false;
			}
			

			const XMFLOAT2 nPos = Lerp(XMFLOAT2(OldPathX, OldPathY), XMFLOAT2(PathX, PathY), m_PathLerpTime);
			XMFLOAT2 dir = XMFLOAT2(OldPathX, OldPathY) - nPos;
			if (dir.x != 0 || dir.y != 0)
			{
				dir = Normalize(dir);
				m_Direction.x = dir.x;
				m_Direction.z = dir.y;
			}
			const int8_t height = Level::s_CurrentLevel->m_LevelData->GetSubtileHeight((int)round(nPos.y) / 3, (int)round(nPos.x) / 3, (int)round(nPos.y) % 3, (int)round(nPos.x) % 3);
			if (height <= 5)
				m_Renderable->SetPosition(nPos.x, height, nPos.y);

			m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Walking;
			m_AnimState = CreatureData::AnimationState::Walking;
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
void Creature::ImpUpdate(float delta)
{
#ifdef _DEBUG
	ImGui::TreePush("Imp");
	ImGui::BulletText("Imp");

	//Different behaviour whether the game is ran with or without debugger
	//This.. kind of scares me but I don't want people to use break without a debugger attached as it just appears to crash/abort() ?
	if (IsDebuggerPresent())
	{
		if (ImGui::Button("Break"))
		{
			DebugBreak();
		}
	}
#endif

	if (m_ImpSpecialTimer > 0.0f)
	{
		m_ImpSpecialTimer -= delta;
	}

	//Check if current task is still completeable or reachable
	//for example, if an enemy imp has not taken over a neighbouring tile, so that claiming his currently tasked tile would be impossible (or reinforcing a wall)
	//if so, set current order on invalid and RemoveTask();
	//
	//uint16_t targetTileType = Level::s_CurrentLevel->m_LevelData->m_Map.m_Tiles[m_Order.targetTilePos.y][m_Order.targetTilePos.x].type & 0xFF;
	//if (targetTileType >= Type_Earth && targetTileType <= )
	//CreatureTaskManager::UnlistCreatureFromTask(this);
	////or
	//CreatureTaskManager::RemoveClaimingTask()
	//CreatureTaskManager::RemoveReinforcingTask()
	//

	taskString = "No Task";
	if (!m_Order.valid)
	{
		m_TaskSearchTimer -= delta;
		if (m_TaskSearchTimer <= 0.0f)
		{
			GetTask();
			if (!m_Order.valid)
			{
				//random timer from 1 to 2 seconds
				m_TaskSearchTimer = 1.0f + ((float)(rand() % 10)) / 10.0f;
			}
		}
	}
	if (m_Order.valid)
	{
		if (PathTo(delta,m_Order.subTilePos))
		{
			const XMFLOAT3 worldPosTile = LevelData::TileToWorld(m_Order.targetTilePos);
			m_Direction.x = m_Renderable->m_Position.x - worldPosTile.x;
			m_Direction.y = 0;
			m_Direction.z = m_Renderable->m_Position.z - worldPosTile.z;
			m_Direction = Normalize(m_Direction);

			const XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
			const int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;
			if (m_Order.orderType == CreatureTaskManager::Order_Mine)//Mine
			{
				taskString = "Executing task: Mining!";
				m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Attacking;
				if (m_ImpSpecialTimer <= 0.0f)
				{
					const InstanceData& digInstance = LevelConfig::instanceData[INSTANCE_DIG];
					m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(digInstance.Time + digInstance.ActionTime + digInstance.ResetTime);
					uint16_t miningType = Level::s_CurrentLevel->m_LevelData->GetTileType(m_Order.targetTilePos.y, m_Order.targetTilePos.x);
					if (miningType == Type_Gold || miningType == Type_Gem)
					{
						int addedGold = LevelConfig::gameSettings[GameSettings::GAME_GOLD_PER_GOLD_BLOCK].Value / LevelConfig::blockHealth[BlockHealth::BLOCK_HEALTH_GOLD].Value;
						m_CurrentGoldHold += addedGold;
						LevelScript::GameValues[m_Owner]["TOTAL_GOLD_MINED"] += addedGold;
					}
					if (Level::s_CurrentLevel->m_LevelData->MineTile(m_Order.targetTilePos.y, m_Order.targetTilePos.x))
					{
						StopOrder();
						CreatureTaskManager::RemoveMiningTask(m_Owner, m_Order.tile);
						//Shouldn't be done here but I can't think of a better way to do this only for player red
						if (m_Owner == Owner_PlayerRed)
						{
							//set the mined neighbouring tiles to visible
							Level::s_CurrentLevel->m_LevelData->m_Map.m_Tiles[m_Order.targetTilePos.y][m_Order.targetTilePos.x].visible = true;
							TileNeighbourTiles n = Level::s_CurrentLevel->m_LevelData->GetNeighbourTiles(m_Order.targetTilePos.y, m_Order.targetTilePos.x);
							for (int i = 0; i < 4; i++)
							{
								if (IsMineable(n.Axii[i]->GetType()))
								{
									n.Axii[i]->visible = true;
								}
								else
								{
									if (!n.Axii[i]->visible)
									{
										Level::s_CurrentLevel->m_LevelData->m_UnexploredTiles[n.Axii[i]] = m_Order.targetTilePos + AxiiDirections[i];
									}
								}
							}
						}

					}
					if (m_CurrentGoldHold >= m_CreatureData.GoldHold && CreatureTaskManager::IsTreasuryAvailable(this, areaCode))
					{
						CreatureTaskManager::UnlistImpFromTask(this);
						GetTask();
					}
				}
			}
			else if (m_Order.orderType == CreatureTaskManager::Order_Claim) //Claim
			{
				taskString = "Executing task: Claiming!";
				m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Claiming;
				if (m_ImpSpecialTimer <= 0.0f)
				{
					const InstanceData& prettyPathInstance = LevelConfig::instanceData[INSTANCE_PRETTY_PATH];
					m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(prettyPathInstance.Time + prettyPathInstance.ActionTime + prettyPathInstance.ResetTime);
					if (Level::s_CurrentLevel->m_LevelData->ReinforceTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x))
					{
						Level::s_CurrentLevel->m_LevelData->ClaimTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x);
						StopOrder();
						CreatureTaskManager::RemoveClaimingTask(m_Owner, m_Order.tile);
						GetTask();
					}
				}
			}
			else if (m_Order.orderType == CreatureTaskManager::Order_Reinforce) //Reinforce
			{
				taskString = "Executing task: Reinforcing!";
				m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Claiming;
				if (m_ImpSpecialTimer <= 0.0f)
				{
					const InstanceData& reinforceInstance = LevelConfig::instanceData[INSTANCE_REINFORCE];
					m_ImpSpecialTimer = GAME_TURNS_TO_SECOND(reinforceInstance.Time + reinforceInstance.ActionTime + reinforceInstance.ResetTime);
					if (Level::s_CurrentLevel->m_LevelData->ReinforceTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x))
					{
						Level::s_CurrentLevel->m_LevelData->ClaimTile(m_Owner, m_Order.targetTilePos.y, m_Order.targetTilePos.x);
						StopOrder();
						CreatureTaskManager::RemoveReinforcingTask(m_Owner, m_Order.tile);
						GetTask();
					}
				}
			}
			else if (m_Order.orderType == CreatureTaskManager::Order_DeliverGold)
			{

				taskString = "Executing task: Delivering Gold!";
				auto& room = Level::s_CurrentLevel->m_LevelData->m_Rooms[m_Owner][m_Order.tile->roomID];
				room.roomFillAmount += m_CurrentGoldHold;
				auto&& t = room.tiles.find(m_Order.tile);
				t->second.tileValue += m_CurrentGoldHold;
				Level::s_CurrentLevel->m_LevelData->AdjustRoomTile(room, t->second);
				LevelScript::GameValues[m_Owner]["MONEY"] += m_CurrentGoldHold;
				m_CurrentGoldHold = 0;
				StopOrder();
				GetTask();
				//	System::Print("Creature.cpp || Unimplemented task: %i Deliver Gold!", m_Order.orderType);
			}
			else if (m_Order.orderType == CreatureTaskManager::Order_IdleMovement)
			{
				m_TaskSearchTimer = 1.0f + ((float)(rand() % 10)) / 10.0f;
				StopOrder();
			}
			else
			{
				taskString = "Executing task: Unimplemented Task!";
				System::Print("Creature.cpp || Unimplemented task: %i", m_Order.orderType);
			}
		}
	}
	else
	{
		//Do Idle stuff
		m_ImpAnimState = CreatureData::ImpAnimationState::IMP_Idling;
	}
#ifdef _DEBUG
	ImGui::Text(taskString);
	ImGui::TreePop();
#endif
}


void Creature::AnimationDoneEvent()
{
	if (m_CreatureID == CreatureData::CREATURE_IMP || m_Owner == Owner_PlayerNone || m_Owner == Owner_PlayerWhite)return;

	if (m_CurrentState == CreatureState::HUNGRY)
	{
		if (m_CurrentHungerLevel < 100)
		{
			//find new chicken to eat.
			m_CurrentHungerLevel += m_CreatureData.HungerFill;
		}
		else
		{
			//dinner's over
			m_CurrentState = CreatureState::UNCERTAIN;
			StopActivity();
		}
	}
	else if (m_CurrentState == CreatureState::SLEEPING)
	{
		if (m_TaskSearchTimer <= 0.0f)
		{
			StopActivity();
			GetActivity();
		}
	}
	else if (m_CurrentState == CreatureState::DYING)
	{
		//replace creature with body entity
		Level::s_CurrentLevel->m_Players[m_Owner]->CreatureDied(this);
	}
}

void Creature::GetActivity()
{
	if (m_Activity.valid)return;
	//if (m_TaskSearchTimer <= 0.0f)
	//{
	//	m_TaskSearchTimer = 1.0 + ((float)(rand() % 10)) / 10.0f;
	//}
	//else
	//{
	//	return;
	//}

	XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
	int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;

	//Primary (base) tasks of a creature
	if (m_CurrentState == CreatureState::JUST_ENTERED)
	{
		m_Activity = CreatureTaskManager::GetDungeonEnteredActivity(this, areaCode);
		if (m_Activity.valid) return;
	}
	if (m_CurrentHungerLevel < 50 && m_CurrentState != CreatureState::HUNGRY)
	{
		m_Activity = CreatureTaskManager::GetFoodActivity(this, areaCode);
		if (m_Activity.valid)
		{
			return;
		}
	}
	else if (m_CurrentState == CreatureState::HUNGRY)
	{
		//We should be doing the eating sequence atm, so don't get a new task
		return;
	}
	if (m_LairLocation.x == -1)
	{
		m_Activity = CreatureTaskManager::GetCreateLairActivity(this, areaCode);
		if (m_Activity.valid)
		{
			m_CurrentState = CreatureState::CREATE_LAIR;
			return;
		}
	}

	//Do primary hobbies
	for (int i = 0; i < 16; i++) //There are 16 jobs
	{
		if ((m_CreatureData.PrimaryJob & (int)CreatureData::JobArray[i]) == (int)CreatureData::JobArray[i])
		{
			m_Activity = CreatureTaskManager::GetActivityByJob(this, areaCode, CreatureData::JobArray[i]);
			if (m_Activity.valid)return;
		}
	}
	//Do secondary hobbies

	//Nothing else to do, wander around or go sleep
	if (rand() % 5 > 2)
	{
		m_Activity = CreatureTaskManager::GetRandomMovementActivity(this, areaCode);
	}
	else
	{
		m_Activity = CreatureTaskManager::GetSleepActivity(this, areaCode);
	}
}

void Creature::CreatureUpdate(float delta)
{
#ifdef _DEBUG
	ImGui::TreePush("Creature");
	ImGui::BulletText("Creature");
#endif
	if (m_CurrentHealth <= 0.0f)
	{
		m_CurrentState = CreatureState::DYING;

		ImGui::Text("Doing Activity: Dying");
		ImGui::TreePop();
		return;
	}

	if (m_CreatureData.HungerRate)
	{
		const float hungerPerDelta = (float)m_CreatureData.HungerRate / 100.0f;
		m_HungerTimer += delta;
		if (m_HungerTimer > GAME_TURNS_TO_SECOND(hungerPerDelta))
		{
			m_HungerTimer -= GAME_TURNS_TO_SECOND(hungerPerDelta);
			m_CurrentHungerLevel--;
		}
	}
	m_TaskSearchTimer -= delta;
	if (!m_Activity.valid && m_HeroLeader == nullptr)
	{
		if (m_TaskSearchTimer <= 0.0f)
		{
			GetActivity();
			if (!m_Activity.valid)
			{
				//random timer from 1 to 2 seconds
				m_TaskSearchTimer = 1.0f + ((float)(rand() % 10)) / 10.0f;
			}
		}
	}
	
	if (m_Activity.valid)
	{
		bool result = false;
		if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_Tunnel)
		{
			m_ImpSpecialTimer -= delta;
			result = TunnelPathTo(delta, m_Activity.subTilePos, true);
		}
		else
		{
			result = PathTo(delta, m_Activity.subTilePos,false);
		}
		if (result)
		{
			if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_GoToHeart)
			{
				taskString = "Doing Activity: Dungeon Heart!";

				System::tSys->m_Audio->PlayOneShot(FileManager::GetSound("STARS3.WAV"));
				m_CurrentState = CreatureState::CREATE_LAIR;
				m_AnimState = CreatureData::AnimationState::Walking;
				StopActivity();
				m_TaskSearchTimer = 0.0f;
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_CreateLair)
			{
				taskString = "Doing Activity: Create Lair!";
				m_CurrentState = CreatureState::UNCERTAIN; //Detect based on needs/wants
				m_AnimState = CreatureData::AnimationState::Dropping; //its a still frame
				CreateLair();
				StopActivity();
				m_TaskSearchTimer = 0.0f;
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_GoToBed)
			{
				taskString = "Doing Activity: Going to Sleep!";
				SetToFreshAnimation(CreatureData::AnimationState::Sleeping);
				m_CurrentState = CreatureState::SLEEPING;
				m_Activity.activityType = CreatureTaskManager::ActivityType::Activity_Sleep;
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_GetFood)
			{
				taskString = "Doing Activity: Getting Food!";
				m_CurrentState = CreatureState::HUNGRY;
				SetToFreshAnimation(CreatureData::AnimationState::Eating);
				m_Activity.activityType = CreatureTaskManager::ActivityType::Activity_Eat;
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_Eat)
			{
				taskString = "Doing Activity: Eating!";
				//we wait for the animation events to finish
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_Sleep)
			{
				taskString = "Doing Activity: Sleeping!"; 
				m_CurrentState = CreatureState::SLEEPING;
				//we wait for the animation events to finish
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_Explore)
			{
				m_CurrentState = CreatureState::UNCERTAIN;
				StopActivity();
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_IdleMovement)
			{
				m_CurrentState = CreatureState::UNCERTAIN;
				StopActivity();
			}
			else if (m_Activity.activityType == CreatureTaskManager::ActivityType::Activity_Tunnel)
			{
				m_CurrentState = CreatureState::UNCERTAIN;
				StopActivity();
			}
			else //implement other tasks like create lair , feed and sleep
			{
				taskString = "Doing Activity: Unimplemented Activity!";
				System::Print("%s || Unimplemented Activity: %i",__FILE__, m_Activity.activityType);
			}
		}
	}
	else
	{
		if (m_HeroLeader != nullptr && m_HeroLeader->m_CurrentHealth > 0) //rudimentary "IsAlive" check
		{
			XMINT3 targetSubTile = LevelData::WorldToSubtile(m_HeroLeader->m_Renderable->m_Position);
			if (PathTo(delta, XMINT2(targetSubTile.x, targetSubTile.z)))
			{
				XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
				int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;
				m_Activity = CreatureTaskManager::GetRandomMovementActivity(this, areaCode);
			}
		}
	}
#ifdef _DEBUG
	ImGui::Text(taskString);
	ImGui::TreePop();
#endif
}

void Creature::StopOrder()
{
	m_Order.valid = false;
	m_Path.clear();
	m_CurrentPathIndex = 0;
	m_PathLerpTime = 0;
}
void Creature::StopActivity()
{
	m_TaskSearchTimer = 1.0f + ((float)(rand() % 10)) / 10.0f;
	m_Activity.valid = false;
	m_Path.clear();
	m_CurrentPathIndex = 0;
	m_PathLerpTime = 0;
}
void Creature::DoAnimationDirectionsImp()
{
	XMFLOAT3 camRight;
	Camera* cam = System::tSys->m_Game->m_Camera;
	Camera::CameraType camType = cam->GetProjection();

	XMFLOAT3 temp = cam->GetForward();
	temp.y = 0;
	XMFLOAT3 camDir = Normalize(temp);

	camRight = cam->GetRight();
	camRight.y = 0;
	camRight = Normalize(camRight);

	XMFLOAT3 creatureDir = XMFLOAT3(m_Direction.x, 0, m_Direction.z);
	creatureDir = Normalize(creatureDir);
	XMFLOAT3 creatureRight = Cross(XMFLOAT3(0, 1.0f, 0), creatureDir);

	int direction = Dot(camDir, creatureRight) < 0.0f ? -1.0f : 1.0f;

	float d = Dot(camDir, creatureDir);
	float angle = acos(std::min(std::max(d,-1.0f),1.0f)) + PI / 8.0f;
	int sector = (int)std::min(std::max(4.0f*angle / PI, 0.0f), 4.0f);
	if (direction < 0 && sector > 0 && sector < 4)
	{
		m_CreatureCBData._isFlipped = true;
	}
	else
	{
		m_CreatureCBData._isFlipped = false;
	}
	SetSprite(m_CreatureSpriteIndex + ImpAnimState[Camera::CameraType::Orthographic][sector][m_ImpAnimState]);
}
void Creature::DoAnimationDirections()
{
	XMFLOAT3 camRight;
	Camera* cam = System::tSys->m_Game->m_Camera;
	Camera::CameraType camType = cam->GetProjection();

	XMFLOAT3 temp = cam->GetForward();
	temp.y = 0;
	XMFLOAT3 camDir = Normalize(temp);

	camRight = cam->GetRight();
	camRight.y = 0;
	camRight = Normalize(camRight);

	XMFLOAT3 creatureDir = XMFLOAT3(m_Direction.x, 0, m_Direction.z);
	creatureDir = Normalize(creatureDir);
	XMFLOAT3 creatureRight = Cross(XMFLOAT3(0, 1.0f, 0), creatureDir);

	int direction = Dot(camDir, creatureRight) < 0.0f ? -1.0f : 1.0f;

	float d = Dot(camDir, creatureDir);
	float angle = acos(std::min(std::max(d, -1.0f), 1.0f)) + PI / 8.0f;
	int sector = (int)std::min(std::max(4.0f*angle / PI, 0.0f), 4.0f);
	if (direction < 0 && sector > 0 && sector < 4)
	{
		m_CreatureCBData._isFlipped = true;
	}
	else
	{
		m_CreatureCBData._isFlipped = false;
	}
	SetSprite(m_CreatureSpriteIndex + CreatureAnimState[Camera::Orthographic].at(m_CreatureSpriteIndex)[sector][m_AnimState]);
}

void Creature::CreateLair()
{
	Entity* mapEntity = Level::s_CurrentLevel->m_LevelData->GetMapEntity();
	mapEntity->SetSpriteFromType(Entity::Entity_LairFly_W);
	mapEntity->ResetScale();
	mapEntity->SetVisibility(true);
	mapEntity->m_Renderable->m_Position = LevelData::SubtileToWorld(XMINT3(m_Activity.subTilePos.x, 2, m_Activity.subTilePos.y));
	m_Activity.tile->placedEntities[1][1] = mapEntity;
	m_LairLocation.x = m_Activity.targetTilePos.x;
	m_LairLocation.y = m_Activity.targetTilePos.y;
}
void Creature::Draw(D3D& d3d)
{
	if (!m_Renderable->isVisible)return;
	if (m_Renderable->isDirty)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		d3d.m_DevCon->Map(m_CreatureCB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &m_CreatureCBData, sizeof(CreatureConstantBuffer));
		d3d.m_DevCon->Unmap(m_CreatureCB, NULL);
	}
	m_Renderable->Draw(d3d);
}
void Creature::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}
void Creature::CheckVisibility()
{
	if (m_AreaNeedsDiscovering && m_Owner == Owner_PlayerRed)
	{
		//creatures can see 7 tiles in each direction
		const int range = 7;
		const XMINT2 tilePos = LevelData::WorldToTile(m_Renderable->m_Position);
		const int areaCode = LevelData::m_Map.m_Tiles[tilePos.y][tilePos.x].areaCode;
		const int minY = tilePos.y - range >= range ? tilePos.y - range : range;
		const int minX = tilePos.x - range >= range ? tilePos.x - range : range;
		const int maxY = tilePos.y + range < MAP_SIZE_TILES ? tilePos.y + range : MAP_SIZE_TILES - range - 1;
		const int maxX = tilePos.x + range < MAP_SIZE_TILES ? tilePos.x + range : MAP_SIZE_TILES - range - 1;
		LevelData* ldata = Level::s_CurrentLevel->m_LevelData;

		for (auto c = ldata->m_UnexploredTiles.begin(); c != ldata->m_UnexploredTiles.end();)
		{
			//Tile is already visible, shouldn't be in the list, remove it and continue
			if (c->first->visible)
			{
				c = ldata->m_UnexploredTiles.erase(c);
				continue;
			}
			//if we're not checking a wall (the area code != 0) and we're checking a tile from a different area
			if ((c->first->areaCode != areaCode && c->first->areaCode != 0))
			{
				c++;
				continue;
			}
			if (abs(c->second.x - tilePos.x) <= range && abs(c->second.y - tilePos.y) <= range)
			{
				int x = c->second.x;
				int y = c->second.y;

				//Tile is owned by us, they are always visible, mark it so and remove from the list 
				if (c->first->owner == Owner_PlayerRed)
				{
					c->first->visible = true;
					//we explored this tile, check if there are any other tiles next to this we gotta mark unexplored
					ldata->AddExploredTileNeighboursVisibility(y, x, areaCode);

					c = ldata->m_UnexploredTiles.erase(c);
					continue;
				}

				//we raycast from our position to the target tile, if we hit it, mark it visible, if we hit another tile we mark that visible instead
				XMFLOAT3 srcPos = LevelData::TileToWorld(LevelData::WorldToTile(m_Renderable->m_Position));
				srcPos.y = 3;

				XMFLOAT3 rayDir = XMFLOAT3((float)(x * 3 + 1), 3,(float)( y * 3 + 1)) - srcPos;
				rayDir = Normalize(rayDir);
				LevelData::HitData hit = ldata->Raycast(srcPos, rayDir, range * 3);
				if (hit.hit)
				{
					//we hit something, now we check if thats our target tile or not
					int tileX = hit.posX / 3;
					int tileY = hit.posZ / 3;
					Tile* hitTile = &LevelData::m_Map.m_Tiles[tileY][tileX];

					if (tileX == x && tileY == y)
					{
						//DebugDraw::Line(XMFLOAT3(srcPos.x, 6, srcPos.z), XMFLOAT3(tileX * 3 + 1, 6, tileY * 3 + 1),0,XMFLOAT3(0,1,0));
						//we hit our target tile
						c->first->visible = true;
						ldata->AddExploredTileNeighboursVisibility(y, x, areaCode);

						if (!IsMineableForPlayer(hitTile->GetType(), hitTile->owner, Owner_PlayerRed))
						{
							hitTile->marked[Owner_PlayerRed] = false;
						}

						c = ldata->m_UnexploredTiles.erase(c);
						continue;
					}
					else
					{
						//DebugDraw::Line(XMFLOAT3(srcPos.x, 6, srcPos.z), XMFLOAT3(tileX * 3 + 1, 6, tileY * 3 + 1), 0, XMFLOAT3(1, 0, 0));
						//DebugDraw::Line(XMFLOAT3(srcPos.x, 6.2, srcPos.z), XMFLOAT3(x * 3 + 1, 6.2, y * 3 + 1), 0, XMFLOAT3(0, 0, 1));
						//we hit a different tile, let's see if this is also in our unexplored tiles list, if so mark it visible and remove it
						if (!hitTile->visible)
						{
							hitTile->visible = true;
							ldata->AddExploredTileNeighboursVisibility(tileY, tileX, areaCode);
							auto& newIt = ldata->m_UnexploredTiles.find(hitTile);
							if (newIt != ldata->m_UnexploredTiles.end())
							{
								ldata->m_UnexploredTiles.erase(newIt);
							}
						}
					}
				}
			}
			c++;
		}
	}
}