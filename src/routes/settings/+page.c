#include <fxTap/config.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include "assets.h"
#include "settings.h"
#include "ui.h"

static constexpr uint8_t ItemCount = 6;

typedef struct MenuItem
{
	void (*Render)(const FXT_Config *config);

	void (*AcceptEvent)(key_event_t e, FXT_Config *config);
} MenuItem;

static const char *Text_Settings(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "设置";
	default:
		dfont(&Font_Piczel);
		return "Settings";
	}
}

void UI_Settings(FXT_Config *config)
{
	const FXT_Config oldConfig = *config;

	const MenuItem items[ItemCount] = {
		{.Render = NotesFallingTime_Render, .AcceptEvent = NotesFallingTime_AcceptEvent},
		{.Render = CustomOverallDifficulty_Render, .AcceptEvent = CustomOverallDifficulty_AcceptEvent},
		{.Render = Appearance_Render, .AcceptEvent = Appearance_AcceptEvent},
		{.Render = KeyBindings_Render, .AcceptEvent = KeyBindings_AcceptEvent},
		{.Render = KeyBindingStyle_Render, .AcceptEvent = KeyBindingStyle_AcceptEvent},
		{.Render = Language_Render, .AcceptEvent = Language_AcceptEvent},
	};

	uint8_t item = 0;
	selectedKeyBinding = 0;

	while (true)
	{
		const bool settingsChanged = ! FXT_Config_Equal(&oldConfig, config);

		dclear(C_WHITE);
		drect(0, 0, 127, 10, C_BLACK);
		dtext(1, 1, C_WHITE, Text_Settings(config));
		dfont(&Font_Piczel);
		dprint_opt(
			DWIDTH - 3, 2, C_WHITE, C_NONE, DTEXT_RIGHT, DTEXT_TOP,
			"%c %d/%u", settingsChanged ? '*' : ' ', item + 1, ItemCount
		);
		items[item].Render(config);
		dupdate();

		const key_event_t e = getkey();

		if (e.key == KEY_EXIT)
		{
			if (settingsChanged)
			{
				FXT_ConfigError configError;

				if (gint[HWFS] == HWFS_CASIOWIN)
					configError = gint_call((gint_call_t){
						.function = &FXT_Config_Save_BFile,
						.args = {{.pv = config}},
					});
				else
					configError = FXT_Config_Save(config);

				if (configError)
				{
					dclear(C_WHITE);
					dprint(1, 1, C_BLACK, "%d", configError);
					dupdate();
					getkey();
					return;
				}
			}

			return;
		}

		if (e.key == KEY_UP)
		{
			if (item > 0)
				item -= 1;
			continue;
		}

		if (e.key == KEY_DOWN)
		{
			if (item < ItemCount - 1)
				item += 1;
			continue;
		}

		items[item].AcceptEvent(e, config);
	}
}
