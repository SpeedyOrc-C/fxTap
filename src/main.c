#include "ui.h"
#include <gint/hardware.h>
#include <gint/display.h>
#include <assert.h>

void Config_New_LoadFromDisk_BFile_Wrapper(Config **config, ConfigError *error)
{
	*config = Config_New_LoadFromDisk_BFile(error);
}

int main(void)
{
	Config *config;
	ConfigError configError;

	#ifdef FX9860G
		if (gint[HWFS] == HWFS_CASIOWIN)
		{
			const gint_call_t call = {
				.function = &Config_New_LoadFromDisk_BFile_Wrapper,
				.args = {{.pv = &config}, {.pv = &configError}}
			};
			gint_call(call);
		}
		else
			config = Config_New_LoadFromDisk(&configError);
	#endif

	#ifdef FXCG50
		config = Config_New_LoadFromDisk(&configError);
	#endif

	if (config == NULL)
	{
		dclear(C_WHITE);
		dprint(1, 1, C_BLACK, "%d", configError);
		dupdate();
		getkey();
		return 1;
	}

	assert(config);

	UI_MainMenu(config);

	return 1;
}
