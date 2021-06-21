#include "platform.h"


#include "graphics_linux.h"


void Graphics_GetTextExtentPoint32(Graphics* g, const wchar_t* str, int l, GSIZE* pgs)
{
}

void Graphics_TextOut(Graphics* g, int x, int y, const wchar_t* str, int l)
{
}

void Graphics_FrameRect_gray(Graphics* g, int left, int top, int right, int bottom)
{
}

void Graphics_FrameRect_black(Graphics* g, int left, int top, int right, int bottom)
{
}

void Graphics_GetTextMetrics(Graphics* g, GTEXTMETRIC* ptm)
{
}

void Graphics_MoveToEx(Graphics* g, int x, int y, void* pv)
{
}

void Graphics_LineTo(Graphics* g, int x, int y)
{
}

FontHandle Graphics_GetCurrentFont(Graphics* g)
{
	FontHandle fh;
	fh._hfont = NULL;
	return fh;
}

void Graphics_SelectFont(Graphics* g, FontHandle fh)
{
}

void Graphics_fontList_init(FontHandle hFont)
{
}

void Graphics_fontList_free()
{
}

FontHandle Graphics_getSmallerFont(FontHandle hFont)
{
	FontHandle fh;
	fh._hfont = hFont._hfont;
	return fh;
}

