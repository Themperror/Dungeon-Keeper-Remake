#pragma once
namespace Themp
{
	class CreatureData
	{
	public:
		enum CreatureType
		{
			CREATURE_NULL,
			CREATURE_WIZARD,
			CREATURE_BARBARIAN,
			CREATURE_ARCHER,
			CREATURE_MONK,
			CREATURE_DWARF,
			CREATURE_KNIGHT,
			CREATURE_AVATAR,
			CREATURE_TUNNELLER,
			CREATURE_WITCH,
			CREATURE_GIANT,
			CREATURE_FAIRY,
			CREATURE_THIEF,
			CREATURE_SAMURAI,
			CREATURE_HORNY, 
			CREATURE_SKELETON,
			CREATURE_TROLL,
			CREATURE_DRAGON,
			CREATURE_DEMONSPAWN,
			CREATURE_FLY, 
			CREATURE_DARK_MISTRESS,
			CREATURE_SORCEROR,
			CREATURE_BILE_DEMON, 
			CREATURE_IMP, 
			CREATURE_BEETLE, 
			CREATURE_VAMPIRE, 
			CREATURE_SPIDER, 
			CREATURE_HELLHOUND,
			CREATURE_GHOST, 
			CREATURE_TENTACLE,
			CREATURE_ORC, 
			CREATURE_FLOATING_SPIRIT,
		};
		enum CreatureSpriteIndex
		{
			CreatureSprite_Hellhound = 0,
			CreatureSprite_Samurai = 72,
			CreatureSprite_Avatar = 252,
			CreatureSprite_Troll = 314,
			CreatureSprite_Monk = 376,
			CreatureSprite_Knight = 438,
			CreatureSprite_Thief = 500,
			CreatureSprite_Fairy = 562,
			CreatureSprite_Giant = 624,
			CreatureSprite_Witch = 686,
			CreatureSprite_Tunneler = 748,
			CreatureSprite_Dwarf = 810,
			CreatureSprite_Archer = 872,
			CreatureSprite_Barbarian = 934,
			CreatureSprite_Wizard = 996,
			CreatureSprite_Tentacle = 1058,
			CreatureSprite_Ghost = 1120,
			CreatureSprite_Spider = 1172,
			CreatureSprite_Vampire = 1234,
			CreatureSprite_Beetle = 1296,
			CreatureSprite_Imp = 1358,
			CreatureSprite_BileDemon = 1454,
			CreatureSprite_Warlock = 1516,
			CreatureSprite_DarkMistress = 1578,
			CreatureSprite_Fly = 1640,
			CreatureSprite_DemonSpawn = 1692,
			CreatureSprite_Dragon = 1754,
			CreatureSprite_Orc = 1816,
			CreatureSprite_Skeleton = 1878,
			CreatureSprite_Horny = 1940,
		};
		enum AnimationState
		{
			Walking,
			Dropping,
			Attacking,
			Hit,
			Begging,
			Dying,
			Held,
			Sleeping,
			Slapped,
			Eating,
			Pissing,
		};
		enum Animations
		{
			FP_WALK_Forward,
			FP_WALK_DiagForwardRight,
			FP_WALK_Right,
			FP_WALK_DiagBackRight,
			FP_WALK_Back,
			W_WALK_Forward,
			W_WALK_DiagForwardRight,
			W_WALK_Right,
			W_WALK_DiagBackRight,
			W_WALK_Back,
			FP_DROPPING_Forward,
			FP_DROPPING_DiagForwardRight,
			FP_DROPPING_Right,
			FP_DROPPING_DiagBackRight,
			FP_DROPPING_Back,
			W_DROPPING_Forward,
			W_DROPPING_DiagForwardRight,
			W_DROPPING_Right,
			W_DROPPING_DiagBackRight,
			W_DROPPING_Back,
			FP_MELEE_Forward,
			FP_MELEE_DiagForwardRight,
			FP_MELEE_Right,
			FP_MELEE_DiagBackRight,
			FP_MELEE_Back,
			W_MELEE_Forward,
			W_MELEE_DiagForwardRight,
			W_MELEE_Right,
			W_MELEE_DiagBackRight,
			W_MELEE_Back,
			FP_HIT_Forward,
			FP_HIT_DiagForwardRight,
			FP_HIT_Right,
			FP_HIT_DiagBackRight,
			FP_HIT_Back,
			W_HIT_Forward,
			W_HIT_DiagForwardRight,
			W_HIT_Right,
			W_HIT_DiagBackRight,
			W_HIT_Back,
			FP_BEG_Forward,
			FP_BEG_DiagForwardRight,
			FP_BEG_Right,
			FP_BEG_DiagBackRight,
			FP_BEG_Back,
			W_BEG_Forward,
			W_BEG_DiagForwardRight,
			W_BEG_Right,
			W_BEG_DiagBackRight,
			W_BEG_Back,
			FP_DIE,
			W_DIE,
			FP_HAND,
			W_HAND,
			FP_HAPPY,
			W_HAPPY,
			FP_SLEEP,
			W_SLEEP,
			FP_SLAPPED_Forward,
			FP_SLAPPED_DiagForwardRight,
			FP_SLAPPED_Right,
			FP_SLAPPED_DiagBackRight,
			FP_SLAPPED_Back,
			W_SLAPPED_Forward,
			W_SLAPPED_DiagForwardRight,
			W_SLAPPED_Right,
			W_SLAPPED_DiagBackRight,
			W_SLAPPED_Back,
			FP_EAT,
			W_EAT,
			FP_PISS_Forward,
			FP_PISS_DiagForwardRight,
			FP_PISS_Right,
			FP_PISS_DiagBackRight,
			FP_PISS_Back,
			W_PISS_Forward,
			W_PISS_DiagForwardRight,
			W_PISS_Right,
			W_PISS_DiagBackRight,
			W_PISS_Back,
		};
		enum ImpAnimationState
		{
			IMP_Walking,
			IMP_Dropping,
			IMP_Attacking,
			IMP_Hit,
			IMP_Idling,
			IMP_Claiming,
			IMP_Dragging,
			IMP_Idling2,
			IMP_Dazed,
			IMP_Dying,
			IMP_Held,
			IMP_Slapped,
		};
		enum ImpAnimations
		{
			IMP_FP_WALK_Forward,
			IMP_FP_WALK_DiagForwardRight,
			IMP_FP_WALK_Right,
			IMP_FP_WALK_DiagBackRight,
			IMP_FP_WALK_Back,
			IMP_W_WALK_Forward,
			IMP_W_WALK_DiagForwardRight,
			IMP_W_WALK_Right,
			IMP_W_WALK_DiagBackRight,
			IMP_W_WALK_Back,
			IMP_FP_DROPPING_Forward,
			IMP_FP_DROPPING_DiagForwardRight,
			IMP_FP_DROPPING_Right,
			IMP_FP_DROPPING_DiagBackRight,
			IMP_FP_DROPPING_Back,
			IMP_W_DROPPING_Forward,
			IMP_W_DROPPING_DiagForwardRight,
			IMP_W_DROPPING_Right,
			IMP_W_DROPPING_DiagBackRight,
			IMP_W_DROPPING_Back,
			IMP_FP_MINE_Forward,
			IMP_FP_MINE_DiagForwardRight,
			IMP_FP_MINE_Right,
			IMP_FP_MINE_DiagBackRight,
			IMP_FP_MINE_Back,
			IMP_W_MINE_Forward,
			IMP_W_MINE_DiagForwardRight,
			IMP_W_MINE_Right,
			IMP_W_MINE_DiagBackRight,
			IMP_W_MINE_Back,
			IMP_FP_HIT_Forward,
			IMP_FP_HIT_DiagForwardRight,
			IMP_FP_HIT_Right,
			IMP_FP_HIT_DiagBackRight,
			IMP_FP_HIT_Back,
			IMP_W_HIT_Forward,
			IMP_W_HIT_DiagForwardRight,
			IMP_W_HIT_Right,
			IMP_W_HIT_DiagBackRight,
			IMP_W_HIT_Back,
			IMP_FP_IDLE_Forward,
			IMP_FP_IDLE_DiagForwardRight,
			IMP_FP_IDLE_Right,
			IMP_FP_IDLE_DiagBackRight,
			IMP_FP_IDLE_Back,
			IMP_W_IDLE_Forward,
			IMP_W_IDLE_DiagForwardRight,
			IMP_W_IDLE_Right,
			IMP_W_IDLE_DiagBackRight,
			IMP_W_IDLE_Back,
			IMP_FP_CLAIM_Forward,
			IMP_FP_CLAIM_DiagForwardRight,
			IMP_FP_CLAIM_Right,
			IMP_FP_CLAIM_DiagBackRight,
			IMP_FP_CLAIM_Back,
			IMP_W_CLAIM_Forward,
			IMP_W_CLAIM_DiagForwardRight,
			IMP_W_CLAIM_Right,
			IMP_W_CLAIM_DiagBackRight,
			IMP_W_CLAIM_Back,
			IMP_FP_DRAG_Forward,
			IMP_FP_DRAG_DiagForwardRight,
			IMP_FP_DRAG_Right,
			IMP_FP_DRAG_DiagBackRight,
			IMP_FP_DRAG_Back,
			IMP_W_DRAG_Forward,
			IMP_W_DRAG_DiagForwardRight,
			IMP_W_DRAG_Right,
			IMP_W_DRAG_DiagBackRight,
			IMP_W_DRAG_Back,
			IMP_FP_IDLE2_Forward,
			IMP_FP_IDLE2_DiagForwardRight,
			IMP_FP_IDLE2_Right,
			IMP_FP_IDLE2_DiagBackRight,
			IMP_FP_IDLE2_Back,
			IMP_W_IDLE2_Forward,
			IMP_W_IDLE2_DiagForwardRight,
			IMP_W_IDLE2_Right,
			IMP_W_IDLE2_DiagBackRight,
			IMP_W_IDLE2_Back,
			IMP_FP_DAZED,
			IMP_W_DAZED,
			IMP_FP_DIE,
			IMP_W_DIE,
			IMP_FP_HAND,
			IMP_W_HAND,
			IMP_FP_SLAPPED_Forward,
			IMP_FP_SLAPPED_DiagForwardRight,
			IMP_FP_SLAPPED_Right,
			IMP_FP_SLAPPED_DiagBackRight,
			IMP_FP_SLAPPED_Back,
			IMP_W_SLAPPED_Forward,
			IMP_W_SLAPPED_DiagForwardRight,
			IMP_W_SLAPPED_Right,
			IMP_W_SLAPPED_DiagBackRight,
			IMP_W_SLAPPED_Back,
		};

		enum class Annoyance
		{
			NoLair,
			NoHatchery,
			WokenUp,
			StandingOnDeadEnemy,
			Sulking,
			NoSalary,
			Slapped,
			StandingOnDeadFriend,
			InTorture,
			InTemple,
			Sleeping,
			GotWage,
			WinBattle,
			UntrainedTime,
			Untrained,
			JobStress,
			Queue,
			WillNotDoJob,
			OthersLeaving,
			EatFood,
			InHand,
		};

		bool Flying;
		bool ImmuneToGas;
		bool RealTraining;
		bool EntranceForce;
		bool CanSeeInvisible;
		bool CanGoThroughLockedDoors;
		bool Bleeds;
		bool AffectedByWind;
		bool HumanoidCreature;
		bool PissOnDead;

		uint8_t CreatureNumber;
		uint8_t HealReq;
		uint8_t HealThreshold;
		uint8_t Strength;
		uint8_t Armor;
		uint8_t Dexterity;
		uint8_t Fear;
		uint8_t Defence;
		uint8_t Luck;
		uint8_t Recovery;
		uint8_t HungerFill;
		uint8_t LairSize;
		uint8_t HurtByLava;
		uint8_t SleepExperienceSlab;
		uint8_t SleepExperience;
		uint8_t ScavengeValue;
		uint8_t BaseSpeed;
		uint8_t ExperienceForHitting;
		uint8_t TrainingValue;
		uint8_t TrainingCost;
		uint8_t VisualRange;
		uint8_t ScavengerCost;
		uint8_t GrowUp; //Creature ID
		uint8_t GrowUpLevel;
		uint8_t WalkingAnimSpeed;
		uint8_t AttackPreference;
		uint8_t MaxAngleChange;
		uint8_t ResearchValue;
		uint8_t HeroVsKeeper;
		uint8_t ManufactureValue;
		uint8_t Hearing;
		uint8_t EntranceRoom1;
		uint8_t EntranceRoom2;
		uint8_t EntranceRoom3;
		uint8_t Room1SlabsRequired;
		uint8_t Room2SlabsRequired;
		uint8_t Room3SlabsRequired;
		uint8_t JobStress;
		uint8_t SlapsToKill;
		uint8_t CreatureLoyalty;
		uint8_t LoyaltyLevel;
		uint8_t LairEnemy; //Creature Index
		uint8_t EyeEffect;
		uint8_t Rebirth;
		uint8_t DamageToBoulder;
		uint8_t Power[10];
		uint8_t PowerLevelRequired[10];

		uint16_t SizeXY;
		uint16_t SizeYZ;
		uint16_t Pay;
		uint16_t PrimaryJob;	//bitflags
		uint16_t SecondaryJob;	//bitflags
		uint16_t NotDoJobs;		//bitflags
		uint16_t AngerJobs;		//bitflags
		uint16_t Health;
		uint16_t HungerRate;
		uint16_t AnnoyLevel;
		uint16_t ScavengeRequired;
		uint16_t GoldHold;
		uint16_t ToLevel2;
		uint16_t ToLevel3;
		uint16_t ToLevel4;
		uint16_t ToLevel5;
		uint16_t ToLevel6;
		uint16_t ToLevel7;
		uint16_t ToLevel8;
		uint16_t ToLevel9;
		uint16_t ToLevel10;
		uint16_t ToLevelGrowUp;
		uint16_t EyeHeight;
		uint16_t FieldOfView;
		uint16_t TortureTime;
		uint16_t ThingSizeXY;
		uint16_t ThingSizeYZ;
		int16_t Annoyance[21];
	};
};
