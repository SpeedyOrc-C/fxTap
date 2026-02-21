#include <math.h>
#include <fxTap/beatmap.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <fxTap/keymap.h>
#include <fxTap/render.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "ui.h"

int32_t Time128Delta(const int32_t start, const int32_t end)
{
	if (start <= end)
		return end - start;
	return 128 * 60 * 60 * 24 - start + end;
}

int ClampHeight(const int y)
{
	if (y < 0) return 0;
	if (y >= DHEIGHT) return DHEIGHT - 1;
	return y;
}

void RenderTap(const int column, const double positionBottom)
{
	const int y = ClampHeight(DHEIGHT - 1 - round(positionBottom));
	drect(column * 8, y - 2, column * 8 + 7, y, C_BLACK);
}

void RenderHold(const int column, const double positionBottom, const double positionTop)
{
	const int y1 = ClampHeight(DHEIGHT - 1 - round(positionTop));
	const int y2 = ClampHeight(DHEIGHT - 1 - round(positionBottom));
	drect(column * 8, y1, column * 8 + 7, y2, C_BLACK);
}

void UI_Play(FxTap *fxTap, const FXT_Config *config)
{
	const KeyMapper keyMapper = FxTap_FetchKeyMapper(fxTap, config);

	if (keyMapper == nullptr)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Can't find key mapper");
		dupdate();
		getkey();
		return;
	}

	dclear(C_WHITE);
	dprint(0, 0, C_BLACK, "%s", fxTap->Beatmap->Metadata.Title);
	dprint(0, 8, C_BLACK, "%s", fxTap->Beatmap->Metadata.Artist);
	dprint(0, 16, C_BLACK, "%f", fxTap->Beatmap->Metadata.OverallDifficulty);
	dprint(0, 24, C_BLACK, "%d %d %d %d",
	       fxTap->Beatmap->Metadata.SizeOfColumn[0],
	       fxTap->Beatmap->Metadata.SizeOfColumn[1],
	       fxTap->Beatmap->Metadata.SizeOfColumn[2],
	       fxTap->Beatmap->Metadata.SizeOfColumn[3]
	);
	dupdate();
	getkey();

	const RendererController Controller = {
		.HeightAbove = DHEIGHT - 1,
		.VisibleTime = config->NotesFallingTime,
		.RenderTap = &RenderTap,
		.RenderHold = &RenderHold,
	};

	const uint32_t startTime128 = rtc_ticks();

	while (true)
	{
		const int32_t timeNow128 = Time128Delta((int32_t) startTime128, (int32_t) rtc_ticks());
		const int32_t timeNowMs = timeNow128 * 1000 / 128;

		bool isPressingColumn[FXT_MaxColumnCount] =
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		while (true)
		{
			const key_event_t e = pollevent();

			if (e.type == KEYEV_NONE) break;

			for (int column = 0; column < fxTap->ColumnCount; column += 1)
			{
				auto const fxTapKey = keyMapper(column);
				auto const physicalKey = config->PhysicalKeyOfFxTapKey[fxTapKey];
				isPressingColumn[column] = keydown(physicalKey);
			}
		}

		FxTap_Update(fxTap, timeNowMs, isPressingColumn);

		dclear(C_WHITE);
		RendererController_Run(&Controller, fxTap, timeNowMs);
		dprint(84, 0 * 8, C_BLACK, "%d", fxTap->Grades.Miss);
		dprint(84, 1 * 8, C_BLACK, "%d", fxTap->Grades.Meh);
		dprint(84, 2 * 8, C_BLACK, "%d", fxTap->Grades.Ok);
		dprint(84, 3 * 8, C_BLACK, "%d", fxTap->Grades.Good);
		dprint(84, 4 * 8, C_BLACK, "%d", fxTap->Grades.Great);
		dprint(84, 5 * 8, C_BLACK, "%d", fxTap->Grades.Perfect);
		dprint(84, 7 * 8, C_BLACK, "%d", fxTap->Combo);
		dupdate();

		if (keydown(KEY_EXIT)) break;
	}
}
