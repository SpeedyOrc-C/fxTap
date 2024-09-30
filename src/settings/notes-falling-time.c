#include <settings.h>
#include <fxconv-assets.h>

void NotesFallingTime_Render(const Config *config)
{
    dsubimage(0, 12, &Img_Settings_NotesFallingTime_Caption, 0, 10 * config->Language, 128, 10, 0);
    dprint(42, 29, C_BLACK, "%4d ms", config->NotesFallingTime);
    dimage(0, 56, &Img_Settings_NotesFallingTime_FN_EN);
}

void AdjestNotesFallingTime(Config *config, int16_t delta)
{
    config->NotesFallingTime += delta;

    if (config->NotesFallingTime < 0)
        config->NotesFallingTime = 0;
    else if (config->NotesFallingTime > 10000)
        config->NotesFallingTime = 10000;
}

void NotesFallingTime_AcceptEvent(key_event_t e, Config *config)
{
    switch (e.key)
    {
        case KEY_F1:
            AdjestNotesFallingTime(config, -100); break;
        case KEY_F2:
            AdjestNotesFallingTime(config, -10 ); break;
        case KEY_F3: case KEY_LEFT:
            AdjestNotesFallingTime(config, -1  ); break;
        case KEY_F4: case KEY_RIGHT:
            AdjestNotesFallingTime(config,  1  ); break;
        case KEY_F5:
            AdjestNotesFallingTime(config,  10 ); break;
        case KEY_F6:
            AdjestNotesFallingTime(config,  100); break;
    }
}
