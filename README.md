# Tetris 现代俄罗斯方块

玩法大致参考 [tetr.io](https://tetr.io/) 中 ZEN 模式下的规则，方块下落速度恒定，使用 7-Bag 和 SRS+ 或 SRS 或 ARS 旋转系统。

支持在 `user_config.txt` 文件中修改部分默认配置，包括：
- 基本参数（行数列数，下落速度，软降重力倍率 SDF，自动移动延迟 DAS，自动重复速率 ARR，移动重置时间，移动重置重置次数限制，Next 显示个数，是否允许 Hold，全隐，反重力等）
- 模式：正常；40 Lines；Blitz
- 垃圾行：无；backfire；unclear（打出的攻击立即以垃圾行返回）；cheese-layer（固定垃圾行层数）；cheese-timer（定时增加垃圾行）；生存模式；联机模式
- Autoplay（测试版）（目前是自己写的估价函数，很菜，希望大家能提供一些 Mochbot 等 Bot 的实现思路）

## 运行方法

1. 使用集成开发环境或 g++ 等编译器对 `tetr.cpp`（`server.cpp`）进行编译：
	- 命令提示符 (cmd) 编译方法：
		1. 自行下载 C++ 编译器，下以 g++ 为例；
		2. 打开 cmd，`cd` 到游戏文件夹内，执行：
			- 编译游戏：`g++ tetr.cpp -o tetr -std=c++11 -lws2_32`；
			- 编译键位修改工具（如果需要）：`g++ keybinding.cpp -o keybinding -std=c++11 -lws2_32`；
			- 编译联机服务端（如果需要）：`g++ server.cpp -o server -std=c++11 -lws2_32`。
	- Linux 下终端编译方法：
		1. 自行下载 C++ 编译器，下以 g++ 为例；
		2. 打开终端 `cd` 到游戏文件夹内，执行：
			- 编译游戏：`g++ tetr.cpp -o tetr -std=c++11 -lpthread`；
			- 编译键位修改工具（如果需要）：`g++ keybinding.cpp -o keybinding -std=c++11 -lpthread`；
			- 编译联机服务端（如果需要）：`g++ server.cpp -o server -std=c++11`。
2. 修改配置：
	`user_config.txt` 文件为各模式/变量的默认值，可在运行游戏前进行修改。其中服务端 IP（若需联机）和 Linux Keyevent（若为 Linux）在运行游戏后不可更改，其他均可在游戏中进行更改。
	- 若为 Win11 用户，需将 `Windows Version` 变量修改为 `11`；
	- 若为 Linux 用户：
		1. 打开终端，键入 `cat /proc/bus/input/devices`；
		2. 找到含有 `N: Name=" ... Keyboard ... "` 的段落，记下形如 `H: Handlers=sysrq kbd event[ ] leds` 的那一行中 `[ ]` 对应的数字；
		3. 将 `Linux Keyevent` 变量修改为刚才的数字；
3. 运行游戏。
	在 Linux 下，以下程序需**以管理员身份运行**。
	1. 运行键位修改工具（如果需要）：`keybinding.exe`/`./keybinding`；
	2. 运行联机服务端（如果需要，必须先于 `tetr` 游戏主程序运行）：`server.exe`/`./server`；
	3. 运行游戏：`tetr.exe`/`./tetr`。

## 游玩方法

直接运行即可进入游戏。

默认键位：左移 (L)【←】；右移 (R)【→】；顺时针转 (CW)【↑】；逆时针转 (CCW)【Z】；180 度旋转 (F)【A】；软降 (SD)【↓】；硬降 (HD)【SPACE】；HOLD(H)【C】；重开 (RE)【R】；修改配置 (SET)【P】。

修改配置说明：在任意时刻按 P 进入修改配置界面，上下键选择项目，左右键切换界面，回车键选中修改，输入值后回车表示输入完成，按 R 重新进入游戏（版面将被清空）。

## 可能的问题

**请先查看自己的操作是否符合【运行方法】中的内容。**

- 方块显示不清楚（有黑色的竖线）：
	
	1. Win11 用户请先检查是否修改 `user_config.h` 中的 `WindowsVersion` 变量为 `11`；
	
	2. 运行游戏，右键点击运行窗口上方；
	
	3. 点击【属性-->字体】，将字体改为黑体。
	
- 显示错位：在某些电脑上，调整窗口大小的代码可能不能起效，需手动调节窗口大小。

- 联机失败：一台电脑仅能开一个服务器，服务器需在客户端运行前运行，且仅支持前两个客户端接入，不允许某个客户端退出后再次接入。

## 附件

### 键码表

- Windows：详见 <https://blog.csdn.net/qq_42372031/article/details/105178789>；
- Linux：详见 <https://blog.csdn.net/lbc2100/article/details/82380025>。

