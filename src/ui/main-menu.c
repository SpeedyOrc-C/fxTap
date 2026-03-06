#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
#include "ui.h"

typedef enum MenuItem
{
	Item_Play,
	Item_KeyTest,
	Item_Settings,
	Item_About,
} MenuItem;

[[nodiscard]]
FXT_BeatmapError TryLoadBeatmap(FXT_Beatmap *dst, const char *path)
{
	FXT_BeatmapError error = 0;

	if (gint[HWFS] == HWFS_CASIOWIN)
		error = gint_call((gint_call_t){
			.function = &FXT_Beatmap_Load_BFile,
			.args = {{.pv = dst}, {.pc_c = path}}
		});
	else
		error = FXT_Beatmap_Load(dst, path);

	if (error != FXT_BeatmapError_FileNotFound)
		return error;

	// Try again with .fxt extension.
	char fileNameWithExtension[strlen(path) + 4 + 1] = {};
	sprintf(fileNameWithExtension, "%s.fxt", path);

	if (gint[HWFS] == HWFS_CASIOWIN)
		return gint_call((gint_call_t){
			.function = &FXT_Beatmap_Load_BFile,
			.args = {{.pv = dst}, {.pc = fileNameWithExtension}}
		});

	return FXT_Beatmap_Load(dst, fileNameWithExtension);
}

void UI_MainMenu(FXT_Config *config, const FXT_Database *database)
{
	MenuItem selectedItem = Item_Play;

	// ReSharper disable once CppDFAEndlessLoop
	while (true)
	{
	beginning:
		dclear(C_WHITE);

		for (MenuItem renderItem = Item_Play; renderItem <= Item_About; renderItem += 1)
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

		dupdate();

		const key_event_t e = getkey();

		if (e.key == KEY_EXE)
		{
			switch (selectedItem)
			{
			case Item_Play: {
				while (true)
				{
					auto const path = UI_AskBeatmapPath_ListLibrary(config, database);

					if (path.Path == nullptr)
						goto beginning;

					FXT_Beatmap beatmap;
					const FXT_BeatmapError error = TryLoadBeatmap(&beatmap, path.Path);

					if (path.NeedFree)
						free(path.Path);

					if (error)
					{
						dclear(C_WHITE);
						dprint(1, 1, C_BLACK, "Error: %d", error);
						dupdate();
						getkey();
						continue;
					}

					FXT_Game game;
					FXT_Game_Init(&game, &beatmap);
					UI_Play(&game, config);

					FXT_Beatmap_FreeInner(&beatmap);
				}
			}
			case Item_KeyTest:
				UI_KeyTest(config);
				continue;
			case Item_Settings:
				UI_Settings(config);
				continue;
			case Item_About:
				UI_About();
				continue;
			default: assert(false);
			}
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
