#include "pch.h"
#include "platform.h"

#include "graphics.h"

HFONT _fontList[5];

void Graphics_GetTextExtentPoint32(Graphics* g, const wchar_t* str, int l, GSIZE* pgs)
{
	SIZE s;
	GetTextExtentPoint32(g->_hdc, str, l, &s);

	pgs->cx = s.cx;
	pgs->cy = s.cy;
}

void Graphics_TextOut(Graphics* g, int x, int y, const wchar_t* str, int l)
{
	TextOut(g->_hdc, x, y, str, l);
}

void Graphics_FrameRect_gray(Graphics* g, int left, int top, int right, int bottom)
{
	RECT rc;

	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;

	FrameRect(g->_hdc, &rc, GetStockObject(LTGRAY_BRUSH));
}

void Graphics_FrameRect_black(Graphics* g, int left, int top, int right, int bottom)
{
	RECT rc;

	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;

	FrameRect(g->_hdc, &rc, GetStockObject(BLACK_BRUSH));
}

void Graphics_GetTextMetrics(Graphics* g, GTEXTMETRIC* ptm)
{
	TEXTMETRIC tm;
	GetTextMetrics(g->_hdc, &tm);

	ptm->tmHeight = tm.tmHeight;
	ptm->tmAscent = tm.tmAscent;
	ptm->tmDescent = tm.tmDescent;
	ptm->tmInternalLeading = tm.tmInternalLeading;
	ptm->tmExternalLeading = tm.tmExternalLeading;
	ptm->tmAveCharWidth = tm.tmAveCharWidth;
	ptm->tmMaxCharWidth = tm.tmMaxCharWidth;
	ptm->tmWeight = tm.tmWeight;
	ptm->tmOverhang = tm.tmOverhang;
	ptm->tmDigitizedAspectX = tm.tmDigitizedAspectX;
	ptm->tmDigitizedAspectY = tm.tmDigitizedAspectY;
	ptm->tmFirstChar = tm.tmFirstChar;
	ptm->tmLastChar = tm.tmLastChar;
	ptm->tmDefaultChar = tm.tmDefaultChar;
	ptm->tmBreakChar = tm.tmBreakChar;
	ptm->tmItalic = tm.tmItalic;
	ptm->tmUnderlined = tm.tmUnderlined;
	ptm->tmStruckOut = tm.tmStruckOut;
	ptm->tmPitchAndFamily = tm.tmPitchAndFamily;
	ptm->tmCharSet = tm.tmCharSet;
}

void Graphics_MoveToEx(Graphics* g, int x, int y, void* pv)
{
	MoveToEx(g->_hdc, x, y, (LPPOINT)pv);
}

void Graphics_LineTo(Graphics* g, int x, int y)
{
	LineTo(g->_hdc, x, y);
}

FontHandle Graphics_GetCurrentFont(Graphics* g)
{
	FontHandle fh;

	HGDIOBJ hObj = GetCurrentObject(g->_hdc, OBJ_FONT);
	fh._hfont = hObj;

	return fh;
}

void Graphics_SelectFont(Graphics* g, FontHandle fh)
{
	SelectObject(g->_hdc, fh._hfont);
}

void Graphics_fontList_init(FontHandle hFont)
{
	_fontList[0] = hFont._hfont;

	for (int ix = 1; ix < sizeof(_fontList) / sizeof(_fontList[0]); ++ix)
	{
		LOGFONT logFont;
		GetObject(hFont._hfont, sizeof(LOGFONT), &logFont);
		logFont.lfHeight = (logFont.lfHeight + ix * 5 < 0) ? logFont.lfHeight + ix * 5 : -3;

		_fontList[ix] = CreateFontIndirect(&logFont);
	}
}

void Graphics_fontList_free()
{
	for (int ix = 1; ix < sizeof(_fontList) / sizeof(_fontList[0]); ++ix)
	{

		DeleteObject(_fontList[ix]);
	}
}

FontHandle Graphics_getSmallerFont(FontHandle hFont)
{
	FontHandle fh;
	int ix = 0;

	for (; ix < sizeof(_fontList) / sizeof(_fontList[0]) - 1; ++ix)
	{
		if (hFont._hfont == _fontList[ix])
		{
			fh._hfont = _fontList[ix + 1];
			return fh;
		}
	}

	fh._hfont = _fontList[ix];
	return fh;
}

