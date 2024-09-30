#ifndef FXTAP_UI_H
#define FXTAP_UI_H

#include <gint/display.h>
#include <gint/keyboard.h>
#include <fxTap/beatmap.h>
#include <fxTap/config.h>
#include <fxTap/config-casiowin.h>
#include <stdlib.h>
#include "fxconv-assets.h"

void UI_MainMenu(Config *config);
void UI_About();
void UI_Error_FxtapFolderNotFound();
void UI_Error_FxtapFolderEmpty();
void UI_SelectSong(const Config *config);
void UI_Play(const char* fileName, const Config *config);
void UI_Settings(Config *config);
void UI_KeyTest(const Config *config);

#endif // FXTAP_UI_H
