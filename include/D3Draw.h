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

#include <include/D3Overlay.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void GradientFunc(int x, int y, int w, int h, int r, int g, int b, int a);
void DrawCenterLine(float x, float y, int r, int g, int b, int a);
void DrawLine(float x, float y, float xx, float yy, int r, int g, int b, int a);
void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a);
void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a);
void DrawGUIBox(float x, float y, float w, float h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
void DrawHealthBar(float x, float y, float w, float h, int r, int g);
void DrawHealthBarBack(float x, float y, float w, float h);

void GenerateTextures(IDirect3DTexture9 ** pTexture, DWORD dwColour);
void DrawPixel(float fX, float fY, unsigned long ulColor);
void DrawLine(float fStartX, float fStartY, float fEndX, float fEndY, unsigned long ulColor);
void DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor);
void DrawBox(float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColour);


int DrawString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);
int DrawShadowString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont);

struct D3DVERTEX
{
	float fX;
	float fY;
	float fZ;
	float fRHW;
	DWORD dwColor;

	D3DVERTEX()
	{
		fX = 0.0f;
		fY = 0.0f;
		fZ = 0.0f;
		fRHW = 1.0f;
		dwColor = 0;
	}

	D3DVERTEX(float _fX, float _fY, float _fZ, float _fRHW, DWORD _dwColor)
	{
		fX = _fX;
		fY = _fY;
		fZ = _fZ;
		fRHW = _fRHW;
		dwColor = _dwColor;
	}
};