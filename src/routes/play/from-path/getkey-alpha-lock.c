#include "getkey-alpha-lock.h"
#include <gint/keyboard.h>

KeyboardAlpha KeyboardAlphaState = KA_Idle;

key_event_t getkey_alpha_lock(void)
{
	auto e = getkey_opt(GETKEY_DEFAULT ^ GETKEY_MOD_ALPHA, nullptr);

	if (e.key == KEY_F5)
		return e;

	switch (KeyboardAlphaState)
	{
	case KA_Idle:
		if (e.key == KEY_ALPHA)
			KeyboardAlphaState = e.shift ? KA_Locked : KA_Active;
		break;
	case KA_Active:
		if (e.key == KEY_ALPHA)
			KeyboardAlphaState = e.shift ? KA_Locked : KA_Idle;
		else
			KeyboardAlphaState = KA_Idle, e.alpha = true;
		break;
	case KA_Locked:
		if (e.key == KEY_ALPHA)
			KeyboardAlphaState = e.shift ? KA_Locked : KA_Idle;
		else
			e.alpha = true;
		break;
	}

	return e;
}
