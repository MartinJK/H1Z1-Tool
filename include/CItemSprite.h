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

#include <Windows.h>
#include <include/Common.h>
#include <include/main.h>
#include <include/D3Draw.h>
#include <include/Vector3.h>

class CItemSprite
{
private:
	std::string				strItemName = "entity.png";
	float					fDistance = 0.f;
	bool					bDisplay = false;
	LPD3DXSPRITE			dxSprite;
	LPDIRECT3DTEXTURE9		dxTexture;

	void Transform( CVector3& _vecWorldPosition, CVector3& _vecPlayerPosition);

public:
	CItemSprite(const std::string& itemName);
	~CItemSprite();

	bool SpriteWorldToScreen(CVector3& vecIn, CVector3& vecOut);
	float CalculateDistanceSize(float _fDistance);

	void SetType(BYTE EntityType);
	void Draw( CVector3& vecWorldPosition, CVector3& vecPlayerPosition);
};