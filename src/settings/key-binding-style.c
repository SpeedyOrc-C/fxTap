#include <assert.h>
#include <fxconv-assets.h>
#include <settings.h>

void KeyBindingStyle_Render(const FXT_Config *config)
{
	dsubimage(0, 12, &Img_Settings_KeyBindingStyle_Caption, 0, 10 * config->Language, 128, 10, 0);
	dsubimage(0, 30, &Img_Settings_KeyBindingStyle_Logos, 0, 16 * config->KeyMapStyle, 128, 16, 0);
	dimage(0, 56, &Img_Settings_KeyBindingStyle_FN_EN);
}

void KeyBindingStyle_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	switch (e.key)
	{
	case KEY_F1:
	case KEY_F2:
		config->KeyMapStyle = FXT_KeyMapStyle_DJMAX;
		break;
	case KEY_F3:
	case KEY_F4:
	case KEY_F5:
		config->KeyMapStyle = FXT_KeyMapStyle_BeatmaniaIIDX;
		break;
	case KEY_F6:
		config->KeyMapStyle = FXT_KeyMapStyle_CoOp;
		break;
	default:
		assert(false && "Invalid key");
	}
}
