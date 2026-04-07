#include <stb_ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <fxTap/database.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "assets.h"
#include "ui.h"

OCharP UI_Play(const FXT_Config *config, const FXT_Database *database, FXT_ModOption *modOption)
{
	auto const db = *database;

	bool descending = true;
	const size_t size = shlenu(db);

	size_t selectedIndex = 0;

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

			// Draw 2 beatmaps before
			if (selectedIndex >= 2)
				dtext(1, 1, C_BLACK, view[selectedIndex - 2]->value.Title);
			if (selectedIndex >= 1)
				dtext(1, 11, C_BLACK, view[selectedIndex - 1]->value.Title);

			// Draw selected beatmap
			dtext(1, 22, C_BLACK, view[selectedIndex]->value.Title);
			drect(0, 21, DWIDTH, 31, C_INVERT);

			// Draw 2 beatmaps after
			if (size - selectedIndex >= 2)
				dtext(1, 33, C_BLACK, view[selectedIndex + 1]->value.Title);
			if (size - selectedIndex >= 3)
				dtext(1, 43, C_BLACK, view[selectedIndex + 2]->value.Title);
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
