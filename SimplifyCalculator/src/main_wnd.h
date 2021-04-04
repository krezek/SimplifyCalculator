#ifndef _MAIN_WND_H_
#define _MAIN_WND_H_

#include "base_wnd.h"
#include "panel.h"

typedef struct
{
	BaseWindow _baseWindow;
	HWND _hWndVScrollBar;
	HWND _hWndStatusBar;
	TEXTMETRIC _tmFixedFont;

	int _client_width, _client_height;
	int _x_current_pos, _y_current_pos;
	int _yMaxScroll;

	PanelLinkedList* _panels;
} MainWindow;

ATOM MainWindow_RegisterClass();

MainWindow* MainWindow_init();
void MainWindow_free(MainWindow* mw);

#endif /* _MAIN_WND_H_ */

