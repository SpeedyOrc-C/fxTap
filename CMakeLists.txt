cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(fxTap)

set(FXTAP_VERSION "01.05.0000")

set(SOURCES
		src/main.c
		src/routes/about/+page.c
		src/routes/key-test/+page.c
		src/routes/main-menu/+page.c
		src/routes/play/+page.c
		src/routes/select-song/+page.c
		src/routes/settings/+page.c
		src/routes/settings/notes-falling-time.c
		src/routes/settings/note-appearance.c
		src/routes/settings/key-bindings.c
		src/routes/settings/key-binding-style.c
		src/routes/settings/language.c
		src/routes/settings/custom-overall-difficulty.c
)
set(ASSETS
)
set(ASSETS_fx
		assets-fx/Banner.png
		assets-fx/Elemental_Creation.png
		assets-fx/Daisuke.png

		assets-fx/MainMenu_Buttons.png
		assets-fx/About.png
		assets-fx/SelectSong_TypeFilename.png
		assets-fx/SelectASong_Title.png
		assets-fx/SelectASong_FN.png
		assets-fx/Path_FN.png

		assets-fx/SongName_FN.png
		assets-fx/Accuracy_FN.png
		assets-fx/Ascending_FN.png
		assets-fx/Descending_FN.png
		assets-fx/Pause_FN.png
		assets-fx/Save_FN.png

		assets-fx/Settings_Language_FN.png
		assets-fx/Settings_Title.png
		assets-fx/Settings_NoteAppearance_Caption.png
		assets-fx/Settings_NoteAppearance_FN.png
		assets-fx/Settings_NotesFallingTime_Caption.png
		assets-fx/Settings_NotesFallingTime_FN_EN.png
		assets-fx/Settings_KeyBindingStyle_Caption.png
		assets-fx/Settings_KeyBindingStyle_Logos.png
		assets-fx/Settings_KeyBindingStyle_FN_EN.png
		assets-fx/Settings_KeyBindings_Caption.png
		assets-fx/Settings_Language_Caption.png
		assets-fx/Settings_Language_Options.png
		assets-fx/Settings_CustomOverallDifficulty_Caption.png
		assets-fx/Settings_CustomOverallDifficulty_Disabled_FN.png
		assets-fx/Settings_CustomOverallDifficulty_Enabled_FN.png
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
target_link_libraries(fxTap Gint::Gint fxTap-Core)

if ("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
	generate_g1a(TARGET fxTap VERSION ${FXTAP_VERSION} OUTPUT "fxTap.g1a" NAME "fxTap" ICON assets-fx/icon.png)
elseif ("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
	generate_g3a(TARGET fxTap VERSION ${FXTAP_VERSION} OUTPUT "fxTap.g3a" NAME "fxTap" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif ()
