#include <assert.h>
#include <gint/keyboard.h>
#include "assets.h"
#include "ui.h"

void UI_About()
{
	static constexpr auto PageCount = 3;
	int selectedPage = 0;

	while (true)
	{
		dclear(C_WHITE);

		switch (selectedPage)
		{
		case 0:
			dsubimage(0, 0, &Img_About, 0, 0, 128, 64, 0);
			break;

		case 1:
			dsubimage(0, 0, &Img_About, 0, 64, 128, 64, 0);
			break;

		case 2:
			dsubimage(0, 0, &Img_About, 0, 128, 128, 64, 0);
			dprint(1, 20, C_BLACK, "GCC Version: %s", __VERSION__);
			dprint(1, 34, C_BLACK, "Compiled at: %s", __DATE__);
			break;

		default:
			assert(false && "Invalid about page");
		}

		dupdate();

		auto const e = getkey();

		switch (e.key)
		{
		case KEY_EXIT:
			return;
		case KEY_UP:
		case KEY_LEFT:
			if (selectedPage > 0)
				selectedPage -= 1;
			break;
		case KEY_DOWN:
		case KEY_RIGHT:
		case KEY_EXE:
			if (selectedPage < PageCount - 1)
				selectedPage += 1;
			break;
		default:
			break;
		}
	}
}
