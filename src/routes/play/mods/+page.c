#include <gint/keyboard.h>
#include "assets.h"
#include "ui.h"

void UI_Play_Mods(FXT_ModOption *option, const FXT_Config *config)
{
	while (true)
	{
		dclear(C_WHITE);
		dsubimage(0, 0, &Img_Modifications, 0, 64 * config->Language, DWIDTH, DHEIGHT, 0);

		if (option->Mirror)
			drect(2, 13, 62, 27, C_INVERT);

		if (option->Random)
			drect(65, 13, 125, 27, C_INVERT);

		dupdate();

		auto const e = getkey();

		switch (e.key)
		{
		case KEY_1:
		case KEY_F1:
			option->Mirror = ! option->Mirror;
			if (option->Mirror)
				option->Random = false;
			break;
		case KEY_4:
		case KEY_F4:
			option->Random = ! option->Random;
			if (option->Random)
				option->Mirror = false;
			break;
		case KEY_EXIT:
			return;
		default:
			break;
		}
	}
}
