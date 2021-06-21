#ifndef _LIST_VIEW_H_
#define _LIST_VIEW_H_

#include <base_wnd.h>

typedef struct
{
	BaseWindow _baseWindow;
} ListView;

ListView* ListView_init();
void ListView_free(ListView* wnd);

#endif /* _LIST_VIEW_H_ */
