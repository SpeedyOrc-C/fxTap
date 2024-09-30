#include "ui.h"
#include <gint/display.h>
#include <fxTap/config.h>
#include <fxTap/keymap.h>

void UI_KeyTest(const Config *config)
{
    while (true)
    {
        while (pollevent().type != KEYEV_NONE);

        dclear(C_WHITE);

        for (FxTapKey key = FxTapKey_K1; key <= FxTapKey_K5; key += 1)
        {
            const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

            dprint_opt(
                36, key * (1 + dfont_default()->line_height),
                isDown ? C_WHITE : C_BLACK,
                isDown ? C_BLACK : C_WHITE,
                DTEXT_LEFT, DTEXT_TOP,
                FxTapKey_ToString(key)
            );
        }

        for (FxTapKey key = FxTapKey_K6; key <= FxTapKey_S1; key += 1)
        {
            const bool isDown = keydown(config->PhysicalKeyOfFxTapKey[key]);

            dprint_opt(
                80, (key - FxTapKey_K6) * (1 + dfont_default()->line_height),
                isDown ? C_WHITE : C_BLACK,
                isDown ? C_BLACK : C_WHITE,
                DTEXT_LEFT, DTEXT_TOP,
                FxTapKey_ToString(key)
            );
        }

        dupdate();

        if (keydown(KEY_EXIT))
            break;
    }
}
