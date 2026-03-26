#include <math.h>
#include <fxTap/beatmap.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <fxTap/keymap.h>
#include <fxTap/render.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "fxconv-assets.h"
#include "ui.h"

static int ColumnWidth = 10;
static int TapNoteHeight = 4;

static void RenderTap(const int column, const double positionBottom)
{
	auto const x = column * ColumnWidth;
	auto const y = (int) round(DHEIGHT - 1 - positionBottom);
	drect(x + 1, y - TapNoteHeight, x + ColumnWidth - 1, y, C_BLACK);
}

static void RenderHold(const int column, const double positionBottom, const double positionTop)
{
	auto const x1 = column * ColumnWidth + 1;
	auto const y1 = (int) round(DHEIGHT - 1 - positionTop);
	auto const x2 = column * ColumnWidth + ColumnWidth - 1;
	auto const y2 = (int) round(DHEIGHT - 1 - positionBottom);

	for (int x = x1; x <= x2; x += 1)
		for (int y = y1; y <= y2; y += 1)
			if ((x + y) % 2 == 0)
				dpixel(x, y, C_BLACK);

	drect_border(x1, y1, x2, y2, C_NONE, 1, C_BLACK);
}

static int32_t Time128Delta(const int32_t start, const int32_t end)
{
	if (start <= end)
		return end - start;
	return 128 * 60 * 60 * 24 - start + end;
}

static void RenderGameFrame(
	const FXT_Game *game,
	const FXT_RendererController *rendererController,
	const FXT_TimeMs timeNow,
	const FXT_TimeMs endTime,
	const bool isPressingColumn[10])
{
	dclear(C_WHITE);

	// Render notes
	FXT_RendererController_Run(rendererController, game, timeNow);

	// Render framework
	for (int column = 0; column <= game->Beatmap->ColumnCount; column += 1)
	{
		auto const x = column * ColumnWidth;

		for (int y = 0; y < DHEIGHT; y += 2)
			dpixel(x, y, C_BLACK);

		// Render pressing effect
		if (isPressingColumn[column])
			drect_border(
				x, DHEIGHT - TapNoteHeight,
				x + ColumnWidth, DHEIGHT - 1,
				C_NONE, 2, C_BLACK);
	}

	for (int x = 0; x < game->Beatmap->ColumnCount * ColumnWidth + 1; x += 2)
	{
		dpixel(x, DHEIGHT - TapNoteHeight, C_BLACK);
		dpixel(x, DHEIGHT - 1, C_BLACK);
	}

	// Display number of notes in different grades
	static constexpr int GradeX = 100;
	dprint(GradeX, 0 * 8, C_BLACK, "%d", game->Grades.Miss);
	dprint(GradeX, 1 * 8, C_BLACK, "%d", game->Grades.Meh);
	dprint(GradeX, 2 * 8, C_BLACK, "%d", game->Grades.Ok);
	dprint(GradeX, 3 * 8, C_BLACK, "%d", game->Grades.Good);
	dprint(GradeX, 4 * 8, C_BLACK, "%d", game->Grades.Great);
	dprint(GradeX, 5 * 8, C_BLACK, "%d", game->Grades.Perfect);
	dprint(GradeX, 7 * 8, C_BLACK, "%d", game->Combo);

	// Render progress bar
	drect_border(DWIDTH - 4, 0, DWIDTH - 1, DHEIGHT - 1, C_WHITE, 1, C_BLACK);
	auto const progress = (int) roundf((float) (DHEIGHT - 2) * (float) timeNow / (float) endTime);
	if (progress > 0)
		drect(DWIDTH - 3, 1, DWIDTH - 2, progress, C_BLACK);

	dupdate();
}

typedef enum PauseAction
{
	PauseAction_Resume,
	PauseAction_Restart,
	PauseAction_Stop,
} PauseAction;

static PauseAction Pause(const FXT_Config *config)
{
	for (int x = 0; x < DWIDTH; x += 1)
		for (int y = 0; y < DHEIGHT; y += 1)
			if ((x + y) % 2 == 1)
				dpixel(x, y, C_WHITE);

	dline(0, 55, DWIDTH - 1, 55, C_WHITE);
	dsubimage(0, 56, &Img_Pause_FN, 0, 8 * config->Language, 128, 8, 0);
	dupdate();

	while (true)
	{
		auto const e = getkey();

		switch (e.key)
		{
		case KEY_F1:
		case KEY_F2:
			return PauseAction_Stop;
		case KEY_F3:
		case KEY_F4:
			return PauseAction_Restart;
		case KEY_F5:
		case KEY_F6:
		case KEY_EXE:
			return PauseAction_Resume;
		default:
			break;
		}
	}
}

static bool ShowGrade(const FXT_Game *game, const FXT_Config *config)
{
	auto const grades = game->Grades;
	auto const scoreV1 = 100 * FXT_Grades_ScoreV1(grades);
	auto const scoreV2 = 100 * FXT_Grades_ScoreV2(grades);

	dclear(C_WHITE);
	dprint(0, 0 * 8, C_BLACK, "ACC v1/v2 %.2f/%.2f", scoreV1, scoreV2);
	dprint(0, 1 * 8, C_BLACK, "MAX COMBO %u", game->Combo);
	dprint(0, 2 * 8, C_BLACK, "  Perfect %u", grades.Perfect);
	dprint(0, 3 * 8, C_BLACK, "    Great %u", grades.Great);
	dprint(0, 4 * 8, C_BLACK, "     Good %u", grades.Good);
	dprint(0, 5 * 8, C_BLACK, "       OK %u", grades.Ok);
	dprint(0, 6 * 8, C_BLACK, "      Meh %u", grades.Meh);
	dprint(0, 7 * 8, C_BLACK, "     Miss %u", grades.Miss);
	dsubimage(107, 56, &Img_Save_FN, 0, 8 * config->Language, 19, 8, 0);
	dupdate();

	while (true)
	{
		auto const e = getkey();

		switch (e.key)
		{
		case KEY_EXIT:
		case KEY_EXE:
			return false;

		case KEY_F6:
			return true;

		default:
			break;
		}
	}
}

static FXT_DatabaseError SaveGradesAlongBeatmap(const char *beatmapPath, const FXT_Grades *grades)
{
	if (gint[HWFS] == HWFS_FUGUE)
		return FXT_SaveGradesAlongBeatmap(beatmapPath, grades);

	return gint_call((gint_call_t){
		.function = FXT_SaveGradesAlongBeatmap_BFile,
		.args = {{.pv_c = beatmapPath}, {.pv_c = grades}},
	});
}

UI_Play_Result UI_Play(const FXT_Beatmap *beatmap, const FXT_Config *config, const char *beatmapPath)
{
	const KeyMapper keyMapper = FXT_FetchKeyMapper(beatmap, config);

	if (keyMapper == nullptr)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Can't find key mapper");
		dupdate();
		getkey();
		return (UI_Play_Result){.Finished = false};
	}

	ColumnWidth = config->ColumnWidth;
	TapNoteHeight = config->TapNoteHeight;

	const FXT_RendererController rendererController = {
		.HeightAbove = DHEIGHT - 1,
		.VisibleTime = config->NotesFallingTime,
		.RenderTap = &RenderTap,
		.RenderHold = &RenderHold,
	};

	static constexpr FXT_TimeMs WaitTimeBeforeStart = 1000;
	static constexpr FXT_TimeMs WaitTimeAfterEnd = 1000;

	FXT_Game game;

restart:
	FXT_Game_Init(&game, beatmap);

	if (config->OverrideDefaultOverallDifficulty)
		game.Tolerance = FXT_Tolerance_FromOverallDifficulty(
			(double) config->CustomOverallDifficulty10 / 10);

	const FXT_TimeMs endTime = FXT_Game_LastNoteEndTime(&game) + WaitTimeAfterEnd;

	FXT_TimeMs timeOffset = -WaitTimeBeforeStart;
	auto startTime128 = rtc_ticks();

	while (true)
	{
		while (pollevent().type != KEYEV_NONE)
		{
		}

		bool isPressingColumn[FXT_MaxColumnCount] = {};

		for (int column = 0; column < game.Beatmap->ColumnCount; column += 1)
		{
			auto const fxTapKey = keyMapper(column);
			auto const physicalKey = config->PhysicalKeyOfFxTapKey[fxTapKey];
			isPressingColumn[column] = keydown(physicalKey) != 0;
		}

		const int32_t timeElapsedSinceStart128 = Time128Delta((int32_t) startTime128, (int32_t) rtc_ticks());
		const FXT_TimeMs timeElapsedSinceStart = timeElapsedSinceStart128 * 1000 / 128;
		const FXT_TimeMs timeNow = timeOffset + timeElapsedSinceStart;

		FXT_Game_Update(&game, timeNow, isPressingColumn);
		RenderGameFrame(&game, &rendererController, timeNow, endTime, isPressingColumn);

		// Game finished normally
		if (timeNow > endTime || keydown(KEY_OPTN))
		{
			auto const saveGrades = ShowGrade(&game, config);

			if (! saveGrades)
				return (UI_Play_Result){.Finished = true, .Grades = game.Grades};

			auto const error = SaveGradesAlongBeatmap(beatmapPath, &game.Grades);

			if (error)
			{
				dclear(C_WHITE);
				dprint(1, 1, C_BLACK, "Database Error: %d", error);
				dupdate();
				getkey();
			}

			return (UI_Play_Result){.Finished = true, .Grades = game.Grades};
		}

		// Pause
		if (keydown(KEY_EXIT))
		{
			switch (Pause(config))
			{
			case PauseAction_Resume:
				timeOffset = timeNow;
				startTime128 = rtc_ticks();
				continue;
			case PauseAction_Restart:
				goto restart;
			case PauseAction_Stop:
				return (UI_Play_Result){.Finished = false};
			}
		}
	}
}
