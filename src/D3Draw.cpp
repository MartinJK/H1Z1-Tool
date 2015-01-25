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
// Main subroutines are written by "Grab" - http://www.unknowncheats.me/forum/member171703.html
// Original Overlay: http://www.unknowncheats.me/forum/downloads.php?do=file&id=10166

#include <include/D3Draw.h>

#pragma warning(push)
#pragma warning(disable:4267)

int DrawString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont)
{
	RECT ShadowPos;
	ShadowPos.left = x + 1;
	ShadowPos.top = y + 1;
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	ifont->DrawTextA(0, String, strlen(String), &ShadowPos, DT_NOCLIP, D3DCOLOR_ARGB(255, r / 3, g / 3, b / 3));
	ifont->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));
	return 0;
}

int DrawShadowString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont)
{
	RECT Font;
	Font.left = x;
	Font.top = y;
	RECT Fonts;
	Fonts.left = x + 1;
	Fonts.top = y;
	RECT Fonts1;
	Fonts1.left = x - 1;
	Fonts1.top = y;
	RECT Fonts2;
	Fonts2.left = x;
	Fonts2.top = y + 1;
	RECT Fonts3;
	Fonts3.left = x;
	Fonts3.top = y - 1;
	ifont->DrawTextA(0, String, strlen(String), &Fonts3, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts2, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts1, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));
	return 0;
}
#pragma warning(pop)

void GradientFunc(int x, int y, int w, int h, int r, int g, int b, int a)
{
	int iColorr, iColorg, iColorb;
	for (int i = 1; i < h; i++)
	{
		iColorr = (int)((float)i / h*r);
		iColorg = (int)((float)i / h*g);
		iColorb = (int)((float)i / h*b);
		FillRGB(x, y + i, w, 1, r - iColorr, g - iColorg, b - iColorb, a);
	}
}

void DrawLine(float x, float y, float xx, float yy, int r, int g, int b, int a)
{
	D3DXVECTOR2 dLine[2];

	p_Line->SetWidth(1);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = xx;
	dLine[1].y = yy;

	p_Line->Draw(dLine, 2, D3DCOLOR_ARGB(a, r, g, b));

}

void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a)
{
	D3DXVECTOR2 vLine[2];

	p_Line->SetWidth(w);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	p_Line->Begin();
	p_Line->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
	p_Line->End();
}

void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a)
{
	D3DXVECTOR2 points[5];
	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
	p_Line->SetWidth(1);
	p_Line->Draw(points, 5, D3DCOLOR_RGBA(r, g, b, a));
}

void DrawGUIBox(float x, float y, float w, float h, int r, int g, int b, int a, int rr, int gg, int bb, int aa)
{
	DrawBox(x, y, w, h, 1, r, g, b, a);
	FillRGB(x, y, w, h, rr, gg, bb, a);
}

void DrawHealthBar(float x, float y, float w, float h, int r, int g)
{
	FillRGB(x, y, w, h, r, g, 0, 255);
}

void DrawHealthBarBack(float x, float y, float w, float h)
{
	FillRGB(x, y, w, h, 0, 0, 0, 255);
}

void DrawCenterLine(float x, float y, int width, int height, int r, int g, int b)
{
	D3DXVECTOR2 dPoints[2];
	dPoints[0] = D3DXVECTOR2(x, y);
	dPoints[1] = D3DXVECTOR2(width / 2, height);
	p_Line->SetWidth(width);
	p_Line->Draw(dPoints, 2, D3DCOLOR_RGBA(r, g, b, 255));
}

void GenerateTextures(IDirect3DTexture9 ** pTexture, DWORD dwColour)
{
	// Create the texture
	p_Device->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, pTexture, NULL);

	// Generate the colour
	WORD colour16 = ((WORD)((dwColour >> 28) & 0xF) << 12) |
		(WORD)(((dwColour >> 20) & 0xF) << 8) |
		(WORD)(((dwColour >> 12) & 0xF) << 4) |
		(WORD)(((dwColour >> 4) & 0xF) << 0);

	D3DLOCKED_RECT lockedRect;
	(*pTexture)->LockRect(0, &lockedRect, 0, 0);

	WORD *pDst16 = (WORD*)lockedRect.pBits;
	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;
	(*pTexture)->UnlockRect(0);
}

void DrawPixel(float fX, float fY, unsigned long ulColor)
{
	D3DVERTEX vertex(fX, fY, 0.0f, 1.0f, ulColor);

	p_Device->SetTexture(0, NULL);
	p_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	p_Device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &vertex, sizeof(D3DVERTEX));
}

void DrawLine(float fStartX, float fStartY, float fEndX, float fEndY, unsigned long ulColor)
{
	D3DVERTEX vertex[2];
	vertex[0] = D3DVERTEX(fStartX, fStartY, 0.0f, 1.0f, ulColor);
	vertex[1] = D3DVERTEX(fEndX, fEndY, 0.0f, 1.0f, ulColor);

	p_Device->SetTexture(0, NULL);
	p_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	p_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vertex[0], sizeof(D3DVERTEX));
}

void DrawRect(float fX, float fY, float fWidth, float fHeight, unsigned long ulColor)
{
	D3DVERTEX vertex[4];
	vertex[0] = D3DVERTEX(fX, fY, 0.0f, 1.0f, ulColor);
	vertex[1] = D3DVERTEX((fX + fWidth), fY, 0.0f, 1.0f, ulColor);
	vertex[2] = D3DVERTEX((fX + fWidth), (fY + fHeight), 0.0f, 1.0f, ulColor);
	vertex[3] = D3DVERTEX(fX, (fY + fHeight), 0.0f, 1.0f, ulColor);
	short indices[6] = { 0,1,2,0,2,3 };

	p_Device->SetTexture(0, NULL);
	p_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	p_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0], D3DFMT_INDEX16, &vertex[0], sizeof(D3DVERTEX));
}

IDirect3DTexture9* texture;
class CVertexList
{
public:
	FLOAT x, y, z, rhw;
	DWORD dwColor;
};
void DrawBox(float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColour)
{
	GenerateTextures(&texture, dwColour);

	CVertexList qV[4];

	qV[0].dwColor = qV[1].dwColor = qV[2].dwColor = qV[3].dwColor = dwColour;
	qV[0].z = qV[1].z = qV[2].z = qV[3].z = 0.0f;
	qV[0].rhw = qV[1].rhw = qV[2].rhw = qV[3].rhw = 0.0f;

	qV[0].x = fLeft;
	qV[0].y = (fTop + fHeight);
	qV[1].x = fLeft;
	qV[1].y = fTop;
	qV[2].x = (fLeft + fWidth);
	qV[2].y = (fTop + fHeight);
	qV[3].x = (fLeft + fWidth);
	qV[3].y = fTop;

	const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	p_Device->SetFVF(D3DFVF_TL);
	p_Device->SetTexture(0, texture);
	p_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV, sizeof(CVertexList));
}
