#include <gint/keyboard.h>
#include "assets.h"
#include "ui.h"

void UI_Play_Mods(FXT_ModOption *option, const FXT_Config *config)
{
	static size_t page = 0;

	while (true)
	{
		dclear(C_WHITE);
		{
			const int left = DWIDTH * config->Language;
			dsubimage(0, 0, &Img_Modifications, left, 0, DWIDTH, 11, 0);
			dsubimage(0, 11, &Img_Modifications, left, 11 + 51 * page, DWIDTH, 51, 0);
		}

		switch (page)
		{
		case 0:
			if (option->Mirror)
				drect(2, 13, 62, 27, C_INVERT);

			if (option->Random)
				drect(65, 13, 125, 27, C_INVERT);

			if (option->NoRelease)
				drect(65, 30, 125, 44, C_INVERT);

			if (option->HoldOff)
				drect(65, 47, 125, 61, C_INVERT);

			break;

		case 1:
			if (option->Reverse)
				drect(2, 13, 62, 27, C_INVERT);

			break;

		default:
			break;
		}

		dupdate();

		auto const e = getkey();

		switch (page)
		{
		case 0:
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
			default:
				break;
			}
			break;

		case 1:
			switch (e.key)
			{
			case KEY_1:
			case KEY_F1:
				option->Reverse = ! option->Reverse;
				break;
			default:
				break;
			}

		default:
			break;
		}

		switch (e.key)
		{
		case KEY_RIGHT:
		case KEY_DOWN:
			if (page < 1)
				page += 1;
			break;
		case KEY_LEFT:
		case KEY_UP:
			if (page > 0)
				page -= 1;
			break;
		case KEY_EXIT:
			return;
		default:
			break;
		}
	}
}
