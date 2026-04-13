#include <ctype.h>
#include <stb_ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gint/keyboard.h>
#include "assets.h"
#include "getkey-alpha-lock.h"
#include "ui.h"

static uint8_t AlphaFromKeyCode(const int keyCode)
{
	const int row = keyCode >> 4;
	const int column = keyCode & 0xf;

	switch (row)
	{
	case 6: return 'A' + column - 1;
	case 5: return 'G' + column - 1;
	case 4: return 'M' + column - 1;
	case 3: return 'P' + column - 1;
	case 2: return 'U' + column - 1;
	case 1:
		switch (column)
		{
		case 1: return 'Z';
		case 2: return ' ';
		case 3: return '\"';
		default: return -1;
		}
	default: return 0;
	}
}

static const char *Text_EnterBeatmapPath(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "输入谱面路径……";
	default:
		dfont(&Font_Piczel);
		return "Enter beatmap's path...";
	}
}

static const char *Text_YouCanOmitItsExtension(const FXT_Config *c)
{
	switch (c->Language)
	{
	case FXT_Language_ZhCn:
		dfont(&Font_Fusion9x9);
		return "你可以忽略文件扩展名。";
	default:
		dfont(&Font_Piczel);
		return "You can omit its extension.";
	}
}

[[nodiscard]]
OCharP UI_Play_FromPath(const FXT_Config *config)
{
	constexpr int FileNameMaxLen = 47;
	bool isCapitalised = true;
	int cursor = 0;
	// ReSharper disable once CppDFAMemoryLeak
	char *fileName = malloc(FileNameMaxLen + 4 + 1);

	assert(fileName != nullptr);

	memset(fileName, 0, FileNameMaxLen + 1);
	KeyboardAlphaState = KA_Locked;

	while (true)
	{
		dclear(C_WHITE);
		drect(0, 0, DWIDTH - 1, 10, C_BLACK);
		dtext(1, 1, C_WHITE, Text_EnterBeatmapPath(config));
		dtext(1, 13, C_BLACK, Text_YouCanOmitItsExtension(config));

		drect_border(0, 33, DWIDTH - 1, 45, C_NONE, 1, C_BLACK);
		dtext(2, 35, C_BLACK, fileName);
		dprint_opt(DWIDTH - 1, 48, C_BLACK, C_WHITE, DTEXT_RIGHT, DTEXT_TOP,
		           KeyboardAlphaState == KA_Idle
			           ? "[1]"
			           : KeyboardAlphaState == KA_Active
				             ? isCapitalised
					               ? "[A]"
					               : "[a]"
				             : isCapitalised
					               ? "L[A]"
					               : "L[a]");
		dimage(0, 56, &Img_Path_FN);
		dupdate();

		const key_event_t e = getkey_alpha_lock();

		switch (e.key)
		{
		case KEY_DEL:
			if (cursor > 0)
			{
				cursor -= 1;
				fileName[cursor] = 0;
			}
			break;

		case KEY_ACON:
			cursor = 0;
			memset(fileName, 0, FileNameMaxLen);
			break;

		case KEY_EXIT:
			free(fileName);
			return (OCharP){.Path = nullptr};

		case KEY_EXE:
			if (cursor <= 3
			    || fileName[cursor - 4] != '.'
			    || fileName[cursor - 3] != 'f'
			    || fileName[cursor - 2] != 'x'
			    || fileName[cursor - 1] != 't'
			)
			{
				fileName[cursor] = '.';
				fileName[cursor + 1] = 'f';
				fileName[cursor + 2] = 'x';
				fileName[cursor + 3] = 't';
				fileName[cursor + 4] = 0;
			}

			// ReSharper disable once CppDFAMemoryLeak
			return (OCharP){.Path = fileName, .NeedFree = true};

		case KEY_F1:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '/';
				cursor += 1;
			}
			break;

		case KEY_F2:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '.';
				cursor += 1;
			}
			break;

		case KEY_F3:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '_';
				cursor += 1;
			}
			break;

		case KEY_F4:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '~';
				cursor += 1;
			}
			break;

		case KEY_F5:
			isCapitalised = ! isCapitalised;
			break;

		default:
			if (cursor >= FileNameMaxLen) break;

			if (e.alpha)
			{
				auto const c = AlphaFromKeyCode(e.key);

				if (c != 0)
				{
					fileName[cursor] = isCapitalised ? (char) c : tolower(c);
					cursor += 1;
				}
			}
			else
			{
				auto const c = keycode_digit(e.key);

				if (c != -1)
				{
					fileName[cursor] = (char) ('0' + c);
					cursor += 1;
				}
			}
			break;
		}
	}
}
