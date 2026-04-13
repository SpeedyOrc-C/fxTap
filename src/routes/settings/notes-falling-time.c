#include "assets.h"
#include "settings.h"

static const char *Text_NoteFallingTime(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "音符下落时间";
	default:
		dfont(&Font_Piczel);
		return "Note Falling Time";
	}
}

void NotesFallingTime_Render(const FXT_Config *config)
{
	dtext(1, 13, C_BLACK, Text_NoteFallingTime(config));
	dfont(&Font_Piczel);
	dprint(42, 29, C_BLACK, "%4d ms", config->NotesFallingTime);
	dimage(0, 56, &Img_Settings_NotesFallingTime_FN_EN);
}

void AdjustNotesFallingTime(FXT_Config *config, const int16_t delta)
{
	config->NotesFallingTime += delta;

	if (config->NotesFallingTime < 0)
		config->NotesFallingTime = 0;
	else if (config->NotesFallingTime > 10000)
		config->NotesFallingTime = 10000;
}

void NotesFallingTime_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	switch (e.key)
	{
	case KEY_F1:
		AdjustNotesFallingTime(config, -100);
		break;
	case KEY_F2:
		AdjustNotesFallingTime(config, -10);
		break;
	case KEY_F3:
	case KEY_LEFT:
		AdjustNotesFallingTime(config, -1);
		break;
	case KEY_F4:
	case KEY_RIGHT:
		AdjustNotesFallingTime(config, 1);
		break;
	case KEY_F5:
		AdjustNotesFallingTime(config, 10);
		break;
	case KEY_F6:
		AdjustNotesFallingTime(config, 100);
		break;
	default:
		break;
	}
}
