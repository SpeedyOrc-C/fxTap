#include <fxlibc/printf.h>
#include <fxTap/config.h>
#include <fxTap/database.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "ui.h"

static FXT_ConfigError LoadConfig(FXT_Config *config)
{
	if (gint[HWFS] == HWFS_FUGUE)
		return FXT_Config_Load(config);

	return gint_call((gint_call_t){
		.function = &FXT_Config_Load_BFile,
		.args = {{.pv = config}}
	});
}

int main(void)
{
	__printf_enable_fp();

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
	FXT_Database_Init(&database);

	auto const databaseError = FXT_Database_SyncFromFileSystem(&database);

	if (databaseError)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "Database Error: %d", configError);
		dupdate();
		getkey();
		return 1;
	}

	UI_MainMenuLoop(&config, &database);

	return 1;
}
