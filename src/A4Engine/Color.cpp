#include <A4Engine/Color.hpp>

Color::Color() :
Color(1.f, 1.f, 1.f) //< couleur par défaut : du blanc
{
}

Color::Color(float red, float green, float blue, float alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{
}

void Color::ToRGBA8(std::uint8_t& red, std::uint8_t& green, std::uint8_t& blue, std::uint8_t& alpha) const
{
	red   = static_cast<std::uint8_t>(r * 255.f);
	green = static_cast<std::uint8_t>(g * 255.f);
	blue  = static_cast<std::uint8_t>(b * 255.f);
	alpha = static_cast<std::uint8_t>(a * 255.f);
}

Color Color::FromRGBA8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
	float invU8 = 1.f / 255.f;
	return { r * invU8, g * invU8, b * invU8, a * invU8 };
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
	return os << "Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
}
