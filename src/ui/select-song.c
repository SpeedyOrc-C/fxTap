#include <ctype.h>
#include <stb_ds.h>
#include <stdlib.h>
#include <string.h>
#include <fxTap/database.h>
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
			isAlpha = ! isAlpha;
			break;

		case KEY_F6:
			isCapitalised = ! isCapitalised;
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

void UI_ShowBeatmapDetail(const FXT_DatabaseRecord *record)
{
	dclear(C_WHITE);

	dprint(0, 0, C_BLACK, "[TITLE]");
	dprint(0, 8, C_BLACK, "%s", record->Title);

	dprint(0, 16, C_BLACK, "[ARTIST]");
	dprint(0, 24, C_BLACK, "%s", record->Artist);

	dprint(0, 32, C_BLACK, "[KEY] %u   [OD] %.2f", record->ColumnCount, record->OverallDifficulty);

	dprint(0, 40, C_BLACK, "[COLUMN SIZES]");
	switch (record->ColumnCount)
	{
	case 4:
		dprint(0, 48, C_BLACK, "%d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3]
		);
		break;
	case 5:
		dprint(0, 48, C_BLACK, "%d %d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3],
		       record->ColumnSize[4]
		);
		break;
	case 6:
		dprint(0, 48, C_BLACK, "%d %d %d %d %d %d",
		       record->ColumnSize[0],
		       record->ColumnSize[1],
		       record->ColumnSize[2],
		       record->ColumnSize[3],
		       record->ColumnSize[4],
		       record->ColumnSize[5]
		);
		break;
	default:
		break;
	}

	dupdate();
	getkey();
}

char *UI_AskBeatmapPath_ListLibrary(const FXT_Config *config, const FXT_Database *database)
{
	auto const db = *database;

	const size_t size = shlenu(db);

	size_t selectedIndex = 0;

	while (true)
	{
		dclear(C_WHITE);

		dsubimage(1, 1, &Img_SelectASong_Title, 0, 10 * config->Language, 128, 10, 0);
		drect(0, 0, 127, 10, C_INVERT);
		dsubimage(0, 56, &Img_SelectASong_FN, 0, 8 * config->Language, 128, 8, 0);

		if (size == 0)
		{
			dtext(56, 27, C_BLACK, "???");
		}
		else
		{
			// A/B means the A-th is being selected now and there are B beatmaps
			dprint(93, 2, C_WHITE, "%2u/%u", selectedIndex + 1, size);

			// Draw 2 beatmaps before
			if (selectedIndex >= 2)
				dtext(1, 13, C_BLACK, db[selectedIndex - 2].value.Title);
			if (selectedIndex >= 1)
				dtext(1, 21, C_BLACK, db[selectedIndex - 1].value.Title);

			// Draw selected beatmap
			dtext(1, 30, C_BLACK, db[selectedIndex].value.Title);
			drect(0, 29, 127, 37, C_INVERT);

			// Draw 2 beatmaps after
			if (size - selectedIndex >= 2)
				dtext(1, 39, C_BLACK, db[selectedIndex + 1].value.Title);
			if (size - selectedIndex >= 3)
				dtext(1, 47, C_BLACK, db[selectedIndex + 2].value.Title);
		}

		dupdate();

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_DOWN:
		case KEY_RIGHT:
			if (size > 0 && selectedIndex < size - 1)
				selectedIndex += 1;
			break;

		case KEY_UP:
		case KEY_LEFT:
			if (size > 0 && selectedIndex > 0)
				selectedIndex -= 1;
			break;

		case KEY_VARS:
			UI_ShowBeatmapDetail(&db[selectedIndex].value);
			break;

		case KEY_EXIT:
			return nullptr;

		case KEY_F5: {
			auto const userPath = UI_AskBeatmapPath_TypeFileNameManually(config);
			if (userPath != nullptr)
				return userPath;
			break;
		}

		case KEY_EXE:
			return db[selectedIndex].key;
		default: break;
		}
	}
}
