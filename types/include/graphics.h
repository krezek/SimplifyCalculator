#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct
{
    HFONT _hfont;
} FontHandle;

typedef struct
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    LONG        tmWeight;
    LONG        tmOverhang;
    LONG        tmDigitizedAspectX;
    LONG        tmDigitizedAspectY;
    WCHAR       tmFirstChar;
    WCHAR       tmLastChar;
    WCHAR       tmDefaultChar;
    WCHAR       tmBreakChar;
    BYTE        tmItalic;
    BYTE        tmUnderlined;
    BYTE        tmStruckOut;
    BYTE        tmPitchAndFamily;
    BYTE        tmCharSet;
} GTEXTMETRIC;

typedef struct
{
	LONG cx;
	LONG cy;
} GSIZE;

typedef struct
{
	HDC _hdc;
} Graphics;

void Graphics_GetTextExtentPoint32(Graphics* g, const wchar_t* str, int l, GSIZE* pgs);
void Graphics_TextOut(Graphics* g, int x, int y, const wchar_t* str, int l);
void Graphics_FrameRect_gray(Graphics* g, int left, int top, int right, int bottom);
void Graphics_FrameRect_black(Graphics* g, int left, int top, int right, int bottom);
void Graphics_GetTextMetrics(Graphics* g, GTEXTMETRIC* ptm);
void Graphics_MoveToEx(Graphics* g, int x, int y, void* pv);
void Graphics_LineTo(Graphics* g, int x, int y);
FontHandle Graphics_GetCurrentFont(Graphics* g);
void Graphics_SelectFont(Graphics* g, FontHandle fh);

void Graphics_fontList_init(FontHandle hFont);
void Graphics_fontList_free();
FontHandle Graphics_getSmallerFont(FontHandle hFont);


#endif /* _GRAPHICS_H_ */

