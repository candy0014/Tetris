# Tetris 现代俄罗斯方块

玩法大致参考 [tetr.io](https://tetr.io/) 中 ZEN 模式下的规则，方块下落速度恒定，使用 7-Bag 和 SRS+ 或 ARS 旋转系统。

支持在 `user_config.h` 文件中修改部分默认配置，包括：
- 基本参数（行数列数，下落速度，软降重力倍率 SDF，自动移动延迟 DAS，自动重复速率 ARR，移动重置时间，移动重置重置次数限制，Next 显示个数，是否允许 Hold，全隐，反重力等）
- 模式：正常；40 Lines；Blitz
- 垃圾行：无；backfire；unclear（打出的攻击立即以垃圾行返回）；cheese-layer（固定垃圾行层数）；cheese-timer（定时增加垃圾行）；生存模式（测试版）；联机模式（测试版）

## 运行方法

0. 按电脑配置修改 `user_config.h` 文件；
	- 键位在编译之后不能改变，请在编译之前修改键位；
	- 若需联机，服务端地址编译之后不能改变，请在编译之前修改服务端地址；
	- 若为 Win11 用户，需将 `user_config.h` 中的 `WindowsVersion` 变量修改为 `11`；
	- 若为 Linux 用户：
		1. 打开终端，键入 `cat /proc/bus/input/devices`；
		2. 找到含有 `N: Name=" ... Keyboard ... "` 的段落，记下形如 `H: Handlers=sysrq kbd event[ ] leds` 的那一行中 `[ ]` 对应的数字；
		3. 将 `user_config.h` 中的 `KEYHELPER keyhelper("/dev/input/event[ ]");` 中的 `[ ]` 替换为刚才的数字； 
1. 使用集成开发环境或 g++ 等编译器对 `tetr.cpp`（`server.cpp`）进行编译；
	- 命令提示符 (cmd) 编译方法：
		1. 自行下载 C++ 编译器，下以 g++ 为例；
		2. 打开 cmd，`cd` 到游戏文件夹内，键入 `g++ tetr.cpp -o tetr -std=c++11 -lws2_32`（如需联机，继续键入 `g++ server.cpp -o server -std=c++11 -lws2_32`）；
	- Linux 下终端编译方法：
		1. 自行下载 C++ 编译器，下以 g++ 为例；
		2. 打开终端 `cd` 到游戏文件夹内，键入 `g++ tetr.cpp -o tetr -std=c++11 -lpthread`（如需联机，继续键入 `g++ server.cpp -o server -std=c++11`）；
2. 运行游戏。
	- 如需联机，Windows 下运行生成的 `server.exe` 文件，Linux 下打开终端进入游戏文件夹键入 `./server`（运行 `server` 必须先于运行 `tetr`）；
	- Windows 下：运行生成的 `tetr.exe` 文件。
	- Linux 下：打开终端 `cd` 到游戏文件夹内，键入 `sudo ./tetr`（或以管理员身份键入 `./tetr`）

## 游玩方法

直接运行即可进入游戏。

默认键位：左移【←】；右移【→】；顺时针转【↑】；逆时针转【Z】；180 度旋转【A】；软降【↓】；硬降【SPACE】；HOLD【C】；重开【R】；修改配置【P】。

修改配置说明：在任意时刻按 P 进入修改配置界面，上下键选择项目，回车键选中修改，输入值后回车表示输入完成，按 R 重新进入游戏（版面将被清空）。

## 可能的问题

**请先查看自己的操作是否符合【运行方法】中的内容。**

- 方块显示不清楚（有黑色的竖线）：
	
	1. Win11 用户请先检查是否修改 `user_config.h` 中的 `WindowsVersion` 变量为 `11`；
	
	2. 运行游戏，右键点击运行窗口上方；
	
	3. 点击【属性-->字体】，将字体改为黑体。
	
- 显示错位：在某些电脑上，调整窗口大小的代码可能不能起效，需手动调节窗口大小。修改配置界面需要至少 33 行的终端大小，对于一些 Linux 中文终端可能无法显示，以后会考虑修改。

- 联机失败：一台电脑仅能开一个服务器，服务器需在客户端运行前运行，且仅支持前两个客户端接入，不允许某个客户端退出后再次接入。

## 附件

### 键码表

- Windows：详见 <https://blog.csdn.net/qq_42372031/article/details/105178789>；
- Linux：详见 <https://blog.csdn.net/lbc2100/article/details/82380025>。
