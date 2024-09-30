#include "ui.h"

void UI_Error_FxtapFolderEmpty()
{
	dclear(C_WHITE);;
	dtext(0, 0 * 8, C_BLACK, "Folder \"FXTAP\".");
	dtext(0, 1 * 8, C_BLACK, "is empty.");
	dtext(0, 3 * 8, C_BLACK, "Please put beatmaps");
	dtext(0, 4 * 8, C_BLACK, "into it.");
	dupdate();

	while (getkey().key != KEY_EXIT)
	{
	}
}
