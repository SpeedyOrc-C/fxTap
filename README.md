# fxTap

A rhythm game runs on CASIO calculators, the successor of [fx4K](https://github.com/SpeedyOrc-C/fx4K).

## How to play?

1. Download your favourite song's beatmap.
    1. [osu!](https://osu.ppy.sh)
        1. Each beatmap is a ZIP archive, so unzip them, ignoring its `osz` extension.
        2. Each archive may contain more than one beatmap, so pick one osu! beatmap with `osu` extension.
    2. [Malody](https://malody.mugzone.net)
        1. Each beatmap is a JSON file, which can be converted directly.
2. Convert that beatmap to fxTap's format with [fxTap Adapter](https://github.com/SpeedyOrc-C/fxTap-Adapter).
3. Copy the executable to your calculator.
    1. G/GII: Use [FA-124](https://edu.casio.com/forteachers/er/software) via USB cable (legacy).
    2. GII SD: Use SD card (legacy).
    3. GIII: Use USB cable directly.
4. Also copy the beatmaps with `fxt` extension in flash memory of your calculator. (GII SD model has an SD
   card slot, please copy your beatmap from SD card to flash memory before play.)
5. Open the settings to configure the key-mappings and notes falling speed.

## Build

Set up [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) first. And then clone
dependency [fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core) inside this folder.

```sh
git clone https://github.com/SpeedyOrc-C/fxTap-Core
```

Run this command to build `fxTap.gia`.

```sh
fxsdk build
```
