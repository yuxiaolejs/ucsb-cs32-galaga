# 游戏引擎
本游戏基于一款我们开发的游戏引擎，名叫*SSSH-GL*，全称*Stupid and Simple but Should Help openGL engine*。严格意义来讲这不算是一款引擎，但是它可以通过自动化多线程，事件处理，材质加载来提高我们的工作效率。

## 名空间
本引擎定义主空间为game::，其下名空间有：

| 名称        | 内容       |
| ----------- | ---------- |
| game::RES   | 游戏资源   |
| game::EVENT | 游戏事件   |
| game::UI    | OpenGL相关 |
| game::PERF  | 游戏性能   |
| game::UTILS | 各类工具   |

三方名空间

| 名称       | 内容         |
| ---------- | ------------ |
| game::HTTP | HTTP请求工具 |

## 类定义
本引擎定义有以下类

| 名空间      | 名称               | 类型            | 描述             |
| ----------- | ------------------ | --------------- | ---------------- |
| game::EVENT | Event              | struct          | 事件对象         |
| game::EVENT | EventQueue         | struct          | 事件队列         |
| game        | Controller         | class(abstract) | 游戏控制器       |
| game::RES   | Layer              | struct          | UI图层对象       |
| game::RES   | Shape              | class           | UI元素对象       |
| game::RES   | Texture            | struct          | 元素材质对象     |
| game::RES   | TextureManager     | class           | 材质管理器       |
| game::UI    | UIConf             | struct          | UI配置对象       |
| game::PERF  | PerformanceManager | class           | 提供游戏性能统计 |
| game::UTILS | Timer              | class           | 提供时钟工具     |

## 函数定义
本引擎定义有以下函数供调用

| 名空间      | 名称             | 返回值   | 参数                               | 描述                               |
| ----------- | ---------------- | -------- | ---------------------------------- | ---------------------------------- |
| game::UTILS | getTimestamp     | uint64_t | -                                  | 返回一个unix时间戳，单位毫秒       |
| game::UTILS | highResTimestamp | uint64_t | -                                  | 返回一个高精度unix时间戳，单位纳秒 |
| game::UTILS | findInEnvp       | bool     | char **envp, std::string varToFind | 返回envp中是否存在varToFind        |
| game::UTILS | redirectCout     | -        | std::string filename               | 将stdio重定向到指定文件            |
| game::UI    | init             | int      | int argc, char **argv, UIConf conf | 初始化OpenGL 窗口                  |
| game::UI    | start            | -        | -                                  | 启动OpenGL渲染循环                 |

## 编程接口
作为用户，你不需要知道OpenGL是如何运行的，你只需要使用以下编程接口与其互动
- std::mutex layersMutex
  - 这是game::layers对象的锁
  - 请在向layers对象写入之前使用lock_guard获取本锁

- std::map<int, game::RES::Layer> game::layers
  - 这是游戏内的全部图层，map中的int为显示顺序，Layer为图层对象
  - 注意！向此对象写入之前请先加锁

- game::EVENT::EventQueue eventQueue
  - 这是游戏的事件队列，所有键盘事件都会被推入此队列
  - 请务必定期使用pop方法缩减队列，队列溢出后将无法获取事件

- game::RES::TextureManager textureManager
  - 材质管理器，用于管理全局元素材质
  - 通常在窗口创建之后，循环创建之前，用于加载材质

- game::RES::TextureManager fontTextureManager
  - 字体材质管理器，用于导入管理字体图片
  - 使用方法与textureManager类似
  - 请注意，字体必须是恒宽的

- game::PERF::PerformanceManager game::PERF::performanceManager
  - 游戏性能监视器
  - 使用performanceManager.start()来启动性能监视器

- game::Controller
  - 控制器类，不是全局对象
  - 这是一个抽象类，用于帮助用户构建游戏控制器
  - 请在设计控制器类时继承本类


## 待续
