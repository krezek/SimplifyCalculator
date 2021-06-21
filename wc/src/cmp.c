#include "platform.h"

#include <cmp.h>

static void SetSize(Component* _this, int width, int height);
static void SetPos(Component* _this, int x, int y);

void Component_default(Component* _this)
{
	_this->_SetSizeFunc = SetSize;
	_this->_SetPosFunc = SetPos;
}

static void SetSize(Component* _this, int width, int height)
{
	_this->_width = width;
	_this->_height = height;
}

static void SetPos(Component* _this, int x, int y)
{
	_this->_x = x;
	_this->_y = y;
}
