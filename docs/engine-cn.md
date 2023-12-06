# 游戏引擎
本游戏基于一款我们开发的游戏引擎，名叫*SSSH-GL*，全称*Stupid and Simple but Should Help openGL engine*。严格意义来讲这不算是一款引擎，但是它可以通过自动化多线程，事件处理，材质加载来提高我们的工作效率。

## 名空间
本引擎定义主空间为game::，其下名空间有：

| 名称        | 内容       |
| ----------- | ---------- |
| game::RES   | 游戏资源   |
| game::EVENT | 游戏事件   |
| game::UI    | OpenGL相关 |
| game::UTILS | 各类工具   |

三方名空间

| 名称       | 内容         |
| ---------- | ------------ |
| game::HTTP | HTTP请求工具 |

## 类定义
本引擎定义有以下类

| 名空间      | 名称           | 类型            | 描述         |
| ----------- | -------------- | --------------- | ------------ |
| game::EVENT | Event          | struct          | 事件对象     |
| game::EVENT | EventQueue     | struct          | 事件队列     |
| game        | Controller     | class(abstract) | 游戏控制器   |
| game::RES   | Layer          | struct          | UI图层对象   |
| game::RES   | Shape          | class           | UI元素对象   |
| game::RES   | Texture        | struct          | 元素材质对象 |
| game::RES   | TextureManager | class           | 材质管理器   |
| game::UI    | UIConf         | struct          | UI配置对象   |


## 待续
