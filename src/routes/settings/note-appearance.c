#include <gint/display.h>
#include "assets.h"
#include "settings.h"

static const char *Text_NoteAppearance(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "音符外观";
	default:
		dfont(&Font_Piczel);
		return "Note Appearance";
	}
}

void Appearance_Render(const FXT_Config *config)
{
	const int x1 = (DWIDTH - config->ColumnWidth) / 2 + 1;
	const int y1 = (DHEIGHT - config->TapNoteHeight) / 2 + 3;
	const int x2 = x1 + config->ColumnWidth - 1;
	const int y2 = y1 + config->TapNoteHeight - 1;

	dtext(1, 13, C_BLACK, Text_NoteAppearance(config));
	drect(x1, y1, x2, y2, C_BLACK);
	dimage(0, 56, &Img_Settings_NoteAppearance_FN);
	dfont(&Font_Piczel);
	dprint_opt(
		DWIDTH / 2, DHEIGHT - 1, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_BOTTOM,
		"%2u*%u", config->ColumnWidth, config->TapNoteHeight
	);
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
