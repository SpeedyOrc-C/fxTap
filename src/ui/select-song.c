#include "ui.h"
#include "fxconv-assets.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <gint/hardware.h>

uint8_t keycode_alpha(int keycode)
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

void UI_SelectSong_G_Casiowin(const Config *config)
{
	bool isCapitalised = true;
	bool isAlpha = true;
	char fileName[8+1] = "\0\0\0\0\0\0\0\0\0";
	int cursor = 0;

	while (true)
	{
		const size_t fileNameLength = strlen(fileName);

		dclear(C_WHITE);
		dsubimage(0, 0, &Img_SelectSong_TypeFilename, 0, 30 * config->Language, 128, 30, 0);
		dprint(24, 35, C_BLACK, "[%-8s].fxt", fileName);
		dpixel(30 + 6 * fileNameLength, 43, C_BLACK);
		dpixel(29 + 6 * fileNameLength, 44, C_BLACK);
		dpixel(30 + 6 * fileNameLength, 44, C_BLACK);
		dpixel(31 + 6 * fileNameLength, 44, C_BLACK);
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
				fileName[0] = 0;
				break;

			case KEY_EXIT:
				return;

			case KEY_EXE:
				return UI_Play(fileName, config);

			case KEY_F1:
				if (cursor < 8)
				{
					fileName[cursor] = '.';
					cursor += 1;
				}
				break;

			case KEY_F2:
				if (cursor < 8)
				{
					fileName[cursor] = '_';
					cursor += 1;
				}
				break;

			case KEY_F3:
				if (cursor < 8)
				{
					fileName[cursor] = '~';
					cursor += 1;
				}
				break;

			case KEY_F4:
				if (cursor < 8)
				{
					fileName[cursor] = '\'';
					cursor += 1;
				}
				break;

			case KEY_F5:
				isAlpha = !isAlpha; break;

			case KEY_F6:
				isCapitalised = !isCapitalised; break;

			default:
				if (cursor >= 8) break;
				if (isAlpha)
				{
					const uint8_t c = keycode_alpha(e.key);

					if (c != 255)
					{
						fileName[cursor] = isCapitalised ? c : tolower(c);
						cursor += 1;
					}
				} else
				{
					const uint8_t c = keycode_digit(e.key);

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

void UI_SelectSong_G_Fugue(const Config *config)
{

}

void UI_SelectSong_CG(const Config *config)
{
	FindError error;
	BeatmapFindEntries *entries = BeatmapFindEntries_New_InsideDirectory("FXTAP", &error);

	if (entries == NULL)
	{
		UI_Error_FxtapFolderNotFound();
		return;
	}

	if (entries->Count == 0)
	{
		BeatmapFindEntries_Free(entries);
		UI_Error_FxtapFolderEmpty();
		return;
	}

	int selectedEntryIndex = 0;

	while (true)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "%2i/%i. %s: %s",
			selectedEntryIndex + 1,
			entries->Count,
			entries->Entries[selectedEntryIndex].FileName,
			entries->Entries[selectedEntryIndex].Metadata.Title
		);
		dupdate();

		key_event_t e = getkey();

		if (e.key == KEY_EXIT)
		{
			BeatmapFindEntries_Free(entries);
			return;
		}

		if (e.key == KEY_EXE)
		{
			char *fileName = malloc(strlen(entries->Entries[selectedEntryIndex].FileName) + 1);

			assert(fileName != NULL);

			strcpy(fileName, entries->Entries[selectedEntryIndex].FileName);

			BeatmapFindEntries_Free(entries);

			UI_Play(fileName, config);

			free(fileName);
			return;
		}

		if (e.key == KEY_UP && selectedEntryIndex > 0)
		{
			selectedEntryIndex -= 1;
			continue;
		}

		if (e.key == KEY_DOWN && selectedEntryIndex < entries->Count - 1)
		{
			selectedEntryIndex += 1;
			continue;
		}
	}
}

void UI_SelectSong(const Config *config)
{
	#ifdef FX9860G
		if (gint[HWFS] == HWFS_CASIOWIN)
			UI_SelectSong_G_Casiowin(config);
		else
			UI_SelectSong_G_Fugue(config);
	#endif

	#ifdef FXCG50
		UI_SelectSong_CG(config);
	#endif
}
