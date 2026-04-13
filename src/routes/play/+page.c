#include <math.h>
#include <fxTap/database.h>
#include <fxTap/database/view.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "assets.h"
#include "ui.h"

OCharP UI_Play(const FXT_Config *config, FXT_DatabaseView *view, FXT_ModOption *modOption)
{
	static bool descending = true;

	while (true)
	{
		dclear(C_WHITE);

		dsubimage(0, 56, &Img_SelectASong_FN, 0, 8 * config->Language, 128, 8, 0);
		dsubimage(2, 56, descending ? &Img_Ascending_FN : &Img_Descending_FN, 0, 8 * config->Language, 19, 8, 0);

		if (view->GroupCount == 0)
		{
			dtext(56, 27, C_BLACK, "???");
		}
		else
		{
			// Scroll bar for beatmaps
			if (view->GroupCount > 2)
			{
				drect_border(DWIDTH - 4, 0, DWIDTH - 1, 54, C_NONE, 1, C_BLACK);

				const float headIndex = (float) view->FocusedGroupIndex;
				constexpr float windowSize = 4;
				const float maxIndex = (float) view->GroupCount + windowSize - 1;
				const float tailIndex = headIndex + windowSize;

				static constexpr float VisualHeight = 53 - 1;

				const int head = (int) roundf(VisualHeight * headIndex / maxIndex);
				const int tail = (int) roundf(VisualHeight * tailIndex / maxIndex);

				const int y1 = 1 + head;
				const int y2 = 1 + tail;

				drect(DWIDTH - 3, y1, DWIDTH - 2, y2, C_BLACK);
			}

			// Scroll bar for versions
			if (view->Groups[view->FocusedGroupIndex].Size > view->VersionWindowSize)
			{
				drect_border(119, 22, 121, 42, C_NONE, 1, C_BLACK);

				const float headIndex = (float) view->VersionWindowHeadIndex;
				const float windowSize = (float) view->VersionWindowSize;
				const float maxIndex = (float) view->Groups[view->FocusedGroupIndex].Size - 1;
				const float tailIndex = headIndex + windowSize - 1;

				static constexpr float VisualHeight = 41 - 23;

				const int head = (int) roundf(VisualHeight * headIndex / maxIndex);
				const int tail = (int) roundf(VisualHeight * tailIndex / maxIndex);

				const int y1 = 23 + head;
				const int y2 = 23 + tail;

				dline(120, y1, 120, y2, C_BLACK);
			}

			if (view->FocusedGroupIndex >= 2)
			{
				dfont(&Font_Tiny);
				dtext(1, 0, C_BLACK, view->Groups[view->FocusedGroupIndex - 2].Title);
			}

			if (view->FocusedGroupIndex >= 1)
			{
				dfont(&Font_Tiny);
				dtext(1, 6, C_BLACK, view->Groups[view->FocusedGroupIndex - 1].Title);
			}

			dfont(&Font_Piczel);
			dtext(1, 12, C_BLACK, view->Groups[view->FocusedGroupIndex].Title);

			auto const group = view->Groups[view->FocusedGroupIndex];

			for (size_t offset = 0; offset < view->VersionWindowSize; offset += 1)
			{
				auto const memberIndex = view->VersionWindowHeadIndex + offset;

				if (memberIndex >= group.Size)
					continue;

				dfont(&Font_Tiny);

				auto const version = group.Members[memberIndex]->value.Version;
				dtext(9, 23 + 7 * offset, C_BLACK, version);

				auto const lastGrades = group.Members[memberIndex]->value.LastGrades;

				if (lastGrades.Exist)
					dprint_opt(
						115, 23 + 7 * offset, C_BLACK, C_NONE,
						DTEXT_RIGHT, DTEXT_TOP,
						"%.2f", 100 * FXT_Grades_ScoreV2(lastGrades.Value)
					);

				if (memberIndex == view->FocusedVersionIndex)
					drect(8, 22 + 7 * offset, 116, 28 + 7 * offset, C_INVERT);
			}

			if (view->GroupCount - view->FocusedGroupIndex >= 2)
			{
				dfont(&Font_Tiny);
				dtext(1, 44, C_BLACK, view->Groups[view->FocusedGroupIndex + 1].Title);
			}

			if (view->GroupCount - view->FocusedGroupIndex >= 3)
			{
				dfont(&Font_Tiny);
				dtext(1, 50, C_BLACK, view->Groups[view->FocusedGroupIndex + 2].Title);
			}
		}

		dfont(&Font_Piczel);

		dupdate();

		const key_event_t e = getkey();

		if (view->GroupCount > 0)
		{
			switch (e.key)
			{
			case KEY_F3:
				view->FocusedGroupIndex = rtc_ticks() % view->GroupCount;
				view->FocusedVersionIndex = 0;
				view->VersionWindowHeadIndex = 0;
				break;

			case KEY_F5:
			case KEY_VARS:
				UI_Play_Detail(FXT_DatabaseView_FocusedMember(view)->key);
				break;

			case KEY_EXE:
				return (OCharP){
					.NeedFree = false,
					.Path = FXT_DatabaseView_FocusedMember(view)->key,
				};

			default: break;
			}
		}

		switch (e.key)
		{
		case KEY_DOWN:
			FXT_DatabaseView_NextVersion(view);
			break;

		case KEY_RIGHT:
			FXT_DatabaseView_NextGroup(view);
			break;

		case KEY_UP:
			FXT_DatabaseView_PreviousVersion(view);
			break;

		case KEY_LEFT:
			FXT_DatabaseView_PreviousGroup(view);
			break;

		case KEY_F4:
		case KEY_OPTN:
			UI_Play_Mods(modOption, config);
			break;

		case KEY_EXIT:
			return (OCharP){.Path = nullptr};

		case KEY_F1:
			descending = ! descending;
			if (descending)
				FXT_DatabaseView_SortDsc(view);
			else
				FXT_DatabaseView_SortAsc(view);
			break;

		case KEY_F6: {
			auto const userPath = UI_Play_FromPath(config);
			if (userPath.Path != nullptr)
				return userPath;
			break;
		}

		default: break;
		}
	}
}
