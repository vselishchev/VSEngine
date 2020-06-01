#pragma once

#include <string>
#include <sstream>

namespace VSUtils {

std::string GetBasePath(const std::string& fileFullPath);

template<typename Out>
void Split(const std::string& str, char delim, Out result)
{
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::string ReplaceSlashes(const std::string& str);
}