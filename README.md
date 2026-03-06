# fxTap

A rhythm game runs on CASIO calculators, the successor of [fx4K](https://github.com/SpeedyOrc-C/fx4K).

# How to play?

## Download

You can download your favourite song's beatmap from the following games:

### [osu!](https://osu.ppy.sh)

Each beatmap is a ZIP archive, so unzip them, ignoring its `osz` extension.
Each archive may contain more than one beatmap,
so pick one osu! beatmap with `osu` extension.

### [Malody](https://malody.mugzone.net)

Each beatmap is a JSON file, which can be converted directly.

## Convert

Convert that beatmap to fxTap's format with [fxTap Adapter](https://github.com/SpeedyOrc-C/fxTap-Adapter).

## Copy

Copy the executable to your calculator.

- G/GII: Use [FA-124](https://edu.casio.com/forteachers/er/software) via USB cable (legacy).
- GII SD: Use SD card (legacy).
- GIII: Use USB cable directly.

Also copy the beatmaps with `fxt` extension in flash memory of your calculator.
(GII SD model has an SD card slot,
please copy your beatmap from SD card to flash memory before play.)
Open the settings to configure the key-mappings and notes falling speed.

# Beatmap Index

[fxTap Index](https://github.com/SpeedyOrc-C/fxTap-Index)

Here you can find some beatmaps ready to play,
especially if you don't want to convert the beatmaps by yourself.


# Build

Set up [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) first. And then clone
dependency [fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core) inside this folder.

```sh
git clone https://github.com/SpeedyOrc-C/fxTap-Core
```

Run this command to build the executable `fxTap.g1a`.

```sh
fxsdk build-fx
```
