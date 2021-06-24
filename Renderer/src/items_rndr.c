#include "pch.h"
#include "platform.h"

#include "items.h"

int Max(int a, int b)
{
	return (a > b) ? a : b;
}

// Item 

void Item_SetBounds(Item* _this, int x, int y, int width, int height, int baseLine)
{
	_this->_x = x;
	_this->_y = y;
	_this->_width = width;
	_this->_height = height;
	_this->_baseLine = baseLine;
}

void Item_setFont(Item* _this, FontHandle hFont)
{
	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	if (_this->_right)
		_this->_right->_setFontFunc(_this->_right, hFont);
}

int Item_width(Item* _this, Graphics* gfx)
{
	return 10;
}

int Item_height(Item* _this, Graphics* gfx)
{
	return 10;
}

int Item_baseLine(Item* _this, Graphics* gfx)
{
	return 5;
}

void Item_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_SetBounds(_this, x, y,
		_this->_widthFunc(_this, gfx), _this->_heightFunc(_this, gfx),
		_this->_baseLineFunc(_this, gfx));
}

void Item_drawFocusRect(Item* item, Graphics* gfx)
{
	Graphics_FrameRect_gray(gfx, 
		item->_x, 
		item->_y - item->_baseLine, 
		item->_x + item->_width, 
		item->_y - item->_baseLine + item->_height);
}

void Item_draw(Item* _this, Graphics* gfx)
{
	Graphics_FrameRect_black(gfx,
		_this->_x,
		_this->_y - _this->_baseLine,
		_this->_x + _this->_width,
		_this->_y - _this->_baseLine + _this->_height);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

void Item_setFocus(Item* _this, unsigned char b)
{
	_this->_hasFocus = b;
}

unsigned char Item_getFocus(Item* _this)
{
	return _this->_hasFocus;
}

// Literal
int ItemLiteral_width(Item* _this, Graphics* gfx)
{
	ItemLiteral* i = (ItemLiteral*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, i->_str->_str, (int)i->_str->_len, &s);

	return s.cx;
}

int ItemLiteral_height(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	return tm.tmHeight;
}

int ItemLiteral_baseLine(Item* _this, Graphics* gfx)
{
	return _this->_heightFunc(_this, gfx) / 2;
}

void ItemLiteral_draw(Item* _this, Graphics* gfx)
{
	ItemLiteral* i = (ItemLiteral*) _this;
	
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, i->_str->_str, (int)i->_str->_len);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Number

int ItemNumber_width(Item* _this, Graphics* gfx)
{
	ItemNumber* i = (ItemNumber*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, i->_str->_str, (int)i->_str->_len, &s); 
	
	return s.cx;
}

int ItemNumber_height(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	return tm.tmHeight;
}

int ItemNumber_baseLine(Item* _this, Graphics* gfx)
{
	ItemNumber* i = (ItemNumber*)_this;

	return i->_item._heightFunc(_this, gfx) / 2;
}

void ItemNumber_draw(Item* _this, Graphics* gfx)
{
	ItemNumber* i = (ItemNumber*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, i->_str->_str, (int)i->_str->_len);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Symbol

int ItemSymbol_width(Item* _this, Graphics* gfx)
{
	ItemSymbol* i = (ItemSymbol*)_this;

	wchar_t str[2];
	str[0] = i->_ch;
	str[1] = 0;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);

	return s.cx;
}

int ItemSymbol_height(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	return tm.tmHeight;
}

int ItemSymbol_baseLine(Item* _this, Graphics* gfx)
{
	ItemSymbol* i = (ItemSymbol*)_this;

	return i->_item._heightFunc(_this, gfx) / 2;
}

void ItemSymbol_draw(Item* _this, Graphics* gfx)
{
	ItemSymbol* i = (ItemSymbol*)_this;

	wchar_t str[2];
	str[0] = i->_ch;
	str[1] = 0;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, str, 1);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// List 

int ItemList_width(Item* _this, Graphics* gfx)
{
	ItemList* i = (ItemList*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L",", 1, &s);
	
	return _this->_left->_widthFunc(_this->_left, gfx) +
		s.cx +
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemList_height(Item* _this, Graphics* gfx)
{
	ItemList* i = (ItemList*)_this;

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);
	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	return Max(lb, rb) + Max(lh - lb, rh - rb);
}

int ItemList_baseLine(Item* _this, Graphics* gfx)
{
	ItemList* i = (ItemList*)_this;

	return Max(_this->_left->_baseLineFunc(_this->_left, gfx),
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemList_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L",", 1, &s);

	int x0 = x;
	int y0 = y;

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	x0 += _this->_left->_width;
	x0 += s.cx;
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0, y0);
}

void ItemList_draw(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	_this->_left->_drawFunc(_this->_left, gfx);
	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, y0 - tm.tmHeight / 2, L",", 1);
	_this->_right->_drawFunc(_this->_right, gfx);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Equ

int ItemEqu_width(Item* _this, Graphics* gfx)
{
	ItemEqu* i = (ItemEqu*)_this;

	wchar_t str[2] = { i->_sy, 0 };

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);
	
	return _this->_left->_widthFunc(_this->_left, gfx) +
		s.cx + 
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemEqu_height(Item* _this, Graphics* gfx)
{
	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);
	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	return Max(lb, rb) + Max(lh - lb, rh - rb);
}

int ItemEqu_baseLine(Item* _this, Graphics* gfx)
{
	return Max(_this->_left->_baseLineFunc(_this->_left, gfx),
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemEqu_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	ItemEqu* i = (ItemEqu*)_this;

	wchar_t str[2] = { i->_sy, 0 };

	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);

	int x0 = x;
	int y0 = y;

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x, y);
	x0 += _this->_left->_width;
	x0 += s.cx;
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0, y0);
}

void ItemEqu_draw(Item* _this, Graphics* gfx)
{
	ItemEqu* i = (ItemEqu*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	wchar_t str[2] = { i->_sy, 0 };

	int x0 = _this->_x;
	int y0 = _this->_y;

	_this->_left->_drawFunc(_this->_left, gfx);
	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, _this->_y - tm.tmHeight / 2, str, 1);
	_this->_right->_drawFunc(_this->_right, gfx);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Add

int ItemAdd_width(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"+", 1, &s);
	
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);
	
	int w = 0;

	return _this->_left->_widthFunc(_this->_left, gfx) + 
		s.cx + w +
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemAdd_height(Item* _this, Graphics* gfx)
{
	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);
	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	return Max(lb, rb) + Max(lh - lb, rh - rb);
}

int ItemAdd_baseLine(Item* _this, Graphics* gfx)
{
	return Max(_this->_left->_baseLineFunc(_this->_left, gfx), 
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemAdd_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"+", 1, &s);

	int x0 = _this->_x;
	int y0 = _this->_y;

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	x0 += _this->_left->_width;
	x0 += s.cx;
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0, y0);
}

void ItemAdd_draw(Item* _this, Graphics* gfx)
{
	ItemAdd* i = (ItemAdd*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"+", 1, &s);

	int x0 = _this->_x;
	int y0 = _this->_y;

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, y0 - tm.tmHeight / 2, L"+", 1);

	// draw right nodes
	{
		_this->_right->_drawFunc(_this->_right, gfx);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Sub

int ItemSub_width(Item* _this, Graphics* gfx)
{
	ItemSub* i = (ItemSub*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"-", 1, &s);
	
	int w = 0;

	return _this->_left->_widthFunc(_this->_left, gfx) + 
		s.cx + w +
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemSub_height(Item* _this, Graphics* gfx)
{
	ItemSub* i = (ItemSub*)_this;

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);
	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	return Max(lb, rb) + Max(lh - lb, rh - rb);
}

int ItemSub_baseLine(Item* _this, Graphics* gfx)
{
	ItemSub* i = (ItemSub*)_this;

	return Max(_this->_left->_baseLineFunc(_this->_left, gfx),
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemSub_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"-", 1, &s);

	int x0 = x;
	int y0 = y;

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	x0 += _this->_left->_width;	
	x0 += s.cx;
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0, y0);
}

void ItemSub_draw(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, _this->_y - tm.tmHeight / 2, L"-", 1);

	// draw right nodes
	{
		_this->_right->_drawFunc(_this->_right, gfx);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Mult

int ItemMult_width(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u22C5", 1, &s);
	
	int w = 0;

	return _this->_left->_widthFunc(_this->_left, gfx) + 
		s.cx + w +
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemMult_height(Item* _this, Graphics* gfx)
{
	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);
	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	return Max(lb, rb) + Max(lh - lb, rh - rb);
}

int ItemMult_baseLine(Item* _this, Graphics* gfx)
{
	return Max(_this->_left->_baseLineFunc(_this->_left, gfx),
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemMult_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u22C5", 1, &s);

	int x0 = x;
	int y0 = y;

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	x0 += _this->_left->_width;
	x0 += s.cx;
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0, y0);
}

void ItemMult_draw(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, _this->_y - tm.tmHeight / 2, L"\u22C5", 1);

	// draw right nodes
	{
		_this->_right->_drawFunc(_this->_right, gfx);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Frac

int ItemFrac_width(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2015", 1, &s);
	
	int w = 0;

	return s.cx + w + Max(_this->_left->_widthFunc(_this->_left, gfx),
		_this->_right->_widthFunc(_this->_right, gfx));
}

int ItemFrac_height(Item* _this, Graphics* gfx)
{
	ItemFrac* i = (ItemFrac*)_this;

	return _this->_left->_heightFunc(_this->_left, gfx) + 
		_this->_right->_heightFunc(_this->_right, gfx) + 
		i->_fHeight;
}

int ItemFrac_baseLine(Item* _this, Graphics* gfx)
{
	ItemFrac* i = (ItemFrac*)_this;

	return _this->_left->_heightFunc(_this->_left, gfx) + i->_fHeight / 2;
}

void ItemFrac_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemFrac* i = (ItemFrac*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2015", 1, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = x;
	int y0 = y;

	int width = s.cx + Max(_this->_left->_widthFunc(_this->_left, gfx),
		_this->_right->_widthFunc(_this->_right, gfx));

	int x1 = 0, x2 = 0;
	if (_this->_left->_widthFunc(_this->_left, gfx) >
		_this->_right->_widthFunc(_this->_right, gfx))
	{
		x1 = 0;
		x2 = _this->_left->_widthFunc(_this->_left, gfx) -
			_this->_right->_widthFunc(_this->_right, gfx);
	}
	else
	{
		x1 = _this->_right->_widthFunc(_this->_right, gfx) -
			_this->_left->_widthFunc(_this->_left, gfx);
		x2 = 0;
	}

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0 + tm.tmHeight / 2 + x1 / 2,
		y0 - _this->_left->_heightFunc(_this->_left, gfx) - i->_fHeight / 2 + _this->_left->_baseLineFunc(_this->_left, gfx));
	_this->_right->_calcCoordinateFunc(_this->_right, gfx, x0 + tm.tmHeight / 2 + x2 / 2,
		y0 + i->_fHeight / 2 + 1 + _this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemFrac_draw(Item* _this, Graphics* gfx)
{
	ItemFrac* i = (ItemFrac*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2015", 1, &s);
	
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	int width = s.cx + Max(_this->_left->_width,	_this->_right->_width);

	// draw fraction line
	Graphics_MoveToEx(gfx, _this->_x, _this->_y + tm.tmDescent - tm.tmInternalLeading, NULL);
	Graphics_LineTo(gfx, _this->_x + width, _this->_y + tm.tmDescent - tm.tmInternalLeading);

	// draw left and right
	_this->_left->_drawFunc(_this->_left, gfx);
	_this->_right->_drawFunc(_this->_right, gfx);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Sign

int ItemSign_width(Item* _this, Graphics* gfx)
{
	ItemSign* i = (ItemSign*)_this;

	wchar_t str[2];
	str[0] = i->_sgn;
	str[1] = 0;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);
	
	return s.cx + _this->_left->_widthFunc(_this->_left, gfx);
}

int ItemSign_height(Item* _this, Graphics* gfx)
{
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	return Max(_this->_left->_heightFunc(_this->_left, gfx), tm.tmHeight);
}

int ItemSign_baseLine(Item* _this, Graphics* gfx)
{
	return _this->_left->_baseLineFunc(_this->_left, gfx);
}

void ItemSign_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemSign* i = (ItemSign*)_this;

	wchar_t str[2];
	str[0] = i->_sgn;
	str[1] = 0;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = x;
	int y0 = y;

	x0 += s.cx;
	
	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
}

void ItemSign_draw(Item* _this, Graphics* gfx)
{
	ItemSign* i = (ItemSign*)_this;

	wchar_t str[2];
	str[0] = i->_sgn;
	str[1] = 0;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, str, 1, &s);
	
	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, str, 1);
	
	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Sqrt

int ItemSqrt_width(Item* _this, Graphics* gfx)
{
	ItemSqrt* i = (ItemSqrt*)_this;

	return _this->_left->_widthFunc(_this->_left, gfx) + i->_sPadding;
}

int ItemSqrt_height(Item* _this, Graphics* gfx)
{
	ItemSqrt* i = (ItemSqrt*)_this;

	return _this->_left->_heightFunc(_this->_left, gfx) + i->_sHeight;
}

int ItemSqrt_baseLine(Item* _this, Graphics* gfx)
{
	return _this->_left->_baseLineFunc(_this->_left, gfx);
}

void ItemSqrt_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	ItemSqrt* i = (ItemSqrt*)_this;

	Item_calcCoordinate(_this, gfx, x, y);

	_this->_left->_calcCoordinateFunc(_this->_left, gfx, x + i->_sPadding / 2, y);
}

void ItemSqrt_draw(Item* _this, Graphics* gfx)
{
	ItemSqrt* i = (ItemSqrt*)_this;

	_this->_left->_drawFunc(_this->_left, gfx);

	Graphics_MoveToEx(gfx, _this->_x + i->_sPadding / 4, 
		_this->_y + _this->_heightFunc(_this, gfx) - _this->_baseLineFunc(_this, gfx) - i->_sHeight / 2, NULL);
	Graphics_LineTo(gfx, _this->_x, 
		_this->_y + _this->_heightFunc(_this, gfx) - _this->_baseLineFunc(_this, gfx) - i->_sHeight / 2 - _this->_heightFunc(_this, gfx) / 2);
	Graphics_MoveToEx(gfx, _this->_x + i->_sPadding / 4, 
		_this->_y + _this->_heightFunc(_this, gfx) - _this->_baseLineFunc(_this, gfx) - i->_sHeight / 2, NULL);
	Graphics_LineTo(gfx, _this->_x + i->_sPadding / 2, 
		_this->_y - _this->_baseLineFunc(_this, gfx) - i->_sHeight / 2);
	Graphics_LineTo(gfx, _this->_x + _this->_widthFunc(_this, gfx), 
		_this->_y - _this->_left->_baseLineFunc(_this->_left, gfx) - i->_sHeight / 2);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Pow

void ItemPow_setFont(Item* _this, FontHandle hFont)
{
	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	FontHandle smallFont = Graphics_getSmallerFont(hFont);	

	if (_this->_right)
		_this->_right->_setFontFunc(_this->_right, smallFont);
}

int ItemPow_width(Item* _this, Graphics* gfx)
{
	ItemPow* i = (ItemPow*)_this;

	int lw = _this->_left->_widthFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int rw = _this->_right->_widthFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);
	
	return lw + rw;
}

int ItemPow_height(Item* _this, Graphics* gfx)
{
	ItemPow* i = (ItemPow*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	int rh = _this->_right->_heightFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lb, rh + tm1.tmHeight / 2 - tm1.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent) +
		lh - lb;
}

int ItemPow_baseLine(Item* _this, Graphics* gfx)
{
	ItemPow* i = (ItemPow*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	int rh = _this->_right->_heightFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lb, tm1.tmHeight / 2 + rh - tm1.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent);
}

void ItemPow_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemPow* i = (ItemPow*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	int xPos = x;

	// draw left nodes
	{
		_this->_left->_calcCoordinateFunc(_this->_left, gfx, xPos, y);
		xPos += _this->_left->_width;
	}

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	_this->_right->_calcCoordinateFunc(_this->_right, gfx,
		xPos,
		y - tm1.tmHeight / 2 - rh + rb + tm1.tmInternalLeading + tm2.tmInternalLeading + tm2.tmDescent);

	Graphics_SelectFont(gfx, hObj);
}

void ItemPow_draw(Item* _this, Graphics* gfx)
{
	ItemPow* i = (ItemPow*)_this;

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	_this->_right->_drawFunc(_this->_right, gfx);
	
	Graphics_SelectFont(gfx, hObj);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Subscript

void ItemSubscript_setFont(Item* _this, FontHandle hFont)
{
	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	FontHandle smallFont = Graphics_getSmallerFont(hFont);

	if (_this->_right)
		_this->_right->_setFontFunc(_this->_right, smallFont);
}

int ItemSubscript_width(Item* _this, Graphics* gfx)
{
	ItemSubscript* i = (ItemSubscript*)_this;

	int lw = _this->_left->_widthFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));
	int rw = _this->_right->_widthFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	return lw + rw;
}

int ItemSubscript_height(Item* _this, Graphics* gfx)
{
	ItemSubscript* i = (ItemSubscript*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	int rh = _this->_right->_heightFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lh - lb, rh + tm1.tmHeight / 2 - tm1.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent) +
		lb;
}

int ItemSubscript_baseLine(Item* _this, Graphics* gfx)
{
	ItemSubscript* i = (ItemSubscript*)_this;

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int rh = _this->_right->_heightFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	return lb;
}

void ItemSubscript_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemSubscript* i = (ItemSubscript*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	int xPos = x;

	// draw left nodes
	{
		_this->_left->_calcCoordinateFunc(_this->_left, gfx, xPos, y);
		xPos += _this->_left->_width;
	}

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	int rh = _this->_right->_heightFunc(_this->_right, gfx);
	int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

	_this->_right->_calcCoordinateFunc(_this->_right, gfx,
		xPos,
		y + tm1.tmHeight / 2 + rb - tm1.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent);

	Graphics_SelectFont(gfx, hObj);
}

void ItemSubscript_draw(Item* _this, Graphics* gfx)
{
	ItemSubscript* i = (ItemSubscript*)_this;

	GTEXTMETRIC tm1;
	Graphics_GetTextMetrics(gfx, &tm1);

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	GTEXTMETRIC tm2;
	Graphics_GetTextMetrics(gfx, &tm2);

	_this->_right->_drawFunc(_this->_right, gfx);

	Graphics_SelectFont(gfx, hObj);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// CommFunc

int ItemCommFunc_width(Item* _this, Graphics* gfx)
{
	ItemCommFunc* i = (ItemCommFunc*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, i->_str, (int)i->_len, &s);

	GSIZE s2;
	Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s2);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int rw = 0;

	if (_this->_right)
	{
		rw = _this->_right->_widthFunc(_this->_right, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	return rw + s2.cx * 2 + _this->_left->_widthFunc(_this->_left, gfx) + s.cx;
}

int ItemCommFunc_height(Item* _this, Graphics* gfx)
{
	ItemCommFunc* i = (ItemCommFunc*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int rh = 0, rb = 0;

	if (_this->_right)
	{
		FontHandle hObj = Graphics_GetCurrentFont(gfx);
		Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

		GTEXTMETRIC tm2;
		Graphics_GetTextMetrics(gfx, &tm2);

		rh = _this->_right->_heightFunc(_this->_right, gfx);
		rb = _this->_right->_baseLineFunc(_this->_right, gfx);

		rh = rh - tm.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent;

		Graphics_SelectFont(gfx, hObj);
	}

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	return Max(rh + tm.tmHeight / 2, lb) + Max(tm.tmHeight / 2, lh - lb);
}

int ItemCommFunc_baseLine(Item* _this, Graphics* gfx)
{
	ItemCommFunc* i = (ItemCommFunc*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int rh = 0;

	if (_this->_right)
	{
		FontHandle hObj = Graphics_GetCurrentFont(gfx);
		Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

		GTEXTMETRIC tm2;
		Graphics_GetTextMetrics(gfx, &tm2);

		rh = _this->_right->_heightFunc(_this->_right, gfx);

		rh = rh - tm.tmInternalLeading - tm2.tmInternalLeading - tm2.tmDescent;

		Graphics_SelectFont(gfx, hObj);
	}

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	return Max(rh + tm.tmHeight / 2, lb);
}

void ItemCommFunc_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemCommFunc* i = (ItemCommFunc*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, i->_str, (int)i->_len, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = x;
	int y0 = y;

	x0 += s.cx;

	if (_this->_right) 
	{
		FontHandle hObj = Graphics_GetCurrentFont(gfx);
		Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

		GTEXTMETRIC tm2;
		Graphics_GetTextMetrics(gfx, &tm2);

		int rh = _this->_right->_heightFunc(_this->_right, gfx);
		int rb = _this->_right->_baseLineFunc(_this->_right, gfx);

		_this->_right->_calcCoordinateFunc(_this->_right, gfx,
			x0,
			y - s.cy / 2 - rh + rb + tm.tmInternalLeading + tm2.tmInternalLeading + tm2.tmDescent);
		x0 += _this->_right->_widthFunc(_this->_right, gfx);

		Graphics_SelectFont(gfx, hObj);
	}

	// draw left nodes
	{
		GSIZE s2;
		Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s2);

		x0 += s2.cx;

		_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	}
}

void ItemCommFunc_draw(Item* _this, Graphics* gfx)
{
	ItemCommFunc* i = (ItemCommFunc*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, i->_str, (int)i->_len, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, i->_str, (int)i->_len);

	x0 += s.cx;

	if(_this->_right)
	{
		FontHandle hObj = Graphics_GetCurrentFont(gfx);
		Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

		_this->_right->_drawFunc(_this->_right, gfx);

		Graphics_SelectFont(gfx, hObj);
		x0 += _this->_right->_width;
	}

	// draw left nodes
	{
		GSIZE s2;
		Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s2);

		Graphics_TextOut(gfx, x0, y0 - s2.cy / 2, L"(", 1);
		x0 += s2.cx;

		_this->_left->_drawFunc(_this->_left, gfx);
		x0 += _this->_left->_width;

		Graphics_TextOut(gfx, x0, y0 - s2.cy / 2, L")", 1);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Factorial

int ItemFactorial_width(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"!", 1, &s);

	return _this->_left->_widthFunc(_this->_left, gfx) + s.cx;
}

int ItemFactorial_height(Item* _this, Graphics* gfx)
{
	return _this->_left->_heightFunc(_this->_left, gfx);
}

int ItemFactorial_baseLine(Item* _this, Graphics* gfx)
{
	return _this->_left->_baseLineFunc(_this->_left, gfx);
}

void ItemFactorial_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"!", 1, &s);

	// draw left nodes
	{
		_this->_left->_calcCoordinateFunc(_this->_left, gfx, x, y);
	}
}

void ItemFactorial_draw(Item* _this, Graphics* gfx)
{
	ItemFactorial* i = (ItemFactorial*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"!", 1, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	Graphics_TextOut(gfx, _this->_x + _this->_left->_width, _this->_y - tm.tmHeight / 2, L"!", 1);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Parentheses

int ItemParentheses_width(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s);

	return _this->_left->_widthFunc(_this->_left, gfx) + s.cx * 2;
}

int ItemParentheses_height(Item* _this, Graphics* gfx)
{
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s);

	return Max(_this->_left->_heightFunc(_this->_left, gfx), s.cy);
}

int ItemParentheses_baseLine(Item* _this, Graphics* gfx)
{
	return _this->_left->_baseLineFunc(_this->_left, gfx);
}

void ItemParentheses_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = x;
	int y0 = y;

	x0 += s.cx;

	// draw left nodes
	{
		_this->_left->_calcCoordinateFunc(_this->_left, gfx, x0, y0);
	}
}

void ItemParentheses_draw(Item* _this, Graphics* gfx)
{
	ItemParentheses* i = (ItemParentheses*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"(", 1, &s);

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm);

	int x0 = _this->_x;
	int y0 = _this->_y;

	Graphics_TextOut(gfx, _this->_x, _this->_y - tm.tmHeight / 2, L"(", 1);

	// draw left nodes
	{
		_this->_left->_drawFunc(_this->_left, gfx);
	}

	Graphics_TextOut(gfx, _this->_x + s.cx + _this->_left->_width, _this->_y - tm.tmHeight / 2, L")", 1);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Sigma

void ItemSigma_setFont(Item* _this, FontHandle hFont)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	FontHandle smallFont = Graphics_getSmallerFont(hFont);

	if (i->_bottom)
		i->_bottom->_setFontFunc(i->_bottom, smallFont);

	if (i->_top)
		i->_top->_setFontFunc(i->_top, smallFont);
}

int ItemSigma_width(Item* _this, Graphics* gfx)
{
	ItemSigma* i = (ItemSigma*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2211", 1, &s);

	int lw = _this->_left->_widthFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int bw = 0, tw = 0;

	if (i->_bottom)
		bw = i->_bottom->_widthFunc(i->_bottom, gfx);

	if (i->_top)
		tw = i->_top->_widthFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	return lw + Max(s.cx, Max(bw, tw));
}

int ItemSigma_height(Item* _this, Graphics* gfx)
{
	ItemSigma* i = (ItemSigma*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2211", 1, &s);

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int bh = 0, th = 0;

	if (i->_bottom)
		bh = i->_bottom->_heightFunc(i->_bottom, gfx);

	if (i->_top)
		th = i->_top->_heightFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lb, th + s.cy / 2) + Max(lh - lb, bh + s.cy / 2);
}

int ItemSigma_baseLine(Item* _this, Graphics* gfx)
{
	ItemSigma* i = (ItemSigma*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2211", 1, &s);

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int th = 0;

	if (i->_top)
		th = i->_top->_heightFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(th + s.cy / 2, lb);
}

void ItemSigma_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemSigma* i = (ItemSigma*)_this; 
	
	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2211", 1, &s);

	int x0 = x;
	int y0 = y;

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int bw = 0, tw = 0;

	if (i->_bottom)
	{
		int bb = i->_bottom->_baseLineFunc(i->_bottom, gfx);
		i->_bottom->_calcCoordinateFunc(i->_bottom, gfx, x0, y0 + s.cy / 2 + bb);
		bw = i->_bottom->_widthFunc(i->_bottom, gfx);
	}

	if (i->_top)
	{
		int tb = i->_top->_baseLineFunc(i->_top, gfx);
		int th = i->_top->_heightFunc(i->_top, gfx);
		i->_top->_calcCoordinateFunc(i->_top, gfx, x0, y0 - s.cy / 2 - th + tb);
		tw = i->_top->_widthFunc(i->_top, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	i->_item._left->_calcCoordinateFunc(i->_item._left, gfx, x0 + Max(s.cx, Max(tw, bw)), y0);
}

void ItemSigma_draw(Item* _this, Graphics* gfx)
{
	ItemSigma* i = (ItemSigma*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u2211", 1, &s);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	Graphics_TextOut(gfx, _this->_x, _this->_y - s.cy / 2, L"\u2211", 1);

	if (i->_bottom)
	{
		i->_bottom->_drawFunc(i->_bottom, gfx);
	}

	if (i->_top)
	{
		i->_top->_drawFunc(i->_top, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	i->_item._left->_drawFunc(i->_item._left, gfx);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Integrate

void ItemIntegrate_setFont(Item* _this, FontHandle hFont)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	if (_this->_right)
		_this->_right->_setFontFunc(_this->_right, hFont);

	FontHandle smallFont = Graphics_getSmallerFont(hFont);

	if (i->_bottom)
		i->_bottom->_setFontFunc(i->_bottom, smallFont);

	if (i->_top)
		i->_top->_setFontFunc(i->_top, smallFont);
}

int ItemIntegrate_width(Item* _this, Graphics* gfx)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u222B", 1, &s);

	int lw = _this->_left->_widthFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int rw = 0, bw = 0, tw = 0;

	if (i->_bottom)
		bw = i->_bottom->_widthFunc(i->_bottom, gfx);

	if (i->_top)
		tw = i->_top->_widthFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	if (i->_item._right)
		rw = i->_item._right->_widthFunc(i->_item._right, gfx);

	return rw + lw + Max(s.cx, Max(bw, tw));
}

int ItemIntegrate_height(Item* _this, Graphics* gfx)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u222B", 1, &s);

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int bh = 0, th = 0;

	if (i->_bottom)
		bh = i->_bottom->_heightFunc(i->_bottom, gfx);

	if (i->_top)
		th = i->_top->_heightFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lb, th + s.cy / 2) + Max(lh - lb, bh + s.cy / 2);
}

int ItemIntegrate_baseLine(Item* _this, Graphics* gfx)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u222B", 1, &s);

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int th = 0;

	if (i->_top)
		th = i->_top->_heightFunc(i->_top, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(th + s.cy / 2, lb);
}

void ItemIntegrate_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemIntegrate* i = (ItemIntegrate*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u222B", 1, &s);

	int x0 = x;
	int y0 = y;

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int bw = 0, tw = 0;

	if (i->_bottom)
	{
		int bb = i->_bottom->_baseLineFunc(i->_bottom, gfx);
		i->_bottom->_calcCoordinateFunc(i->_bottom, gfx, x0, y0 + s.cy / 2 + bb);
		bw = i->_bottom->_widthFunc(i->_bottom, gfx);
	}

	if (i->_top)
	{
		int tb = i->_top->_baseLineFunc(i->_top, gfx);
		int th = i->_top->_heightFunc(i->_top, gfx);
		i->_top->_calcCoordinateFunc(i->_top, gfx, x0, y0 - s.cy / 2 - th + tb);
		tw = i->_top->_widthFunc(i->_top, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	int xpos = x0 + Max(s.cx, Max(tw, bw));

	i->_item._left->_calcCoordinateFunc(i->_item._left, gfx, xpos, y0);

	if (i->_item._right)
	{
		xpos += i->_item._left->_widthFunc(i->_item._left, gfx);
		Item_calcCoordinate(i->_item._right, gfx, xpos, y0);
	}
}

void ItemIntegrate_draw(Item* _this, Graphics* gfx)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"\u222B", 1, &s);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	Graphics_TextOut(gfx, _this->_x, _this->_y - s.cy / 2, L"\u222B", 1);

	if (i->_bottom)
	{
		i->_bottom->_drawFunc(i->_bottom, gfx);
	}

	if (i->_top)
	{
		i->_top->_drawFunc(i->_top, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	i->_item._left->_drawFunc(i->_item._left, gfx);

	if (i->_item._right)
	{
		i->_item._right->_drawFunc(i->_item._right, gfx);
	}

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

// Derivative

int ItemDerivative_width(Item* _this, Graphics* gfx)
{
	ItemDerivative* i = (ItemDerivative*)_this;

	return _this->_left->_widthFunc(_this->_left, gfx) + 
		_this->_right->_widthFunc(_this->_right, gfx);
}

int ItemDerivative_height(Item* _this, Graphics* gfx)
{
	ItemDerivative* i = (ItemDerivative*)_this;

	return Max(_this->_left->_heightFunc(_this->_left, gfx),
		_this->_right->_heightFunc(_this->_right, gfx));
}

int ItemDerivative_baseLine(Item* _this, Graphics* gfx)
{
	ItemDerivative* i = (ItemDerivative*)_this;

	return Max(_this->_left->_baseLineFunc(_this->_left, gfx),
		_this->_right->_baseLineFunc(_this->_right, gfx));
}

void ItemDerivative_draw(Item* _this, Graphics* gfx)
{
	/*ItemDerivative* i = (ItemDerivative*)_this;

	GTEXTMETRIC tm;
	Graphics_GetTextMetrics(gfx, &tm); 
	
	int x0 = x;
	int y0 = y;

	_this->_right->_drawFunc(_this->_right, gfx, x0, y);
	x0 += _this->_right->_widthFunc(_this->_right, gfx);
	
	_this->_left->_drawFunc(_this->_left, gfx, x0, y);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
		
	*/
}

// Limit

void ItemLimit_setFont(Item* _this, FontHandle hFont)
{
	ItemLimit* i = (ItemLimit*)_this;

	_this->_hFont = hFont;

	if (_this->_left)
		_this->_left->_setFontFunc(_this->_left, hFont);

	FontHandle smallFont = Graphics_getSmallerFont(hFont);

	if (i->_t1)
		i->_t1->_setFontFunc(i->_t1, smallFont);

	if (i->_t2)
		i->_t2->_setFontFunc(i->_t2, smallFont);
}

int ItemLimit_width(Item* _this, Graphics* gfx)
{
	ItemLimit* i = (ItemLimit*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"Lim", 3, &s);

	GSIZE s2;
	Graphics_GetTextExtentPoint32(gfx, L"\u2192", 1, &s2);

	int lw = _this->_left->_widthFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int t1w = 0, t2w = 0;

	if (i->_t1)
		t1w = i->_t1->_widthFunc(i->_t1, gfx);

	if (i->_t2)
		t2w = i->_t2->_widthFunc(i->_t2, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(s.cx + lw, s2.cx + t1w + t2w);
}

int ItemLimit_height(Item* _this, Graphics* gfx)
{
	ItemLimit* i = (ItemLimit*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"Lim", 3, &s);

	GSIZE s2;
	Graphics_GetTextExtentPoint32(gfx, L"\u2192", 1, &s2);

	int lh = _this->_left->_heightFunc(_this->_left, gfx);
	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int t1h = 0, t2h = 0;

	if (i->_t1)
		t1h = i->_t1->_heightFunc(i->_t1, gfx);

	if (i->_t2)
		t2h = i->_t2->_heightFunc(i->_t2, gfx);

	Graphics_SelectFont(gfx, hObj);

	return Max(lb, s.cy / 2) + Max(lh - lb, Max(t1h, t2h) + s.cy / 2);
}

int ItemLimit_baseLine(Item* _this, Graphics* gfx)
{
	ItemLimit* i = (ItemLimit*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"Lim", 3, &s);

	int lb = _this->_left->_baseLineFunc(_this->_left, gfx);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	Graphics_SelectFont(gfx, hObj);

	return Max(s.cy / 2, lb);
}

void ItemLimit_calcCoordinate(Item* _this, Graphics* gfx, int x, int y)
{
	Item_calcCoordinate(_this, gfx, x, y);

	ItemLimit* i = (ItemLimit*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"Lim", 3, &s);

	GSIZE s2;
	Graphics_GetTextExtentPoint32(gfx, L"\u2192", 1, &s2);

	int x0 = x;
	int y0 = y;

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	int t1w = 0, t2w = 0;

	if (i->_t1)
	{
		int t1b = i->_t1->_baseLineFunc(i->_t1, gfx);
		i->_t1->_calcCoordinateFunc(i->_t1, gfx, x0, y0 + s.cy / 2 + t1b);
		t1w = i->_t1->_widthFunc(i->_t1, gfx);
	}

	if (i->_t2)
	{
		int t2b = i->_t2->_baseLineFunc(i->_t2, gfx);
		i->_t2->_calcCoordinateFunc(i->_t2, gfx, x0 + t1w + s2.cx, y0 + s.cy / 2 + t2b);
		t2w = i->_t2->_widthFunc(i->_t2, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	i->_item._left->_calcCoordinateFunc(i->_item._left, gfx, x0 + Max(s.cx, t1w + t2w + s2.cx), y0);
}

void ItemLimit_draw(Item* _this, Graphics* gfx)
{
	ItemLimit* i = (ItemLimit*)_this;

	GSIZE s;
	Graphics_GetTextExtentPoint32(gfx, L"Lim", 3, &s);

	GSIZE s2;
	Graphics_GetTextExtentPoint32(gfx, L"\u2192", 1, &s2);

	Graphics_TextOut(gfx, _this->_x, _this->_y - s.cy / 2, L"Lim", 3);

	FontHandle hObj = Graphics_GetCurrentFont(gfx);
	Graphics_SelectFont(gfx, Graphics_getSmallerFont(hObj));

	if (i->_t1)
	{
		i->_t1->_drawFunc(i->_t1, gfx);

		Graphics_TextOut(gfx, i->_t1->_x + i->_t1->_width, i->_t1->_y - s2.cy / 2, L"\u2192", 1);
	}

	if (i->_t2)
	{
		i->_t2->_drawFunc(i->_t2, gfx);
	}

	Graphics_SelectFont(gfx, hObj);

	i->_item._left->_drawFunc(i->_item._left, gfx);

	if (_this->_hasFocus)
		Item_drawFocusRect(_this, gfx);
}

