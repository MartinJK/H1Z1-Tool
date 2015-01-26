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
#pragma message("!--> USE PATTERN - NO STATIC OFFSETS! [<include/H1Z1Def.h>]")
		enum class v033134511 : UINT64
		{
			IMAGEBASE = 0x14000000,
			MAX_ENTITY_TYPE = 256, // BYTE

			// General game structures
			CGame = 0x142CB5EA8,
			CGraphic = 0x142CB5C08, // absolute
			LocalPlayerOffset = 0x11D8, // CGame + Offset

			// Functions
			pfnWorldToScreen = 0x14030B490, // absolute

			// Entities
			EntityTableOffset = 0x350,
			EntityPoolCount = 0x1020, // int32/BYTE

			// Player
			CPlayerOffset_Position = 0x1D0, // Vector3
			CPlayerOffset_Heading = 0x1F0, // float
			CPlayerOffset_Pitch = 0x1F4, // float
			CPlayerOffset_State = 0x4F8, // BYTE 
			CPlayerOffset_Velocity = 0x0200, // Vector3
			CPlayerOffset_WorldState = 0x0028, // BYTE

			// Entity
			CEntityOffset_Name = 0x3B8, // char [64]
			CEntityOffset_Position = 0x1D0, // Vector3
			CEntityOffset_Type = 0x500, // int32/BYTE
			CEntityObjectOffset_Position = 0x1330, // Vector3

			// Graphics
			CGraphicsOffset_ScreenWidth = 0x28, // int32
			CGraphicsOffset_ScreenHeight = 0x2C, // int32
			CGraphicsOffset_Camera = 0x48, // CCamera
			CGraphicsOffset_Camera__Matrix = 0x20, // CameraMatrix
			CGraphicsOffset_D3DXMATRIX = 0x1A0, // D3DXMATRIX
		};
	};

	enum class EntityTypes : UINT8
	{
		TYPE_Player = 4,
		TYPE_Zombie = 91,
		TYPE_Zombie2 = 12,

		// Lootables
		TYPE_Dresser = 22,
		TYPE_Armoire = 23,
		TYPE_FileCabinet = 25,
		TYPE_WreckedSedan = 68,
		TYPE_GarbageCan = 38,
		TYPE_Dumpster = 39,
		TYPE_Cabinets = 34,
		TYPE_Cabinets2 = 40,
		TYPE_Cabinets3 = 130,
		TYPE_WreckedTruck = 70,
		TYPE_WreckedVan = 69,
		TYPE_Refrigirator = 37,
		TYPE_MilitaryCache = 147,
		TYPE_ToolCabinet = 49,
		TYPE_Desk = 25,
		TYPE_StorageContainer = 51,
		TYPE_Chest = 66,
		TYPE_LootCache = 127,
		TYPE_Locker = 111,
		TYPE_WeaponsLocker = 112,

		TYPE_WoodenBarricade = 61,

		TYPE_Deer = 19,
		TYPE_Wolf = 20,
		TYPE_Bear = 80,
		TYPE_Rabbit = 85,
		TYPE_Raven = 89,

		TYPE_Food = 21,

		// Disabled since v0.33.1.34511s
		TYPE_GroundTamper = 95,

		TYPE_Furance = 47,
		TYPE_CampFire = 27,
		TYPE_Barbeque = 83,
		TYPE_DewCollector = 54,
		TYPE_FireDrum = 55,

		TYPE_TheMedic_Cache = 152,
		TYPE_TheLoneWolf_Cache = 150,
		TYPE_TheBuilder_Cache = 155,
		TYPE_TheFarmer_Cache = 155,

		TYPE_Trap = 53,
		TYPE_AggressiveItems = 56, // punji sticks, barbed wire

		TYPE_Shed_Wood = 76, // wood
		TYPE_LargeShelter = 95,
		TYPE_Shed_Metal = 94, // metal
		TYPE_BasicShackDoor = 98,
		TYPE_Stash = 109,

		TYPE_Bed = 122,
		TYPE_Misc = 44, // Seems to be a major group like cloths, bandages
		TYPE_Loot = 46,

		TYPE_Door = 25,
		TYPE_Door2 = 124,
		TYPE_Door3 = 121,
		TYPE_FireHydrant = 125,

		TYPE_OffRoader = 17,
		TYPE_PickupTruck = 114,
		TYPE_PoliceCar = 118,

		TYPE_308_ROUND = 134,

		TYPE_Flare = 45,

		TYPE_HandWeapons = 52,
	};

	class CObject                                
	{
	public:
		CVector3 _position;
		CVector3 _objectPosition;
		char _name[64];
		int32 _type;
		bool _isObject = false;
		bool _isEntity = false;
		bool _isPlayer = false;
	};

	class CCameraMatrix
	{
	public:
		char __pad0[0x1B0];
		//Matrix4x4 m_SomeMatrix; // 0x01B0
	};

	class CCamera
	{
	public:
		char __pad0[0x20];
		CCameraMatrix* m_pCameraMatrix; // 0x0020
	};

	class CGraphics
	{
	public:
		char __pad0[40];
		int m_nScreenWidth; // 0x0028
		int m_nScreenHeight; // 0x002C
		char __pad1[24];
		CCamera* m_pCamera; // 0x0048
	};
}