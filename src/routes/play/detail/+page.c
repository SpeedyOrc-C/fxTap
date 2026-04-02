#include <stdio.h>
#include <fxTap/database.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include "ui.h"

void UI_Play_Detail(const FXT_DatabaseRecord *record)
{
	dclear(C_WHITE);

	dprint(0, 0, C_BLACK, "%s", record->Title);
	dprint(0, 8, C_BLACK, "%s", record->Artist);
	dprint(0, 24, C_BLACK, "%uK  OD:%.1f", record->ColumnCount, record->OverallDifficulty);
	dprint(0, 40, C_BLACK, "Note count:");

	{
		char buffer[48] = {};
		char *p = buffer;

		for (size_t i = 0; i < record->ColumnCount; i += 1)
			p += sprintf(p, i == 0 ? "%d" : ",%d", record->ColumnSize[i]);

		dtext(0, 48, C_BLACK, buffer);
	}

	dupdate();
	getkey();
}
