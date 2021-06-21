#ifndef _CMP_H_
#define _CMP_H_

typedef struct _Component Component;

typedef enum {CMP_WINDOW, CMP_LAYOUT} CMP_TYPE;

typedef void(*SetSizeFunc)(Component* _this, int width, int height);
typedef void(*SetPosFunc)(Component* _this, int x, int y);

typedef struct _Component
{
	int _x, _y;
	int _width, _height;
	int _minWidth, _minHeight;
	int _maxWidth, _maxHeight;

	CMP_TYPE _type;

	SetSizeFunc _SetSizeFunc;
	SetPosFunc _SetPosFunc;
} Component;

void Component_default(Component* _this);


#endif /* _CMP_H_ */
