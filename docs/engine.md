# The engine
This game is based on a engine we developed, named *SSSH-GL*, which stands for *Stupid and Simple but Should Help openGL engine*. **It's technically NOT a game engine**, but it's designed to simplify our work by providing automated thread managing, event handling, and texture loading.

## Namespaces
The main namespace for this engine is `game::`, under which these namespace are defined:

| Name        | Content          |
| ----------- | ---------------- |
| game::RES   | Game resources   |
| game::EVENT | Game events      |
| game::UI    | OpenGL related   |
| game::PERF  | Game performance |
| game::UTILS | Utilities        |

Third party namespaces

| Name       | Content            |
| ---------- | ------------------ |
| game::HTTP | HTTP request tools |

## Classes
Following classes are defined, please go to corresponding hpp file for details.

| Namespace   | Name               | Type            | Description                                       |
| ----------- | ------------------ | --------------- | ------------------------------------------------- |
| game::EVENT | Event              | struct          | Event object                                      |
| game::EVENT | EventQueue         | struct          | A queue of Event objects                          |
| game        | Controller         | class(abstract) | Game controller template                          |
| game::RES   | Layer              | struct          | UI Layers that contains Shapes                    |
| game::RES   | Shape              | class           | UI Elements in a Layer                            |
| game::RES   | Texture            | struct          | Texture object                                    |
| game::RES   | TextureManager     | class           | Texture Manager that loads and distributs texture |
| game::UI    | UIConf             | struct          | UI Configuration object                           |
| game::PERF  | PerformanceManager | class           | Game performance system                           |
| game::UTILS | Timer              | class           | Timing utilities                                  |

## Functions
Following functions are defined, please go to corresponding hpp file for details.

| Namespace   | Name             | Returns  | Parameters                         | Description                        |
| ----------- | ---------------- | -------- | ---------------------------------- | ---------------------------------- |
| game::UTILS | getTimestamp     | uint64_t | -                                  | Returns unix timestamp, in ms      |
| game::UTILS | highResTimestamp | uint64_t | -                                  | Returns high res timestamp, in ns  |
| game::UTILS | findInEnvp       | bool     | char **envp, std::string varToFind | Returns if envp contains varToFind |
| game::UTILS | redirectCout     | -        | std::string filename               | Redirect stdio to a file           |
| game::UI    | init             | int      | int argc, char **argv, UIConf conf | Init OpenGL window                 |
| game::UI    | start            | -        | -                                  | Start OpenGL rendering loop        |

## APIs
### Overview
As a user, you don't need to know how OpenGL runs, you just need to interact with it using the following programming interfaces
- std::mutex game::layersMutex
  - This is a lock for game::layers
  - Please obtain this using lock_guard before writing to layers object

- std::map<int, game::RES::Layer> game::layers
  - This is all the layers in the game, int in the map indicates the order of display
  - Obtain lock before writing to this!

- game::EVENT::EventQueue game::eventQueue
  - This is the game's event queue, all keyboard events will be pushed into this queue
  - Please be sure to use the pop method to reduce the queue regularly. Events will not be obtained after the queue overflows.

- game::RES::TextureManager game::textureManager
  - Texture manager, used to manage global element textures
  - Usually used to load textures after the window is created and before the loop is created.

- game::RES::TextureManager game::fontTextureManager
  - Font texture manager, used to import and manage font images
  - The usage is similar to textureManager
  - Please note that the font must be constant width

- game::PERF::PerformanceManager game::PERF::performanceManager
  - Game performance monitor
  - Use performanceManager.start() to start the performance monitor

- game::Controller
  - Controller class, not global object
  - This is an abstract class used to help users build game controllers
  - Please derive from this class when designing the controller class

### Detailed explaination
#### std::map<int, game::RES::Layer> game::layers
This is a global object used to store all layers displayed on the screen. Each layer contains a vector used to carry all elements in the layer. The smallest unit is Shape, please refer to the detailed explanation of class definition. In this object, the `int` key is used to determine display order (overlapping of the layers), and `Layer` key is the Layer object to display. This object is extremely important and will be read frequently. Please **be sure to comply with the locking specifications of game::layersMutex**.

Use case: Create an element that displays as a texture for res/logo.png
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
This is a lock object. Please be sure to obtain this lock before modifying any content in game::layers. This lock is extremely important because the game rendering thread and the controller thread run asynchronously, and the content of game::layers may be modified when the rendering thread reads the content. This situation is common because the rendering thread and the controller thread run on the same cycle. In most cases, not acquiring the lock correctly will cause the program to crash (verified in week 2). So far, no errors have been observed from reading without locking, please note that if multiple controllers are running simultaneously, locking must be performed before reading.

Use case: out-of-bounds detection
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
The event queue contains two major functions:
- Triggerred event
- Continuous key press

Among them, the triggering event must be processed, otherwise the event queue will overflow after more than 10 events are triggered. The queue is std::queue\<Event>, which contains event objects and is defined as follows:
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
Exposure method:
- Event pop() - used to obtain the queue head event, please use this method to handle the event
- bool empty() - Returns whether the queue is empty. Please use this method to decide whether the event needs to be processed.
- bool overflow() - returns whether the queue overflows, used for debugging

Exposed objects:
- std::set<int> pressedKeys - the key code of the pressed key, used for continuous key detection
- bool block - flag to disable new event input

Use cases: Detect whether to exit, detect whether to move
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
This object is the global texture manager and is used to load and assign textures. This manager enables single loading and automatic management of textures to save memory space. There are two important life cycles to note:
- Load textures
   - void changePath(std::string path) - Change the texture loading path, the default is "res/"
   - void loadAllTextures(bool overrideColor = false, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) - Load all textures under this path, with optional override color. If override color is enabled, only the alpha channel will be retained. Overwrites all other channels to the specified color. **Note, this method will only load PNG files**
- Get texture
   - Texture &getTexture(std::string fileName) - Get the specified texture. Note that the file name here does not include the path and extension. Example: To obtain "res/a.png", directly call getTexture("a").
   - The returned object is a texture object, defined as follows:

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
*In most cases, texture objects can be used directly without modification. Please do not modify the member variables of the texture object at will*

Use case:
```
// Load texture, must be in main.cpp, between UI::init and UI::start
game::textureManager.loadAllTextures();

// Use texture
shape.texture = game::textureManager.getTexture("logo");
```

#### game::RES::TextureManager game::fontTextureManager
It is also a texture manager, but it is used to manage font files. Please refer to the previous part for details.

Example:
```
// Load texture, must be in main.cpp, between UI::init and UI::start
game::fontTextureManager.changePath("res/font1/");
game::fontTextureManager.loadAllTextures(true, 255, 255, 255);
```

#### game::PERF::PerformanceManager
The game performance manager is used to output real-time game performance information on the terminal. It is only used for debugging and should not be used for production.
Exposure method:
- game::PERF::performanceManager.start() - Start the performance manager. After startup, everything printed to the terminal will be overwritten. Please disable it when debugging output.

Example:
```
game::PERF::performanceManager.start();
```

#### game::Controller
Controller class, used to control the entire game lifecycle. Please note that this is an abstract class and you need to create a derived class from it and declare a custom controller before using it.

It's defined as follows:
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
There are three abstract methods that **must** be implemented:
- tick() - called every logical frame for continuous processing of game logic
   - You can exit using the `quit` flag. After setting `quit` to true, the controller will exit before the next frame starts.
- init() - called when the controller starts, used to initialize the game object
- exit() - called when the controller exits, used to clean up the internal elements of the controller

Example:
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

Exposure method:
- start() - Start the controller. Calling this method will start the current controller object and place it to run in a new thread.
- then(std::function<void()> callback) - Specify the callback function. The passed callback function will be called after the controller exits.

Example:
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
## The Game
This game is driven by the following controllers, which run in order from top to bottom:
- StartScreenController
- TestController
- EndScreenController

## Controller
All controllers mentioned below are located in controllers.cpp, please check the source code for details
### StartScreenController
This controller is the startup interface controller, which renders the entire startup interface in the init method.
In the tick method, the controller checks whether the user clicked the `c` key to decide whether to start the game, and also adds a breathing animation to the prompt character for clicking the c key.
#### External dependencies
- None
#### Custom member variables
The following custom member variables are undefined in the parent class Controller and are added in the derived class.
- bool dir - breathing effect direction flag
- int counter - breathing effect progress counter
#### Custom functions
The following custom functions are not defined in the parent class Controller and are added in the derived class
- None

### TestController
The main interface controller of the game is used to control the logic of the entire game, including player movement, enemy generation, projectile generation, collision detection, out-of-bounds detection, score calculation, health value calculation, and death detection. It also take in `w, s, a, d` key events to move the ally ship accordingly.
#### External dependencies
- text.cpp
   - Text class
- projectile.cpp
   - StupidProjectile class
   -SmartProjectile class
   - ProjectileManager class
-vec.hpp
   - Vec2 class
#### Custom member variables
The following custom member variables are undefined in the parent class Controller and are added in the derived class.
- constant
   - const float MAX_HORIZ_COORD - Maximum absolute horizontal coordinate value
   - const float MAX_VERTI_COORD - maximum absolute value of vertical coordinate
   - const float COLLISION_BOX_RADIUS - Collision box radius
   - const size_t ENERMY_SHIPS_IN_A_GEN - Number of enemies spawned in a single attempt
   - const size_t FRAMES_PER_ALLY_PROJECTILE_GEN - friendly missile launch interval
   - const size_t FRAMES_PER_ENERMY_PROJECTILE_GEN - Enemy missile launch interval
- variables
   - int framesPerShipGeneration - Ship generation interval
   - Shape background - background element object
   - int framesSinceLastShipGeneration - Count of frames since the last ship generation
   - uint64_t frameCount - frame count
   - Vec2 shipVelocity - Friendly ship speed
   - uint32_t score - player score
   - int hp - player health value
   - game::RES::Text scoreText - player score display font object
   - game::RES::Text hpText - player health value display font object
   - game::projectile::ProjectileManager projectileManager - projectile manager
#### Custom functions
The following custom functions are not defined in the parent class Controller and are added in the derived class
-private:
   - void calcVelocity() - Calculate velocity (deprecated)
   - void moveBackground() - Translates the background to the left, causing the spacecraft to move to the right.
   - void generateEnermyShips() - Generate enemy ships and use random numbers to determine whether to generate them.
   - void generateProjectiles() - launches projectiles, all ships launch StupidProjectile, and a random enemy launches SmartProjectile
   - void collisionDetection() - detects collision (ship/ship, projectile/ship), and completes blood volume operation at the same time
   - void hpVerification() - blood volume detection, if the blood volume is too low, the game will end
   - bool outOfBound(game::RES::Shape &shape) - out-of-bounds detection, returns whether an element leaves the visible range of the screen

### EndScreenController
This controller is an exit interface controller. It renders the entire game end interface in the init method. At the same time, it uses the http tool in 3rdparty to initiate an http request to the api server to obtain and render the leaderboard. In the tick method, the controller checks whether the user clicked the `q` key to decide whether to exit, and also adds a breathing animation to the prompt character for clicking the q key.
#### External dependencies
- text.cpp
   - Text class
- 3rdparty/request.cpp
   - HTTP::get function
   - HTTP::post function
#### Custom member variables
The following custom member variables are undefined in the parent class Controller and are added in the derived class.
- uint32_t score - score for display, default is 0
- std::string userName - the name of the current player, default is ""
- Json::Value leaderboard - leaderboard data, empty by default
- bool dir - breathing effect direction flag
- int counter - breathing effect progress counter
#### Custom function
The following custom functions are not defined in the parent class Controller and are added in the derived class
- public:
   - void setScore(uint32_t score) - Called in main.cpp to set the user's score for this game. It should be called before start()
   - void setUserName(std::string userName) - called in main.cpp to set the user name, should be called before start()
   - void submitScore() - Called in main.cpp, submit the current user score to the api server and join the ranking list
-private:
   - void renderLeaderboard() - Read the leaderboard information in the member variable Json::Value leaderboard and render it on the screen using the Text class