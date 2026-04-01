#pragma once

#include <gint/keyboard.h>

typedef enum KeyboardAlpha
{
	KA_Idle,
	KA_Active,
	KA_Locked,
} KeyboardAlpha;

extern KeyboardAlpha KeyboardAlphaState;

key_event_t getkey_alpha_lock(void);
