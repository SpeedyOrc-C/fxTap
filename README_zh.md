# fxTap

在卡西欧计算器上的音游，同时也是 [fx4K](https://github.com/SpeedyOrc-C/fx4K) 的续作。

![Holding fx-9860GIII with fxTap running](./showcase.jpg)

## 功能

- 支持一键至九键。
- 支持 beatmania IIDX 和 DJMax 的轨道映射。
- 支持谱面的准度要求。
- 可自定义的按键映射、下落速度、音符的高度与轨道的宽度。
- 支持暂停。
- 会自动在文件系统里面搜索所有的谱面。
- 支持保存成绩。
- 英文和中文用户界面。

# 怎么玩？

请前往 Wiki 查看。

# 谱面目录

[fxTap Index](https://github.com/SpeedyOrc-C/fxTap-Index)

如果你不想自己转换谱面的话，这里有一些能直接开始玩的谱面。

# 构建

先用 [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) 搭建开发环境。然后把依赖
[fxTap Core](https://github.com/SpeedyOrc-C/fxTap-Core) 下载到这个文件夹里。

```sh
git clone https://github.com/SpeedyOrc-C/fxTap-Core
```

运行这个命令来编译 `fxTap.g1a`。

```sh
fxsdk build-fx
```
