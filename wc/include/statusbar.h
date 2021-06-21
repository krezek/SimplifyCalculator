#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include <base_wnd.h>

typedef struct
{
	BaseWindow _baseWindow;

} StatusBar;

StatusBar* StatusBar_init();
void StatusBar_free(StatusBar* wnd);


#endif /* _STATUSBAR_H_ */
