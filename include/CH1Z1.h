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
#include <include/Vector3.h>

class CH1Z1
{
private:
	static CH1Z1*	_instance;

	DWORD_PTR player;
	DWORD_PTR game;
	CVector3 playerPos;

	HANDLE proc;

public:
	CH1Z1(HANDLE proc);
	~CH1Z1();

	CH1Z1*	GetInstance(HANDLE hProc = NULL) { if(_instance == nullptr) { _instance = new CH1Z1(hProc); } return _instance; };

	void ParseEntities();
	void Process();
	char* GetItemName();

	CVector3 GetEntityDirection(DWORD64 entity);
	bool WorldToScreen(const CVector3& World, CVector3& Out);

	std::string CalculateWorldCompassHeading(float playerHeading);

	/*D3DXVECTOR3& GetMatrixAxis(D3DXMATRIX matrix, UINT i)
	{
		return *(D3DXVECTOR3*)&matrix.m[i][0];
	}*/
};
