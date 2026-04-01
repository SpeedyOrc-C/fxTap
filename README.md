# fxTap

A rhythm game on CASIO calculators,
the successor of [fx4K](https://github.com/SpeedyOrc-C/fx4K).

![Holding fx-9860GIII with fxTap running](./showcase.jpg)

## Features

- Support 1K to 9K.
- Support beatmania IIDX & DJMax column mapping.
- Support Overall Difficulty.
- Customisable key mapping, note falling speed, column width and tap note height.
- Support modifications:
  - Mirror
  - Random
- Support pausing.
- Walking the file system to find out all your beatmaps.
- Previewing beatmap.
- Saving grades.
- Viewing timing distribution as a histogram.
- English & Chinese UI.

## Screenshots

| Gameplay                          | Settings                                  |
|:----------------------------------|:------------------------------------------|
| ![](Gallery/Main%20Menu.png)      | ![](Gallery/Set%20note%20fall%20time.png) |
| ![](Gallery/Song%20selection.png) | ![](Gallery/Set%20note%20appearance.png)  |
| ![](Gallery/Gameplay.png)         | ![](Gallery/Set%20language.png)           |
| ![](Gallery/Modifications.png)    |                                           |

Go to [gallary](Gallery) for more...

## How to play?

Please go to wiki.

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

# Related Projects

- Beatmap converter：[fxTap Adapater](https://github.com/SpeedyOrc-C/fxTap-Adapter)
- Core library：[fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core)
- This repo on
    - [Planète Casio](https://git.planet-casio.com/Chen-Zhanming/fxTap)
    - [GitHub](https://github.com/SpeedyOrc-C/fxTap)
