#ifndef _TEXTEDIT_H_
#define _TEXTEDIT_H_

#include <base_wnd.h>

typedef struct
{
	BaseWindow _baseWindow;

} TextEdit;

TextEdit* TextEdit_init();
void TextEdit_free(TextEdit* wnd);


#endif /* _TEXTEDIT_H_ */
