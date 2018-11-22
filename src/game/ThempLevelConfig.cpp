#include "ThempSystem.h"
#include "ThempLevelConfig.h"
#include "ThempFileManager.h"
#include "ThempTileArrays.h"
#include <sstream>
#include <unordered_map>
using namespace Themp;

const std::unordered_map<std::string, CreatureData::CreatureType> creatureMap =
{
	{"CREATURE_WIZARD",CreatureData::CreatureType::CREATURE_WIZARD},
	{"CREATURE_BARBARIAN",CreatureData::CreatureType::CREATURE_BARBARIAN},
	{"CREATURE_ARCHER",CreatureData::CreatureType::CREATURE_ARCHER},
	{"CREATURE_MONK",CreatureData::CreatureType::CREATURE_MONK},
	{"CREATURE_DWARF",CreatureData::CreatureType::CREATURE_DWARF},
	{"CREATURE_KNIGHT",CreatureData::CreatureType::CREATURE_KNIGHT},
	{"CREATURE_AVATAR",CreatureData::CreatureType::CREATURE_AVATAR},
	{"CREATURE_TUNNELLER",CreatureData::CreatureType::CREATURE_TUNNELLER},
	{"CREATURE_WITCH",CreatureData::CreatureType::CREATURE_WITCH},
	{"CREATURE_GIANT",CreatureData::CreatureType::CREATURE_GIANT},
	{"CREATURE_FAIRY",CreatureData::CreatureType::CREATURE_FAIRY},
	{"CREATURE_THIEF",CreatureData::CreatureType::CREATURE_THIEF},
	{"CREATURE_SAMURAI",CreatureData::CreatureType::CREATURE_SAMURAI},
	{"CREATURE_HORNY",CreatureData::CreatureType::CREATURE_HORNY},
	{"CREATURE_SKELETON",CreatureData::CreatureType::CREATURE_SKELETON},
	{"CREATURE_TROLL",CreatureData::CreatureType::CREATURE_TROLL},
	{"CREATURE_DRAGON",CreatureData::CreatureType::CREATURE_DRAGON},
	{"CREATURE_DEMONSPAWN",CreatureData::CreatureType::CREATURE_DEMONSPAWN},
	{"CREATURE_FLY",CreatureData::CreatureType::CREATURE_FLY},
	{"CREATURE_DARK_MISTRESS",CreatureData::CreatureType::CREATURE_DARK_MISTRESS},
	{"CREATURE_SORCEROR",CreatureData::CreatureType::CREATURE_SORCEROR},
	{"CREATURE_BILE_DEMON",CreatureData::CreatureType::CREATURE_BILE_DEMON},
	{"CREATURE_IMP",CreatureData::CreatureType::CREATURE_IMP},
	{"CREATURE_BEETLE",CreatureData::CreatureType::CREATURE_BEETLE},
	{"CREATURE_VAMPIRE",CreatureData::CreatureType::CREATURE_VAMPIRE},
	{"CREATURE_SPIDER",CreatureData::CreatureType::CREATURE_SPIDER},
	{"CREATURE_HELLHOUND",CreatureData::CreatureType::CREATURE_HELLHOUND},
	{"CREATURE_GHOST",CreatureData::CreatureType::CREATURE_GHOST},
	{"CREATURE_TENTACLE",CreatureData::CreatureType::CREATURE_TENTACLE},
	{"CREATURE_ORC",CreatureData::CreatureType::CREATURE_ORC},
	{"CREATURE_FLOATING_SPIRIT",CreatureData::CreatureType::CREATURE_FLOATING_SPIRIT},
};
const std::unordered_map<std::string, Rooms> roomsMap =
{
	{"ROOM_ENTRANCE", ROOM_ENTRANCE },
	{"ROOM_TREASURE_ROOM", ROOM_TREASURE_ROOM },
	{"ROOM_RESEARCH_ROOM", ROOM_RESEARCH_ROOM },
	{"ROOM_PRISON_ROOM", ROOM_PRISON_ROOM },
	{"ROOM_TORTURE_ROOM", ROOM_TORTURE_ROOM },
	{"ROOM_TRAINING_AREA", ROOM_TRAINING_AREA },
	{"ROOM_DUNGEON_HEART", ROOM_DUNGEON_HEART },
	{"ROOM_WORKSHOP_ROOM", ROOM_WORKSHOP_ROOM },
	{"ROOM_SCAVENGER", ROOM_SCAVENGER },
	{"ROOM_TEMPLE", ROOM_TEMPLE },
	{"ROOM_GRAVEYARD", ROOM_GRAVEYARD },
	{"ROOM_BARRACK_ROOM", ROOM_BARRACK_ROOM },
	{"ROOM_HATCHERY", ROOM_HATCHERY },
	{"ROOM_LAIR", ROOM_LAIR },
	{"ROOM_BRIDGE", ROOM_BRIDGE },
	{"ROOM_GUARD_POST", ROOM_GUARD_POST },
};
const std::unordered_map<std::string, MagicConfig> magicConfigMap =
{
	{"MAGIC_HAND",MAGIC_HAND },
	{"MAGIC_CREATE_IMP",MAGIC_CREATE_IMP },
	{"MAGIC_MUST_OBEY",MAGIC_MUST_OBEY },
	{"MAGIC_SLAP",MAGIC_SLAP },
	{"MAGIC_SIGHT",MAGIC_SIGHT },
	{"MAGIC_CALL_TO_ARMS",MAGIC_CALL_TO_ARMS },
	{"MAGIC_CAVE_IN",MAGIC_CAVE_IN },
	{"MAGIC_HEAL_CREATURE",MAGIC_HEAL_CREATURE},
	{"MAGIC_HOLD_AUDIENCE",MAGIC_HOLD_AUDIENCE},
	{"MAGIC_LIGHTNING",MAGIC_LIGHTNING },
	{"MAGIC_SPEED",MAGIC_SPEED },
	{"MAGIC_PROTECT",MAGIC_PROTECT },
	{"MAGIC_CONCEAL",MAGIC_CONCEAL },
	{"MAGIC_DISEASE",MAGIC_DISEASE },
	{"MAGIC_CHICKEN",MAGIC_CHICKEN },
	{"MAGIC_DESTROY_WALLS",MAGIC_DESTROY_WALLS},
	{"MAGIC_TIME_BOMB",MAGIC_TIME_BOMB },
	{"MAGIC_POSSESS",MAGIC_POSSESS },
	{"MAGIC_ARMAGEDON",MAGIC_ARMAGEDON },
};
const std::unordered_map<std::string, GameSettings> gameSettingsMap =
{
	{"GAME_GOLD_PER_GOLD_BLOCK",GAME_GOLD_PER_GOLD_BLOCK },
	{"GAME_POT_OF_GOLD_HOLDS",GAME_POT_OF_GOLD_HOLDS },
	{"GAME_GOLD_PILE_VALUE",GAME_GOLD_PILE_VALUE },
	{"GAME_GOLD_PILE_MAXIMUM",GAME_GOLD_PILE_MAXIMUM },
	{"GAME_RECOVERY_FREQUENCY",GAME_RECOVERY_FREQUENCY },
	{"GAME_FIGHT_MAX_HATE",GAME_FIGHT_MAX_HATE },
	{"GAME_FIGHT_BORDERLINE",GAME_FIGHT_BORDERLINE },
	{"GAME_FIGHT_MAX_LOVE",GAME_FIGHT_MAX_LOVE },
	{"GAME_FOOD_LIFE_OUT_OF_HATCHERY",GAME_FOOD_LIFE_OUT_OF_HATCHERY },
	{"GAME_FIGHT_HATE_KILL_VALUE",GAME_FIGHT_HATE_KILL_VALUE },
	{"GAME_DEATH_MATCH_STATUE_REAPPER_TIME",GAME_DEATH_MATCH_STATUE_REAPPER_TIME },
	{"GAME_DEATH_MATCH_OBJECT_REAPPER_TIME",GAME_DEATH_MATCH_OBJECT_REAPPER_TIME },
	{"GAME_HITS_PER_SLAB",GAME_HITS_PER_SLAB },
	{"GAME_COLLAPSE_DUNGEON_DAMAGE",GAME_COLLAPSE_DUNGEON_DAMAGE },
	{"GAME_TURNS_PER_COLLAPSE_DUNGEON_DAMAGE",GAME_TURNS_PER_COLLAPSE_DUNGEON_DAMAGE },
	{"GAME_IMP_JOB_NOT_SO_CLOSE(UNUSED)",GAME_IMP_JOB_NOT_SO_CLOSE_UNUSED },
	{"GAME_IMP_JOB_FAR_AWAY(UNUSED)",GAME_IMP_JOB_FAR_AWAY_UNUSED },
	{"GAME_TIME_SPENT_IN_PRISON(UNUSED)",GAME_TIME_SPENT_IN_PRISON_UNUSED },
	{"GAME_AUTO_DIG_LIMIT(UNUSED)",GAME_AUTO_DIG_LIMIT_UNUSED },
	{"GAME_DOUBLE_CLICK_SPEED(UNUSED)",GAME_DOUBLE_CLICK_SPEED_UNUSED },
	{"GAME_IMP_GENERATE_TIME(UNUSED)",GAME_IMP_GENERATE_TIME_UNUSED },
	{"GAME_GHOST_CONVERT_CHANCE",GAME_GHOST_CONVERT_CHANCE },
	{"GAME_ARMORY_TIME",GAME_ARMORY_TIME },
	{"GAME_WORKSHOP_TIME",GAME_WORKSHOP_TIME },
	{"GAME_OBSERVATORY_TIME",GAME_OBSERVATORY_TIME },
	{"GAME_OBSERVATORY_GENERATE",GAME_OBSERVATORY_GENERATE },
	{"GAME_IMPROVE_AREA",GAME_IMPROVE_AREA },
	{"GAME_DOOR_OPEN_FOR",GAME_DOOR_OPEN_FOR },
	{"GAME_BOULDER_REDUCE_HEALTH_SLAP",GAME_BOULDER_REDUCE_HEALTH_SLAP },
	{"GAME_BOULDER_REDUCE_HEALTH_WALL",GAME_BOULDER_REDUCE_HEALTH_WALL },
	{"GAME_BOULDER_REDUCE_HEALTH_ROOM",GAME_BOULDER_REDUCE_HEALTH_ROOM },
	{"GAME_TILE_STRENGTH",GAME_TILE_STRENGTH },
	{"GAME_GOLD_TILE_STRENGTH",GAME_GOLD_TILE_STRENGTH },
	{"GAME_ARMEGEDDON_COUNT_DOWN",GAME_ARMEGEDDON_COUNT_DOWN },
	{"GAME_ARMEGEDDON_DURATION",GAME_ARMEGEDDON_DURATION },
	{"GAME_MINIMUM_GOLD",GAME_MINIMUM_GOLD },
	{"GAME_MAX_GOLD_LOOKUP",GAME_MAX_GOLD_LOOKUP },
	{"GAME_MIN_GOLD_TO_RECORD",GAME_MIN_GOLD_TO_RECORD },
	{"GAME_WAIT_FOR_ROOM_TIME",GAME_WAIT_FOR_ROOM_TIME },
	{"GAME_CHECK_EXPAND_TIME",GAME_CHECK_EXPAND_TIME },
	{"GAME_MAX_DISTANCE_TO_DIG",GAME_MAX_DISTANCE_TO_DIG },
	{"GAME_WAIT_AFTER_ROOM_AREA",GAME_WAIT_AFTER_ROOM_AREA },
	{"GAME_PER_IMP_GOLD_DIG_SIZE",GAME_PER_IMP_GOLD_DIG_SIZE },
	{"GAME_DEFAULT_GENERATE_SPEED",GAME_DEFAULT_GENERATE_SPEED },
	{"GAME_BARRACK_TIME",GAME_BARRACK_TIME },
	{"GAME_FOOD_GENERATION_SPEED",GAME_FOOD_GENERATION_SPEED },
	{"GAME_DEFAULT_NEUTRAL_ENTRANCE_LEVEL",GAME_DEFAULT_NEUTRAL_ENTRANCE_LEVEL },
	{"GAME_DEFAULT_MAX_CREATURES_GENERATE_ENTRANCE",GAME_DEFAULT_MAX_CREATURES_GENERATE_ENTRANCE},
	{"GAME_DEFAULT_IMP_DIG_DAMAGE",GAME_DEFAULT_IMP_DIG_DAMAGE },
	{"GAME_DEFAULT_IMP_DIG_OWN_DAMAGE",GAME_DEFAULT_IMP_DIG_OWN_DAMAGE },
	{"GAME_PRISON_SKELETON_CHANCE",GAME_PRISON_SKELETON_CHANCE },
	{"GAME_DEFAULT_SACRIFICE_SCORE_FOR_HORNY",GAME_DEFAULT_SACRIFICE_SCORE_FOR_HORNY },
	{"GAME_GAME_TURNS_IN_FLEE",GAME_GAME_TURNS_IN_FLEE },
	{"GAME_TRAIN_COST_FREQUENCY",GAME_TRAIN_COST_FREQUENCY },
	{"GAME_TORTURE_CONVERT_CHANCE",GAME_TORTURE_CONVERT_CHANCE },
	{"GAME_PAY_DAY_GAP",GAME_PAY_DAY_GAP },
	{"GAME_CHEST_GOLD_HOLD",GAME_CHEST_GOLD_HOLD },
	{"GAME_SLAB_COLLAPSE_TIME",GAME_SLAB_COLLAPSE_TIME },
	{"GAME_DUNGEON_HEART_HEALTH",GAME_DUNGEON_HEART_HEALTH },
	{"GAME_POWER_HAND_GOLD_GRAB_AMOUNT",GAME_POWER_HAND_GOLD_GRAB_AMOUNT },
	{"GAME_DUNGEON_HEART_HEAL_TIME",GAME_DUNGEON_HEART_HEAL_TIME },
	{"GAME_DUNGEON_HEART_HEAL_HEALTH",GAME_DUNGEON_HEART_HEAL_HEALTH },
	{"GAME_HERO_DOOR_WAIT_TIME",GAME_HERO_DOOR_WAIT_TIME },
	{"GAME_DISEASE_TRANSFER_PERCENTAGE",GAME_DISEASE_TRANSFER_PERCENTAGE },
	{"GAME_DISEASE_LOSE_PERCENTAGE_HEALTH",GAME_DISEASE_LOSE_PERCENTAGE_HEALTH },
	{"GAME_DISEASE_LOSE_HEALTH_TIME",GAME_DISEASE_LOSE_HEALTH_TIME },
	{"GAME_HOLD_AUDIENCE_TIME",GAME_HOLD_AUDIENCE_TIME },
	{"GAME_ARMAGEDON_TELEPORT_YOUR_TIME_GAP",GAME_ARMAGEDON_TELEPORT_YOUR_TIME_GAP },
	{"GAME_ARMAGEDON_TELEPORT_ENEMY_TIME_GAP",GAME_ARMAGEDON_TELEPORT_ENEMY_TIME_GAP },
	{"GAME_SCAVENGE_COST_FREQUENCY",GAME_SCAVENGE_COST_FREQUENCY },
	{"GAME_TEMPLE_SCAVENGE_PROTECTION_TIME",GAME_TEMPLE_SCAVENGE_PROTECTION_TIME },
	{"GAME_BODIES_FOR_VAMPIRE",GAME_BODIES_FOR_VAMPIRE },
	{"GAME_BODY_REMAINS_FOR",GAME_BODY_REMAINS_FOR },
	{"GAME_GRAVEYARD_CONVERT_TIME",GAME_GRAVEYARD_CONVERT_TIME },
	{"GAME_MIN_DISTANCE_FOR_TELEPORT",GAME_MIN_DISTANCE_FOR_TELEPORT },
};


const std::unordered_map<std::string, Spells> spellMap =
{
	{"SPELL_NULL", SPELL_NULL },
	{"SPELL_FIREBALL", SPELL_FIREBALL },
	{"SPELL_FIRE_BOMB", SPELL_FIRE_BOMB },
	{"SPELL_FREEZE", SPELL_FREEZE },
	{"SPELL_ARMOUR", SPELL_ARMOUR },
	{"SPELL_LIGHTNING", SPELL_LIGHTNING },
	{"SPELL_REBOUND", SPELL_REBOUND },
	{"SPELL_HEAL", SPELL_HEAL },
	{"SPELL_POISON_CLOUD", SPELL_POISON_CLOUD },
	{"SPELL_INVISIBILITY", SPELL_INVISIBILITY },
	{"SPELL_TELEPORT", SPELL_TELEPORT },
	{"SPELL_SPEED", SPELL_SPEED },
	{"SPELL_SLOW", SPELL_SLOW },
	{"SPELL_DRAIN", SPELL_DRAIN },
	{"SPELL_FEAR", SPELL_FEAR },
	{"SPELL_MISSILE", SPELL_MISSILE },
	{"SPELL_NAVIGATING_MISSILE", SPELL_NAVIGATING_MISSILE },
	{"SPELL_FLAME_BREATH", SPELL_FLAME_BREATH },
	{"SPELL_WIND", SPELL_WIND },
	{"SPELL_LIGHT", SPELL_LIGHT },
	{"SPELL_FLY", SPELL_FLY },
	{"SPELL_SIGHT", SPELL_SIGHT },
	{"SPELL_GRENADE", SPELL_GRENADE },
	{"SPELL_HAILSTORM", SPELL_HAILSTORM },
	{"SPELL_WORD_OF_POWER", SPELL_WORD_OF_POWER },
	{"SPELL_CRAZY_GAS", SPELL_CRAZY_GAS },
	{"SPELL_DISEASE", SPELL_DISEASE },
	{"SPELL_CHICKEN", SPELL_CHICKEN },
	{"SPELL_TIME_BOMB", SPELL_TIME_BOMB },
};

const std::unordered_map<std::string, Shots> shotsMap =
{
	{"SHOT_NULL", SHOT_NULL },
	{"SHOT_FIREBALL", SHOT_FIREBALL },
	{"SHOT_FIREBOMB", SHOT_FIREBOMB },
	{"SHOT_FREEZE", SHOT_FREEZE },
	{"SHOT_LIGHTNING", SHOT_LIGHTNING },
	{"SHOT_POISON_CLOUD", SHOT_POISON_CLOUD },
	{"SHOT_NAVIGATING_MISSILE", SHOT_NAVIGATING_MISSILE },
	{"SHOT_FLAME_BREATHE", SHOT_FLAME_BREATHE },
	{"SHOT_WIND", SHOT_WIND },
	{"SHOT_MISSILE", SHOT_MISSILE },
	{"SHOT_SLOW", SHOT_SLOW },
	{"SHOT_GRENADE", SHOT_GRENADE },
	{"SHOT_DRAIN", SHOT_DRAIN },
	{"SHOT_HAIL_STORM", SHOT_HAIL_STORM },
	{"SHOT_ARROW", SHOT_ARROW },
	{"SHOT_BOULDER", SHOT_BOULDER },
	{"SHOT_GOD_LIGHTNING", SHOT_GOD_LIGHTNING },
	{"SHOT_SPIKE", SHOT_SPIKE },
	{"SHOT_VORTEX", SHOT_VORTEX },
	{"SHOT_ALARM", SHOT_ALARM },
	{"SHOT_SOLID_BOULDER", SHOT_SOLID_BOULDER },
	{"SHOT_SWING_WEAPON_SWORD", SHOT_SWING_WEAPON_SWORD },
	{"SHOT_SWING_WEAPON_FIST", SHOT_SWING_WEAPON_FIST },
	{"SHOT_DIG", SHOT_DIG },
	{"SHOT_GOD_LIGHTNING_BALL", SHOT_GOD_LIGHTNING_BALL },
	{"SHOT_GROUP", SHOT_GROUP },
	{"SHOT_DISEASE", SHOT_DISEASE },
	{"SHOT_CHICKEN", SHOT_CHICKEN },
	{"SHOT_TIME_BOMB", SHOT_TIME_BOMB },
	{"SHOT_TRAP_LIGHTNING", SHOT_TRAP_LIGHTNING },
};
const std::unordered_map<std::string, Traps> trapsMap =
{
	{"TRAP_NULL",TRAP_NULL },
	{"TRAP_BOULDER",TRAP_BOULDER },
	{"TRAP_ALARM",TRAP_ALARM },
	{"TRAP_POISON_GAS",TRAP_POISON_GAS },
	{"TRAP_LIGHTNING",TRAP_LIGHTNING },
	{"TRAP_WORD_OF_POWER",TRAP_WORD_OF_POWER },
	{"TRAP_LAVA",TRAP_LAVA },
};
const std::unordered_map<std::string, Doors> doorsMap =
{
	{"DOOR_WOODEN",DOOR_WOODEN },
	{"DOOR_BRACE",DOOR_BRACE },
	{"DOOR_STEEL",DOOR_STEEL },
	{"DOOR_MAGIC",DOOR_MAGIC },
};
const std::unordered_map<std::string, HealthConfig> healthConfigsMap =
{
	{"HEALTH_HUNGER_HEALTH_LOSS",HEALTH_HUNGER_HEALTH_LOSS },
	{"HEALTH_GAME_TURNS_PER_HUNGER_HEALTH_LOSS",HEALTH_GAME_TURNS_PER_HUNGER_HEALTH_LOSS },
	{"HEALTH_FOOD_HEALTH_GAIN",HEALTH_FOOD_HEALTH_GAIN },
	{"HEALTH_(UNUSED)PRISON_HEALTH_GAIN",HEALTH_UNUSED_PRISON_HEALTH_GAIN },
	{"HEALTH_(UNUSED)GAME_TURNS_PER_PRISON_HEALTH_GAIN",HEALTH_UNUSED_GAME_TURNS_PER_PRISON_HEALTH_GAIN },
	{"HEALTH_TORTURE_HEALTH_LOSS",HEALTH_TORTURE_HEALTH_LOSS },
	{"HEALTH_GAME_TURNS_PER_TORTURE_HEALTH_LOSS",HEALTH_GAME_TURNS_PER_TORTURE_HEALTH_LOSS },
};
const std::unordered_map<std::string, BlockHealth> blockHealthMap =
{
	{"BLOCK_HEALTH_ROCK",BLOCK_HEALTH_ROCK },
	{"BLOCK_HEALTH_GOLD",BLOCK_HEALTH_GOLD },
	{"BLOCK_HEALTH_PRETTY",BLOCK_HEALTH_PRETTY },
	{"BLOCK_HEALTH_FLOOR",BLOCK_HEALTH_FLOOR },
	{"BLOCK_HEALTH_ROOM",BLOCK_HEALTH_ROOM },
	{"BLOCK_HEALTH_DOOR1",BLOCK_HEALTH_DOOR1 },
	{"BLOCK_HEALTH_DOOR2",BLOCK_HEALTH_DOOR2 },
	{"BLOCK_HEALTH_DOOR3",BLOCK_HEALTH_DOOR3 },
	{"BLOCK_HEALTH_DOOR4",BLOCK_HEALTH_DOOR4 },
};
const std::unordered_map<std::string, Instance> instancesMap =
{
	{"INSTANCE_NULL",INSTANCE_NULL},
	{"INSTANCE_SWING_WEAPON_SWORD",INSTANCE_SWING_WEAPON_SWORD},
	{"INSTANCE_SWING_WEAPON_FIST",INSTANCE_SWING_WEAPON_FIST},
	{"INSTANCE_ESCAPE",INSTANCE_ESCAPE},
	{"INSTANCE_FIRE_ARROW",INSTANCE_FIRE_ARROW},
	{"INSTANCE_FIREBALL",INSTANCE_FIREBALL},
	{"INSTANCE_FIRE_BOMB",INSTANCE_FIRE_BOMB},
	{"INSTANCE_FREEZE",INSTANCE_FREEZE},
	{"INSTANCE_ARMOUR",INSTANCE_ARMOUR},
	{"INSTANCE_LIGHTNING",INSTANCE_LIGHTNING},
	{"INSTANCE_REBOUND",INSTANCE_REBOUND},
	{"INSTANCE_HEAL",INSTANCE_HEAL},
	{"INSTANCE_POISON_CLOUD",INSTANCE_POISON_CLOUD},
	{"INSTANCE_INVISIBILITY",INSTANCE_INVISIBILITY},
	{"INSTANCE_TELEPORT",INSTANCE_TELEPORT},
	{"INSTANCE_SPEED",INSTANCE_SPEED},
	{"INSTANCE_SLOW",INSTANCE_SLOW},
	{"INSTANCE_DRAIN",INSTANCE_DRAIN},
	{"INSTANCE_FEAR",INSTANCE_FEAR},
	{"INSTANCE_MISSILE",INSTANCE_MISSILE},
	{"INSTANCE_NAVIGATING_MISSILE",INSTANCE_NAVIGATING_MISSILE},
	{"INSTANCE_FLAME_BREATH",INSTANCE_FLAME_BREATH},
	{"INSTANCE_WIND",INSTANCE_WIND},
	{"INSTANCE_LIGHT",INSTANCE_LIGHT},
	{"INSTANCE_FLY",INSTANCE_FLY},
	{"INSTANCE_SIGHT",INSTANCE_SIGHT},
	{"INSTANCE_GRENADE",INSTANCE_GRENADE},
	{"INSTANCE_HAILSTORM",INSTANCE_HAILSTORM},
	{"INSTANCE_WORD_OF_POWER",INSTANCE_WORD_OF_POWER},
	{"INSTANCE_FART",INSTANCE_FART},
	{"INSTANCE_DIG",INSTANCE_DIG},
	{"INSTANCE_PRETTY_PATH",INSTANCE_PRETTY_PATH},
	{"INSTANCE_DESTROY_AREA",INSTANCE_DESTROY_AREA},
	{"INSTANCE_TUNNEL",INSTANCE_TUNNEL},
	{"INSTANCE_WAVE",INSTANCE_WAVE},
	{"INSTANCE_REINFORCE",INSTANCE_REINFORCE},
	{"INSTANCE_EAT",INSTANCE_EAT},
	{"INSTANCE_ATTACK_ROOM_SLAB",INSTANCE_ATTACK_ROOM_SLAB},
	{"INSTANCE_DAMAGE_WALL",INSTANCE_DAMAGE_WALL},
	{"INSTANCE_FIRST_PERSON_DIG",INSTANCE_FIRST_PERSON_DIG},
	{"INSTANCE_CAST_SPELL_GROUP",INSTANCE_CAST_SPELL_GROUP},
	{"INSTANCE_CAST_SPELL_DISEASE",INSTANCE_CAST_SPELL_DISEASE},
	{"INSTANCE_CAST_SPELL_CHICKEN",INSTANCE_CAST_SPELL_CHICKEN},
	{"INSTANCE_CAST_SPELL_TIME_BOMB",INSTANCE_CAST_SPELL_TIME_BOMB},
	{"INSTANCE_MOAN",INSTANCE_MOAN},
	{"INSTANCE_TORTURED",INSTANCE_TORTURED},
};
const std::unordered_map<std::string, Research> researchMap = 
{
	{"RESEARCH_MAGIC_HAND",RESEARCH_MAGIC_HAND},
	{"RESEARCH_MAGIC_POSSESS",RESEARCH_MAGIC_POSSESS},
	{"RESEARCH_MAGIC_SLAP",RESEARCH_MAGIC_SLAP},
	{"RESEARCH_MAGIC_CREATE_IMP",RESEARCH_MAGIC_CREATE_IMP},
	{"RESEARCH_ROOM_TREASURE",RESEARCH_ROOM_TREASURE},
	{"RESEARCH_ROOM_HATCHERY",RESEARCH_ROOM_HATCHERY},
	{"RESEARCH_ROOM_LAIR",RESEARCH_ROOM_LAIR},
	{"RESEARCH_ROOM_RESEARCH",RESEARCH_ROOM_RESEARCH},
	{"RESEARCH_ROOM_TRAINING",RESEARCH_ROOM_TRAINING},
	{"RESEARCH_MAGIC_POWER_SIGHT",RESEARCH_MAGIC_POWER_SIGHT},
	{"RESEARCH_ROOM_BRIDGE",RESEARCH_ROOM_BRIDGE},
	{"RESEARCH_MAGIC_POWER_SPEED",RESEARCH_MAGIC_POWER_SPEED},
	{"RESEARCH_MAGIC_POWER_OBEY",RESEARCH_MAGIC_POWER_OBEY},
	{"RESEARCH_ROOM_GUARD_POST",RESEARCH_ROOM_GUARD_POST},
	{"RESEARCH_MAGIC_POWER_CALL_TO_ARMS",RESEARCH_MAGIC_POWER_CALL_TO_ARMS},
	{"RESEARCH_ROOM_WORKSHOP",RESEARCH_ROOM_WORKSHOP},
	{"RESEARCH_MAGIC_POWER_CONCEAL",RESEARCH_MAGIC_POWER_CONCEAL},
	{"RESEARCH_ROOM_BARRACKS",RESEARCH_ROOM_BARRACKS},
	{"RESEARCH_MAGIC_POWER_HOLD_AUDIENCE",RESEARCH_MAGIC_POWER_HOLD_AUDIENCE},
	{"RESEARCH_ROOM_PRISON",RESEARCH_ROOM_PRISON},
	{"RESEARCH_MAGIC_POWER_CAVE_IN",RESEARCH_MAGIC_POWER_CAVE_IN},
	{"RESEARCH_ROOM_TORTURE",RESEARCH_ROOM_TORTURE},
	{"RESEARCH_MAGIC_POWER_HEAL_CREATURE",RESEARCH_MAGIC_POWER_HEAL_CREATURE},
	{"RESEARCH_ROOM_TEMPLE",RESEARCH_ROOM_TEMPLE},
	{"RESEARCH_MAGIC_POWER_LIGHTNING",RESEARCH_MAGIC_POWER_LIGHTNING},
	{"RESEARCH_ROOM_GRAVEYARD",RESEARCH_ROOM_GRAVEYARD},
	{"RESEARCH_MAGIC_POWER_PROTECT",RESEARCH_MAGIC_POWER_PROTECT},
	{"RESEARCH_ROOM_SCAVENGER",RESEARCH_ROOM_SCAVENGER},
	{"RESEARCH_MAGIC_POWER_CHICKEN",RESEARCH_MAGIC_POWER_CHICKEN},
	{"RESEARCH_MAGIC_POWER_DISEASE",RESEARCH_MAGIC_POWER_DISEASE},
	{"RESEARCH_MAGIC_POWER_ARMAGEDDON",RESEARCH_MAGIC_POWER_ARMAGEDDON},
	{"RESEARCH_MAGIC_POWER_DESTROY_WALLS",RESEARCH_MAGIC_POWER_DESTROY_WALLS},
};

CreatureData LevelConfig::creatureData[32];
RoomData LevelConfig::roomData[17];
MagicData LevelConfig::magicData[20];
GameData LevelConfig::gameSettings[76];
SpellData LevelConfig::spellData[29];
ShotData LevelConfig::shotData[30];
TrapData LevelConfig::trapData[7];
DoorData LevelConfig::doorData[5];
HealthData LevelConfig::healthConfig[8];
BlockData LevelConfig::blockHealth[10];
InstanceData LevelConfig::instanceData[46];
ResearchData LevelConfig::researchData[33];

LevelConfig::~LevelConfig()
{

}

LevelConfig::LevelConfig()
{

}
bool StartsWithCapsUnderscore(std::string& line)
{
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == ' ' || line[i] == '\t') return false;
		if (line[i] == '_')return true;
		if (islower(line[i]))return false;
	}
	return false;
}

bool LevelConfig::LoadConfiguration()
{
	FileData creatureTxt = FileManager::GetFileData(L"DATA\\CREATURE.TXT");
	if (creatureTxt.size == 0) return false;

	std::string text = std::string((char*)creatureTxt.data, creatureTxt.size);

	std::stringstream data(text);

	for (std::string line; std::getline(data, line);)
	{
		if (!StartsWithCapsUnderscore(line))continue;

		std::stringstream lineStream(line);
		if (line.find("CREATURE_", 0) == 0)
		{ //creature data
			CreatureData c = { 0 };
			std::string name;

			//instead of directly writing into the CreatureData struct I have to write into these temporaries first, stringstream doesnt play nicely with uint8_t's as it'll only read a single char instead of the entire number
			uint16_t CreatureNumber, HealReq, HealThreshold, Strength, Armor, Dexterity, Fear, Defence, Luck, Recovery, HungerFill, LairSize, HurtByLava, SleepExperienceSlab, SleepExperience, ScavengeValue;
			uint16_t BaseSpeed, ExperienceForHitting, TrainingValue, TrainingCost, VisualRange, ScavengerCost, GrowUp, GrowUpLevel, MaxAngleChange, WalkingAnimSpeed, AttackPreference;
			uint16_t Hearing, EntranceRoom1, EntranceRoom2, EntranceRoom3, Room1SlabsRequired, Room2SlabsRequired, Room3SlabsRequired, ResearchValue, HeroVsKeeper, ManufactureValue;
			uint16_t JobStress, SlapsToKill, CreatureLoyalty, LoyaltyLevel, LairEnemy, EyeEffect, Rebirth, DamageToBoulder;
			uint16_t Power[10];
			uint16_t PowerLevelRequired[10];
			lineStream >> name >> CreatureNumber >> c.PrimaryJob >> c.SecondaryJob >> c.NotDoJobs >> c.AngerJobs;
			lineStream >> c.Health >> HealReq >> HealThreshold;
			lineStream >> Strength >> Armor >> Dexterity >> Fear >> Defence >> Luck >> Recovery;
			lineStream >> c.HungerRate >> HungerFill >> c.AnnoyLevel >> LairSize >> HurtByLava;
			lineStream >> SleepExperienceSlab >> SleepExperience >> ScavengeValue >> c.ScavengeRequired;
			lineStream >> BaseSpeed >> ExperienceForHitting >> TrainingValue >> TrainingCost >> c.GoldHold;
			lineStream >> VisualRange >> ScavengerCost;
			lineStream >> c.ToLevel2 >> c.ToLevel3 >> c.ToLevel4 >> c.ToLevel5 >> c.ToLevel6 >> c.ToLevel7 >> c.ToLevel8 >> c.ToLevel9 >> c.ToLevel10;
			lineStream >> c.ToLevelGrowUp >> GrowUp >> GrowUpLevel >> MaxAngleChange >> c.EyeHeight >> c.SizeXY >> c.SizeYZ;
			lineStream >> c.Flying >> WalkingAnimSpeed >> c.ImmuneToGas >> AttackPreference >> c.FieldOfView;
			lineStream >> Power[0] >> Power[1] >> Power[2] >> Power[3] >> Power[4] >> Power[5] >> Power[6] >> Power[7] >> Power[8] >> Power[9];
			lineStream >> PowerLevelRequired[0] >> PowerLevelRequired[1] >> PowerLevelRequired[2] >> PowerLevelRequired[3] >> PowerLevelRequired[4] >> PowerLevelRequired[5] >> PowerLevelRequired[6] >> PowerLevelRequired[7] >> PowerLevelRequired[8] >> PowerLevelRequired[9];
			lineStream >> ResearchValue >> c.HumanoidCreature >> c.PissOnDead >> c.Pay >> HeroVsKeeper >> ManufactureValue;
			lineStream >> Hearing >> EntranceRoom1 >> EntranceRoom2 >> EntranceRoom3 >> Room1SlabsRequired >> Room2SlabsRequired >> Room3SlabsRequired;
			lineStream >> c.Annoyance[static_cast<int>(CreatureData::Annoyance::NoLair)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::NoHatchery)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::WokenUp)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::StandingOnDeadEnemy)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::Sulking)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::NoSalary)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::Slapped)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::StandingOnDeadFriend)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::InTorture)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::InTemple)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::Sleeping)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::GotWage)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::WinBattle)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::UntrainedTime)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::Untrained)];
			lineStream >> JobStress;
			lineStream >> c.Annoyance[static_cast<int>(CreatureData::Annoyance::JobStress)] >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::Queue)];
			lineStream >> c.RealTraining >> c.TortureTime >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::WillNotDoJob)];
			lineStream >> SlapsToKill >> CreatureLoyalty >> LoyaltyLevel >> LairEnemy >> EyeEffect >> c.EntranceForce;
			lineStream >> Rebirth >> c.CanSeeInvisible >> c.CanGoThroughLockedDoors >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::OthersLeaving)] >>
				DamageToBoulder >>
				c.ThingSizeXY >> c.ThingSizeYZ >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::EatFood)] >>
				c.Bleeds >>
				c.Annoyance[static_cast<int>(CreatureData::Annoyance::InHand)] >>
				c.AffectedByWind;

			//assign temporaries
			c.CreatureNumber = CreatureNumber;
			c.HealReq = HealReq;
			c.HealThreshold = HealThreshold;
			c.Strength = Strength;
			c.Armor = Armor;
			c.Dexterity = Dexterity;
			c.Fear = Fear;
			c.Defence = Defence;
			c.Luck = Luck;
			c.Recovery = Recovery;
			c.HungerFill = HungerFill;
			c.LairSize = LairSize;
			c.HurtByLava = HurtByLava;
			c.BaseSpeed = BaseSpeed;
			c.SleepExperienceSlab = SleepExperienceSlab;
			c.SleepExperience = SleepExperience;
			c.ScavengeValue = ScavengeValue;
			c.ExperienceForHitting = ExperienceForHitting;
			c.TrainingValue = TrainingValue;
			c.TrainingCost = TrainingCost;
			c.VisualRange = VisualRange;
			c.ScavengerCost = ScavengerCost;
			c.GrowUp = GrowUp;
			c.GrowUpLevel = GrowUpLevel;
			c.MaxAngleChange = MaxAngleChange;
			c.WalkingAnimSpeed = WalkingAnimSpeed;
			c.AttackPreference = AttackPreference;
			c.Hearing = Hearing;
			c.EntranceRoom1 = EntranceRoom1;
			c.EntranceRoom2 = EntranceRoom2;
			c.EntranceRoom3 = EntranceRoom3;
			c.Room1SlabsRequired = Room1SlabsRequired;
			c.Room2SlabsRequired = Room2SlabsRequired;
			c.Room3SlabsRequired = Room3SlabsRequired;
			c.JobStress = JobStress;
			c.ResearchValue = ResearchValue;
			c.HeroVsKeeper = HeroVsKeeper;
			c.ManufactureValue = ManufactureValue;
			c.SlapsToKill = SlapsToKill;
			c.CreatureLoyalty = CreatureLoyalty;
			c.LoyaltyLevel = LoyaltyLevel;
			c.LairEnemy = LairEnemy;
			c.EyeEffect = EyeEffect;
			c.Rebirth = Rebirth;
			c.DamageToBoulder = DamageToBoulder;
			c.Power[0] = Power[0];
			c.Power[1] = Power[1];
			c.Power[2] = Power[2];
			c.Power[3] = Power[3];
			c.Power[4] = Power[4];
			c.Power[5] = Power[5];
			c.Power[6] = Power[6];
			c.Power[7] = Power[7];
			c.Power[8] = Power[8];
			c.Power[9] = Power[9];
			c.PowerLevelRequired[0] = PowerLevelRequired[0];
			c.PowerLevelRequired[1] = PowerLevelRequired[1];
			c.PowerLevelRequired[2] = PowerLevelRequired[2];
			c.PowerLevelRequired[3] = PowerLevelRequired[3];
			c.PowerLevelRequired[4] = PowerLevelRequired[4];
			c.PowerLevelRequired[5] = PowerLevelRequired[5];
			c.PowerLevelRequired[6] = PowerLevelRequired[6];
			c.PowerLevelRequired[7] = PowerLevelRequired[7];
			c.PowerLevelRequired[8] = PowerLevelRequired[8];
			c.PowerLevelRequired[9] = PowerLevelRequired[9];

			//copy into the right array index
			auto&& it = creatureMap.find(name);
			if (it != creatureMap.end())
			{
				creatureData[it->second] = c;
			}
			else
			{
				creatureData[CreatureData::CREATURE_NULL] = c;
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("ROOM_", 0) == 0)
		{
			RoomData r = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> r.Cost >> r.Health;
			r.Number = Number;
			auto&& it = roomsMap.find(name);
			if (it != roomsMap.end())
			{
				roomData[it->second] = r;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("MAGIC_", 0) == 0)
		{
			MagicData m = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> m.Cost[0] >> m.Cost[1] >> m.Cost[2] >> m.Cost[3] >> m.Cost[4] >> m.Cost[5] >> m.Cost[6] >> m.Cost[7] >> m.Cost[8] >> m.Time;
			lineStream >> m.Power[0] >> m.Power[1] >> m.Power[2] >> m.Power[3] >> m.Power[4] >> m.Power[5] >> m.Power[6] >> m.Power[7] >> m.Power[8];
			m.Number = Number;
			auto&& it = magicConfigMap.find(name);
			if (it != magicConfigMap.end())
			{
				magicData[it->second] = m;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("GAME_", 0) == 0)
		{
			GameData g = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> g.Value;
			g.Number = Number;
			auto&& it = gameSettingsMap.find(name);
			if (it != gameSettingsMap.end())
			{
				gameSettings[it->second] = g;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("SPELL_", 0) == 0)
		{
			SpellData s = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> s.Duration;
			s.Number = Number;
			auto&& it = spellMap.find(name);
			if (it != spellMap.end())
			{
				spellData[it->second] = s;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("SHOT_", 0) == 0)
		{
			ShotData s = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> s.Health >> s.Damage >> s.Speed;
			s.Number = Number;
			auto&& it = shotsMap.find(name);
			if (it != shotsMap.end())
			{
				shotData[it->second] = s;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("TRAP_", 0) == 0)
		{
			TrapData t = { 0 };
			std::string name;
			uint16_t Number,ManufactureLevel,Shots;
			lineStream >> name >> Number >> ManufactureLevel >> t.ManufactureRequired >> Shots >> t.TimeBetweenShots >> t.SellingValue;
			t.Number = Number;
			t.ManufactureLevel = ManufactureLevel;
			t.Shots = Shots;
			auto&& it = trapsMap.find(name);
			if (it != trapsMap.end())
			{
				trapData[it->second] = t;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("DOOR_", 0) == 0)
		{
			DoorData d = { 0 };
			std::string name;
			uint16_t Number, ManufactureLevel;
			lineStream >> name >> Number >> ManufactureLevel >> d.ManufactureRequired >> d.SellingValue >> d.Health;
			d.Number = Number;
			d.ManufactureLevel = ManufactureLevel;
			auto&& it = doorsMap.find(name);
			if (it != doorsMap.end())
			{
				doorData[it->second] = d;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("HEALTH_", 0) == 0)
		{
			HealthData h = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> h.Value;
			h.Number = Number;
			auto&& it = healthConfigsMap.find(name);
			if (it != healthConfigsMap.end())
			{
				healthConfig[it->second] = h;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("BLOCK_", 0) == 0)
		{
			BlockData b = { 0 };
			std::string name;
			uint16_t Number;
			lineStream >> name >> Number >> b.Value;
			b.Number = Number;
			auto&& it = blockHealthMap.find(name);
			if (it != blockHealthMap.end())
			{
				blockHealth[it->second] = b;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("INSTANCE_", 0) == 0)
		{
			InstanceData i = { 0 };
			std::string name;
			uint16_t Number, Time, ActionTime, FPTime, FPActionTime, ForceVisibility;
			lineStream >> name >> Number >> Time >> ActionTime >> i.ResetTime >> FPTime >> FPActionTime >> i.FPResetTime >> ForceVisibility;
			i.Number = Number;
			i.Time = Time;
			i.ActionTime = ActionTime;
			i.FPTime = FPTime;
			i.FPActionTime = FPActionTime;
			i.ForceVisibility = ForceVisibility;
			auto&& it = instancesMap.find(name);
			if (it != instancesMap.end())
			{
				instanceData[it->second] = i;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
		else if (line.find("RESEARCH_", 0) == 0)
		{
			ResearchData r = { 0 };
			std::string name;
			uint16_t Model;
			lineStream >> name >> Model >> r.Amount;
			r.Model = Model;
			auto&& it = researchMap.find(name);
			if (it != researchMap.end())
			{
				researchData[it->second] = r;
			}
			else
			{
				System::Print("%s type not found!", name.c_str());
			}
		}
	}
	return true;
}
int LevelConfig::TypeToRoom(uint16_t type)
{
	type = type & 0xFF;
	switch (type)
	{
		case Type_Portal: return ROOM_ENTRANCE;
		case Type_Treasure_Room: return ROOM_TREASURE_ROOM;
		case Type_Library:return ROOM_RESEARCH_ROOM;
		case Type_Prison:return ROOM_PRISON_ROOM;
		case Type_Torture_Room :return ROOM_TORTURE_ROOM;
		case Type_Training_Room :return ROOM_TRAINING_AREA;
		case Type_Dungeon_Heart:return ROOM_DUNGEON_HEART;
		case Type_Workshop:return ROOM_WORKSHOP_ROOM;
		case Type_Scavenger_Room:return ROOM_SCAVENGER;
		case Type_Temple:return ROOM_TEMPLE;
		case Type_Graveyard:return ROOM_GRAVEYARD;
		case Type_Hatchery :return ROOM_HATCHERY;
		case Type_Lair:return ROOM_LAIR;
		case Type_Barracks:return ROOM_BARRACK_ROOM;
		case Type_Bridge:return ROOM_BRIDGE;
		case Type_Guardpost:return ROOM_GUARD_POST;
	}
	return 0;
}