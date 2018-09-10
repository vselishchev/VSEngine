#ifndef _CPUGRAPHICS_UTILS_COMMONUTILS_H_
#define _CPUGRAPHICS_UTILS_COMMONUTILS_H_

#include <string>
#include <sstream>

namespace Utils
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
}

#endif //_CPUGRAPHICS_UTILS_COMMONUTILS_H_
