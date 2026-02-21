#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fxTap/config.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
#include "ui.h"
#include "fxTap/game.h"

typedef enum SettingsItem { Item_Play, Item_KeyTest, Item_Settings, Item_About } SettingsItem;

void Beatmap_New_LoadFromPath_BFile_Wrapper(FXT_Beatmap **beatmap, const char *path, FXT_BeatmapError *error)
{
	*beatmap = FXT_Beatmap_Load_BFile(path, error);
}

[[nodiscard]]
FXT_Beatmap *TryLoadBeatmap(const char *fileName, FXT_BeatmapError *error)
{
	FXT_Beatmap *beatmap;

	if (gint[HWFS] == HWFS_CASIOWIN)
		gint_call((gint_call_t){
			.function = &Beatmap_New_LoadFromPath_BFile_Wrapper,
			.args = {{.pv = &beatmap}, {.pc = fileName}, {.pv = error}}
		});
	else
		beatmap = FXT_Beatmap_Load(fileName, error);

	if (beatmap != nullptr)
		return beatmap;

	// File not found. So try again with .fxt extension.
	char *fileNameWithExtension = malloc(strlen(fileName) + 4 + 1);
	sprintf(fileNameWithExtension, "%s.fxt", fileName);

	if (gint[HWFS] == HWFS_CASIOWIN)
		gint_call((gint_call_t){
			.function = &Beatmap_New_LoadFromPath_BFile_Wrapper,
			.args = {{.pv = &beatmap}, {.pc = fileNameWithExtension}, {.pv = error}}
		});
	else
		beatmap = FXT_Beatmap_Load(fileNameWithExtension, error);

	free(fileNameWithExtension);

	if (beatmap != nullptr)
		return beatmap;

	return nullptr;
}

void UI_MainMenu(FXT_Config *config)
{
	SettingsItem selectedItem = Item_Play;

	// ReSharper disable once CppDFAEndlessLoop
	while (true)
	{
		dclear(C_WHITE);

		for (SettingsItem renderItem = Item_Play; renderItem <= Item_About; renderItem += 1)
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
				auto const filePath = UI_AskBeatmapPath_TypeFileNameManually(config);

				if (filePath == nullptr)
					continue;

				FXT_BeatmapError error;
				const auto beatmap = TryLoadBeatmap(filePath, &error);

				free(filePath);

				if (beatmap == nullptr)
				{
					dclear(C_WHITE);
					dprint(1, 1, C_BLACK, "Error: %d", error);
					dupdate();
					getkey();
					continue;
				}

				FxTap fxtap;
				FxTap_Init(&fxtap, beatmap);

				UI_Play(&fxtap, config);

				FXT_Beatmap_Free(beatmap);

				continue;
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
