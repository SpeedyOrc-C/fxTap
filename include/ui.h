#ifndef FXTAP_UI_H
#define FXTAP_UI_H

#include <fxTap/config.h>

void UI_MainMenu(FXT_Config *config);
void UI_About();
void UI_Error_FxtapFolderNotFound();
void UI_Error_FxtapFolderEmpty();
void UI_SelectSong(const FXT_Config *config);
void UI_Play(const char* fileName, const FXT_Config *config);
void UI_Settings(FXT_Config *config);
void UI_KeyTest(const FXT_Config *config);

#endif // FXTAP_UI_H
