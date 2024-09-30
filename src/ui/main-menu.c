#include "ui.h"
#include <fxTap/config.h>
#include <fxTap/config-casiowin.h>
#include <assert.h>
#include <gint/display.h>
#include <gint/hardware.h>

typedef enum Item { Item_Play, Item_KeyTest, Item_Settings, Item_About } Item;

void UI_MainMenu(Config *config)
{
	Item selectedItem = Item_Play;

	while (1)
	{
		dclear(C_WHITE);

		#ifdef FXCG50
			dprint(1, 1 + 0 * (1 + dfont_default()->line_height), C_BLACK, "  [EXE] Start");
			dprint(1, 1 + 1 * (1 + dfont_default()->line_height), C_BLACK, "[SETUP] Settings");
			dprint(1, 1 + 2 * (1 + dfont_default()->line_height), C_BLACK, "   [F5] Key Test");
			dprint(1, 1 + 3 * (1 + dfont_default()->line_height), C_BLACK, "   [F6] About");
		#endif

		#ifdef FX9860G
			for (Item renderItem = Item_Play; renderItem <= Item_About; renderItem += 1)
			{
				const int y = 16 * renderItem;

				dsubimage(
					0, y, &Img_MainMenu_Buttons,
					48 * config->Language, y,
					48, 16, 0
				);

				if (selectedItem == renderItem)
					drect(0, y, 48, y + 15, C_INVERT);
			}
		#endif

		dupdate();

		key_event_t e = getkey();

		if (e.key == KEY_EXE)
		{
			switch (selectedItem)
			{
				case Item_Play: UI_SelectSong(config); break;
				case Item_KeyTest: UI_KeyTest(config); break;
				case Item_Settings: UI_Settings(config); break;
				case Item_About: UI_About(); break;
				default: assert(false);
			}
			continue;
		}

		if (e.key == KEY_MENU && e.shift)
		{
			UI_Settings(config);
			continue;
		}

		if (e.key == KEY_UP && selectedItem > Item_Play)
		{
			selectedItem -= 1;
			continue;
		}

		if (e.key == KEY_DOWN && selectedItem < Item_About)
		{
			selectedItem += 1;
			continue;
		}

		if (e.key == KEY_F1)
			assert(false);
	}
}
