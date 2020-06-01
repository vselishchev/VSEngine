#include "Color.h"

namespace VSUtils {
Color Color::operator*(float value) const
{
    return Color((unsigned char)(red * value),
                 (unsigned char)(green * value),
                 (unsigned char)(blue * value),
                 (unsigned char)(alpha * value));
}

Color& Color::operator*=(float value)
{
    red *= (unsigned char)value;
    green *= (unsigned char)value;
    blue *= (unsigned char)value;
    alpha *= (unsigned char)value;

    return *this;
}

Color Color::operator+(const Color& c)
{
    return Color(c.red + red, c.green + green, c.blue + blue, c.alpha + alpha);
}
}
