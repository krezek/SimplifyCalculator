#include "platform.h"

#include "panel.h"

static void Draw(Panel* p, HDC hdc)
{
	p->_x = 10;
	p->_y = 10;
	p->_width = 100;
	p->_height = 50;

	RECT rc;
	rc.left = p->_x;
	rc.top = p->_y;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));
}

Panel* Panel_init()
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_DrawFunc = Draw;

	return p;
}

void Panel_free(Panel* p)
{
	free(p);
}
