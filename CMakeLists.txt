cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 23)
project(fxTap)

set(SOURCES
		src/main.c
		src/ui/main-menu.c
		src/ui/about.c
		src/ui/select-song.c
		src/ui/play.c
		src/ui/settings.c
		src/ui/key-test.c
		src/settings/notes-falling-time.c
		src/settings/key-bindings.c
		src/settings/key-binding-style.c
		src/settings/language.c
)
set(ASSETS
)
set(ASSETS_fx
		assets-fx/MainMenu_Buttons.png
		assets-fx/About.png
		assets-fx/SelectASong_Title.png
		assets-fx/SelectASong_FN.png
		assets-fx/Path_FN.png

		assets-fx/SongName_FN.png
		assets-fx/Accuracy_FN.png
		assets-fx/Ascending_FN.png
		assets-fx/Descending_FN.png

		assets-fx/Settings_Language_FN.png
		assets-fx/Settings_Title.png
		assets-fx/Settings_NotesFallingTime_Caption.png
		assets-fx/Settings_NotesFallingTime_FN_EN.png
		assets-fx/Settings_KeyBindingStyle_Caption.png
		assets-fx/Settings_KeyBindingStyle_Logos.png
		assets-fx/Settings_KeyBindingStyle_FN_EN.png
		assets-fx/Settings_KeyBindings_Caption.png
		assets-fx/Settings_Language_Caption.png
		assets-fx/Settings_Language_Options.png
		assets-fx/SelectSong_TypeFilename.png
)
set(ASSETS_cg
		assets-cg/example.png
)

include(GenerateG1A)
include(GenerateG3A)
include(GenerateHH2Bin)
include(Fxconv)
find_package(Gint REQUIRED)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(fxTap ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
add_subdirectory(fxTap-Core)
target_include_directories(fxTap PUBLIC fxTap-Core/include)
target_include_directories(fxTap PUBLIC include)
target_compile_options(fxTap PRIVATE -Wall -Wextra -Os -g -flto)
target_compile_definitions(fxTap-Core PUBLIC FXTAP_CORE_ON_GINT FXTAP_CORE_USE_CASIOWIN)
target_link_libraries(fxTap Gint::Gint fxTap-Core)

if ("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
	generate_g1a(TARGET fxTap OUTPUT "fxTap.g1a" NAME "fxTap" ICON assets-fx/icon.png)
elseif ("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
	generate_g3a(TARGET fxTap OUTPUT "fxTap-fx.g3a" NAME "fxTap-fx" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif ()
