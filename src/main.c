#include <fxTap/config.h>
#include <gint/display.h>
#include <gint/hardware.h>
#include <gint/keyboard.h>
#include "ui.h"

FXT_Config_Error FXT_Config_Load_BFile_Wrapper(FXT_Config *dst)
{
	return FXT_Config_Load_BFile(dst);
}

FXT_Config_Error LoadConfig(FXT_Config *config)
{
	if (gint[HWFS] == HWFS_FUGUE)
		return FXT_Config_Load(config);

	return gint_call((gint_call_t){
		.function = &FXT_Config_Load_BFile_Wrapper,
		.args = {{.pv = config}}
	});
}

int main(void)
{
	FXT_Config config;
	const FXT_Config_Error configError = LoadConfig(&config);

	if (configError != 0)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "%d", configError);
		dupdate();
		getkey();
		return 1;
	}

	UI_MainMenu(&config);

	return 1;
}
