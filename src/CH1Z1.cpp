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
#include <include/D3Draw.h>

CH1Z1* CH1Z1::_instance = nullptr;

CH1Z1::CH1Z1(HANDLE proc) : 
	proc(proc)
{
	if (!proc)
		return;

	ReadProcessMemory(this->proc, (void*)(0x142CB5EA8), &game, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(game + STATIC_CAST(H1Z1_DEF_LATEST::PlayerOffset)), &player, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);

	auto test = GetItemName();
}

CH1Z1::~CH1Z1()
{

}

void CH1Z1::ParseEntities()
{
	INT64 addr = player;
	while (*(INT64*)(addr + STATIC_CAST(H1Z1_DEF_LATEST::_EntityTableOffset)))
	{
		H1Z1Def::CObject * obj = *(H1Z1Def::CObject**)(addr + STATIC_CAST(H1Z1_DEF_LATEST::_EntityTableOffset));
			
		// Entity Found

		addr = (INT64)obj;
	}

}

void CH1Z1::Process()
{
	ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);

	char szString[512] = { 0 };
	sprintf_s(szString, "World Position: %2.f, %2.f, %2.f", playerPos.fX, playerPos.fY, playerPos.fZ);
	DrawString(szString, 15, 50, 240, 240, 250, pFontSmaller);
}

char* CH1Z1::GetItemName()
{
	static char itemName[64];

	DWORD64 ItemNamePtr;
	ReadProcessMemory(proc, (void*)(this->player + 0x3B8), &ItemNamePtr, sizeof(DWORD64), NULL);
	ReadProcessMemory(proc, (void*)(ItemNamePtr), &itemName, sizeof(itemName), NULL);

	return itemName;
}

std::string CH1Z1::CalculateWorldCompassHeading()
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

bool CH1Z1::WorldToScreen(const CVector3& World, CVector3& Out)
{
	/*
	DWORD_PTR Graphics = *(DWORD_PTR*)(0x142CD83A8);
	DWORD_PTR Camera = *(DWORD_PTR*)(Graphics + 0x48);
	DWORD_PTR CameraMatrix = *(DWORD_PTR*)(Camera + 0x20);

	CameraMatrix += 0x10;

	D3DXMATRIX Matrix = *(D3DXMATRIX*)(CameraMatrix + 0x1A0);

	int ScreenWidth = *(int*)(Graphics + 0x28);
	int ScreenHeight = *(int*)(Graphics + 0x2C);

	D3DXMatrixTranspose(&Matrix, &Matrix);

	Matrix._21 *= -1;
	Matrix._22 *= -1;
	Matrix._23 *= -1;
	Matrix._24 *= -1;

	float w = GetMatrixAxis(Matrix, 3).Dot(World) + Matrix.m[3][3];

	if (w < 0.098)
		return false;

	float x = GetMatrixAxis(Matrix, 0).Dot(World) + Matrix.m[0][3];
	float y = GetMatrixAxis(Matrix, 1).Dot(World) + Matrix.m[1][3];

	Out.fX = (ScreenWidth / 2) * (1.0 + x / w);
	Out.fY = (ScreenHeight / 2) * (1.0 - y / w);
	*/
	return true;
}

CVector3 CH1Z1::GetEntityDirection(DWORD64 entity)
{
	float fx = static_cast<float>(*(DWORD64 *)entity + 0x1F0);
	float fy = static_cast<float>(*(DWORD64 *)entity + 0x1F4);

	CVector3 r;
	r.fX = sinf(fx) * cosf(fy);
	r.fY = sin(fy);
	r.fZ = cosf(fy) * cosf(fx);

	return r;
}