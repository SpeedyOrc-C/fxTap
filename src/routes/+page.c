#include <stdlib.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include "assets.h"
#include "stb_ds.h"
#include "ui.h"

static void SelectPlayLoop(
	const FXT_Config *config,
	FXT_Database *database,
	FXT_DatabaseView *view,
	FXT_ModOption *modOption)
{
	while (true)
	{
		auto const path = UI_Play(config, database, view, modOption);

		if (path.Path == nullptr)
			break;

		FXT_Beatmap beatmap;
		const FXT_BeatmapError error = FXT_Beatmap_Load_BFile_Auto(&beatmap, path.Path);

		if (error)
		{
			if (path.NeedFree)
				free(path.Path);

			dclear(C_WHITE);
			dprint(1, 1, C_BLACK, "Beatmap Error: %d", error);
			dupdate();
			getkey();
			continue;
		}

		auto const result = UI_Play_FromList(&beatmap, config, modOption, path.Path);

		if (result.Finished)
		{
			auto const record = &shgets(*database, path.Path).value;
			record->LastGrades.Exist = true;
			record->LastGrades.Value = result.Grades;
		}

		if (path.NeedFree)
			free(path.Path);

		FXT_Beatmap_FreeInner(&beatmap);
	}
}

typedef enum MenuItem
{
	MenuItem_Play,
	MenuItem_KeyTest,
	MenuItem_Settings,
	MenuItem_About,
} MenuItem;

typedef enum MenuImage
{
	MenuImage_Banner,
	MenuImage_ElementalCreation,
	MenuImage_Daisuke,
} MenuImage;

static void RenderMainMenu(
	const FXT_Config *config,
	const MenuImage selectedImage,
	const MenuItem selectedItem)
{
	dclear(C_WHITE);

	switch (selectedImage)
	{
	case MenuImage_Banner:
		dimage(0, 0, &Img_Banner);
		break;
	case MenuImage_ElementalCreation:
		dimage(0, 0, &Img_Elemental_Creation);
		break;
	case MenuImage_Daisuke:
		dimage(0, 0, &Img_Daisuke);
		break;
	}

	for (MenuItem item = MenuItem_Play; item <= MenuItem_About; item += 1)
	{
		const int y = 16 * item;

		dsubimage(
			80, y, &Img_MainMenu_Buttons,
			48 * config->Language, y,
			48, 16, 0
		);

		if (selectedItem == item)
			drect(81, y + 1, 126, y + 14, C_INVERT);
	}

	dupdate();
}

void UI_Root(FXT_Config *config, FXT_Database *database, FXT_DatabaseView *view, FXT_ModOption *modOption)
{
	MenuItem selectedItem = MenuItem_Play;
	MenuImage selectedImage = MenuImage_Banner;

	// ReSharper disable once CppDFAEndlessLoop
	while (true)
	{
		RenderMainMenu(config, selectedImage, selectedItem);

		const key_event_t e = getkey();

		switch (e.key)
		{
		case KEY_MENU:
			if (e.shift)
			{
				selectedItem = MenuItem_Settings;
				UI_Settings(config);
			}
			break;
		case KEY_UP:
			if (selectedItem > MenuItem_Play)
				selectedItem -= 1;
			break;
		case KEY_DOWN:
			if (selectedItem < MenuItem_About)
				selectedItem += 1;
			break;
		case KEY_LEFT:
			if (selectedImage > MenuImage_Banner)
				selectedImage -= 1;
			break;
		case KEY_RIGHT:
			if (selectedImage < MenuImage_Daisuke)
				selectedImage += 1;
			break;
		case KEY_OPTN:
			selectedItem = MenuItem_KeyTest;
			UI_KeyTest(config);
			break;
		case KEY_VARS:
			selectedItem = MenuItem_About;
			UI_About();
			break;
		case KEY_EXE:
			switch (selectedItem)
			{
			case MenuItem_Play:
				SelectPlayLoop(config, database, view, modOption);
				break;
			case MenuItem_KeyTest:
				UI_KeyTest(config);
				break;
			case MenuItem_Settings:
				UI_Settings(config);
				break;
			case MenuItem_About:
				UI_About();
				break;
			}
		default:
			break;
		}
	}
}
