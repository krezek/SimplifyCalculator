#ifndef _MAIN_WND_H_
#define _MAIN_WND_H_

#include "base_wnd.h"

typedef struct
{
	BaseWindow _baseWindow;

} MainWindow;

ATOM MainWindow_RegisterClass();

MainWindow* MainWindow_init();
void MainWindow_free(MainWindow* mw);

#endif /* _MAIN_WND_H_ */

