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
本引擎定义有以下类，具体详情请前往对应的hpp文件查看

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
本引擎定义有以下函数供调用，具体详情请前往对应的hpp文件查看

| 名空间      | 名称             | 返回值   | 参数                               | 描述                               |
| ----------- | ---------------- | -------- | ---------------------------------- | ---------------------------------- |
| game::UTILS | getTimestamp     | uint64_t | -                                  | 返回一个unix时间戳，单位毫秒       |
| game::UTILS | highResTimestamp | uint64_t | -                                  | 返回一个高精度unix时间戳，单位纳秒 |
| game::UTILS | findInEnvp       | bool     | char **envp, std::string varToFind | 返回envp中是否存在varToFind        |
| game::UTILS | redirectCout     | -        | std::string filename               | 将stdio重定向到指定文件            |
| game::UI    | init             | int      | int argc, char **argv, UIConf conf | 初始化OpenGL 窗口                  |
| game::UI    | start            | -        | -                                  | 启动OpenGL渲染循环                 |

## 编程接口
### 总览
作为用户，你不需要知道OpenGL是如何运行的，你只需要使用以下编程接口与其互动
- std::mutex game::layersMutex
  - 这是game::layers对象的锁
  - 请在向layers对象写入之前使用lock_guard获取本锁

- std::map<int, game::RES::Layer> game::layers
  - 这是游戏内的全部图层，map中的int为显示顺序，Layer为图层对象
  - 注意！向此对象写入之前请先加锁

- game::EVENT::EventQueue game::eventQueue
  - 这是游戏的事件队列，所有键盘事件都会被推入此队列
  - 请务必定期使用pop方法缩减队列，队列溢出后将无法获取事件

- game::RES::TextureManager game::textureManager
  - 材质管理器，用于管理全局元素材质
  - 通常在窗口创建之后，循环创建之前，用于加载材质

- game::RES::TextureManager game::fontTextureManager
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

### 详解
#### std::map<int, game::RES::Layer> game::layers
这是一个全局对象，用来存储所有屏幕上显示的图层，每一个图层又包含一个向量，用于承载所有该图层内的元素。最小单位为Shape，请参见类定义详解。本对象中的Layer为图层对象，int为显示顺序（表示了图层的重叠关系）。本对象异常重要，会被频繁读取，请**务必遵守game::layersMutex的加锁规范**。

用例：创建一个元素，使其显示为res/logo.png的材质
```
game::layers.insert({1, game::RES::Layer()});

game::RES::Shape shape;
shape.texture = game::textureManager.getTexture("logo");
shape.x = 0;
shape.y = -1;
shape.width = 8;
shape.height = 8;

game::layers[0].shapes.push_back(shape);
```

#### std::mutex game::layersMutex
这是一个锁对象，请务必在修改game::layers的任何内容之前获取本锁。本锁异常重要，因为本游戏渲染线程和控制器线程异步运行，可能会出现在渲染线程读取game::layers内容时其内容被修改。由于渲染线程与控制器线程运行周期相同，该状况较为普遍。在大多数情况下，没有正确获取锁会导致程序崩溃（在第二周被验证）。目前为止，未观测到不加锁读取产生的错误，请注意，如果多个控制器同时运行，则必须在读取之前加锁。

用例：出界检测
```
std::lock_guard<std::mutex> *layersLock = new std::lock_guard<std::mutex>(game::layersMutex);
for (size_t i = 1; i < game::layers[1].shapes.size(); i++)
{
    if (outOfBound(game::layers[1].shapes[i]))
    {
        game::layers[1].shapes.erase(game::layers[1].shapes.begin() + i);
    }
    game::layers[1].shapes[i].x -= 0.05;
}
delete layersLock;
```

#### game::EVENT::EventQueue game::eventQueue
事件队列，内含两大功能：
- 触发事件
- 持续按键

其中，触发事件必须被处理，否则事件队列在超过10个事件被触发之后会溢出。该队列为std::queue\<Event>，包含事件对象，定义如下：
```
struct Event
{
    Event();
    enum EventType
    {
        NONE,
        KEYBOARD_EVENT,
        KEYBOARD_UP_EVENT,
    };
    EventType type;

    std::vector<int> data;
};
```
暴露方法：
- Event pop() - 用于获取队头事件，请使用该方法处理事件
- bool empty() - 返回队列是否为空，请使用该方法决定是否需要处理事件
- bool overflow() - 返回队列是否溢出，调试使用

暴露对象：
- std::set<int> pressedKeys - 被按下的按键之键码，用于持续按键检测
- bool block - 禁止新事件输入的标志

用例： 检测是否退出，检测是否移动
```
if (!game::eventQueue.empty())
{
    // One time key down detection
    game::EVENT::Event event = game::eventQueue.pop();
    if (event.type == game::EVENT::Event::EventType::KEYBOARD_EVENT && event.data[0] == 'q')
        quit = true;
}

// Continuous key press detection - press to move our ship
auto &eqk = game::eventQueue.pressedKeys; // Shorten the stupid long name of a game variable
auto &ourShip = game::layers[1].shapes[0];
shipVelocity = Vec2(0.02, 0);
if (eqk.find('w') != eqk.end() && ourShip.y > -MAX_VERTI_COORD)
{
    game::layers[1].shapes[0].y -= 0.1;
    shipVelocity.y -= 0.1;
}
```

#### game::RES::TextureManager game::textureManager
该对象为全局材质管理器，用于加载和分配材质。该管理器可实现材质的单次加载和自动管理，便于节约内存空间。有两个重要的生命周期需要注意：
- 加载材质
  - void changePath(std::string path) - 更改材质加载路径，默认为"res/"
  - void loadAllTextures(bool overrideColor = false, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) - 加载该路径下所有材质，可选覆写颜色，如果启用覆写颜色，将只保留alpha通道，将其他所有通道覆写为指定颜色。**注意，本方法只会加载PNG文件**
- 获取材质
  - Texture &getTexture(std::string fileName) - 获取指定材质，注意这里的文件名不包括路径及扩展名。例：获取“res/a.png”则直接调用getTexture("a")。
  - 返回对象为一个材质对象，定义如下：

```
struct Texture
{
    Texture();
    ~Texture();
    Texture(std::string fileName);
    Texture(std::string fileName, float r, float g, float b);
    std::string fileName;
    int width;
    int height;
    bool hasAlpha;
    GLubyte **data;
    GLuint textureID;

private:
    void initTexture();
    bool loadPngImage(bool overrideColor = false, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
};
```
*大多数情况下，材质对象不需要修改就可以直接使用。请不要随意修改材质对象的成员变量*

用例：
```
// Load texture, must be in main.cpp, between UI::init and UI::start
game::textureManager.loadAllTextures();

// Use texture
shape.texture = game::textureManager.getTexture("logo");
```

#### game::RES::TextureManager game::fontTextureManager
同样也是材质管理器，但是用于管理字体文件，详情请参考上条。

用例：
```
// Load texture, must be in main.cpp, between UI::init and UI::start
game::fontTextureManager.changePath("res/font1/");
game::fontTextureManager.loadAllTextures(true, 255, 255, 255);
```

#### game::PERF::PerformanceManager
游戏性能管理器，用于在终端输出实时游戏性能信息，只做调试使用，请勿用于生产。
暴露方法：
- game::PERF::performanceManager.start() - 启动性能管理器，启动后将覆写一切打印到终端的内容，请在调试输出时禁用。

用例：
```
game::PERF::performanceManager.start();
```

#### game::Controller
控制器类，用于控制整个游戏的声明周期。请注意这是一个抽象类，在使用之前需要从其创建子类，声明自定义的控制器。

本类声明如下：
```
class Controller
{
public:
    Controller();
    Controller(std::function<void()> callback);
    Controller *start();
    Controller *then(std::function<void()> callback);

protected:
    void _start();
    virtual void tick() = 0;
    virtual void init() = 0;
    virtual void exit() = 0;
    void blockEvent(bool block);

    std::thread controllerThread;
    std::function<void()> callback;
    uint64_t TICK_DELAY = 16 * 1000 * 1000; // Nanoseconds
    bool quit = false;
};
```
其中有三个必须实现的抽象方法：
- tick() - 每一个逻辑帧被调用，用于连续处理游戏逻辑
  - 可以使用quit标记退出，将quit设置为true之后，控制器将在下一帧开始之前退出
- init() - 控制器启动时被调用，用于初始化游戏对象
- exit() - 控制器退出时被调用，用于清理控制器内部元素

用例：
```
void game::StartScreenController::tick()
{
    if (!game::eventQueue.empty())
    {
        // One time key down detection
        game::EVENT::Event event = game::eventQueue.pop();
        if (event.type == game::EVENT::Event::EventType::KEYBOARD_EVENT && event.data[0] == 'c')
            quit = true;
    }
}
void game::StartScreenController::init()
{
    std::cout << "TestController init" << std::endl;

    game::layers.insert({1, game::RES::Layer()});

    game::RES::Shape shape;
    shape.texture = game::textureManager.getTexture("logo");
    shape.x = 0;
    shape.y = -1;
    shape.width = 8;
    shape.height = 8;

    game::layers[0].shapes.push_back(shape);
    srand(game::UTILS::getTimestamp() % 10000);
}

void game::StartScreenController::exit()
{
    std::cout << "TestController exit" << std::endl;
    game::layers.clear();
}
```

暴露方法：
- start() - 启动该控制器，调用本方法将启动当前控制器对象，并将其置于新线程内运行
- then(std::function<void()> callback) - 指定回调函数，传入的回调函数将在该控制器退出之后被调用

用例：
```
game::StartScreenController controller;
game::TestController controller2;
game::EndScreenController controller3;

controller.start()->then([&]()
                          {
    controller2.start(); });

controller2.then([&]()
                  {
        controller3.setScore(controller2.getScore());
        controller3.setUserName(getenv("LOGNAME"));
        controller3.start(); });

controller3.then([&]()
                  {
        glutLeaveMainLoop(); });
```


# 引擎部分结束，游戏本体待续
