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

		if (option->NoRelease)
			drect(65, 30, 125, 44, C_INVERT);

		if (option->HoldOff)
			drect(65, 47, 125, 61, C_INVERT);

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
		case KEY_5:
		case KEY_F5:
			option->NoRelease = ! option->NoRelease;
			if (option->NoRelease)
				option->HoldOff = false;
			break;
		case KEY_6:
		case KEY_F6:
			option->HoldOff = ! option->HoldOff;
			if (option->HoldOff)
				option->NoRelease = false;
			break;
		case KEY_EXIT:
			return;
		default:
			break;
		}
	}
}
