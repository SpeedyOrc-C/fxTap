#include <settings.h>
#include <fxconv-assets.h>
#include <fxTap/keymap.h>
#include <assert.h>

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
