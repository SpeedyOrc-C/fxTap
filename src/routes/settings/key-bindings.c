#include <assert.h>
#include <fxTap/keymap.h>
#include "assets.h"
#include "settings.h"
#include "utils/keycode.h"

FXT_Key selectedKeyBinding;

void KeyBindings_Render(const FXT_Config *config)
{
	dsubimage(0, 12, &Img_Settings_KeyBindings_Caption, 0, 50 * config->Language, 128, 50, 0);

	const char *keyString = KeyCode_ToString(config->PhysicalKeyOfFxTapKey[selectedKeyBinding]);
	assert(FXT_Key_K1 <= selectedKeyBinding && selectedKeyBinding <= FXT_Key_S1);

	drect_border(92, 26, 106, 36, C_WHITE, 1, C_BLACK);
	dtext(94, 28, C_BLACK, FXT_Key_ToString(selectedKeyBinding));
	drect_border(92, 40, 124, 50, C_WHITE, 1, C_BLACK);
	dtext(94, 42, C_BLACK, keyString != nullptr ? keyString : "UNSET");
}

void KeyBindings_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	assert(0 <= selectedKeyBinding && selectedKeyBinding <= FXT_Key_S1);

	switch (e.key)
	{
	case KEY_LEFT:
		if (selectedKeyBinding > FXT_Key_K1)
			selectedKeyBinding -= 1;
		break;

	case KEY_RIGHT:
		if (selectedKeyBinding < FXT_Key_S1)
			selectedKeyBinding += 1;
		break;

	default:
		if (KeyCode_ToString(e.key) != nullptr)
			config->PhysicalKeyOfFxTapKey[selectedKeyBinding] = e.key;
		break;
	}
}
