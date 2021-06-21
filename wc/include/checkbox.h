#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <base_wnd.h>

typedef struct
{
	BaseWindow _baseWindow;

} CheckBox;

CheckBox* CheckBox_init();
void CheckBox_free(CheckBox* wnd);


#endif /* _CHECKBOX_H_ */
