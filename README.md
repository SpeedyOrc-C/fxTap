# fxTap

A rhythm game runs on CASIO calculators,
successor of [fx4K](https://github.com/SpeedyOrc-C/fx4K),
written in pure C, powered by gint.

## Build

1. Set up [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk).
2. Clone dependency [fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core) inside this folder.

```sh
git clone https://github.com/SpeedyOrc-C/fxTap-Core
```

3. Run this command:

```sh
fxsdk build
```

## How to play?

1. Download your favourite song's beatmap on [osu!](https://osu.ppy.sh).
2. All beatmap is a ZIP archive, so unzip them, ignoring its `osz` extension.
3. Each archive may contain more than one beatmaps, so pick one osu! beatmap with `osu` extension.
4. Convert that beatmap to fxTap's format with [fxTap Adapter](https://github.com/SpeedyOrc-C/fxTap-Adapter).
5. Copy the executable to your calculator.
6. Copy the beatmaps with `fxt` extension to folder `FXTAP` in flash memory of your calculator. (One GII model has a SD card slot, don't use it.)
7. Open the settings to configure the key-mappings and notes falling speed.
8. For old models (G and GII) that use CASIOWIN file system, you have to type the file name manually. For new models (GIII and CG), you can simply choose a beatmap from the list.

## Compatibility

When reading files, this app is compatible with both BFile interface from CASIO
and stream interface in C standard. So it is safe to run on GII and GIII.

Also, the UI is implemented differently on monochrome and chromatic models.

## Related Projects

* [fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core)
* [fxTap Adapter](https://github.com/SpeedyOrc-C/fxTap-Adapter)
