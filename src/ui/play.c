#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fxTap/beatmap-casiowin.h>
#include <fxTap/beatmap.h>
#include <fxTap/config.h>
#include <fxTap/game.h>
#include <fxTap/keymap.h>
#include <fxTap/render.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/rtc.h>
#include "ui.h"

int32_t time128Delta(const int32_t start, const int32_t end)
{
    if (start <= end)
        return end - start;
    return 128 * 60 * 60 * 24 - start + end;
}

int clampHeight(const int y)
{
    if (y < 0) return 0;
    if (y >= DHEIGHT) return DHEIGHT - 1;
    return y;
}

void RenderTap(const int column, const double positionBottom)
{
    const int y = clampHeight(DHEIGHT - 1 - round(positionBottom));
    drect(column * 8, y - 2, column * 8 + 7, y, C_BLACK);
}

void RenderHold(const int column, const double positionBottom, const double positionTop)
{
    const int y1 = clampHeight(DHEIGHT - 1 - round(positionTop));
    const int y2 = clampHeight(DHEIGHT - 1 - round(positionBottom));
    drect(column * 8, y1, column * 8 + 7, y2, C_BLACK);
}

#if defined(FX9860G)
void Beatmap_New_LoadFromPath_BFile_Wrapper(Beatmap **beatmap, const char *path, BeatmapError *error)
{
    *beatmap = Beatmap_New_LoadFromPath_BFile(path, error);
}
#endif

Beatmap *Beatmap_New_LoadFromFile_OptionalFolder(const char *fileName, const bool insideFxTapFolder, BeatmapError *error)
{
    char *path = malloc(6 + strlen(fileName) + 4 + 1);
    assert(path != NULL);

    Beatmap *beatmap;
    sprintf(path, insideFxTapFolder ? "FXTAP/%s.fxt" : "%s.fxt", fileName);

#if defined(FX9860G)
    if (gint[HWFS] == HWFS_CASIOWIN)
    {
        const gint_call_t call = {
            .function = &Beatmap_New_LoadFromPath_BFile_Wrapper,
            .args = {{.pv = &beatmap}, {.pc = path}, {.pv = error}}
        };

        gint_call(call);
    } else
        beatmap = Beatmap_New_LoadFromPath(path, error);
#else
    beatmap = Beatmap_New_LoadFromPath(path, error);
#endif

    free(path);

    return beatmap;
}

void UI_Play(const char *fileName, const Config *config)
{
    BeatmapError beatmapError;
    Beatmap *beatmap = Beatmap_New_LoadFromFile_OptionalFolder(fileName, true, &beatmapError);

    if (beatmap == NULL)
        beatmap = Beatmap_New_LoadFromFile_OptionalFolder(fileName, false, &beatmapError);

    if (beatmap == NULL)
    {
        dclear(C_WHITE);
        dprint(1, 1, C_BLACK, "%d", beatmapError);
        dupdate();
        getkey();

        return;
    }

    FxTap fxTap;
    FxTap_Init(&fxTap, beatmap);

    const KeyMapper keyMapper = FxTap_FetchKeyMapper(&fxTap, config);

    if (keyMapper == NULL)
    {
        dclear(C_WHITE);
        dprint(1, 1, C_BLACK, "Can't find key mapper");
        dupdate();
        getkey();

        free(beatmap);

        return;
    }

    const RendererController Controller = {
        .HeightAbove = DHEIGHT - 1,
        .VisibleTime = config->NotesFallingTime,
        .RenderTap = &RenderTap,
        .RenderHold = &RenderHold,
    };

    const uint32_t startTime128 = rtc_ticks();

    while (true)
    {
        const int32_t timeNow128 = time128Delta((int32_t) startTime128, (int32_t) rtc_ticks());
        const int32_t timeNowMs = timeNow128 * 1000 / 128;

        bool isPressingColumn[MAX_COLUMN_COUNT] =
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        while (true)
        {
            const key_event_t e = pollevent();

            if (e.type == KEYEV_NONE) break;

            for (int column = 0; column < fxTap.ColumnCount; column += 1)
            {
                const FxTapKey fxTapKey = keyMapper(column);
                const int physicalKey = config->PhysicalKeyOfFxTapKey[fxTapKey];
                isPressingColumn[column] = keydown(physicalKey);
            }
        }

        FxTap_Update(&fxTap, timeNowMs, isPressingColumn);

        dclear(C_WHITE);
        RendererController_Run(&Controller, &fxTap, timeNowMs);
        dprint(84, 0 * 8, C_BLACK, "%d", fxTap.Grades.Miss);
        dprint(84, 1 * 8, C_BLACK, "%d", fxTap.Grades.Meh);
        dprint(84, 2 * 8, C_BLACK, "%d", fxTap.Grades.Ok);
        dprint(84, 3 * 8, C_BLACK, "%d", fxTap.Grades.Good);
        dprint(84, 4 * 8, C_BLACK, "%d", fxTap.Grades.Great);
        dprint(84, 5 * 8, C_BLACK, "%d", fxTap.Grades.Perfect);
        dprint(84, 7 * 8, C_BLACK, "%d", fxTap.Combo);
        dupdate();

        if (keydown(KEY_EXIT)) break;
    }

    Beatmap_Free(beatmap);
}
