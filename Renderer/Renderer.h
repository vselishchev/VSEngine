#include <string>

namespace VSEngine
{
class Renderer
{
public:
    Renderer() = delete;
public:
    struct ApplicationInfo
    {
        std::string title;
        unsigned int windowHeight;
        unsigned int windowWidth;
        unsigned int majorVersion;
        unsigned int minorVersion;
    };
};
}
