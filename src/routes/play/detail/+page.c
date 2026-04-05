#include <stdio.h>
#include <fxTap/beatmap.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include "ui.h"

void UI_Play_Detail(const char *beatmapPath)
{
	FXT_Beatmap beatmap;
	auto const beatmapError = FXT_Beatmap_Load_BFile_Auto(&beatmap, beatmapPath);

	if (beatmapError)
	{
		dclear(C_WHITE);
		dprint(0, 0, C_BLACK, "Beatmap Error: %i", beatmapError);
		dupdate();
		getkey();
		return;
	}

	dclear(C_WHITE);

	dprint(0, 0, C_BLACK, "%s", beatmap.Title);
	dprint(0, 8, C_BLACK, "%s", beatmap.Artist);
	dprint(0, 21, C_BLACK, "%uK  OD:%.1f", beatmap.ColumnCount, beatmap.OverallDifficulty);
	dprint(0, 34, C_BLACK, "Note count:");

	{
		char buffer[48] = {};
		char *p = buffer;

		for (size_t i = 0; i < beatmap.ColumnCount; i += 1)
			p += sprintf(p, i == 0 ? "%d" : ",%d", beatmap.ColumnSize[i]);

		dtext(0, 42, C_BLACK, buffer);
	}

	{
		size_t tap = 0, hold = 0;
		size_t iOverall = 0;

		for (size_t column = 0; column < beatmap.ColumnCount; column += 1)
		{
			auto const size = beatmap.ColumnSize[column];

			for (size_t i = 0; i < size; i += 1, iOverall += 1)
				if (beatmap.Notes[iOverall].Duration == 0)
					tap += 1;
				else
					hold += 1;
		}

		dprint(0, 56, C_BLACK, "Tap:%u  Hold:%u", tap, hold);
	}


	dupdate();
	getkey();

	FXT_Beatmap_FreeInner(&beatmap);
}
