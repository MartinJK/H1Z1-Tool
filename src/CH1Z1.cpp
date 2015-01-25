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
#include <include/CH1Z1.h>
#include <include/D3Keys.h>
#include <include/H1Z1Def.h>

CH1Z1* CH1Z1::_instance = nullptr;

CH1Z1::CH1Z1(HANDLE proc) : 
	proc(proc)
{
	if (!proc)
		return;

	ReadProcessMemory(this->proc, (void*)(H1Z1_DEF_LATEST::cGame), &game, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(game + 0x11D8), &player, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);

	auto test = GetItemName();
}

CH1Z1::~CH1Z1()
{

}

void CH1Z1::ParseEntities()
{
	INT64 addr = reinterpret_cast<INT64>(player);
	while (*(INT64*)(addr + STATIC_CAST(H1Z1_DEF_LATEST::_EntityTableOffset)))
	{
		H1Z1Def::CObject * obj = *(H1Z1Def::CObject**)(addr + STATIC_CAST(H1Z1_DEF_LATEST::_EntityTableOffset));
			
		// Entity Found

		addr = (INT64)obj;
	}

}

void CH1Z1::Process()
{
	
}

char* CH1Z1::GetItemName()
{
	static char itemName[64];

	DWORD64 ItemNamePtr;
	ReadProcessMemory(proc, (void*)(this->player + 0x3B8), &ItemNamePtr, sizeof(DWORD64), NULL);
	ReadProcessMemory(proc, (void*)(ItemNamePtr), &itemName, sizeof(itemName), NULL);

	return itemName;
}

std::string CalculateWordlCompassHeading()
{
	std::string auxHead;
	float playerHeading = 0.f;

	if (playerHeading < 1.9625 && playerHeading > 1.1775) {
		auxHead = "N";
	}
	else if (playerHeading < 1.1775 && playerHeading > 0.3925) {
		auxHead = "NE";
	}
	else if (playerHeading < 0.3925 && playerHeading > -0.3925) {
		auxHead = "E";
	}
	else if (playerHeading < -0.3925 && playerHeading > -1.1775) {
		auxHead = "SE";
	}
	else if (playerHeading < -1.1775 && playerHeading > -1.9625) {
		auxHead = "S";
	}
	else if (playerHeading < -1.9625 && playerHeading > -2.7475) {
		auxHead = "SW";
	}
	else if (playerHeading < -2.7475 && playerHeading > -3.14) {
		auxHead = "W";
	}
	else {
		auxHead = "NW";
	}

	return auxHead;
}