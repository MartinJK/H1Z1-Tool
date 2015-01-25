/*****************************************************************************\
Copyright (C) 2013-2014 <martinjk at outloook dot com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
\*****************************************************************************/
#pragma once
#include <d3dx9math.h>

#define H1Z1_VERSION "0.33.1.34511"
#define H1Z1_WINDOW "H1Z1 v" H1Z1_VERSION "  **RELEASE**"
#define H1Z1_DEF_LATEST H1Z1Def::versions::v033134511

#define STATIC_CAST(address) static_cast<DWORD>(address)

namespace H1Z1Def
{
	namespace versions
	{
		enum class v033134511 : UINT64
		{
#pragma message("!--> USE PATTERN - NO STATIC FUNCS!")
			// General game funcs
			cGame = 0x142CB5EA8,
			cGraphic = 0x142CB5C08,

			// Functions
			WorldToScreen = 0x14030B490,

			// Entities
			EntityTableOffset = 0x678,
			_EntityTableOffset = 0x350,

			// Player
			PlayerOffset = 0x11D8,
			PlayerPositionOffset = 0x1D0,
			PlayerHeadingOffset = 0x1F0,

			PlayerWorldState = 0x0028, // BYTE - 1(on foot) - 2(in vehicle)
		};
	};

	enum class EntityTypes : UINT8
	{
		Player = 4,
		Zombie0 = 12,
		OffRoad = 17,
		Deer = 19,
		Wolf = 20,
		Food = 21,
		Armoire = 23,
		FileCabinet = 25,
		CampFire = 27,
		Cabinets1 = 29,
		Cabinets2 = 30,
		Cabinets3 = 35,
		Cabinets = 40,
		ToolCabinet = 49,
		Misc = 44,
		PungiSticks = 46,
		StorageContainer = 51,
		Weapons = 52,
		AnimalTrap = 53,
		PungiSticks2 = 56,
		WaterWell = 62,
		Dressers = 64,
		Wreck0 = 68,
		Wreck1 = 69,
		Wreck2 = 70,
		Shed = 76,
		Door2 = 79,
		Bear = 80,
		Barbeque = 83,
		Rabbit = 85,
		Raven = 89,
		Zombie1 = 91,
		Shed1 = 94,
		Door = 98,
		PickupTruck = 114,
		PoliceCar = 118,
		Door1 = 121,
		SleepingBag = 122
	};

	class CObject                                
	{
	public:
		char _0x0000[464];
		D3DXVECTOR3 pos; //0x01D0 
		char _0x01DC[476];
		char * name; //0x03CC 
		char _0x03D0[320];
		EntityTypes type;
	};
}