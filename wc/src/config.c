#include "platform.h"

#include "config.h"

Config* Config_init()
{
	Config* cfg = (Config*)malloc(sizeof(Config));
	assert(cfg != NULL);

	cfg->_char_width = cfg->_char_height = 0;

	return cfg;
}

void Config_free(Config* cfg)
{
	free(cfg);
}

void Config_SetValues(Config* cfg, HWND hWnd)
{
	TEXTMETRIC tm;

	HDC hdc = GetDC(hWnd);

	GetTextMetrics(hdc, &tm);
	cfg->_char_width = tm.tmMaxCharWidth;
	cfg->_char_height = tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading;

	ReleaseDC(hWnd, hdc);
}
