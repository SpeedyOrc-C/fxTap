#include <fxTap/config.h>
#include <fxTap/keymap.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include "ui.h"
#include "utils/keycode.h"

static const char *ShowPhysicalKey(const FXT_Config *config, const FXT_Key key)
{
	auto const physicalKeyCode = config->PhysicalKeyOfFxTapKey[key];

	if (physicalKeyCode == 0)
		return "";

	return KeyCode_ToString(physicalKeyCode);
}

void UI_KeyTest(const FXT_Config *config)
{
	const char *keyNames[FXT_MaxKeyCount] = {
		ShowPhysicalKey(config, FXT_Key_K1),
		ShowPhysicalKey(config, FXT_Key_K2),
		ShowPhysicalKey(config, FXT_Key_K3),
		ShowPhysicalKey(config, FXT_Key_K4),
		ShowPhysicalKey(config, FXT_Key_K5),
		ShowPhysicalKey(config, FXT_Key_K6),
		ShowPhysicalKey(config, FXT_Key_K7),
		ShowPhysicalKey(config, FXT_Key_K8),
		ShowPhysicalKey(config, FXT_Key_K9),
		ShowPhysicalKey(config, FXT_Key_S1),
	};

	while (true)
	{
		while (pollevent().type != KEYEV_NONE)
		{
		}

		dclear(C_WHITE);

		for (FXT_Key key = FXT_Key_K1; key <= FXT_Key_K5; key += 1)
		{
			const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

			dprint_opt(
				1, key * 11 + 1,
				isDown ? C_WHITE : C_BLACK,
				isDown ? C_BLACK : C_WHITE,
				DTEXT_LEFT, DTEXT_TOP,
				"[%s] %s",
				FXT_Key_ToString(key),
				keyNames[key]
			);
		}

		for (FXT_Key key = FXT_Key_K6; key <= FXT_Key_S1; key += 1)
		{
			const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

			dprint_opt(
				64, (key - FXT_Key_K6) * 11 + 1,
				isDown ? C_WHITE : C_BLACK,
				isDown ? C_BLACK : C_WHITE,
				DTEXT_LEFT, DTEXT_TOP,
				"[%s] %s",
				FXT_Key_ToString(key),
				keyNames[key]
			);
		}

		dupdate();

		if (keydown(KEY_EXIT))
			break;
	}
}
