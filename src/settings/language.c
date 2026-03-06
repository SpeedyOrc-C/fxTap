#include <assert.h>
#include <fxconv-assets.h>
#include <settings.h>

void Language_Render(const FXT_Config *config)
{
	dsubimage(0, 12, &Img_Settings_Language_Caption, 0, 10 * config->Language, 128, 10, 0);
	dsubimage(48, 29, &Img_Settings_Language_Options, 0, 10 * config->Language, 48, 10, 0);
	dsubimage(0, 56, &Img_Settings_Language_FN, 0, 0, 128, 8, 0);
}

void Language_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	switch (e.key)
	{
	case KEY_F1:
	case KEY_F2:
		config->Language = FXT_Language_EnUs;
		break;
	case KEY_F5:
	case KEY_F6:
		config->Language = FXT_Language_ZhCn;
		break;
	default:
		break;
	}
}
