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

D3DXVECTOR3& GetMatrixAxis(D3DXMATRIX matrix, UINT i)
{
	return *(D3DXVECTOR3*)&matrix.m[i][0];
}

CH1Z1::CH1Z1(HANDLE proc) : 
	proc(proc)
{
	if (!proc)
		return;

	ReadProcessMemory(this->proc, (void*)(0x142CB5EA8), &game, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(game + STATIC_CAST(H1Z1_DEF_LATEST::PlayerOffset)), &player, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);
	
	//DWORD addr = player;
	//__debugbreak();

	// seems to be the player name
	auto test = GetItemName();
}

CH1Z1::~CH1Z1()
{

}

void CH1Z1::ParseEntities()
{
	int iOffset = 100;
	DWORD SizeOfEntity = 0;
	ReadProcessMemory(this->proc, (void*)(game + 0x1020), &SizeOfEntity, sizeof(SizeOfEntity), NULL);

	uint16 _temp = 0;
	DWORD_PTR _obj;
	DWORD64 _ptr;
	
	// Set the current object to localplayer so we parse every entity
	_obj = player;

	// Now parse all
	while (_temp < SizeOfEntity)
	{
		// Read entity from memory
		ReadProcessMemory(this->proc, (void*)(_obj + STATIC_CAST(H1Z1_DEF_LATEST::_EntityTableOffset)), &_obj, sizeof(DWORD64), NULL);

		// Generate new entity/object
		H1Z1Def::CObject scopeobj;

		// Set the iteration to the next entity
		_temp += 1;

		ReadProcessMemory(proc, (void*)(_obj + 0x3B8), &_ptr, sizeof(DWORD64), NULL);
		ReadProcessMemory(proc, (void*)(_ptr), &scopeobj._name, sizeof(scopeobj._name), NULL);
		ReadProcessMemory(proc, (void*)(_obj + 0x1D0), &scopeobj._position, sizeof(CVector3), NULL);
		ReadProcessMemory(proc, (void*)(_obj + 0x500), &scopeobj._type, sizeof(int32), NULL);

		// Ignore game designer placed stuff
		if (scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door2
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door3
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_FireHydrant)
			continue;

		// Entity Found
		if (scopeobj._type < 255 && scopeobj._type >= 0)
		{
			char szString[256];
			float fDinstance = (playerPos - scopeobj._position).Length();

			sprintf_s(szString, "Entity %s type[%d], pos[%.2f, %.2f, %.2f], distance[%.2f]",
				scopeobj._name,
				scopeobj._type,
				scopeobj._position.fX,
				scopeobj._position.fY,
				scopeobj._position.fZ,
				fDinstance);

			// Draw it to the item list (left)

			// Do not draw zombies to the entity list, just add a warning if they're close!
			// Also disable empty strings and punji sticks/ wire
			if (scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Zombie
				|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Wolf
				|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Zombie2
				|| scopeobj._name == ""
				|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_AggressiveItems 
				|| scopeobj._type == 55 /*unkown obj*/)
			{
				if (fDinstance < 10.f) 
				{
					DWORD_PTR Graphics;
					ReadProcessMemory(this->proc, (void*)(H1Z1_DEF_LATEST::cGraphic), &Graphics, sizeof(DWORD64), NULL);

					RECT desktop;
					const HWND hDesktop = GetDesktopWindow();
					GetWindowRect(hDesktop, &desktop);

					int ScreenWidth = 0;
					ReadProcessMemory(this->proc, (void*)(Graphics + 0x28), &ScreenWidth, sizeof(ScreenWidth), NULL);

					DrawString("Attention! There\'s a zombie close to you!", desktop.right-(ScreenWidth/2)-150, 15, 255, 0, 0, pFontSmall);
				}
				continue;
			}

			// Check if the coordinates are messed up
			if (scopeobj._position.fX == 0.f
				&& scopeobj._position.fY == 0.f
				&& scopeobj._position.fZ == 0.f)
			{
				sprintf_s(szString, "Entity %s type[%d]",
					scopeobj._name,
					scopeobj._type);

				DrawString(szString, 15, iOffset, 240, 240, 250, pFontSmaller);
				iOffset += 15;

				continue; // do not draw to 3D-2-2D
			}

			DrawString(szString, 15, iOffset, 240, 240, 250, pFontSmaller);
			iOffset += 15;

			// Draw it on the screen(World 2 Screen)
			CVector3 _vecScreen;
			bool bResult = this->WorldToScreen(scopeobj._position, _vecScreen);
			if (bResult)
			{
				sprintf_s(szString, "%s", scopeobj._name);

				DrawString(szString, _vecScreen.fX, _vecScreen.fY, 240, 240, 250, pFontSmaller);
			}
		}
		else
			return;
	}
}

void CH1Z1::Process()
{
	// Global over scrope reachable needed:
	float fHeading = 0.f;

	{
		ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);

		char szString[512] = { 0 };
		sprintf_s(szString, "World Position: %.2f, %.2f, %.2f", playerPos.fX, playerPos.fY, playerPos.fZ);
		DrawString(szString, 15, 50, 240, 240, 250, pFontSmaller);
	}

	{
		ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerHeadingOffset)), &fHeading, sizeof(float), NULL);
		
		auto compass = this->CalculateWorldCompassHeading(fHeading);
		char szString[512] = { 0 };
		sprintf_s(szString, "Heading to %s [%.2f]", compass.c_str(), fHeading);
		DrawString(szString, 15, 65, 240, 240, 250, pFontSmaller);
	}

	{
		// Do not read the playerPos again as we've already read the position this frame
		CVector3 PleasentValley = CVector3(0, 0, -1200);
		
		float fRange = (PleasentValley - playerPos).Length();

		char szString[512] = { 0 };
		sprintf_s(szString, "Distance to Pleasent Valley: %.1f", fRange);
		DrawString(szString, 15, 80, 240, 240, 250, pFontSmaller);
	}

	{
		// Minimap
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		auto fWidth = 200;
		auto fHeight = 200;
		auto fX = (desktop.right - 20 -fWidth);
		auto fY = (desktop.bottom - 75);

		//DrawHealthBarBack(fX, fY, fWidth, fHeight);

		FillRGB(fX, fY - (fHeight / 2), fWidth, 1, 240, 240, 250, 255);
		FillRGB(fX + (fWidth / 2), fY-fHeight, 1, fHeight, 240, 240, 250, 255);

		// Border of map
		// Horicontal
		FillRGB(fX, fY - fHeight, 1, fHeight, 240, 240, 250, 255);
		FillRGB(fX+fWidth, fY - fHeight, 1, fHeight, 240, 240, 250, 255);

		// Vertical
		FillRGB(fX, fY - fHeight, fWidth, 1, 240, 240, 250, 255);
		FillRGB(fX, fY, fWidth, 1, 240, 240, 250, 255);

		// Draw local player pixel
		FillRGB((fX + (fWidth / 2)-1), (fY - (fHeight / 2)-1), 4, 4, 255, 0, 0, 255);

		// Draw heading line
		if(fHeading > 0)
			FillRGB((fX + (fWidth / 2) - 1), (fY - (fHeight / 2) - 1), fHeading*15, fHeading*15, 255, 0, 0, 255);
		/*else
			FillRGB(fX, (fY - fHeight), fHeading * 15, fHeading * 15, 255, 0, 0, 255);*/
	}

	// Prase entities(objects)
	this->ParseEntities();
}

char* CH1Z1::GetItemName()
{
	static char itemName[64];

	DWORD64 ItemNamePtr;
	ReadProcessMemory(proc, (void*)(this->player + 0x3B8), &ItemNamePtr, sizeof(DWORD64), NULL);
	ReadProcessMemory(proc, (void*)(ItemNamePtr), &itemName, sizeof(itemName), NULL);

	return itemName;
}

std::string CH1Z1::CalculateWorldCompassHeading(float playerHeading)
{
	std::string auxHead;
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
	DWORD_PTR Graphics;
	DWORD_PTR Camera;
	DWORD_PTR CameraMatrix;

	ReadProcessMemory(this->proc, (void*)(H1Z1_DEF_LATEST::cGraphic), &Graphics, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(Graphics + 0x48), &Camera, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(Camera + 0x20), &CameraMatrix, sizeof(DWORD64), NULL);

	CameraMatrix += 0x10;

	D3DXMATRIX Matrix;
	ReadProcessMemory(this->proc, (void*)(CameraMatrix + 0x1A0), &Matrix, sizeof(D3DXMATRIX), NULL);
	
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	ReadProcessMemory(this->proc, (void*)(Graphics + 0x28), &ScreenWidth, sizeof(ScreenWidth), NULL);
	ReadProcessMemory(this->proc, (void*)(Graphics + 0x2C), &ScreenHeight, sizeof(ScreenHeight), NULL);

	D3DXMatrixTranspose(&Matrix, &Matrix);

	Matrix._21 *= -1;
	Matrix._22 *= -1;
	Matrix._23 *= -1;
	Matrix._24 *= -1;

	// Convert D3Vector3 to CVector3
	auto _tmp = GetMatrixAxis(Matrix, 3);
	float w = CVector3(_tmp.x, _tmp.y, _tmp.z).Dot(World) + Matrix.m[3][3];

	if (w < 0.098)
		return false;

	_tmp = GetMatrixAxis(Matrix, 0);
	float x = CVector3(_tmp.x, _tmp.y, _tmp.z).Dot(World) + Matrix.m[0][3];

	_tmp = GetMatrixAxis(Matrix, 1);
	float y = CVector3(_tmp.x, _tmp.y, _tmp.z).Dot(World) + Matrix.m[1][3];

	Out.fX = (ScreenWidth / 2) * (1.0 + x / w);
	Out.fY = (ScreenHeight / 2) * (1.0 - y / w);
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