#include <ctype.h>
#include <stb_ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fxTap/database.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "fxconv-assets.h"
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

[[nodiscard]]
OCharP UI_AskBeatmapPath_TypeFileNameManually(const FXT_Config *config)
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
		dsubimage(0, 0, &Img_SelectSong_TypeFilename, 0, 30 * config->Language, 128, 30, 0);
		dprint(0, 35, C_BLACK, "[%s]", fileName);
		dprint(98, 48, C_BLACK,
		       KeyboardAlphaState == KA_Idle
			       ? "[1]"
			       : KeyboardAlphaState == KA_Active
				         ? isCapitalised
					           ? "[A]"
					           : "[a]"
				         : isCapitalised
					           ? "L[A]"
					           : "L[a]"
		);
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

static void UI_ShowBeatmapDetail(const FXT_DatabaseRecord *record)
{
	// TODO)) Allow word wrapping in strings
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

static void UI_Modifications(FXT_ModOption *option, const FXT_Config *config)
{
	while (true)
	{
		dclear(C_WHITE);
		dsubimage(0, 0, &Img_Modifications, 0, 64 * config->Language, DWIDTH, DHEIGHT, 0);

		if (option->Mirror)
			drect(2, 13, 62, 27, C_INVERT);

		if (option->Random)
			drect(65, 13, 125, 27, C_INVERT);

		dupdate();

		auto const e = getkey();

		switch (e.key)
		{
		case KEY_1:
		case KEY_F1:
			option->Mirror = ! option->Mirror;
			if (option->Mirror)
				option->Random = false;
			break;
		case KEY_4:
		case KEY_F4:
			option->Random = ! option->Random;
			if (option->Random)
				option->Mirror = false;
			break;
		case KEY_EXIT:
			return;
		default:
			break;
		}
	}
}

OCharP UI_AskBeatmapPath_ListLibrary(const FXT_Config *config, const FXT_Database *database, FXT_ModOption *modOption)
{
	auto const db = *database;

	bool descending = true;
	const size_t size = shlenu(db);

	size_t selectedIndex = 0;

	const struct FXT_Database *viewOrderByNameAsc[size] = {};
	const struct FXT_Database *viewOrderByNameDsc[size] = {};

	for (size_t i = 0; i < size; i += 1)
	{
		viewOrderByNameAsc[i] = &db[i];
		viewOrderByNameDsc[i] = &db[i];
	}

	qsort(viewOrderByNameAsc, size, sizeof(FXT_Database), FXT_Database_Compare_Reverse_Void);
	qsort(viewOrderByNameDsc, size, sizeof(FXT_Database), FXT_Database_Compare_Void);

	while (true)
	{
		auto const view = descending ? viewOrderByNameDsc : viewOrderByNameAsc;

		dclear(C_WHITE);

		dsubimage(1, 1, &Img_SelectASong_Title, 0, 10 * config->Language, 128, 10, 0);
		drect(0, 0, 127, 10, C_INVERT);
		dsubimage(0, 56, &Img_SelectASong_FN, 0, 8 * config->Language, 128, 8, 0);

		dsubimage(2, 56, descending ? &Img_Ascending_FN : &Img_Descending_FN, 0, 8 * config->Language, 19, 8, 0);

		if (size == 0)
		{
			dtext(56, 27, C_BLACK, "???");
		}
		else
		{
			auto const lastGrades = view[selectedIndex]->value.LastGrades;

			if (lastGrades.Exist)
			{
				auto const scoreV2 = FXT_Grades_ScoreV2(lastGrades.Value);
				dprint(91, 2, C_WHITE, "%.2f", scoreV2 * 100);
			}

			// Draw 2 beatmaps before
			if (selectedIndex >= 2)
				dtext(1, 13, C_BLACK, view[selectedIndex - 2]->value.Title);
			if (selectedIndex >= 1)
				dtext(1, 21, C_BLACK, view[selectedIndex - 1]->value.Title);

			// Draw selected beatmap
			dtext(1, 30, C_BLACK, view[selectedIndex]->value.Title);
			drect(0, 29, 127, 37, C_INVERT);

			// Draw 2 beatmaps after
			if (size - selectedIndex >= 2)
				dtext(1, 39, C_BLACK, view[selectedIndex + 1]->value.Title);
			if (size - selectedIndex >= 3)
				dtext(1, 47, C_BLACK, view[selectedIndex + 2]->value.Title);
		}

		dupdate();

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_DOWN:
		case KEY_RIGHT:
			if (size > 0)
				selectedIndex = (selectedIndex + 1) % size;
			break;

		case KEY_UP:
		case KEY_LEFT:
			if (size > 0)
				selectedIndex = selectedIndex > 0 ? selectedIndex - 1 : size - 1;
			break;

		case KEY_F3:
			if (size > 0)
				selectedIndex = rtc_ticks() % size;
			break;

		case KEY_F4:
		case KEY_OPTN:
			UI_Modifications(modOption, config);
			break;

		case KEY_F5:
		case KEY_VARS:
			if (size > 0)
				UI_ShowBeatmapDetail(&view[selectedIndex]->value);
			break;

		case KEY_EXIT:
			return (OCharP){.Path = nullptr};

		case KEY_F1:
			descending = ! descending;
			break;

		case KEY_F6: {
			auto const userPath = UI_AskBeatmapPath_TypeFileNameManually(config);
			if (userPath.Path != nullptr)
				return userPath;
			break;
		}

		case KEY_EXE:
			if (size > 0)
				return (OCharP){.Path = view[selectedIndex]->key, .NeedFree = false};
		default: break;
		}
	}
}
