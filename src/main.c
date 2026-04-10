#include <fxlibc/printf.h>
#include <fxTap/config.h>
#include <fxTap/database.h>
#include <fxTap/mod.h>
#include <fxTap/database/view.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "assets.h"
#include "ui.h"

static FXT_ConfigError LoadConfig(FXT_Config *dst)
{
	if (gint[HWFS] == HWFS_FUGUE)
		return FXT_Config_Load(dst);

	return gint_call((gint_call_t){
		.function = FXT_Config_Load_BFile,
		.args = {{.pv = dst}}
	});
}

static FXT_DatabaseError LoadDatabase(FXT_Database *dst)
{
	FXT_Database_Init(dst);

	if (gint[HWFS] == HWFS_FUGUE)
		return FXT_Database_SyncFromFileSystem(dst);

	return gint_call((gint_call_t){
		.function = FXT_Database_SyncFromFileSystem_BFile,
		.args = {{.pv = (void *) dst}}
	});
}

int main(void)
{
	__printf_enable_fp();
	dfont(&Font_Piczel);

	dclear(C_WHITE);
	dprint_opt(
		DWIDTH / 2, DHEIGHT / 2, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE,
		"Loading..."
	);
	dupdate();

	FXT_Config config;
	auto const configError = LoadConfig(&config);

	if (configError)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Config Error: %d", configError);
		dupdate();
		getkey();
		return 1;
	}

	FXT_Database database;
	auto const databaseError = LoadDatabase(&database);

	if (databaseError)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Database Error: %d", databaseError);
		dupdate();
		getkey();
		return 1;
	}

	FXT_DatabaseView view;
	auto const databaseViewError = FXT_DatabaseView_Init(&view, &database);

	if (databaseViewError)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Database View Error: %d", databaseViewError);
		dupdate();
		getkey();
		return 1;
	}

	FXT_DatabaseView_SortDsc(&view);

	FXT_ModOption modOption = {};

	UI_Root(&config, &database, &view, &modOption);

	return 1;
}
