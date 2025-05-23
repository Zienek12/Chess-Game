#pragma once
enum class Color {
	White,
	Black,
	None
};
#include <ostream>

inline std::ostream& operator<<(std::ostream& os, const Color& color)
{
    switch (color)
    {
    case Color::White:
        os << "White";
        break;
    case Color::Black:
        os << "Black";
        break;
    default:
        os << "Unknown";
        break;
    }
    return os;
}
