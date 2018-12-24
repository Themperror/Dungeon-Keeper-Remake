#pragma once
#include "ThempFileManager.h"
#include <array>
#include <vector>
#include <unordered_map>
#include "ThempLevelConfig.h"
namespace Themp
{
	struct AvailableObject
	{
		bool canBeAvailable = false;
		bool isAvailable = false;
	};
	class LevelScript
	{
	public:
		struct Command
		{
			public:
			enum class ScriptFunctions
			{
				UNKNOWN_COMMAND,
				SET_FLAG,
				CREATE_PARTY,
				ADD_TO_PARTY,
				ADD_PARTY_TO_LEVEL,
				ADD_TUNNELLER_PARTY_TO_LEVEL,
				ADD_TUNNELLER_TO_LEVEL,
				ADD_CREATURE_TO_LEVEL,
				MAGIC_AVAILABLE,
				ROOM_AVAILABLE,
				CREATURE_AVAILABLE,
				DOOR_AVAILABLE,
				TRAP_AVAILABLE,
				SET_TIMER,
				BONUS_LEVEL_TIME,
				SET_CREATURE_FEAR,
				SET_CREATURE_ARMOUR,
				SET_CREATURE_HEALTH,
				SET_CREATURE_STRENGTH,
				SET_CREATURE_MAX_LEVEL,
				TUTORIAL_FLASH_BUTTON,
				DISPLAY_INFORMATION,
				DISPLAY_OBJECTIVE,
				CREATE_TEXT,
				PRINT,
				QUICK_OBJECTIVE,
				QUICK_INFORMATION,
				COMPUTER_PLAYER,
				SET_COMPUTER_PROCESS,
				SET_COMPUTER_CHECKS,
				SET_COMPUTER_GLOBALS,
				ALLY_PLAYERS,
				START_MONEY,
				SET_GENERATE_SPEED,
				NEXT_COMMAND_REUSABLE,
				WIN_GAME,
				LOSE_GAME,
				MAX_CREATURES,
				ADD_CREATURE_TO_POOL
			};
			ScriptFunctions function;

			//max 6 args for the biggest function, they can either be an int or string
			std::array<std::string, 6> argsStrings;
			std::array<int, 6> argsInts; 
		};
		struct IfStatement
		{
			enum class Evaluator { SMALLERTHAN, SMALLEROREQUALTO, EQUALTO, BIGGEROREQUALTO, BIGGERTHAN, NOTEQUAL};
			IfStatement* child;

			uint8_t owner;
			std::string var;
			Evaluator eval;
			int number;
			
			std::vector<Command>  commands;
		};
		
		static void AddRoom(uint8_t owner, uint16_t roomType, int count);


		~LevelScript();

		LevelScript(std::wstring file);
		void Parse(FileData data);
		void ExecuteCommand(Command * c);
		bool EvaluateIfStatement(IfStatement * ifs);
		void RunInitCommands();
		void Update(float dt);
		std::vector<Command> m_Commands;
		std::vector<IfStatement*> m_IfStatements;
		static std::array<std::unordered_map<std::string, int>,6> GameValues;
		static std::array<std::unordered_map<CreatureData::CreatureType, AvailableObject>, 6> AvailableCreatures;
		static std::array<std::unordered_map<uint16_t, AvailableObject>, 6> AvailableRooms;
		static std::array<std::unordered_map<Spells, AvailableObject>, 6> AvailableSpells;
		static std::array<std::unordered_map<Traps, AvailableObject>, 6> AvailableTraps;
		static std::array<std::unordered_map<Doors, AvailableObject>, 6> AvailableDoors;
	};
};