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

void RenderTap(const int column, const double positionBottom)
{
	const int y = (int) round(DHEIGHT - 1 - positionBottom);
	drect(column * 8 + 1, y - 2, column * 8 + 7, y, C_BLACK);
}

void RenderHold(const int column, const double positionBottom, const double positionTop)
{
	const int y1 = (int) round(DHEIGHT - 1 - positionTop);
	const int y2 = (int) round(DHEIGHT - 1 - positionBottom);
	drect(column * 8 + 1, y1, column * 8 + 7, y2, C_BLACK);
}

int32_t Time128Delta(const int32_t start, const int32_t end)
{
	if (start <= end)
		return end - start;
	return 128 * 60 * 60 * 24 - start + end;
}

void RenderGameFrame(
	const FXT_Game *game,
	const FXT_RendererController *rendererController,
	const int32_t timeNowMs)
{
	dclear(C_WHITE);

	// Render notes
	FXT_RendererController_Run(rendererController, game, timeNowMs);

	// Render framework
	for (int i = 0; i < game->ColumnCount + 1; i += 1)
	{
		auto const x = i * 8;
		dline(x, 0, x, DHEIGHT - 1, C_BLACK);
	}

	// Render number of notes in different grades
	dprint(84, 0 * 8, C_BLACK, "%d", game->Grades.Miss);
	dprint(84, 1 * 8, C_BLACK, "%d", game->Grades.Meh);
	dprint(84, 2 * 8, C_BLACK, "%d", game->Grades.Ok);
	dprint(84, 3 * 8, C_BLACK, "%d", game->Grades.Good);
	dprint(84, 4 * 8, C_BLACK, "%d", game->Grades.Great);
	dprint(84, 5 * 8, C_BLACK, "%d", game->Grades.Perfect);
	dprint(84, 7 * 8, C_BLACK, "%d", game->Combo);

	dupdate();
}

void UI_Play(FXT_Game *game, const FXT_Config *config)
{
	const KeyMapper keyMapper = FXT_Game_FetchKeyMapper(game, config);

	if (keyMapper == nullptr)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Can't find key mapper");
		dupdate();
		getkey();
		return;
	}

	dclear(C_WHITE);
	dprint(0, 0, C_BLACK, "%s", game->Beatmap->Metadata.Title);
	dprint(0, 8, C_BLACK, "%s", game->Beatmap->Metadata.Artist);
	dprint(0, 16, C_BLACK, "%f", game->Beatmap->Metadata.OverallDifficulty);
	dprint(0, 24, C_BLACK, "%d %d %d %d",
	       game->Beatmap->Metadata.SizeOfColumn[0],
	       game->Beatmap->Metadata.SizeOfColumn[1],
	       game->Beatmap->Metadata.SizeOfColumn[2],
	       game->Beatmap->Metadata.SizeOfColumn[3]
	);
	dupdate();
	getkey();

	const FXT_RendererController rendererController = {
		.HeightAbove = DHEIGHT - 1,
		.VisibleTime = config->NotesFallingTime,
		.RenderTap = &RenderTap,
		.RenderHold = &RenderHold,
	};

	const uint32_t startTime128 = rtc_ticks();

	while (true)
	{
		bool isPressingColumn[FXT_MaxColumnCount] =
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		while (true)
		{
			auto const e = pollevent();

			if (e.type == KEYEV_NONE) break;

			for (int column = 0; column < game->ColumnCount; column += 1)
			{
				auto const fxTapKey = keyMapper(column);
				auto const physicalKey = config->PhysicalKeyOfFxTapKey[fxTapKey];
				isPressingColumn[column] = keydown(physicalKey);
			}
		}

		auto const timeNow128 = Time128Delta((int32_t) startTime128, (int32_t) rtc_ticks());
		// Wait for 1000ms before start
		const int32_t timeNowMs = -1000 + timeNow128 * 1000 / 128;

		FXT_Game_Update(game, timeNowMs, isPressingColumn);
		RenderGameFrame(game, &rendererController, timeNowMs);

		if (keydown(KEY_EXIT)) break;
	}
}
