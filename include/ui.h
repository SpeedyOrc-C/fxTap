#pragma once

#include <fxTap/config.h>
#include <fxTap/database.h>
#include <fxTap/mod.h>

void UI_MainMenuLoop(FXT_Config *config, const FXT_Database *database, FXT_ModOption *modOption);

void UI_About();

typedef struct
{
	bool Finished;
	FXT_Grades Grades;
} UI_Play_Result;

UI_Play_Result UI_Play(const FXT_Beatmap *beatmap, const FXT_Config *config, const FXT_ModOption *modOption, const char *beatmapPath);

void UI_Settings(FXT_Config *config);

void UI_KeyTest(const FXT_Config *config);

// char* with ownership transfer mark
typedef struct OCharP
{
	char *Path;
	bool NeedFree;
} OCharP;

[[nodiscard]]
OCharP UI_AskBeatmapPath_TypeFileNameManually(const FXT_Config *config);

[[nodiscard]]
OCharP UI_AskBeatmapPath_ListLibrary(const FXT_Config *config, const FXT_Database *database, FXT_ModOption *modOption);
