#pragma once

#include <fxTap/config.h>
#include <fxTap/keymap.h>
#include <gint/keyboard.h>

const char *KeyCode_ToString(uint8_t key);

void NotesFallingTime_Render(const FXT_Config *config);

void NotesFallingTime_AcceptEvent(key_event_t e, FXT_Config *config);

extern FxTapKey selectedKeyBinding;

void KeyBindings_Render(const FXT_Config *config);

void KeyBindings_AcceptEvent(key_event_t e, FXT_Config *config);

void KeyBindingStyle_Render(const FXT_Config *config);

void KeyBindingStyle_AcceptEvent(key_event_t e, FXT_Config *config);

void Language_Render(const FXT_Config *config);

void Language_AcceptEvent(key_event_t e, FXT_Config *config);
