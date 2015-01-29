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
#include <include/CItemSprite.h>
#include <include/CH1Z1.h>

CItemSprite::CItemSprite(const std::string& itemName)
{
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
	loc.append(itemName.begin(), itemName.end());

	D3DXCreateTextureFromFile(p_Device, loc.c_str(), &this->dxTexture);

	D3DXCreateSprite(p_Device, &dxSprite);
}

CItemSprite::~CItemSprite()
{
	this->dxSprite->Release();
	this->dxTexture->Release();
}

bool CItemSprite::SpriteWorldToScreen(CVector3& vecIn, CVector3& vecOut)
{
	return CH1Z1::GetInstance()->WorldToScreen(vecIn, vecOut);
}

float CItemSprite::CalculateDistanceSize(float _fDistance)
{
	auto distance = 10 / _fDistance;

	if (distance < 0.35f)
		distance = 0.35f;
	
	if (distance > 2.5f)
		distance = 2.5f;

	return distance;
}

void CItemSprite::Transform(CVector3& _vecWorldPosition, CVector3& _vecPlayerPosition)
{
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(32.0f, 32.0f);

	CVector3 vecScreenPosition;
	this->bDisplay = this->SpriteWorldToScreen(_vecWorldPosition, vecScreenPosition);
	D3DXVECTOR2 screenPosition = D3DXVECTOR2(vecScreenPosition.fX, vecScreenPosition.fY);

	D3DXMATRIX mat;

	auto __vecScreenPosition = this->CalculateDistanceSize((_vecWorldPosition - _vecPlayerPosition).Length());
	D3DXVECTOR2 scaling(__vecScreenPosition, __vecScreenPosition);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, 0.f, &screenPosition);

	this->dxSprite->SetTransform(&mat);
}

void CItemSprite::Draw(CVector3& vecWorldPosition, CVector3& vecPlayerPosition)
{
	this->dxSprite->Begin(D3DXSPRITE_ALPHABLEND);

	this->Transform(vecWorldPosition, vecPlayerPosition);

	if(this->dxTexture && this->bDisplay)
		this->dxSprite->Draw(this->dxTexture, NULL, NULL, NULL, 0xFFFFFFFF);

	this->dxSprite->End();
}