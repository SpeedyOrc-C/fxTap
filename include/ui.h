#pragma once

#include <fxTap/config.h>
#include <fxTap/database.h>
#include <fxTap/mod.h>

// char* with ownership transfer mark
typedef struct OCharP
{
	char *Path;
	bool NeedFree;
} OCharP;

typedef struct
{
	bool Finished;
	FXT_Grades Grades;
} UI_Play_Result;

void UI_Root(
	FXT_Config *config,
	const FXT_Database *database,
	FXT_ModOption *modOption
);

[[nodiscard]] OCharP UI_Play(
	const FXT_Config *config,
	const FXT_Database *database,
	FXT_ModOption *modOption
);

void UI_Play_Detail(const FXT_DatabaseRecord *record);

void UI_Play_Mods(FXT_ModOption *option, const FXT_Config *config);

[[nodiscard]] OCharP UI_Play_FromPath(const FXT_Config *config);

UI_Play_Result UI_Play_FromList(
	const FXT_Beatmap *beatmap,
	const FXT_Config *config,
	const FXT_ModOption *modOption,
	const char *beatmapPath
);

void UI_KeyTest(const FXT_Config *config);

void UI_Settings(FXT_Config *config);

void UI_About();
