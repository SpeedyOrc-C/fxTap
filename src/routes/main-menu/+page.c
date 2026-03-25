#include <stdlib.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
#include "ui.h"

[[nodiscard]]
static FXT_BeatmapError TryLoadBeatmap(FXT_Beatmap *dst, const char *path)
{
	if (gint[HWFS] == HWFS_CASIOWIN)
		return gint_call((gint_call_t){
			.function = FXT_Beatmap_Load_BFile,
			.args = {{.pv = dst}, {.pc_c = path}}
		});

	return FXT_Beatmap_Load(dst, path);
}

static void UI_SelectPlayLoop(const FXT_Config *config, const FXT_Database *database)
{
	while (true)
	{
		auto const path = UI_AskBeatmapPath_ListLibrary(config, database);

		if (path.Path == nullptr)
			break;

		FXT_Beatmap beatmap;
		const FXT_BeatmapError error = TryLoadBeatmap(&beatmap, path.Path);

		if (error)
		{
			if (path.NeedFree)
				free(path.Path);

			dclear(C_WHITE);
			dprint(1, 1, C_BLACK, "Beatmap Error: %d", error);
			dupdate();
			getkey();
			continue;
		}

		UI_Play(&beatmap, config, path.Path);

		if (path.NeedFree)
			free(path.Path);

		FXT_Beatmap_FreeInner(&beatmap);
	}
}

void UI_MainMenuLoop(FXT_Config *config, const FXT_Database *database)
{
	typedef enum MenuItem
	{
		MenuItem_Play,
		MenuItem_KeyTest,
		MenuItem_Settings,
		MenuItem_About,
	} MenuItem;

	typedef enum MenuImage
	{
		MenuImage_Banner,
		MenuImage_ElementalCreation,
		MenuImage_Daisuke,
	} MenuImage;

	MenuItem selectedItem = MenuItem_Play;
	MenuImage selectedImage = MenuImage_Banner;

	// ReSharper disable once CppDFAEndlessLoop
	while (true)
	{
		dclear(C_WHITE);

		switch (selectedImage)
		{
		case MenuImage_Banner:
			dimage(0, 0, &Img_Banner);
			break;
		case MenuImage_ElementalCreation:
			dimage(0, 0, &Img_Elemental_Creation);
			break;
		case MenuImage_Daisuke:
			dimage(0, 0, &Img_Daisuke);
			break;
		}

		for (MenuItem renderItem = MenuItem_Play; renderItem <= MenuItem_About; renderItem += 1)
		{
			const int y = 16 * renderItem;

			dsubimage(
				80, y, &Img_MainMenu_Buttons,
				48 * config->Language, y,
				48, 16, 0
			);

			if (selectedItem == renderItem)
				drect(80, y, 127, y + 15, C_INVERT);
		}

		dupdate();

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_MENU:
			if (e.shift)
				UI_Settings(config);
			break;
		case KEY_UP:
			if (selectedItem > MenuItem_Play)
				selectedItem -= 1;
			break;
		case KEY_DOWN:
			if (selectedItem < MenuItem_About)
				selectedItem += 1;
			break;
		case KEY_LEFT:
			if (selectedImage > MenuImage_Banner)
				selectedImage -= 1;
			break;
		case KEY_RIGHT:
			if (selectedImage < MenuImage_Daisuke)
				selectedImage += 1;
			break;
		case KEY_EXE:
			switch (selectedItem)
			{
			case MenuItem_Play:
				UI_SelectPlayLoop(config, database);
				break;
			case MenuItem_KeyTest:
				UI_KeyTest(config);
				break;
			case MenuItem_Settings:
				UI_Settings(config);
				break;
			case MenuItem_About:
				UI_About();
				break;
			}
		default:
			break;
		}
	}
}
