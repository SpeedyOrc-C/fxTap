#include <settings.h>
#include <fxconv-assets.h>

void Language_Render(const Config *config)
{
    dsubimage(0, 12, &Img_Settings_Language_Caption, 0, 10 * config->Language, 128, 10, 0);
    dsubimage(48, 29, &Img_Settings_Language_Options, 0, 10 * config->Language, 48, 10, 0);
    dsubimage(0, 56, &Img_FN_PreviousNext, 0, 8 * config->Language, 128, 8, 0);
}

void Language_AcceptEvent(key_event_t e, Config *config)
{
    switch (e.key)
    {
        case KEY_F1:
        case KEY_F2:
        case KEY_LEFT:
            if (config->Language > EN_US)
                config->Language -= 1;
            break;

        case KEY_F5:
        case KEY_F6:
        case KEY_RIGHT:
            if (config->Language < ZH_CN)
                config->Language += 1;
            break;
    }
}
