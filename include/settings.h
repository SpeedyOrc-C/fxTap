#pragma once

#include <fxTap/config.h>
#include <fxTap/keymap.h>
#include <gint/keyboard.h>

void NotesFallingTime_Render(const FXT_Config *config);

void NotesFallingTime_AcceptEvent(key_event_t e, FXT_Config *config);

void Appearance_Render(const FXT_Config *config);

void Appearance_AcceptEvent(key_event_t e, FXT_Config *config);

extern FXT_Key selectedKeyBinding;

void KeyBindings_Render(const FXT_Config *config);

void KeyBindings_AcceptEvent(key_event_t e, FXT_Config *config);

void KeyBindingStyle_Render(const FXT_Config *config);

void KeyBindingStyle_AcceptEvent(key_event_t e, FXT_Config *config);

void Language_Render(const FXT_Config *config);

void Language_AcceptEvent(key_event_t e, FXT_Config *config);

void CustomOverallDifficulty_Render(const FXT_Config *config);

void CustomOverallDifficulty_AcceptEvent(key_event_t e, FXT_Config *config);
