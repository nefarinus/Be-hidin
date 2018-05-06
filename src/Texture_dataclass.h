#ifndef TEXTURE_DATACLASS_H
#define TEXTURE_DATACLASS_H

#include <cstring>
#include <map>

class Texture_dataclass {
   public:
    static std::map<int, std::string> paths;

    static char* return_path(int id) {
        std::string s    = paths.find(id)->second;
        char*       path = new char[s.length() + 1];
        std::strcpy(path, s.c_str());
        return path;
    }
};

std::map<int, std::string> Texture_dataclass::paths = {
    {3, "assets/wall.png"},
    {1, "assets/platform.png"},
    {2, "assets/sky.png"}};

#endif