#ifndef _PANEL_H_
#define _PANEL_H_

typedef struct _Panel
{
	int _x, _y, _width, _height;
} Panel;

Panel* Panel_init();
void Panel_free(Panel* p);

#endif /* _PANEL_H_ */

