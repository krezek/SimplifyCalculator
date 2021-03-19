#include "platform.h"

#include "panel.h"

Panel* Panel_init()
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	return p;
}

void Panel_free(Panel* p)
{
	free(p);
}