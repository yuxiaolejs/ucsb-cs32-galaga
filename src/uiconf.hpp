#ifndef UICONF_HPP
#define UICONF_HPP
#include <string>

namespace game::UI
{
    struct UIConf
    {
        UIConf()
        {
            windowTitle = "Untitled";
            windowWidth = 800;
            windowHeight = 600;
            windowPosX = 100;
            windowPosY = 100;
            canvasSize = 10;
        };
        std::string windowTitle;
        int windowWidth;
        int windowHeight;
        int windowPosX;
        int windowPosY;

        float canvasSize;

        bool enableAlpha = true;
    };
}
#endif