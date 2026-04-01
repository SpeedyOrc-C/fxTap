#include <fxTap/database.h>
#include <gint/keyboard.h>
#include "assets.h"

void UI_Play_Detail(const FXT_DatabaseRecord *record)
{
	// TODO)) Allow word wrapping in strings
	dclear(C_WHITE);

	dprint(0, 0, C_BLACK, "[TITLE]");
	dprint(0, 8, C_BLACK, "%s", record->Title);

	dprint(0, 16, C_BLACK, "[ARTIST]");
	dprint(0, 24, C_BLACK, "%s", record->Artist);

	dprint(0, 32, C_BLACK, "[KEY] %u   [OD] %.2f", record->ColumnCount, record->OverallDifficulty);

	dprint(0, 40, C_BLACK, "[COLUMN SIZES]");
	switch (record->ColumnCount)
	{
	case 4:
		dprint(0, 48, C_BLACK, "%d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3]
		);
		break;
	case 5:
		dprint(0, 48, C_BLACK, "%d %d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3],
		       record->ColumnSize[4]
		);
		break;
	case 6:
		dprint(0, 48, C_BLACK, "%d %d %d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3],
		       record->ColumnSize[4],
		       record->ColumnSize[5]
		);
		break;
	default:
		break;
	}

	dupdate();
	getkey();
}
