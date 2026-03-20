#include "fxconv-assets.h"
#include "settings.h"
#include "ui.h"

void CustomOverallDifficulty_Render(const FXT_Config *config)
{
	auto const od = (double) config->CustomOverallDifficulty10 / 10;
	auto const tolerance = FXT_Tolerance_FromOverallDifficulty((double) config->CustomOverallDifficulty10 / 10);

	dsubimage(0, 12, &Img_Settings_CustomOverallDifficulty_Caption, 0, 10 * config->Language, 128, 10, 0);

	if (config->OverrideDefaultOverDifficulty)
	{
		dprint(42, 24, C_BLACK, "[OD: %.1f]", od);

		dprint(24, 34, C_BLACK, "%4i %4i %4i",
		       tolerance.Perfect, tolerance.Great, tolerance.Good);

		dprint(24, 44, C_BLACK, "%4i %4i %4i",
		       tolerance.Ok, tolerance.Meh, tolerance.Miss);
	}

	{
		auto const image =
				config->OverrideDefaultOverDifficulty
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
		if (config->OverrideDefaultOverDifficulty && config->CustomOverallDifficulty10 >= 5)
			config->CustomOverallDifficulty10 -= 5;
		break;
	case KEY_F2:
		if (config->OverrideDefaultOverDifficulty && config->CustomOverallDifficulty10 >= 1)
			config->CustomOverallDifficulty10 -= 1;
		break;
	case KEY_F3:
	case KEY_F4:
		config->OverrideDefaultOverDifficulty = ! config->OverrideDefaultOverDifficulty;
		config->CustomOverallDifficulty10 = 70;
		break;
	case KEY_F5:
		if (config->OverrideDefaultOverDifficulty && config->CustomOverallDifficulty10 <= 199)
			config->CustomOverallDifficulty10 += 1;
		break;
	case KEY_F6:
		if (config->OverrideDefaultOverDifficulty && config->CustomOverallDifficulty10 <= 195)
			config->CustomOverallDifficulty10 += 5;
		break;
	default:
		break;
	}
}
