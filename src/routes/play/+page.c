#include <stb_ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <fxTap/database.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "assets.h"
#include "ui.h"

static void RenderBeatmapListItem(const struct FXT_Database *const db[], const size_t index, const int y)
{
	dtext(1, y, C_BLACK, db[index]->value.Title);
	auto const key = db[index]->key;
	char keyNoExtension[strlen(key) + 1 - 4] = {};
	memcpy(keyNoExtension, key, strlen(key) - 4);
	dfont(&Font_Tiny);
	dprint_opt(DWIDTH - 2, y + 2, C_BLACK, C_NONE, DTEXT_RIGHT, DTEXT_TOP, keyNoExtension);
	dfont(&Font_Piczel);
}

OCharP UI_Play(const FXT_Config *config, const FXT_Database *database, FXT_ModOption *modOption)
{
	auto const db = *database;

	bool descending = true;
	const size_t size = shlenu(db);

	static size_t selectedIndex = 0;

	const struct FXT_Database *viewOrderByNameAsc[size] = {};
	const struct FXT_Database *viewOrderByNameDsc[size] = {};

	for (size_t i = 0; i < size; i += 1)
	{
		viewOrderByNameAsc[i] = &db[i];
		viewOrderByNameDsc[i] = &db[i];
	}

	qsort(viewOrderByNameAsc, size, sizeof(FXT_Database), FXT_Database_Compare_Reverse_Void);
	qsort(viewOrderByNameDsc, size, sizeof(FXT_Database), FXT_Database_Compare_Void);

	while (true)
	{
		auto const view = descending ? viewOrderByNameDsc : viewOrderByNameAsc;

		dclear(C_WHITE);

		dsubimage(0, 56, &Img_SelectASong_FN, 0, 8 * config->Language, 128, 8, 0);
		dsubimage(2, 56, descending ? &Img_Ascending_FN : &Img_Descending_FN, 0, 8 * config->Language, 19, 8, 0);

		if (size == 0)
		{
			dtext(56, 27, C_BLACK, "???");
		}
		else
		{
			auto const lastGrades = view[selectedIndex]->value.LastGrades;

			if (lastGrades.Exist)
			{
				auto const scoreV2 = FXT_Grades_ScoreV2(lastGrades.Value);
				dprint(91, 2, C_WHITE, "%.2f", scoreV2 * 100);
			}

			if (selectedIndex >= 2)
				RenderBeatmapListItem(view, selectedIndex - 2, 1);
			if (selectedIndex >= 1)
				RenderBeatmapListItem(view, selectedIndex - 1, 11);

			RenderBeatmapListItem(view, selectedIndex, 22);
			drect(0, 21, DWIDTH, 31, C_INVERT);

			if (size - selectedIndex >= 2)
				RenderBeatmapListItem(view, selectedIndex + 1, 33);
			if (size - selectedIndex >= 3)
				RenderBeatmapListItem(view, selectedIndex + 2, 43);
		}

		dupdate();

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_DOWN:
		case KEY_RIGHT:
			if (size > 0)
				selectedIndex = (selectedIndex + 1) % size;
			break;

		case KEY_UP:
		case KEY_LEFT:
			if (size > 0)
				selectedIndex = selectedIndex > 0 ? selectedIndex - 1 : size - 1;
			break;

		case KEY_F3:
			if (size > 0)
				selectedIndex = rtc_ticks() % size;
			break;

		case KEY_F4:
		case KEY_OPTN:
			UI_Play_Mods(modOption, config);
			break;

		case KEY_F5:
		case KEY_VARS:
			if (size > 0)
				UI_Play_Detail(view[selectedIndex]->key);
			break;

		case KEY_EXIT:
			return (OCharP){.Path = nullptr};

		case KEY_F1:
			descending = ! descending;
			break;

		case KEY_F6: {
			auto const userPath = UI_Play_FromPath(config);
			if (userPath.Path != nullptr)
				return userPath;
			break;
		}

		case KEY_EXE:
			if (size > 0)
				return (OCharP){.Path = view[selectedIndex]->key, .NeedFree = false};
		default: break;
		}
	}
}
