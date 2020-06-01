#include "CommonUtils.h"

namespace VSUtils {

std::string GetBasePath(const std::string& fileFullPath)
{
    if (fileFullPath.find_last_of("/\\") != std::string::npos)
        return fileFullPath.substr(0, fileFullPath.find_last_of("/\\"));
    return "";
}

std::string ReplaceSlashes(const std::string& str)
{
    std::string result = str;
    std::size_t found = result.find_first_of("\\");
    while (found != std::string::npos)
    {
        result[found] = '/';
        found = result.find_first_of("\\", found + 1);
    }

    return result;
}
}
