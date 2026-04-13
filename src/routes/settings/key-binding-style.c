#include "assets.h"
#include "settings.h"

static const char *Text_KeyBindingStyle(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "按键绑定风格";
	default:
		dfont(&Font_Piczel);
		return "Key Binding Style";
	}
}

void KeyBindingStyle_Render(const FXT_Config *config)
{
	dtext(1, 13, C_BLACK, Text_KeyBindingStyle(config));
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
		config->KeyMapStyle = FXT_KeyMapStyle_Beatmania;
		break;
	case KEY_F6:
		config->KeyMapStyle = FXT_KeyMapStyle_CoOp;
		break;
	default:
		break;
	}
}
