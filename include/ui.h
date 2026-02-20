#ifndef FXTAP_UI_H
#define FXTAP_UI_H

#include <fxTap/config.h>

void UI_MainMenu(Config *config);
void UI_About();
void UI_Error_FxtapFolderNotFound();
void UI_Error_FxtapFolderEmpty();
void UI_SelectSong(const Config *config);
void UI_Play(const char* fileName, const Config *config);
void UI_Settings(Config *config);
void UI_KeyTest(const Config *config);

#endif // FXTAP_UI_H
