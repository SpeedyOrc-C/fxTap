#ifndef FXTAP_SETTINGS
#define FXTAP_SETTINGS

#include <fxTap/config.h>
#include <fxTap/keymap.h>
#include <gint/keyboard.h>

void NotesFallingTime_Render(const Config *config);
void NotesFallingTime_AcceptEvent(key_event_t e, Config *config);

extern FxTapKey selectedKeyBinding;
void KeyBindings_Render(const Config *config);
void KeyBindings_AcceptEvent(key_event_t e, Config *config);

void KeyBindingStyle_Render(const Config *config);
void KeyBindingStyle_AcceptEvent(key_event_t e, Config *config);

void Language_Render(const Config *config);
void Language_AcceptEvent(key_event_t e, Config *config);

#endif //FXTAP_SETTINGS
