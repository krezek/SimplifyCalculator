#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef struct _Config
{
	int _char_width;
	int _char_height;
} Config;

Config* Config_init();
void Config_free(Config* cfg);

void Config_SetValues(Config* cfg, HWND hWnd);

#endif /* _CONFIG_H_ */

