#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h>
#include <stdio.h> // vsprintf
#include "color.h"
#include <stdarg.h>
#include "myClass.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dwmapi.lib")
#define MAX_FONTS 6

enum circle_type { full, half, quarter };

enum text_alignment { lefted, centered, righted };

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

class CDraw
{
public:
	CDraw()
	{
		g_pVB = NULL;
		g_pIB = NULL;
		int FontNr = 0;
	}

	struct sScreen
	{
		float Width;
		float Height;
		float x_center;
		float y_center;
	} Screen;

	ID3DXFont *pFont[MAX_FONTS];

	void Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation);

	//=============================================================================================
	void Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color);

	void Box(float x, float y, float w, float h, float linewidth, DWORD color);
	void BoxFilled(float x, float y, float w, float h, DWORD color);
	void BoxBordered(float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border);
	void BoxRounded(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor);

	void Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color);
	void CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color);

	//void Text(float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor, const char *text);
	void Message(float x, float y, int font, int orientation, const char *text);

	void ESPBox(int x, int y, int w, int h, int thickness, D3DCOLOR Colour);
	void FilledRect(int x, int y, int w, int h, D3DCOLOR color);

	void Text(int x, int y, D3DCOLOR color, int font, const char* text, ...);
	void MyFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void BorderedRect(int x, int y, int w, int h, int thickness, D3DCOLOR colorFilled, D3DCOLOR colorBordered);
	//=============================================================================================

	//=============================================================================================
	bool Font();
	void AddFont(const char* Caption, float size, bool bold, bool italic);
	void FontReset();
	void OnLostDevice();
	//=============================================================================================

	void GetDevice(LPDIRECT3DDEVICE9 pDev) { pDevice = pDev; }
	void Reset();
private:
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DVERTEXBUFFER9 g_pVB;    // Buffer to hold vertices
	LPDIRECT3DINDEXBUFFER9  g_pIB;    // Buffer to hold indices

	int FontNr = 0;
	LPD3DXSPRITE sSprite;
};

extern CDraw Draw;

