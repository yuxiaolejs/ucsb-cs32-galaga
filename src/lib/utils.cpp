#include "utils.hpp"
#include <chrono>
#include <string>

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