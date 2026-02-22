#include <fxTap/config.h>
#include <fxTap/keymap.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include "settings.h"
#include "ui.h"

static const char *ShowPhysicalKey(const FXT_Config *config, FxTapKey key)
{
	auto const physicalKeyCode = config->PhysicalKeyOfFxTapKey[key];

	if (physicalKeyCode == 0)
		return "UNSET";

	return KeyCode_ToString(physicalKeyCode);
}

void UI_KeyTest(const FXT_Config *config)
{
	const char *keyNames[FXT_MaxKeyCount] = {
		ShowPhysicalKey(config, FxTapKey_K1),
		ShowPhysicalKey(config, FxTapKey_K2),
		ShowPhysicalKey(config, FxTapKey_K3),
		ShowPhysicalKey(config, FxTapKey_K4),
		ShowPhysicalKey(config, FxTapKey_K5),
		ShowPhysicalKey(config, FxTapKey_K6),
		ShowPhysicalKey(config, FxTapKey_K7),
		ShowPhysicalKey(config, FxTapKey_K8),
		ShowPhysicalKey(config, FxTapKey_K9),
		ShowPhysicalKey(config, FxTapKey_S1),
	};

	while (true)
	{
		while (pollevent().type != KEYEV_NONE)
		{
		}

		dclear(C_WHITE);

		for (FxTapKey key = FxTapKey_K1; key <= FxTapKey_K5; key += 1)
		{
			const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

			dprint_opt(
				0, key * (1 + dfont_default()->line_height),
				isDown ? C_WHITE : C_BLACK,
				isDown ? C_BLACK : C_WHITE,
				DTEXT_LEFT, DTEXT_TOP,
				"%s %s",
				FxTapKey_ToString(key),
				keyNames[key]
			);
		}

		for (FxTapKey key = FxTapKey_K6; key <= FxTapKey_S1; key += 1)
		{
			const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

			dprint_opt(
				64, (key - FxTapKey_K6) * (1 + dfont_default()->line_height),
				isDown ? C_WHITE : C_BLACK,
				isDown ? C_BLACK : C_WHITE,
				DTEXT_LEFT, DTEXT_TOP,
				"%s %s",
				FxTapKey_ToString(key),
				keyNames[key]
			);
		}

		dupdate();

		if (keydown(KEY_EXIT))
			break;
	}
}
