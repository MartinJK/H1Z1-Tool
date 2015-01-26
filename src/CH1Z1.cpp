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

D3DXVECTOR3& GetMatrixAxis(D3DXMATRIX matrix, UINT i)
{
	return *(D3DXVECTOR3*)&matrix.m[i][0];
}

CH1Z1::CH1Z1(HANDLE proc) : 
	proc(proc)
{
	if (!proc)
		return;

	// Create basic ptr's
	ReadProcessMemory(this->proc, (void*)(H1Z1_DEF_LATEST::cGame), &game, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(game + STATIC_CAST(H1Z1_DEF_LATEST::PlayerOffset)), &player, sizeof(DWORD64), NULL);
	ReadProcessMemory(this->proc, (void*)(player + STATIC_CAST(H1Z1_DEF_LATEST::PlayerPositionOffset)), &playerPos, sizeof(CVector3), NULL);
	
	// Create player heading line
	D3DXCreateLine(p_Device, &this->line);
	this->line->SetWidth(2);
	this->line->SetPattern(0xffffffff);

	// Create fullscreen map
	D3DXCreateSprite(p_Device, &sprite);

	wchar_t szExePath[MAX_PATH] = { 0 };
	GetModuleFileNameW(GetModuleHandle(NULL), szExePath, MAX_PATH);

	// Fix path in string
	for (size_t i = wcslen(szExePath); i > 0; --i)
	{
		if (szExePath[i] == L'\\')
		{
			szExePath[i + 1] = L'\0';
			break;
		}
	}

	std::wstring loc = szExePath;
	loc.append(L"map.png");

	D3DXCreateTextureFromFile(p_Device, loc.c_str(), &texture);
}

CH1Z1::~CH1Z1()
{
	texture->Release();
	line->Release();
	sprite->Release();
	p_Device->Release();
}

void CH1Z1::ParseEntities()
{
	DrawString("Entities nearby(300m)", 15, 120, 240, 240, 250, pFontSmall);
	DrawString("Players nearby(300m)", 515, 120, 240, 240, 250, pFontSmall);
	DrawString("Objects nearby(300m)", 915, 120, 240, 240, 250, pFontSmall);

	int entityOffset = 150;
	int playerOffset = 150;
	int objectOffset = 150;
	int warningOffset = 15;

	DWORD SizeOfEntity = 0;
	ReadProcessMemory(this->proc, (void*)(game + 0x1020), &SizeOfEntity, sizeof(SizeOfEntity), NULL);

	uint16 _temp = 0;
	DWORD_PTR _obj;
	DWORD64 _ptr;
	
	// Set the current object to localplayer so we parse every entity
	_obj = player;

	// Now parse all
	while (_temp < SizeOfEntity/*-1? so the localplayer will be extracted from the entity list?*/)
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
		// Also disable empty strings and punji sticks/ wire and wood arrows
		if (scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door2
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Door3
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_FireHydrant
			|| scopeobj._name == ""
			|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_AggressiveItems
			|| scopeobj._type == 55 /*unkown obj*/
			|| scopeobj._type == 44 /*wood arrow*/)
			continue;

		// Entity Found
		if (scopeobj._type < 255 && scopeobj._type >= 0)
		{
			char szString[256];
			float fDinstance = (playerPos - scopeobj._position).Length();

			sprintf_s(szString, "- %s type[%d], pos[%.2f, %.2f, %.2f], distance[%.2f]",
				scopeobj._name,
				scopeobj._type,
				scopeobj._position.fX,
				scopeobj._position.fY,
				scopeobj._position.fZ,
				fDinstance);

			// Draw it to the item list (left)

			// Do not draw zombies to the entity list, just add a warning if they're close!
			if (scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Zombie
				|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Wolf
				|| scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Zombie2)
			{
				if (fDinstance < 20.f) 
				{
					DWORD_PTR Graphics;
					ReadProcessMemory(this->proc, (void*)(H1Z1_DEF_LATEST::cGraphic), &Graphics, sizeof(DWORD64), NULL);

					RECT desktop;
					const HWND hDesktop = GetDesktopWindow();
					GetWindowRect(hDesktop, &desktop);

					int ScreenWidth = 0;
					ReadProcessMemory(this->proc, (void*)(Graphics + 0x28), &ScreenWidth, sizeof(ScreenWidth), NULL);

					char szMessage[512];
					sprintf_s(szMessage, "Attention! There\'s a %s close to you!", scopeobj._name);

					DrawString(szMessage, desktop.right-(ScreenWidth/2)-150, warningOffset, 255, 0, 0, pFontSmall);

					warningOffset += 15;

					// Draw the zombie in 3D so the player will see him
					CVector3 _vecScreen;
					bool bResult = this->WorldToScreen(scopeobj._position, _vecScreen);
					if (bResult)
					{
						sprintf_s(szString, ">> -%s- <<", scopeobj._name);

						DrawString(szString, _vecScreen.fX, _vecScreen.fY, 255, 50, 50, pFontSmall);
					}

				}
				continue;
			}

			// Sort out types
			if (scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Player)
				scopeobj._isPlayer = true;

			if (scopeobj._position.fX == 0.f
				&& scopeobj._position.fY == 0.f
				&& scopeobj._position.fZ == 0.f && !scopeobj._isPlayer)
			{
				// Grab the original position
				ReadProcessMemory(proc, (void*)(_obj + 0x1330), &scopeobj._objectPosition, sizeof(CVector3), NULL);
				fDinstance = (playerPos - scopeobj._objectPosition).Length();

				scopeobj._isObject = true;
			}

			if (!scopeobj._isPlayer && !scopeobj._isObject)
				scopeobj._isEntity = true;

			// Draw to list
			if (scopeobj._isObject)
			{
				sprintf_s(szString, "- %s, Type[%d], Position[%.2f, %.2f, %.2f], Distance[%.2fm]",
					scopeobj._name,
					scopeobj._type,
					scopeobj._objectPosition.fX,
					scopeobj._objectPosition.fY,
					scopeobj._objectPosition.fZ,
					fDinstance);

				DrawString(szString, 915, objectOffset, 240, 240, 250, pFontSmaller);
				objectOffset += 15;
			}

			if (scopeobj._isPlayer)
			{
				sprintf_s(szString, "- %s, Position[%.2f, %.2f, %.2f], Distance[%.1fm]",
					scopeobj._name,
					scopeobj._position.fX,
					scopeobj._position.fY,
					scopeobj._position.fZ,
					fDinstance);

				DrawString(szString, 515, playerOffset, 240, 240, 250, pFontSmaller);
				playerOffset += 15;
			}

			if (scopeobj._isEntity)
			{
				sprintf_s(szString, "- %s, Type[%d], Position[%.2f, %.2f, %.2f], Distance[%.2fm]",
					scopeobj._name,
					scopeobj._type,
					scopeobj._position.fX,
					scopeobj._position.fY,
					scopeobj._position.fZ,
					fDinstance);

				DrawString(szString, 15, entityOffset, 240, 240, 250, pFontSmaller);
				entityOffset += 15;
			}


			// Check if he's a player so we draw a big text with a hint
			if (!scopeobj._isObject)
			{
				// Draw it on the screen(World 2 Screen)
				CVector3 _vecScreen;
				bool bResult = this->WorldToScreen(scopeobj._position, _vecScreen);
				
				if (bResult)
				{
					if (scopeobj._isPlayer)
					{
						sprintf_s(szString, "Player: %s  (%2.fm)", scopeobj._name, fDinstance);
						DrawString(szString, _vecScreen.fX, _vecScreen.fY, 240, 240, 250, pFontSmall);
					}
					else
					{
						sprintf_s(szString, "%s  (%2.fm)", scopeobj._name, fDinstance);
						DrawString(szString, _vecScreen.fX, _vecScreen.fY, 240, 240, 250, pFontSmaller);
					}
				}
			}
			else // objects
			{
				// Draw it on the screen(World 2 Screen)
				CVector3 _vecScreen;
				bool bResult = this->WorldToScreen(scopeobj._objectPosition, _vecScreen);

				if (bResult)
				{
					sprintf_s(szString, "%s  (%2.fm)", scopeobj._name, fDinstance);
					DrawString(szString, _vecScreen.fX, _vecScreen.fY, 240, 240, 250, pFontSmaller);
				}

			}

			// Draw to minimap
			RECT desktop;
			const HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);

			auto fWidth = 200;
			auto fHeight = 200;
			auto fX = (desktop.right - 20 - (fWidth/2));
			auto fY = (desktop.bottom - 75 - (fHeight/2));

			// Check if we're a lootable thing or whatever else
			CVector3 diff;
			if (scopeobj._isObject) // Parse objects
				diff = CVector3(scopeobj._objectPosition.fX - playerPos.fX,
					scopeobj._objectPosition.fY - playerPos.fY,
					scopeobj._objectPosition.fZ - playerPos.fZ);
			else // Parse entities
				diff = CVector3(scopeobj._position.fX - playerPos.fX,
					scopeobj._position.fY - playerPos.fY,
					scopeobj._position.fZ - playerPos.fZ);

			if (diff.Length() <= 200)
			{
				if(diff.fX >= 0)
					fX += diff.fX > 95 ? 95 : diff.fX;
				else
					fX += diff.fX < -95 ? -95 : diff.fX;

				if (diff.fY >= 0)
					fY += diff.fY > 95 ? 95 : diff.fY;
				else
					fY += diff.fY < -95 ? -95 : diff.fY;

				if(scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_Player)
					FillRGB(fX, fY, 4, 4, 255, 0, 0, 255);
				else if(scopeobj._type == (int32)H1Z1Def::EntityTypes::TYPE_OffRoader)
					FillRGB(fX, fY, 4, 4, 0, 0, 255, 255);
				else if(scopeobj._isObject) // Lootable objects
					FillRGB(fX, fY, 4, 4, 0, 255, 255, 255);
				else // other entities
					FillRGB(fX, fY, 4, 4, 0, 255, 0, 255);
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
		auto fX = (desktop.right - 20 - fWidth);
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

		// Draw compass
		DrawString("N", fX + (fWidth / 2) - 5, fY - fHeight - 20, 240, 240, 250, pFontSmall);
		DrawString("E", fX - 15 , fY - (fHeight / 2) - 8, 240, 240, 250, pFontSmall);
		DrawString("W", fX + fWidth + 5, fY - (fHeight / 2) - 5, 240, 240, 250, pFontSmall);
		DrawString("S", fX + (fWidth / 2) - 5, fY + 5, 240, 240, 250, pFontSmall);

		// Draw player heading line
#pragma message("FIX CONVERSION!")
		D3DXVECTOR2 points[2];

		points[0] = D3DXVECTOR2((fX + (fWidth / 2) - 1), (fY - (fHeight / 2) - 1));
		points[1] = D3DXVECTOR2(fX + (100 * fHeading) , fY + (100 * fHeading));

		line->Draw(points, 2, 0xffffffff);
	}

	// Prase entities(objects)
	this->ParseEntities();
}

void CH1Z1::DrawFullMap()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	// Draw huge fullscreen map
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	RECT rc = { (desktop.right / 4) / 2, (desktop.bottom / 4) / 2, (desktop.right / 4) * 3, (desktop.bottom / 4) * 3 };
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(32.0f, 32.0f);
	D3DXVECTOR2 trans = D3DXVECTOR2((desktop.right / 2) - desktop.right / 4, (desktop.bottom / 8));
	float rotation = 0.f;
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling(0.4f, 0.4f);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);
	sprite->SetTransform(&mat);
	sprite->Draw(texture, NULL, NULL, NULL, 0xFFFFFFFF);
	sprite->End();

	/*
	// WHOLE WORLD MAP
	int32 einheit = 200 / 6000;

	int32 x = 0;
	if (scopeobj._position.fX < 0)
	x += -(scopeobj._position.fX);
	else
	x += 3000 + scopeobj._position.fX;

	x = (x * einheit);

	int32 y = 0;
	if (scopeobj._position.fY < 0)
	y += 3000 + scopeobj._position.fY;
	else
	y += (3000 - scopeobj._position.fX);

	x = (x * einheit);
	y = (y * einheit);

	fX += x;
	fY += y;

	FillRGB(fX, fY, 4, 4, 0, 255, 0, 255);
	*/
}

char* CH1Z1::GetItemName(DWORD_PTR ptr)
{
	static char itemName[64];

	DWORD64 ItemNamePtr;
	ReadProcessMemory(proc, (void*)(ptr + 0x3B8), &ItemNamePtr, sizeof(DWORD64), NULL);
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