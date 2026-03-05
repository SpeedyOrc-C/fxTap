#pragma once

#include <fxTap/config.h>
#include <fxTap/database.h>
#include <fxTap/game.h>

void UI_MainMenu(FXT_Config *config, const FXT_Database *database);

void UI_About();

void UI_Play(FXT_Game *game, const FXT_Config *config);

void UI_Settings(FXT_Config *config);

void UI_KeyTest(const FXT_Config *config);

[[nodiscard]]
char *UI_AskBeatmapPath_TypeFileNameManually(const FXT_Config *config);

[[nodiscard]]
char *UI_AskBeatmapPath_ListLibrary(const FXT_Config *config, const FXT_Database *database);
