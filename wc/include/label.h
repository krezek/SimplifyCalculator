#ifndef _LABEL_H_
#define _LABEL_H_

#include <base_wnd.h>

typedef struct
{
	BaseWindow _baseWindow;

} Label;

Label* Label_init();
void Label_free(Label* wnd);


#endif /* _LABEL_H_ */
