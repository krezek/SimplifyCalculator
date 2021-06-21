#include "platform.h"

#include <layout_mgr.h>
#include <base_wnd.h>

#ifdef _DEBUG
#define _SHOWGRID
#endif

static void Init(LayoutManager* _this, int rows, int columns);
static void SetRowsHeight(LayoutManager* _this, ...);
static void SetColumnsWidth(LayoutManager* _this, ...);
static void SetCmp(LayoutManager* _this, int i, int j, Component* cmp, STYLE style);
static int GetMinWidth(LayoutManager* _this);
static int GetMinHeight(LayoutManager* _this);
static void DoLayout(LayoutManager* _this, int x0, int y0, int width, int height, BOOL repiant, RECT rc, HDC hdc);


LayoutManager* LayoutManager_init()
{
	LayoutManager* lm = (LayoutManager*)malloc(sizeof(LayoutManager));
	assert(lm != NULL);

	lm->_cmp._type = CMP_LAYOUT;

	lm->_rows = lm->_columns = 0;
	
	lm->_rowsHeight = NULL;
	lm->_columnsWidth = NULL;
	lm->_components = NULL;
	lm->_style = NULL;

	lm->_InitFunc = Init;
	lm->_SetRowsHeightFunc = SetRowsHeight;
	lm->_SetColumnWidthFunc = SetColumnsWidth;
	lm->_SetCmpFunc = SetCmp;
	lm->_GetMinWidthFunc = GetMinWidth;
	lm->_GetMinHeightFunc = GetMinHeight;
	lm->_DoLayoutFunc = DoLayout;

	return lm;
}

void LayoutManager_free(LayoutManager* lm)
{
	if (lm->_style)
		free(lm->_style);

	if (lm->_components)
		free(lm->_components);

	if (lm->_rowsHeight)
		free(lm->_rowsHeight);

	if (lm->_columnsWidth)
		free(lm->_columnsWidth);

	free(lm);
}

static void Init(LayoutManager* _this, int rows, int columns)
{
	_this->_rows = rows;
	_this->_columns = columns;

	_this->_rowsHeight = (double*)malloc(_this->_rows * sizeof(double));
	assert(_this->_rowsHeight != NULL);

	memset(_this->_rowsHeight, 0, _this->_rows);

	_this->_columnsWidth = (double*)malloc(_this->_columns * sizeof(double));
	assert(_this->_columnsWidth != NULL);

	memset(_this->_columnsWidth, 0, _this->_columns);

	_this->_components = (Component**)malloc(_this->_rows * _this->_columns * sizeof(Component*));
	assert(_this->_components != NULL);

	memset(_this->_components, 0, _this->_rows * _this->_columns * sizeof(Component*));

	_this->_style = (STYLE*)malloc(_this->_rows * _this->_columns * sizeof(STYLE));
	assert(_this->_style != NULL);

	memset(_this->_style, 0, _this->_rows * _this->_columns * sizeof(STYLE));
}

static void SetRowsHeight(LayoutManager* _this, ...)
{
	va_list valist;

	va_start(valist, _this);
	for (int i = 0; i < _this->_rows; i++) {
		_this->_rowsHeight[i] = va_arg(valist, double);
	}
	va_end(valist);
}

static void SetColumnsWidth(LayoutManager* _this, ...)
{
	va_list valist;

	va_start(valist, _this);
	for (int i = 0; i < _this->_columns; i++) {
		_this->_columnsWidth[i] = va_arg(valist, double);
	}
	va_end(valist);
}

static void SetCmp(LayoutManager* _this, int i, int j, Component* cmp, STYLE style)
{
	_this->_components[i * _this->_columns + j] = cmp;
	_this->_style[i * _this->_columns + j] = style;
}

static int GetMinWidth(LayoutManager* _this)
{
	int minWidth = 0;

	for (int iy = 0; iy < _this->_rows; ++iy)
	{
		int w = 0;
		for (int ix = 0; ix < _this->_columns; ++ix)
		{
			Component* cmp = _this->_components[iy * _this->_columns + ix];
			if (cmp)
			{
				if (cmp->_type == CMP_WINDOW)
				{
					if (_this->_columnsWidth[ix] > 1)
					{
						w += (int)_this->_columnsWidth[ix];
					}
					else
					{
						w += cmp->_width;
					}
				}
				else if (cmp->_type == CMP_LAYOUT)
				{
					w += GetMinWidth((LayoutManager*)cmp);
				}
			}
		}

		minWidth = max(minWidth, w);
	}

	return minWidth;
}

static int GetMinHeight(LayoutManager* _this)
{
	int minHeight = 0;

	for (int iy = 0; iy < _this->_rows; ++iy)
	{
		int h = 0;
		for (int ix = 0; ix < _this->_columns; ++ix)
		{
			Component* cmp = _this->_components[iy * _this->_columns + ix];
			if (cmp)
			{
				if (cmp->_type == CMP_WINDOW)
				{
					if (_this->_rowsHeight[iy] > 1)
					{
						h = max(h, (int)_this->_rowsHeight[iy]);
					}
					else
					{
						h = max(h, cmp->_height);
					}
				}
				else if (cmp->_type == CMP_LAYOUT)
				{
					h = max(h, GetMinHeight((LayoutManager*)cmp));
				}
			}
		}

		minHeight += h;
	}

	return minHeight;
}

static void DoLayout(LayoutManager* _this, int x0, int y0, int width, int height, BOOL repiant, RECT rc, HDC hdc)
{
	_this->_x = x0;
	_this->_y = y0;
	_this->_width = width;
	_this->_height = height;

	double* rh = (double*)malloc(_this->_rows * sizeof(double));
	assert(rh != NULL);

	double* cw = (double*)malloc(_this->_columns * sizeof(double));
	assert(cw != NULL);

	int newHeight = height;

	for (int i = 0; i < _this->_rows; ++i)
	{
		if (_this->_rowsHeight[i] > 1)
		{
			rh[i] = _this->_rowsHeight[i];
			newHeight -= (int)rh[i];
		}
	}

	for (int i = 0; i < _this->_rows; ++i)
	{
		if (_this->_rowsHeight[i] <= 1)
		{
			rh[i] = _this->_rowsHeight[i] * newHeight;
		}
	}

	int newWidth = width;
	
	for (int i = 0; i < _this->_columns; ++i)
	{
		if (_this->_columnsWidth[i] > 1)
		{
			cw[i] = _this->_columnsWidth[i];
			newWidth -= (int)cw[i];
		}
	}

	for (int i = 0; i < _this->_columns; ++i)
	{
		if (_this->_columnsWidth[i] <= 1)
		{
			cw[i] = _this->_columnsWidth[i] * newWidth;
		}
	}

	for (int iy = 0, y = y0; iy < _this->_rows; y += (int)rh[iy], ++iy)
	{
		for (int ix = 0, x = x0; ix < _this->_columns; x += (int)cw[ix], ++ix)
		{
			Component* cmp = _this->_components[iy * _this->_columns + ix];
			STYLE style = _this->_style[iy * _this->_columns + ix];

			if (cmp)
			{
				if (cmp->_type == CMP_WINDOW)
				{
					BaseWindow* bw = (BaseWindow*)cmp;

					int nx = x;
					int ny = y;
#ifdef _SHOWGRID
					if (hdc)
					{
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, x + (int)cw[ix], y);
						LineTo(hdc, x + (int)cw[ix], y + (int)rh[iy]);
						LineTo(hdc, x, y + (int)rh[iy]);
						LineTo(hdc, x, y);
					}
#endif
					if (style & LM_H_CENTER)
					{
						nx += (int)(cw[ix] - cmp->_width) / 2;
					}

					if (style & LM_H_RIGHT)
					{
						nx += (int)(cw[ix] - cmp->_width);
					}

					if (style & LM_V_CENTER)
					{
						ny += (int)(rh[iy] - cmp->_height) / 2;
					}

					if (style & LM_V_BOTOM)
					{
						ny += (int)(rh[iy] - cmp->_height);
					}

					cmp->_SetPosFunc(cmp, nx, ny);

					RECT cmpRC, resultRC;
					cmpRC.left = cmp->_x;
					cmpRC.top = cmp->_y;
					cmpRC.right = cmpRC.left + cmp->_width;
					cmpRC.bottom = cmpRC.top + cmp->_height;

					if (IntersectRect(&resultRC, &rc, &cmpRC))
					{
						MoveWindow(bw->_hWnd, cmp->_x, cmp->_y, cmp->_width, cmp->_height, repiant);
					}
				}
				else if (cmp->_type == CMP_LAYOUT)
				{
					LayoutManager* lm = (LayoutManager*)cmp;
#ifdef _SHOWGRID
					if (hdc)
					{
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, x + (int)cw[ix], y);
						LineTo(hdc, x + (int)cw[ix], y + (int)rh[iy]);
						LineTo(hdc, x, y + (int)rh[iy]);
						LineTo(hdc, x, y);
					}
#endif
					
					lm->_DoLayoutFunc(lm, x, y, (int)cw[ix], (int)rh[iy], repiant, rc, hdc);
				}
			}
		}
	}


	free(rh);
	free(cw);
}
