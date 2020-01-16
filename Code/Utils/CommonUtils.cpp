#include "CommonUtils.h"

namespace VSUtils
{
std::string ReplaceSlashes(const std::string &str)
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
