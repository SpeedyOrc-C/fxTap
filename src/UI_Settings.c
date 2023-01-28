#include "router.h"
#include <gint/hardware.h>
#include <fxTap/keymap.h>
#include <fxTap/config.h>
#include <string.h>
#include <assert.h>

#define ITEM_COUNT 4

typedef struct Item
{
    void (*Render)(const Config *config);
    void (*AcceptEvent)(key_event_t e, Config *config);
} Item;

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

FxTapKey selectedKeyBinding;

const char * KeyCode_ToString(uint8_t key)
{
    switch (key)
    {
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_XOT: return "X,O,T";
        case KEY_LOG: return "log";
        case KEY_LN: return "ln";
        case KEY_SIN: return "sin";
        case KEY_COS: return "cos";
        case KEY_TAN: return "tan";
        case KEY_FRAC: return "ab/c";
        case KEY_FD: return "F-D";
        case KEY_LEFTP: return "(";
        case KEY_RIGHTP: return ")";
        case KEY_COMMA: return ",";
        case KEY_ARROW: return "->";
        case KEY_7: return "7";
        case KEY_8: return "8";
        case KEY_9: return "9";
        case KEY_DEL: return "DEL";
        case KEY_ACON: return "AC/ON";
        case KEY_4: return "4";
        case KEY_5: return "5";
        case KEY_6: return "6";
        case KEY_MUL: return "*";
        case KEY_DIV: return "/";
        case KEY_1: return "1";
        case KEY_2: return "2";
        case KEY_3: return "3";
        case KEY_ADD: return "+";
        case KEY_SUB: return "-";
        case KEY_0: return "0";
        case KEY_DOT: return ".";
        case KEY_EXP: return "EXP";
        case KEY_NEG: return "(-)";
        case KEY_EXE: return "EXE";
        default: return NULL;
    }
}

void KeyBindings_Render(const Config *config)
{
    dsubimage(0, 12, &Img_Settings_KeyBindings_Caption,
        0, 50 * config->Language, 128, 50, 0);

    const char * keyString = KeyCode_ToString(config->PhysicalKeyOfFxTapKey[selectedKeyBinding]);
    assert(0 <= selectedKeyBinding && selectedKeyBinding < MAX_KEY_COUNT);

    drect_border(92, 26, 106, 36, C_WHITE, 1, C_BLACK);
    dtext(94, 28, C_BLACK, FxTapKey_ToString(selectedKeyBinding));
    drect_border(92, 40, 124, 50, C_WHITE, 1, C_BLACK);
    dtext(94, 42, C_BLACK, keyString != NULL ? keyString : "UNSET");
}

void KeyBindings_AcceptEvent(key_event_t e, Config *config)
{
    assert(0 <= selectedKeyBinding && selectedKeyBinding < MAX_KEY_COUNT);

    switch (e.key)
    {
        case KEY_LEFT:
            if (selectedKeyBinding > 0)
                selectedKeyBinding -= 1;
            break;

        case KEY_RIGHT:
            if (selectedKeyBinding < MAX_KEY_COUNT - 1)
                selectedKeyBinding += 1;
            break;

        default:
            if (KeyCode_ToString(e.key) != NULL)
                config->PhysicalKeyOfFxTapKey[selectedKeyBinding] = e.key;
            break;
    }
}

void KeyBindingStyle_Render(const Config *config)
{
    dsubimage(0, 12, &Img_Settings_KeyBindingStyle_Caption,
        0, 10 * config->Language, 128, 10, 0);
    dsubimage(0, 30, &Img_Settings_KeyBindingStyle_Logos,
        0, 16 * config->KeyMappingStyle, 128, 16, 0);
    dimage(0, 56, &Img_Settings_KeyBindingStyle_FN_EN);
}

void KeyBindingStyle_AcceptEvent(key_event_t e, Config *config)
{
    switch (e.key)
    {
        case KEY_F1:
        case KEY_F2:
            config->KeyMappingStyle = KeyMappingStyle_DJMAX;
            break;
        case KEY_F3:
        case KEY_F4:
        case KEY_F5:
            config->KeyMappingStyle = KeyMappingStyle_BeatmaniaIIDX;
            break;
        case KEY_F6:
            config->KeyMappingStyle = KeyMappingStyle_Coop;
            break;
    }
}

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

void UI_Settings(Config *config)
{
    const Config oldConfig = *config;

    const Item items[ITEM_COUNT] = {
        { .Render = &NotesFallingTime_Render
        , .AcceptEvent = &NotesFallingTime_AcceptEvent},
        { .Render = &KeyBindings_Render
        , .AcceptEvent = &KeyBindings_AcceptEvent},
        { .Render = &KeyBindingStyle_Render
        , .AcceptEvent = &KeyBindingStyle_AcceptEvent},
        { .Render = &Language_Render,
          .AcceptEvent = &Language_AcceptEvent},
    };

    int item = 0;
    selectedKeyBinding = 0;

    while (true)
    {
        const bool settingsChanged = memcmp(&oldConfig, config, sizeof(Config)) != 0;

        dclear(C_WHITE);
        dsubimage(1, 1, &Img_Settings_Title, 0, 10 * config->Language, 64, 10, 0);
        dprint(98, 2, C_BLACK, "%c %d/%d",
            settingsChanged ? '*' : ' ',
            item + 1,
            ITEM_COUNT
        );
        drect(0, 0, 127, 10, C_INVERT);
        items[item].Render(config);
        dupdate();

        const key_event_t e = getkey();

        if (e.key == KEY_EXIT)
        {
            if (settingsChanged)
			{
				ConfigError configError;

				#ifdef FX9860G
					if (gint[HWFS] == HWFS_CASIOWIN)
					{
						const gint_call_t call = {
							.function = &Config_SaveToDisk_BFile,
							.args = {{.pv = config}},
						};
						configError = gint_call(call);
					}
					else
						configError = Config_SaveToDisk(config);
				#endif

				#ifdef FXCG50
					configError = Config_SaveToDisk(config);
				#endif

				if (configError)
				{
					dclear(C_WHITE);
					dprint(1, 1, C_BLACK, "%d", configError);
					dupdate();
					getkey();
					return;
				}
			}

            return;
        }

        if (e.key == KEY_UP)
        {
            if (item > 0)
                item -= 1;
            continue;
        }

        if (e.key == KEY_DOWN)
        {
            if (item < ITEM_COUNT - 1)
                item += 1;
            continue;
        }

        items[item].AcceptEvent(e, config);
    }
}
