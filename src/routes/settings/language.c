#include "assets.h"
#include "settings.h"

static const char *Text_Language(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "语言";
	default:
		dfont(&Font_Piczel);
		return "Language";
	}
}

static const char *Text_LanguageOption(const FXT_Language language)
{
	switch (language)
	{
	case FXT_Language_ZhCn:
		return "简体中文（中国大陆）";
	default:
		return "English (United States)";
	}
}

void Language_Render(const FXT_Config *config)
{
	dtext(1, 13, C_BLACK, Text_Language(config));
	dprint_opt(DWIDTH / 2, DHEIGHT / 2, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, Text_LanguageOption(config->Language));
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
