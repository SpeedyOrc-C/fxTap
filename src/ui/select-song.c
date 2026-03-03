#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <gint/keyboard.h>
#include "fxconv-assets.h"
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
		dimage(0, 56, &Img_Path_FN);
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
				auto const c = AlphaFromKeyCode(e.key);

				if (c != 255)
				{
					fileName[cursor] = isCapitalised ? (char) c : tolower(c);
					cursor += 1;
				}
			}
			else
			{
				auto const c = keycode_digit(e.key);

				if (0 <= c && c <= 9)
				{
					fileName[cursor] = (char) ('0' + c);
					cursor += 1;
				}
			}
			break;
		}
	}
}

char *UI_AskBeatmapPath_ListLibrary(const FXT_Config *config)
{
	while (true)
	{
		dclear(C_WHITE);
		dsubimage(1, 1, &Img_SelectASong_Title, 0, 10 * config->Language, 128, 10, 0);
		drect(0, 0, 127, 10, C_INVERT);
		dsubimage(0, 56, &Img_SelectASong_FN, 0, 8 * config->Language, 128, 8, 0);
		dupdate();

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_F5:
			return UI_AskBeatmapPath_TypeFileNameManually(config);
		case KEY_EXIT:
			return nullptr;
		default: break;
		}
	}
}
