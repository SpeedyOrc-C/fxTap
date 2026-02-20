#include <assert.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
#include "ui.h"

#define ITEM_COUNT 3

void UI_About()
{
    int item = 0;

    while (true)
    {
        dclear(C_WHITE);

#ifdef FX9860G
        switch (item)
        {
            case 0:
                dsubimage(0, 0, &Img_About, 0, 0, 128, 64, 0);
                break;

            case 1:
                dsubimage(0, 0, &Img_About, 0, 64, 128, 64, 0);

                break;

            case 2:
                dsubimage(0, 0, &Img_About, 0, 128, 128, 64, 0);

                dtext(0, 12, C_BLACK, "GCC Version:");
                dprint(0, 12 + 1 + dfont_default()->line_height, C_BLACK,__VERSION__);
                dtext(0, 12 + 2 * (1 + dfont_default()->line_height), C_BLACK, "Compiled at:");
                dprint(0, 12 + 3 * (1 + dfont_default()->line_height), C_BLACK, "%s, %s", __DATE__, __TIME__);
                break;
            default:
                assert(false && "Invalid about page");
        }
#endif

        dupdate();

        const key_event_t e = getkey();

        if (e.key == KEY_EXIT)
            return;

        if (e.key == KEY_UP)
        {
            if (item > 0)
                item -= 1;
            continue;
        }

        if (e.key == KEY_DOWN || e.key == KEY_EXE)
        {
            if (item < ITEM_COUNT - 1)
                item += 1;
        }
    }
}
