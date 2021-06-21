#ifndef _LAYOUT_MGR_H_
#define _LAYOUT_MGR_H_

#include <cmp.h>

typedef struct _LayoutManager LayoutManager;

typedef enum {LM_H_LEFT = 1, LM_H_CENTER = 2, LM_H_RIGHT = 4, LM_V_TOP = 8, LM_V_CENTER = 16, LM_V_BOTOM = 32} STYLE;

typedef void (*LayoutManagerInitFunc) (LayoutManager* _this, int rows, int columns);
typedef void (*LayoutManagerSetRowsHeightFunc)(LayoutManager* _this, ...);
typedef void (*LayoutManagerSetColumnsWidthFunc)(LayoutManager* _this, ...);
typedef void (*LayoutManagerSetCmpFunc)(LayoutManager* _this, int i, int j, Component* cmp, STYLE style);
typedef int (*LayoutManagerGetMinWidthFunc)(LayoutManager* _this);
typedef int (*LayoutManagerGetMinHeightFunc)(LayoutManager* _this);
typedef void (*LayoutManagerDoLayoutFunc)(LayoutManager* _this, int x0, int y0, int width, int height, BOOL repiant, RECT rc, HDC hdc);

typedef struct _LayoutManager
{
	Component _cmp;

	int _rows, _columns;

	double* _rowsHeight;
	double* _columnsWidth;

	Component** _components;
	STYLE* _style;

	int _x, _y, _width, _height;

	LayoutManagerInitFunc _InitFunc;
	LayoutManagerSetRowsHeightFunc _SetRowsHeightFunc;
	LayoutManagerSetColumnsWidthFunc _SetColumnWidthFunc;
	LayoutManagerSetCmpFunc _SetCmpFunc;
	LayoutManagerGetMinWidthFunc _GetMinWidthFunc;
	LayoutManagerGetMinHeightFunc _GetMinHeightFunc;
	LayoutManagerDoLayoutFunc _DoLayoutFunc;
} LayoutManager;

LayoutManager* LayoutManager_init();
void LayoutManager_free(LayoutManager* lm);

#endif /* _LAYOUT_MGR_H_ */
