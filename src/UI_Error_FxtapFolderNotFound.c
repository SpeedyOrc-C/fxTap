#include "router.h"

void UI_Error_FxtapFolderNotFound()
{
	dclear(C_WHITE);
	dtext(0, 0 * 8, C_BLACK, "Cannot find");
	dtext(0, 1 * 8, C_BLACK, "folder \"FXTAP\".");
	dtext(0, 3 * 8, C_BLACK, "Please create");
	dtext(0, 4 * 8, C_BLACK, "this folder and");
	dtext(0, 5 * 8, C_BLACK, "put beatmaps into it.");
	dupdate();

	while (getkey().key != KEY_EXIT)
	{
	}
}
