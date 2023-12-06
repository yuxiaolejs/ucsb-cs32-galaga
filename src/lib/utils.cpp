#include "utils.hpp"
#include "game.hpp"
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>

uint64_t game::UTILS::getTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t game::UTILS::highResTimestamp()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

bool game::UTILS::findInEnvp(char **envp, std::string varToFind)
{
    char **ptr = envp;
    bool found = false;
    while (ptr && *ptr)
    {
        char *currStr = *ptr;
        int i = 0;
        found = true;
        while (*currStr && *currStr != '=')
        {
            if (varToFind[i] != *currStr)
            {
                found = false;
                break;
            }
            currStr++;
            i++;
        }
        if (found)
            return true;
        ptr++;
    }
    return false;
}

void game::UTILS::redirectCout(std::string filename)
{
    std::cout << "\033[0;31m";
    std::cout << "\033[0;34m--------------------------------------------" << std::endl
              << "  ________       .__                        " << std::endl
              << " /  _____/_____  |  | _____     _________   " << std::endl
              << "/   \\  ___\\__  \\ |  | \\__  \\   / ___\\__  \\  " << std::endl
              << "\\    \\_\\  \\/ __ \\|  |__/ __ \\_/ /_/  > __ \\_" << std::endl
              << " \\______  (____  /____(____  /\\___  (____  /" << std::endl
              << "        \\/     \\/          \\//_____/     \\/ " << std::endl
              << "\033[0;34m--------------------------------------------" << std::endl
              << "\033[1;32m"
              << " A CS 32 project (F23 w/ Burk K. & Liu J.)  " << std::endl
              << "\033[0;34m--------------------------------------------" << std::endl
              << "\033[0;35m"
              << " Group Members:" << std::endl
              << " - Andy J." << std::endl
              << " - Luis B." << std::endl
              << " - Luxiang Z." << std::endl
              << " - Tianle Y." << std::endl
              << "\033[0;34m--------------------------------------------" << std::endl
              << "\033[1;32m"
              << "   Active API gateway:\033[1;0m" << std::endl
              << "      " << game::API_PREFIX << std::endl
              << "\033[1;32m"
              << "   Redirecting stdio to:\033[1;0m" << std::endl
              << "      " << filename << std::endl
              << "\033[0;34m--------------------------------------------" << std::endl
              << "\033[1;32m"
              << "   " << game::VERSION_NUMBER << std::endl
              << "\033[0;34m--------------------------------------------" << std::endl
              << "\033[0;0m"
              << "https://yuxiaolejs.github.io/ucsb-cs32-galaga" << std::endl;
    // game::logFile.open(filename);
    // std::streambuf *coutbuf = std::cout.rdbuf();
    // std::cout.rdbuf(logFile.rdbuf());

    // Use C magic to redirect stdout and stderr to a file
    freopen(filename.c_str(), "a+", stdout);
    freopen((filename + ".err").c_str(), "a+", stderr);
}