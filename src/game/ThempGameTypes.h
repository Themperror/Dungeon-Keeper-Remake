#pragma once
#include <cstdint>


struct PlayerID
{
	static constexpr uint8_t Red = 0;
	static constexpr uint8_t Blue = 1;
	static constexpr uint8_t Green = 2;
	static constexpr uint8_t Yellow = 3;
	static constexpr uint8_t White = 4;
	static constexpr uint8_t None = 5;

	operator int8_t() const
	{
		return val;
	}
	operator int16_t() const
	{
		return val;
	}
	operator int32_t() const
	{
		return val;
	}
	operator int64_t() const
	{
		return val;
	}
	operator uint8_t() const
	{
		return val;
	}
	operator uint16_t() const
	{
		return val;
	}
	operator uint32_t() const
	{
		return val;
	}
	operator uint64_t() const
	{
		return val;
	}

	bool operator==(uint8_t v) const
	{
		return val == v;
	}
	bool operator==(uint16_t v) const
	{
		return val == v;
	}
	bool operator==(uint32_t v) const
	{
		return val == v;
	}
	bool operator==(uint64_t v) const
	{
		return val == v;
	}

	bool operator==(int8_t v) const
	{
		return val == v;
	}
	bool operator==(int16_t v) const
	{
		return val == v;
	}
	bool operator==(int32_t v) const
	{
		return val == v;
	}
	bool operator==(int64_t v) const
	{
		return val == v;
	}
	
	bool operator==(PlayerID v) const
	{
		return val == v;
	}
	
	bool operator!=(PlayerID v) const
	{
		return val != v;
	}
	
	bool operator!=(uint8_t v) const
	{
		return val != v;
	}
	bool operator<(uint8_t v)
	{
		return val < v;
	}

	void operator=(uint8_t v)
	{
		val = v;
	}
	PlayerID(uint8_t v)
	{
		val = v;
	}
	PlayerID()
	{
		val = 0;
	}

private:
	uint8_t val;
};


//3 bits
static constexpr int N_DIFF = 0b00;
static constexpr int N_SAME = 0b01;
static constexpr int N_WATER = 0b10;
static constexpr int N_LAVA = 0b11;
static constexpr int N_WALKABLE = 0b100;


enum TileType : uint8_t
{
	Rock = 0,
	Gold = 1,
	Earth = 2,
	Earth_Torch = 3,
	Wall0 = 4,
	Wall1 = 5,
	Wall2 = 6,
	Wall3 = 7,
	Wall4 = 8,
	Wall5 = 9,
	Unclaimed_Path = 10,
	Claimed_Land = 11,
	Lava = 12,
	Water = 13,
	Portal = 14,
	Treasure_Room = 16,
	Library = 18,
	Prison = 20,
	Torture_Room = 22,
	Training_Room = 24,
	Dungeon_Heart = 26,
	Workshop = 28,
	Scavenger_Room = 30,
	Temple = 32,
	Graveyard = 34,
	Hatchery = 36,
	Lair = 38,
	Barracks = 40,
	Wooden_DoorH = 42,
	Wooden_DoorV = 43,
	Braced_DoorH = 44,
	Braced_DoorV = 45,
	Iron_DoorH = 46,
	Iron_DoorV = 47,
	Magic_DoorH = 48,
	Magic_DoorV = 49,
	Bridge = 51,
	Gem = 52,
	Guardpost = 53,

	//not real types currently for future doors unless I think of something better
	Wooden_DoorH_Locked = 54,
	Wooden_DoorV_Locked = 55,
	Braced_DoorH_Locked = 56,
	Braced_DoorV_Locked = 57,
	Iron_DoorH_Locked = 58,
	Iron_DoorV_Locked = 59,
	Magic_DoorH_Locked = 60,
	Magic_DoorV_Locked = 61,

	MAX,
	None = 255,
};



struct Light
{
	uint8_t range = 0;
	uint8_t lightIntensity = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;
};

struct TileTypeAndVariant
{
	TileType type;
	uint8_t variant;
};




static bool IsWall(TileType type)
{
	return type >= TileType::Wall0 && type <= TileType::Wall5;
}
static bool IsWalkable(TileType type)
{
	return (type > TileType::Wall5 && type != TileType::Gem);
}
static bool IsMineable(TileType type)
{
	return (type >= TileType::Gold && type <= TileType::Wall5 || type == TileType::Gem);
}
static bool IsMineableForPlayer(TileType type, PlayerID tileOwner, PlayerID player)
{
	if ((type >= TileType::Gold && type < TileType::Wall0 || type == TileType::Gem))
	{
		return true;
	}
	else if (type >= TileType::Wall0 && type <= TileType::Wall5)
	{
		return tileOwner == player;
	}
	return false;
}
static bool IsClaimableRoom(TileType type)
{
	return ((type >= TileType::Portal && type <= TileType::Barracks || type == TileType::Bridge || type == TileType::Guardpost) && type != TileType::Dungeon_Heart);
}
static uint16_t TypeToTexture(TileType type)
{
	switch (type)
	{
	case TileType::Earth:
		return 1;
		break;
	case TileType::Water:
		return 2;
		break;
	case TileType::Lava:
		return 8;
		break;
	case TileType::Gold:
		return 3;
		break;
	case TileType::Wall0:
	case TileType::Wall1:
	case TileType::Wall2:
	case TileType::Wall3:
	case TileType::Wall4:
	case TileType::Wall5:
		return 6;
		break;
	case TileType::Unclaimed_Path:
		return 4;
		break;
	case TileType::Claimed_Land:
	case TileType::Portal:
		return 5;
		break;
	case TileType::Treasure_Room:
		return 9;
	case TileType::Lair:
		return 10;
	case TileType::Hatchery:
		return 11;
	default:
		return 0;
		break;
	}
}