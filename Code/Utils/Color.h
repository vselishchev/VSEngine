#pragma once

namespace VSUtils {
struct Color
{
    inline Color() : red(0), green(0), blue(0), alpha(0) {}
    inline Color(unsigned char r, unsigned char g,
                 unsigned char b, unsigned char a) :
        red(r), green(g), blue(b), alpha(a)
    {}
    inline Color(unsigned char r,
                 unsigned char g,
                 unsigned char b) :
        red(r), green(g), blue(b), alpha(255)
    {}

    inline bool operator==(const Color& rhs) const
    {
        return red == rhs.red &&
            green == rhs.green &&
            blue == rhs.blue &&
            alpha == rhs.alpha;
    }

    Color operator*(float value) const;
    Color& operator*=(float value);

    Color operator+(const Color& c);

    inline bool operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    union
    {
        unsigned int color;
        struct
        {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
            unsigned char alpha;
        };
    };
};

static const Color defaultColor = Color();
}
