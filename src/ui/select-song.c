#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
#include "ui.h"

uint8_t keycode_alpha(const int keycode)
{
	const int Row = keycode >> 4;
	const int Column = keycode & 0xf;

	switch (Row)
	{
	case 6: return 'A' + Column - 1;
	case 5: return 'G' + Column - 1;
	case 4: return 'M' + Column - 1;
	case 3: return 'P' + Column - 1;
	case 2: return 'U' + Column - 1;
	case 1:
		switch (Column)
		{
		case 1: return 'Z';
		case 2: return ' ';
		case 3: return '\"';
		default: return -1;
		}
	default: return -1;
	}
}

char *UI_AskBeatmapPath_TypeFileNameManually(const FXT_Config *config)
{
	constexpr int FileNameMaxLen = 47;
	bool isCapitalised = true;
	bool isAlpha = true;
	int cursor = 0;
	char *fileName = malloc(FileNameMaxLen + 1);

	assert(fileName != nullptr);

	memset(fileName, 0, FileNameMaxLen + 1);

	while (true)
	{
		dclear(C_WHITE);
		dsubimage(0, 0, &Img_SelectSong_TypeFilename, 0, 30 * config->Language, 128, 30, 0);
		dprint(0, 35, C_BLACK, "[%s]", fileName);
		dprint(98, 48, C_BLACK, isAlpha ? (isCapitalised ? "[ABC]" : "[abc]") : "[123]");
		dimage(0, 56, &Img_FN_SelectSong);
		dupdate();

		const key_event_t e = getkey();

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
			return nullptr;

		case KEY_EXE:
			return fileName;

		case KEY_F1:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '.';
				cursor += 1;
			}
			break;

		case KEY_F2:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '_';
				cursor += 1;
			}
			break;

		case KEY_F3:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '~';
				cursor += 1;
			}
			break;

		case KEY_F4:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '\'';
				cursor += 1;
			}
			break;

		case KEY_F5:
			isAlpha = !isAlpha;
			break;

		case KEY_F6:
			isCapitalised = !isCapitalised;
			break;

		case KEY_NEG:
			if (cursor < FileNameMaxLen)
			{
				fileName[cursor] = '/';
				cursor += 1;
			}
			break;

		default:
			if (cursor >= FileNameMaxLen) break;

			if (isAlpha)
			{
				auto const c = keycode_alpha(e.key);

				if (c != 255)
				{
					fileName[cursor] = isCapitalised ? (char) c : tolower(c);
					cursor += 1;
				}
			}
			else
			{
				auto const c = keycode_digit(e.key);

				if (c != 255)
				{
					fileName[cursor] = '0' + c;
					cursor += 1;
				}
			}
			break;
		}
	}
}
