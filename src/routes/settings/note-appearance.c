#include <gint/display.h>
#include "assets.h"
#include "settings.h"

void Appearance_Render(const FXT_Config *config)
{
	const int x1 = (DWIDTH - config->ColumnWidth) / 2 + 1;
	const int y1 = (DHEIGHT - config->TapNoteHeight) / 2 + 3;
	const int x2 = x1 + config->ColumnWidth - 1;
	const int y2 = y1 + config->TapNoteHeight - 1;

	dsubimage(0, 12, &Img_Settings_NoteAppearance_Caption, 0, 10 * config->Language, 128, 10, 0);
	drect(x1, y1, x2, y2, C_BLACK);
	dimage(0, 56, &Img_Settings_NoteAppearance_FN);
	dprint(49, 56, C_BLACK, "%2u*%u", config->ColumnWidth, config->TapNoteHeight);
}

void Appearance_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	switch (e.key)
	{
	case KEY_F1:
		if (config->ColumnWidth > 1)
			config->ColumnWidth -= 1;
		break;

	case KEY_F2:
		if (config->TapNoteHeight > 1)
			config->TapNoteHeight -= 1;
		break;

	case KEY_F5:
		if (config->TapNoteHeight < 10)
			config->TapNoteHeight += 1;
		break;

	case KEY_F6:
		if (config->ColumnWidth < 24)
			config->ColumnWidth += 1;
		break;

	default:
		break;
	}
}
