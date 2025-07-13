# Terminal Games

一些 C++ 终端小游戏。

作者是一名码风极其丑陋的 OIer，所以绝大部分代码没有注释，变量等命名并不规范 ~~，还喜欢压行~~，大家当看个乐子就行。

目前已经编写完成：

**俄罗斯方块 (tetris) [Windows]**

玩法大致参考 [tetr.io](https://tetr.io/) 中 ZEN 模式下的规则，方块下落速度恒定，使用 7-Bag 和 SRS+ 或 ARS 旋转系统。

支持在 `config.h` 文件中修改部分默认配置，包括：
- 基本参数（行数列数，下落速度，软降重力倍率 SDF，自动移动延迟 DAS，自动重复速率 ARR，移动重置时间，移动重置重置次数限制，Next 显示个数，是否允许 Hold）
- 模式：正常；全隐
- 垃圾行：无；unclear（打出的攻击立即以垃圾行返回）；cheese-layer（固定垃圾行层数）；cheese-timer（定时增加垃圾行）

**斗地主联机**

玩法参考 [Wiki 百科](https://zh.wikipedia.org/zh-cn/%E9%AC%A5%E5%9C%B0%E4%B8%BB)。暂未加入抢地主规则（目前是随机地主）。