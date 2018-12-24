#include "ThempSystem.h"
#include "ThempLevelScript.h"
#include "ThempLevel.h"
#include "Players/ThempCPUPlayer.h"
#include "Players/ThempPlayer.h"
#include "ThempTileArrays.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <list>
using namespace Themp;

const std::unordered_map<std::string, LevelScript::Command::ScriptFunctions> StringToScriptFunction =
{
	{"CREATE_PARTY", LevelScript::Command::ScriptFunctions::CREATE_PARTY},
	{"ADD_TO_PARTY", LevelScript::Command::ScriptFunctions::ADD_TO_PARTY},
	{"ADD_PARTY_TO_LEVEL", LevelScript::Command::ScriptFunctions::ADD_PARTY_TO_LEVEL},
	{"ADD_TUNNELLER_PARTY_TO_LEVEL", LevelScript::Command::ScriptFunctions::ADD_TUNNELLER_PARTY_TO_LEVEL},
	{"ADD_TUNNELLER_TO_LEVEL", LevelScript::Command::ScriptFunctions::ADD_TUNNELLER_TO_LEVEL},
	{"ADD_CREATURE_TO_LEVEL", LevelScript::Command::ScriptFunctions::ADD_CREATURE_TO_LEVEL},
	{"MAGIC_AVAILABLE", LevelScript::Command::ScriptFunctions::MAGIC_AVAILABLE},
	{"ROOM_AVAILABLE", LevelScript::Command::ScriptFunctions::ROOM_AVAILABLE},
	{"CREATURE_AVAILABLE", LevelScript::Command::ScriptFunctions::CREATURE_AVAILABLE},
	{"DOOR_AVAILABLE", LevelScript::Command::ScriptFunctions::DOOR_AVAILABLE},
	{"TRAP_AVAILABLE ", LevelScript::Command::ScriptFunctions::TRAP_AVAILABLE },
	{"SET_TIMER", LevelScript::Command::ScriptFunctions::SET_TIMER},
	{"BONUS_LEVEL_TIME", LevelScript::Command::ScriptFunctions::BONUS_LEVEL_TIME},
	{"SET_CREATURE_FEAR", LevelScript::Command::ScriptFunctions::SET_CREATURE_FEAR},
	{"SET_CREATURE_ARMOUR", LevelScript::Command::ScriptFunctions::SET_CREATURE_ARMOUR},
	{"SET_CREATURE_HEALTH", LevelScript::Command::ScriptFunctions::SET_CREATURE_HEALTH},
	{"SET_CREATURE_STRENGTH", LevelScript::Command::ScriptFunctions::SET_CREATURE_STRENGTH},
	{"SET_CREATURE_MAX_LEVEL", LevelScript::Command::ScriptFunctions::SET_CREATURE_MAX_LEVEL},
	{"TUTORIAL_FLASH_BUTTON", LevelScript::Command::ScriptFunctions::TUTORIAL_FLASH_BUTTON},
	{"DISPLAY_INFORMATION", LevelScript::Command::ScriptFunctions::DISPLAY_INFORMATION},
	{"DISPLAY_OBJECTIVE", LevelScript::Command::ScriptFunctions::DISPLAY_OBJECTIVE},
	{"CREATE_TEXT", LevelScript::Command::ScriptFunctions::CREATE_TEXT},
	{"PRINT", LevelScript::Command::ScriptFunctions::PRINT},
	{"QUICK_OBJECTIVE", LevelScript::Command::ScriptFunctions::QUICK_OBJECTIVE},
	{"QUICK_INFORMATION", LevelScript::Command::ScriptFunctions::QUICK_INFORMATION},
	{"COMPUTER_PLAYER", LevelScript::Command::ScriptFunctions::COMPUTER_PLAYER},
	{"SET_COMPUTER_PROCESS", LevelScript::Command::ScriptFunctions::SET_COMPUTER_PROCESS},
	{"SET_COMPUTER_CHECKS", LevelScript::Command::ScriptFunctions::SET_COMPUTER_CHECKS},
	{"SET_COMPUTER_GLOBALS ", LevelScript::Command::ScriptFunctions::SET_COMPUTER_GLOBALS },
	{"ALLY_PLAYERS", LevelScript::Command::ScriptFunctions::ALLY_PLAYERS},
	{"START_MONEY", LevelScript::Command::ScriptFunctions::START_MONEY},
	{"SET_GENERATE_SPEED", LevelScript::Command::ScriptFunctions::SET_GENERATE_SPEED},
	{"NEXT_COMMAND_REUSABLE", LevelScript::Command::ScriptFunctions::NEXT_COMMAND_REUSABLE},
	{"WIN_GAME", LevelScript::Command::ScriptFunctions::WIN_GAME},
	{"LOSE_GAME", LevelScript::Command::ScriptFunctions::LOSE_GAME},
	{"SET_FLAG", LevelScript::Command::ScriptFunctions::SET_FLAG},
	{"MAX_CREATURES", LevelScript::Command::ScriptFunctions::MAX_CREATURES},
	{"ADD_CREATURE_TO_POOL", LevelScript::Command::ScriptFunctions::ADD_CREATURE_TO_POOL},
};

std::unordered_map<std::string, CreatureData::CreatureType> StringToCreatureType =
{
	{"FLOATING_SPIRIT",CreatureData::CreatureType::CREATURE_FLOATING_SPIRIT},
	{"ORC",CreatureData::CreatureType::CREATURE_ORC},
	{"TENTACLE",CreatureData::CreatureType::CREATURE_TENTACLE},
	{"GHOST",CreatureData::CreatureType::CREATURE_GHOST},
	{"HELL_HOUND",CreatureData::CreatureType::CREATURE_HELLHOUND},
	{"SPIDER",CreatureData::CreatureType::CREATURE_SPIDER},
	{"VAMPIRE",CreatureData::CreatureType::CREATURE_VAMPIRE},
	{"BUG",CreatureData::CreatureType::CREATURE_BEETLE},
	{"IMP",CreatureData::CreatureType::CREATURE_IMP},
	{"BILE_DEMON",CreatureData::CreatureType::CREATURE_BILE_DEMON},
	{"SORCEROR",CreatureData::CreatureType::CREATURE_SORCEROR},
	{"DARK_MISTRESS",CreatureData::CreatureType::CREATURE_DARK_MISTRESS},
	{"FLY",CreatureData::CreatureType::CREATURE_FLY},
	{"DEMONSPAWN",CreatureData::CreatureType::CREATURE_DEMONSPAWN},
	{"DRAGON",CreatureData::CreatureType::CREATURE_DRAGON},
	{"TROLL",CreatureData::CreatureType::CREATURE_TROLL},
	{"SKELETON",CreatureData::CreatureType::CREATURE_SKELETON},
	{"HORNY",CreatureData::CreatureType::CREATURE_HORNY},
	{"SAMURAI",CreatureData::CreatureType::CREATURE_SAMURAI},
	{"THIEF",CreatureData::CreatureType::CREATURE_THIEF},
	{"FAIRY",CreatureData::CreatureType::CREATURE_FAIRY},
	{"GIANT",CreatureData::CreatureType::CREATURE_GIANT},
	{"WITCH",CreatureData::CreatureType::CREATURE_WITCH},
	{"TUNNELLER",CreatureData::CreatureType::CREATURE_TUNNELLER},
	{"AVATAR",CreatureData::CreatureType::CREATURE_AVATAR},
	{"KNIGHT",CreatureData::CreatureType::CREATURE_KNIGHT},
	{"DWARF",CreatureData::CreatureType::CREATURE_DWARF},
	{"MONK",CreatureData::CreatureType::CREATURE_MONK},
	{"ARCHER",CreatureData::CreatureType::CREATURE_ARCHER},
	{"BARBARIAN",CreatureData::CreatureType::CREATURE_BARBARIAN},
	{"WIZARD",CreatureData::CreatureType::CREATURE_WIZARD},
};

std::unordered_map<std::string, uint16_t> StringToRoomType
{
	{"TREASURE", Type_Treasure_Room},
	{"LAIR", Type_Lair},
	{"GARDEN", Type_Hatchery},
	{"BRIDGE", Type_Bridge},
	{"BARRACKS", Type_Barracks},
	{"TRAINING", Type_Training_Room},
	{"RESEARCH",Type_Library},
	{"WORKSHOP",Type_Workshop},
	{"PRISON",Type_Prison},
	{"TORTURE",Type_Torture_Room},
	{"SCAVENGER",Type_Scavenger_Room},
	{"TEMPLE",Type_Temple},
	{"GRAVEYARD",Type_Graveyard},
	{"GUARD_POST",Type_Guardpost},
	{"ENTRANCE",Type_Portal},
};

struct GameTurnTimer
{
	int turns = 0;
	bool started = false;
};
std::array<std::unordered_map<std::string, GameTurnTimer>, 6> turnTimers;

std::array<std::unordered_map<std::string,int>,6> LevelScript::GameValues;

std::array<std::unordered_map<CreatureData::CreatureType, AvailableObject>, 6> LevelScript::AvailableCreatures;
std::array<std::unordered_map<uint16_t, AvailableObject>, 6> LevelScript::AvailableRooms;
std::array<std::unordered_map<Spells, AvailableObject>, 6> LevelScript::AvailableSpells;
std::array<std::unordered_map<Traps, AvailableObject>, 6> LevelScript::AvailableTraps;
std::array<std::unordered_map<Doors, AvailableObject>, 6> LevelScript::AvailableDoors;

LevelScript::~LevelScript()
{
	for (int i = 0; i < m_IfStatements.size(); i++)
	{
		std::vector<IfStatement*> ifsToFree;
		IfStatement* node = m_IfStatements[i];
		while (node != nullptr)
		{
			ifsToFree.push_back(node);
			node = node->child;
		}
		for (int  j = 0; j < ifsToFree.size(); j++)
		{
			delete ifsToFree[j];
		}
	}
}
LevelScript::LevelScript(std::wstring file)
{
	Parse(FileManager::GetFileData(file+L".TXT"));

	for (int i = 0; i < 6; i++)
	{
		GameValues[i]["GAME_TURN"] = 0;

		GameValues[i]["TREASURE"] = 0;
		GameValues[i]["LAIR"] = 0;
		GameValues[i]["GARDEN"] = 0;
		GameValues[i]["BRIDGE"] = 0;
		GameValues[i]["BARRACKS"] = 0;
		GameValues[i]["TRAINING"] = 0;
		GameValues[i]["RESEARCH"] = 0;
		GameValues[i]["WORKSHOP"] = 0;
		GameValues[i]["PRISON"] = 0;
		GameValues[i]["TORTURE"] = 0;
		GameValues[i]["SCAVENGER"] = 0;
		GameValues[i]["TEMPLE"] = 0;
		GameValues[i]["GRAVEYARD"] = 0;
		GameValues[i]["GUARD_POST"] = 0;
		GameValues[i]["ENTRANCE"] = 0;

		GameValues[i]["TIMER0"] = 0;
		GameValues[i]["TIMER1"] = 0;
		GameValues[i]["TIMER2"] = 0;
		GameValues[i]["TIMER3"] = 0;
		GameValues[i]["TIMER4"] = 0;
		GameValues[i]["TIMER5"] = 0;
		GameValues[i]["TIMER6"] = 0;
		GameValues[i]["TIMER7"] = 0;
		GameValues[i]["FLAG0"] = 0;
		GameValues[i]["FLAG1"] = 0;
		GameValues[i]["FLAG2"] = 0;
		GameValues[i]["FLAG3"] = 0;
		GameValues[i]["FLAG4"] = 0;
		GameValues[i]["FLAG5"] = 0;
		GameValues[i]["FLAG6"] = 0;
		GameValues[i]["FLAG7"] = 0;

		GameValues[i]["ALL_DUNGEONS_DESTROYED"] = 0;
		GameValues[i]["CREATURES_SCAVENGED_GAINED"] = 0;
		GameValues[i]["CREATURES_SCAVENGED_LOST"] = 0;
		GameValues[i]["DOORS_DESTROYED"] = 0;
		GameValues[i]["TOTAL_GOLD_MINED"] = 0;
		GameValues[i]["DUNGEON_DESTROYED"] = 0;
		GameValues[i]["GOLD_POTS_STOLEN"] = 0;
		GameValues[i]["TIMES_BROKEN_INTO"] = 0;
		GameValues[i]["SPELLS_STOLEN"] = 0;
		GameValues[i]["ROOMS_DESTROYED"] = 0;
		GameValues[i]["BATTLES_WON"] = 0;
		GameValues[i]["BATTLES_LOST"] = 0;
		GameValues[i]["CREATURES_ANNOYED"] = 0;
		GameValues[i]["TOTAL_CREATURES_LEFT"] = 0;
		GameValues[i]["TOTAL_AREA"] = 0;
		GameValues[i]["TOTAL_DOORS"] = 0;
		GameValues[i]["TOTAL_RESEARCH"] = 0;
		GameValues[i]["TOTAL_CREATURES"] = 0;
		GameValues[i]["TOTAL_IMPS"] = 0;
		GameValues[i]["BREAK_IN"] = 0;
		GameValues[i]["GAME_TURN"] = 0;
		GameValues[i]["MONEY"] = 0;

		GameValues[i]["FLOATING_SPIRIT"] = 0;
		GameValues[i]["ORC"] = 0;
		GameValues[i]["TENTACLE"] = 0;
		GameValues[i]["GHOST"] = 0;
		GameValues[i]["HELL_HOUND"] = 0;
		GameValues[i]["SPIDER"] = 0;
		GameValues[i]["VAMPIRE"] = 0;
		GameValues[i]["BUG"] = 0;
		GameValues[i]["IMP"] = 0;
		GameValues[i]["BILE_DEMON"] = 0;
		GameValues[i]["SORCEROR"] = 0;
		GameValues[i]["DARK_MISTRESS"] = 0;
		GameValues[i]["FLY"] = 0;
		GameValues[i]["DEMONSPAWN"] = 0;
		GameValues[i]["DRAGON"] = 0;
		GameValues[i]["TROLL"] = 0;
		GameValues[i]["SKELETON"] = 0;
		GameValues[i]["HORNY"] = 0;
		GameValues[i]["SAMURAI"] = 0;
		GameValues[i]["THIEF"] = 0;
		GameValues[i]["FAIRY"] = 0;
		GameValues[i]["GIANT"] = 0;
		GameValues[i]["WITCH"] = 0;
		GameValues[i]["TUNNELLER"] = 0;
		GameValues[i]["AVATAR"] = 0;
		GameValues[i]["KNIGHT"] = 0;
		GameValues[i]["DWARF"] = 0;
		GameValues[i]["MONK"] = 0;
		GameValues[i]["ARCHER"] = 0;
		GameValues[i]["BARBARIAN"] = 0;
		GameValues[i]["WIZARD"] = 0;
	}

}


void RemoveWhiteSpace(std::string& str)
{
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}


std::list<LevelScript::IfStatement*> currentIf;

void LevelScript::Parse(FileData data)
{
	std::string stringFile = std::string((char*)data.data, data.size);
	std::stringstream file(stringFile);
	
	int linenum = 0;
	std::string line;
	while (std::getline(file, line))
	{
		linenum++;
		//convert string to full uppercase, making the script case insensitive (it's non-consistent in its use so this prevents us from having to handle a lot of case-issues)
		std::transform(line.begin(), line.end(), line.begin(), toupper);
		RemoveWhiteSpace(line);
		
		//empty line
		if (line.size() <= 1)
		{
			continue;
		}
		//this line is a comment / remark, skip it
		if (line.find("REM") == 0)
		{
			continue;
		}
		//If we got here then the line is not a comment, let's start parsing some stuff..
		
		//temporary to deal with IF_AVAILABLE on later levels
		if (line.find("IF_AVAILABLE(") == 0)
		{
			IfStatement* ifs = new IfStatement();
			ifs->child = nullptr;

			if (currentIf.size() > 0)
			{
				currentIf.back()->child = ifs;
			}
			currentIf.push_back(ifs);
			continue;
		}

		//Line is a if-statement
		if (line.find("IF(") == 0)
		{
			//we enter a if-statement, next lines are nested inside
			IfStatement* ifs = new IfStatement();
			ifs->child = nullptr;

			int argCount = 0;
			int offset = 3; //skip the IF(
			while (offset < line.size() - 1 && line[offset] != ')' )
			{
				std::string token = "";
				while (isalnum(line[offset]) || line[offset] == '_')
				{
					token += line[offset];
					offset++;
				}

				ifs->owner =
					(token == "PLAYER0" ? Owner_PlayerRed :
						token == "PLAYER1" ? Owner_PlayerBlue :
						token == "PLAYER2" ? Owner_PlayerGreen :
						token == "PLAYER3" ? Owner_PlayerYellow :
						token == "PLAYER_GOOD" ? Owner_PlayerWhite :
						token == "ALL_PLAYERS" ? Owner_PlayerNone : 255);

				if (ifs->owner == 255)
				{
					System::Print("Syntax Error!, expected a valid player qualifier! Got: %s  @L%i",token.c_str(), linenum);
					ifs->owner = 0;
					goto END;
				}
				//followed by a ,
				if (line[offset] != ',')
				{
					System::Print("Syntax Error!, expected ',' @L%i", linenum);
					goto END;
				}
				offset++; //skip the ,

				//followed by a variable name
				token = "";
				while (isalnum(line[offset]) || line[offset] == '_')
				{
					token += line[offset];
					offset++;
				}

				ifs->var = token;

				if (token.size() <= 1)
				{
					System::Print("Syntax Error!, expected engine variable name, got: %s  @L%i",token.c_str(), linenum);
					//goto END;
				}

				//evaluator is up next
				token = "";
				while ((offset < line.size()-1) && (line[offset] == '<' || line[offset] == '>' || line[offset] == '=' || line[offset] == '!'))
				{
					token.push_back(line[offset]);
					offset++;
				}
				ifs->eval =
					(token == "<" ? IfStatement::Evaluator::SMALLERTHAN :
						token == "<=" ? IfStatement::Evaluator::SMALLEROREQUALTO :
						token == "==" ? IfStatement::Evaluator::EQUALTO :
						token == ">=" ? IfStatement::Evaluator::BIGGEROREQUALTO :
						token == ">" ? IfStatement::Evaluator::BIGGERTHAN :
						IfStatement::Evaluator::NOTEQUAL);
				if (token != "!=" && ifs->eval == IfStatement::Evaluator::NOTEQUAL)
				{
					System::Print("Syntax Error!,invalid evaluator!, got: %s  @L%i", token.c_str(), linenum);
				}
				token = "";
				while (offset < line.size() - 1 && isdigit(line[offset]))
				{
					token += line[offset];
					offset++;
				}
				if (token.size() == 0)
				{
					System::Print("Syntax Error!, expected integral value after evaluator @L%i", token.c_str(), linenum);
					//goto END;
				}
				ifs->number = atoi(token.c_str());
			}

			if (currentIf.size() > 0)
			{
				currentIf.back()->child = ifs;
			}
			currentIf.push_back(ifs);
			continue;
		}
		if (line.find("ENDIF") == 0)
		{
			//exit a level of the nested if
			if (currentIf.size() == 1)
			{
				m_IfStatements.push_back(currentIf.back());
			}
			currentIf.pop_back();
			continue;
		}

		//line is a function call, there's only 2 0-argument functions WIN_GAME and LOSE_GAME, which doesnt use parenthesis, so we check that first
		if (line.find("WIN_GAME") == 0 || line.find("LOSE_GAME") == 0)
		{
			Command c = {};
			if (line.find("WIN_GAME") == 0)
			{
				c.function = LevelScript::Command::ScriptFunctions::WIN_GAME;
			}
			else
			{
				c.function = LevelScript::Command::ScriptFunctions::LOSE_GAME;
			}
			//add win-game to the execution list
			if (currentIf.size() > 0)
			{
				currentIf.back()->commands.push_back(c);
			}
			else
			{
				m_Commands.push_back(c);
			}
			continue;
		}

		int offset = line.find_first_of('(');
		if (offset > 0)
		{
			std::string command = line.substr(0, offset);
			//command contains one of the so many function calls

			Command c = {  };
			auto it = StringToScriptFunction.find(command);
			if (it != StringToScriptFunction.end())
			{
				c.function = it->second;
				int argCount = 0;
				offset++; //skip the ( character
				while (offset < line.size()-1 && line[offset] != ')' && line[offset] != '\r' && line[offset] != '\n')
				{
					std::string arg = "";
					while (offset < line.size()-1 && line[offset] != ',' && line[offset] != ')' && line[offset] != '\r' && line[offset] != '\n')
					{
						arg.push_back(line[offset]);
						offset++;
					}
					offset++;
					if (isdigit(arg[0]))
					{
						c.argsInts[argCount] = atoi(arg.c_str());
					}
					else
					{
						c.argsStrings[argCount] = arg;
					}
					argCount++;
				}
			}
			else
			{
				System::Print("Unrecognized Command: %s",command.c_str());
			}
			if (currentIf.size() > 0)
			{
				currentIf.back()->commands.push_back(c);
			}
			else
			{
				m_Commands.push_back(c);
			}
		}
	}
END:
	System::Print("Finished Parsing the level script!");
}

uint8_t PlayerTagToNumber(const std::string& tag)
{
	return tag == "PLAYER0" ? Owner_PlayerRed :
		tag == "PLAYER1" ? Owner_PlayerBlue :
		tag == "PLAYER2" ? Owner_PlayerGreen :
		tag == "PLAYER3" ? Owner_PlayerYellow :
		tag == "PLAYER_GOOD" ? Owner_PlayerWhite :
		tag == "ALL_PLAYERS" ? Owner_PlayerNone : 0;
}
void LevelScript::ExecuteCommand(Command* c)
{
	//if PlayerTagToNumber == ALL_PLAYERS ( Owner_PlayerNone )
	//execute it for ALL players

	uint8_t nr = 0;
	switch (c->function)
	{
	case Command::ScriptFunctions::WIN_GAME:
		System::Print("Level beaten! You win! Yay!");
		System::Print("Unimplemented: WIN_GAME");
		break;
		break;
	case Command::ScriptFunctions::LOSE_GAME:
		System::Print("Level lost! Sad times.. Aww..");
		System::Print("Unimplemented: LOSE_GAME");
		break;
		break;
	case Command::ScriptFunctions::DISPLAY_INFORMATION:
		System::Print("%s", FileManager::GetText(c->argsInts[0]).c_str());
		System::Print("Unimplemented: DISPLAY_INFORMATION");
		break;
	case Command::ScriptFunctions::DISPLAY_OBJECTIVE:
		System::Print("%s", FileManager::GetText(c->argsInts[0]).c_str());
		System::Print("Unimplemented: DISPLAY_OBJECTIVE");
		break;
	case Command::ScriptFunctions::SET_FLAG:
		GameValues[PlayerTagToNumber(c->argsStrings[0])][c->argsStrings[1]] = c->argsInts[2];
		break;
	case Command::ScriptFunctions::CREATE_PARTY:
		System::Print("Unimplemented: CREATE_PARTY");
		break;
	case Command::ScriptFunctions::ADD_TO_PARTY:
		System::Print("Unimplemented: ADD_TO_PARTY");
		break;
	case Command::ScriptFunctions::ADD_PARTY_TO_LEVEL:
		System::Print("Unimplemented: ADD_PARTY_TO_LEVEL");
		break;
	case Command::ScriptFunctions::ADD_TUNNELLER_PARTY_TO_LEVEL:
		System::Print("Unimplemented: ADD_TUNNELLER_PARTY_TO_LEVEL");
		break;
	case Command::ScriptFunctions::ADD_TUNNELLER_TO_LEVEL:
		System::Print("Unimplemented: ADD_TUNNELLER_TO_LEVEL");
		break;
	case Command::ScriptFunctions::ADD_CREATURE_TO_LEVEL:
		System::Print("Unimplemented: ADD_CREATURE_TO_LEVEL");
		break;
	case Command::ScriptFunctions::MAGIC_AVAILABLE:
		{

			System::Print("Unimplemented: MAGIC_AVAILABLE");
		}
		break;
	case Command::ScriptFunctions::ROOM_AVAILABLE:
		{
			AvailableObject& ao = AvailableRooms[PlayerTagToNumber(c->argsStrings[0])][StringToRoomType[c->argsStrings[1]]];
			ao.canBeAvailable = c->argsInts[2] > 0 ? true : false;
			ao.isAvailable = c->argsInts[3] > 0 ? true : false;
			Level::s_CurrentLevel->AvailableRoomsChanged();
		}
		break;
	case Command::ScriptFunctions::CREATURE_AVAILABLE:
		{
			uint8_t player = PlayerTagToNumber(c->argsStrings[0]);
			AvailableObject& ao = AvailableCreatures[player][StringToCreatureType[c->argsStrings[1]]];
			ao.canBeAvailable = c->argsInts[2] > 0 ? true : false;
			ao.isAvailable = c->argsInts[3] > 0 ? true : false;
		}
		break;
	case Command::ScriptFunctions::DOOR_AVAILABLE:
		System::Print("Unimplemented: DOOR_AVAILABLE");
		break;
	case Command::ScriptFunctions::TRAP_AVAILABLE:
		System::Print("Unimplemented: TRAP_AVAILABLE");
		break;
	case Command::ScriptFunctions::SET_TIMER:
		{
			GameTurnTimer& timer = turnTimers[PlayerTagToNumber(c->argsStrings[0])][c->argsStrings[1]];
			timer.started = true;
			timer.turns = 0;
		}
		break;
	case Command::ScriptFunctions::BONUS_LEVEL_TIME:
		System::Print("Unimplemented: BONUS_LEVEL_TIME");
		break;
	case Command::ScriptFunctions::SET_CREATURE_FEAR:
		System::Print("Unimplemented: SET_CREATURE_FEAR");
		break;
	case Command::ScriptFunctions::SET_CREATURE_ARMOUR:
		System::Print("Unimplemented: SET_CREATURE_ARMOUR");
		break;
	case Command::ScriptFunctions::SET_CREATURE_HEALTH:
		System::Print("Unimplemented: SET_CREATURE_HEALTH");
		break;
	case Command::ScriptFunctions::SET_CREATURE_STRENGTH:
		System::Print("Unimplemented: SET_CREATURE_STRENGTH");
		break;
	case Command::ScriptFunctions::SET_CREATURE_MAX_LEVEL:
		System::Print("Unimplemented: SET_CREATURE_MAX_LEVEL");
		break;
	case Command::ScriptFunctions::TUTORIAL_FLASH_BUTTON:
		System::Print("Unimplemented: TUTORIAL_FLASH_BUTTON");
		break;
	case Command::ScriptFunctions::CREATE_TEXT:
		System::Print("Unimplemented: CREATE_TEXT");
		break;
	case Command::ScriptFunctions::PRINT:
		System::Print("Unimplemented: PRINT");
		break;
	case Command::ScriptFunctions::QUICK_OBJECTIVE:
		System::Print("Unimplemented: QUICK_OBJECTIVE");
		break;
	case Command::ScriptFunctions::QUICK_INFORMATION:
		System::Print("Unimplemented: QUICK_INFORMATION");
		break;
	case Command::ScriptFunctions::COMPUTER_PLAYER:
		nr = PlayerTagToNumber(c->argsStrings[0]);
		Themp::Level::s_CurrentLevel->m_Players[nr] = new CPUPlayer(nr);
		Themp::Level::s_CurrentLevel->m_Players[nr]->m_IsEnabled = true;

		break;
	case Command::ScriptFunctions::SET_COMPUTER_PROCESS:
		System::Print("Unimplemented: SET_COMPUTER_PROCESS");
		break;
	case Command::ScriptFunctions::SET_COMPUTER_CHECKS:
		System::Print("Unimplemented: SET_COMPUTER_CHECKS");
		break;
	case Command::ScriptFunctions::SET_COMPUTER_GLOBALS:
		System::Print("Unimplemented: SET_COMPUTER_GLOBALS");
		break;
	case Command::ScriptFunctions::ALLY_PLAYERS:
		System::Print("Unimplemented: ALLY_PLAYERS");
		break;
	case Command::ScriptFunctions::START_MONEY:
		LevelScript::GameValues[PlayerTagToNumber(c->argsStrings[0])]["START_MONEY"] = c->argsInts[1];
		break;
	case Command::ScriptFunctions::SET_GENERATE_SPEED:
		for (int i = 0; i < 6; i++)
		{
			LevelScript::GameValues[i]["GENERATE_SPEED"] = c->argsInts[0];
		}
		break;
	case Command::ScriptFunctions::NEXT_COMMAND_REUSABLE:
		//System::Print("Unimplemented: NEXT_COMMAND_REUSABLE");
		break;
	case Command::ScriptFunctions::MAX_CREATURES:
		System::Print("Unimplemented: MAX_CREATURES");
		break;
	case Command::ScriptFunctions::ADD_CREATURE_TO_POOL:
		{
			AvailableCreatureInPool available;
			available.type = StringToCreatureType[c->argsStrings[0]];
			available.amount = c->argsInts[1];
			Level::s_CurrentLevel->m_AvailableCreatures.push_back(available); 
		}
		break;
	}
}
bool LevelScript::EvaluateIfStatement(IfStatement* ifs)
{
	switch (ifs->eval)
	{
	case IfStatement::Evaluator::SMALLERTHAN:
		return GameValues[ifs->owner][ifs->var] < ifs->number;
	case IfStatement::Evaluator::SMALLEROREQUALTO:
		return GameValues[ifs->owner][ifs->var] <= ifs->number;
	case IfStatement::Evaluator::EQUALTO:
		return GameValues[ifs->owner][ifs->var] == ifs->number;
	case IfStatement::Evaluator::BIGGEROREQUALTO:
		return GameValues[ifs->owner][ifs->var] >= ifs->number;
	case IfStatement::Evaluator::BIGGERTHAN:
		return GameValues[ifs->owner][ifs->var] > ifs->number;
	case IfStatement::Evaluator::NOTEQUAL:
		return GameValues[ifs->owner][ifs->var] != ifs->number;
	}
	return false;
}
void LevelScript::AddRoom(uint8_t owner, uint16_t roomType, int count)
{
	switch (roomType)
	{
	case Type_Treasure_Room:
		GameValues[owner]["TREASURE"] += count;
		break;
	case Type_Lair:
		GameValues[owner]["LAIR"] += count;
		break;
	case Type_Hatchery:
		GameValues[owner]["GARDEN"] += count;
		break;
	case Type_Bridge:
		GameValues[owner]["BRIDGE"] += count;
		break;
	case Type_Barracks:
		GameValues[owner]["BARRACKS"] += count;
		break;
	case Type_Training_Room:
		GameValues[owner]["TRAINING"] += count;
		break;
	case Type_Library:
		GameValues[owner]["RESEARCH"] += count;
		break;
	case Type_Workshop:
		GameValues[owner]["WORKSHOP"] += count;
		break;
	case Type_Prison:
		GameValues[owner]["PRISON"] += count;
		break;
	case Type_Torture_Room:
		GameValues[owner]["TORTURE"] += count;
		break;
	case Type_Scavenger_Room:
		GameValues[owner]["SCAVENGER"] += count;
		break;
	case Type_Temple:
		GameValues[owner]["TEMPLE"] += count;
		break;
	case Type_Graveyard:
		GameValues[owner]["GRAVEYARD"] += count;
		break;
	case Type_Guardpost:
		GameValues[owner]["GUARD_POST"] += count;
		break;
	case Type_Portal:
		GameValues[owner]["ENTRANCE"] += count;
		break;
	}

}


void LevelScript::RunInitCommands()
{
	//loose commands are only executed once (think of it like an init function)
	for (int i = 0; i < m_Commands.size(); i++)
	{
		ExecuteCommand(&m_Commands[i]);
	}
	m_Commands.clear();
	Level* level = Themp::Level::s_CurrentLevel;
	//Set all initial player values
	for (int i = 0; i < 6; i++)
	{
		PlayerBase* player = level->m_Players[i];
		if (player != nullptr)
		{
			Player* human = dynamic_cast<Player*>(player);
			if (human)
			{
				human->m_GoldAmount = GameValues[i]["START_MONEY"];
				continue;
			}
			CPUPlayer* CPU = dynamic_cast<CPUPlayer*>(player);
			if (CPU)
			{
				//CPU->m_GoldAmount = GameValues[i]["START_MONEY"];
			}
			
		}
	}
}

float TurnTimer = 0;
void LevelScript::Update(float dt)
{
	TurnTimer += dt;
	const float turnDelta = 1.0f / GAME_TURNS_PER_SECOND;
	if (TurnTimer > turnDelta)
	{
		TurnTimer -= turnDelta;
		for (int i = 0; i < 6; i++)
		{
			GameValues[i]["GAME_TURN"]++;
			for (auto&& timer : turnTimers[i])
			{
				if(timer.second.started)
				{
					timer.second.turns++;
					GameValues[i][timer.first] = timer.second.turns;
				}
			}
		}
		if (TurnTimer > turnDelta * 2)
		{
			TurnTimer = turnDelta * 2;
		}
	}

	for (int i = m_IfStatements.size()-1; i >= 0; i--)
	{
		IfStatement* node = m_IfStatements[i];
		while (node != nullptr)
		{
			if (EvaluateIfStatement(node))
			{
				bool containsReusableCommand = false;
				std::vector<Command> commandsToKeep;
				for (int j = 0; j < node->commands.size(); j++)
				{
					if (node->commands[j].function == LevelScript::Command::ScriptFunctions::NEXT_COMMAND_REUSABLE)
					{
						commandsToKeep.push_back(node->commands[j]);
						commandsToKeep.push_back(node->commands[j+1]);
						containsReusableCommand = true;
						System::Print("Reusable command found, WATCH FOR BUGS!, this might not be implemented correctly yet!");
						continue;
					}
					ExecuteCommand(&node->commands[j]);
				}
				node->commands = commandsToKeep;
				node = node->child;
				if (node == nullptr && !containsReusableCommand)
				{
					//we went through the entire tree, remove this statement from the list
					m_IfStatements.erase(m_IfStatements.begin() + i);
				}
			}
			else
			{
				break;
			}
		}
	}
}