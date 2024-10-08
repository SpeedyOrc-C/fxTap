# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(MyAddin)

include(GenerateG1A)
include(GenerateG3A)
include(GenerateHH2Bin)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/main.c
  src/UI_MainMenu.c
  src/UI_About.c
  src/UI_Error_FxtapFolderNotFound.c
  src/UI_Error_FxtapFolderEmpty.c
  src/UI_SelectSong.c
  src/UI_Play.c
  src/UI_Settings.c
  src/UI_KeyTest.c
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
)
set(ASSETS_fx
  assets-fx/example.png
  assets-fx/MainMenu_Buttons.png
  assets-fx/About.png
  assets-fx/FN_SelectSong.png
  assets-fx/FN_PreviousNext.png
  assets-fx/Settings_Title.png
  assets-fx/Settings_NotesFallingTime_Caption.png
  assets-fx/Settings_NotesFallingTime_FN_EN.png
  assets-fx/Settings_KeyBindingStyle_Caption.png
  assets-fx/Settings_KeyBindingStyle_Logos.png
  assets-fx/Settings_KeyBindingStyle_FN_EN.png
  assets-fx/Settings_KeyBindings_Caption.png
  assets-fx/Settings_Language_Caption.png
  assets-fx/Settings_Language_Options.png
)
set(ASSETS_cg
  assets-cg/example.png
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
add_subdirectory(fxTap-Core/lib)
target_include_directories(myaddin PUBLIC fxTap-Core/lib/include)
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os -g -flto)
target_link_libraries(myaddin Gint::Gint fxTap-Core)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET myaddin OUTPUT "fxTap.g1a"
    NAME "fxTap" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "fxTap.g3a"
    NAME "fxTap" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
  generate_g3a(TARGET myaddin OUTPUT "fxTap-fx.g3a"
    NAME "fxTap-fx" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCP)
  generate_hh2_bin(TARGET myaddin OUTPUT "MyAddin-hh2.bin")
endif()
