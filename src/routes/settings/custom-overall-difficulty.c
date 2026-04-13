#include "assets.h"
#include "settings.h"
#include "ui.h"

static const char *Text_CustomOverallDifficulty(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "自定义总体难度";
	default:
		dfont(&Font_Piczel);
		return "Custom Overall Difficulty";
	}
}

void CustomOverallDifficulty_Render(const FXT_Config *config)
{
	auto const od = (double) config->CustomOverallDifficulty10 / 10;
	auto const tolerance = FXT_Tolerance_FromOverallDifficulty((double) config->CustomOverallDifficulty10 / 10);

	dtext(1, 13, C_BLACK, Text_CustomOverallDifficulty(config));

	if (config->OverrideDefaultOverallDifficulty)
	{
		dfont(&Font_Piczel);
		dprint(42, 24, C_BLACK, "[OD: %.1f]", od);

		dprint(24, 34, C_BLACK, "%4i %4i %4i",
		       tolerance.Perfect, tolerance.Great, tolerance.Good);

		dprint(24, 44, C_BLACK, "%4i %4i %4i",
		       tolerance.Ok, tolerance.Meh, tolerance.Miss);
	}

	{
		auto const image =
				config->OverrideDefaultOverallDifficulty
					? &Img_Settings_CustomOverallDifficulty_Enabled_FN
					: &Img_Settings_CustomOverallDifficulty_Disabled_FN;

		auto const top = 8 * config->Language;

		dsubimage(0, 56, image, 0, top, 128, 8, 0);
	}
}

void CustomOverallDifficulty_AcceptEvent(const key_event_t e, FXT_Config *config)
{
	switch (e.key)
	{
	case KEY_F1:
		if (config->OverrideDefaultOverallDifficulty && config->CustomOverallDifficulty10 >= 5)
			config->CustomOverallDifficulty10 -= 5;
		break;
	case KEY_F2:
		if (config->OverrideDefaultOverallDifficulty && config->CustomOverallDifficulty10 >= 1)
			config->CustomOverallDifficulty10 -= 1;
		break;
	case KEY_F3:
	case KEY_F4:
		config->OverrideDefaultOverallDifficulty = ! config->OverrideDefaultOverallDifficulty;
		config->CustomOverallDifficulty10 = 70;
		break;
	case KEY_F5:
		if (config->OverrideDefaultOverallDifficulty && config->CustomOverallDifficulty10 <= 199)
			config->CustomOverallDifficulty10 += 1;
		break;
	case KEY_F6:
		if (config->OverrideDefaultOverallDifficulty && config->CustomOverallDifficulty10 <= 195)
			config->CustomOverallDifficulty10 += 5;
		break;
	default:
		break;
	}
}
