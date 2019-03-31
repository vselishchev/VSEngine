#ifndef _VSENGINE_UTILS_COMMONUTILS_H_
#define _VSENGINE_UTILS_COMMONUTILS_H_

#include <string>
#include <sstream>

namespace VSUtils
{
	template<typename Out>
	void Split(std::string const& str, char delim, Out result)
	{
		std::stringstream ss(str);
		std::string item;
    while (std::getline(ss, item, delim))
    {
      *(result++) = item;
    }
  }

  std::string ReplaceSlashes(const std::string &str);
}

#endif //_VSENGINE_UTILS_COMMONUTILS_H_
