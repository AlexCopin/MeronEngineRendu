#include "A4Engine/Vector3.h"


Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3{ x + vec.x, y + vec.y , z + vec.z };
}


Vector3 Vector3::operator-(const Vector3& vec) const
{
	return Vector3{ x - vec.x, y - vec.y , z - vec.z };
}


Vector3 Vector3::operator*(const Vector3& vec) const
{
	return Vector3{ x * vec.x, y * vec.y , z * vec.z };
}


Vector3 Vector3::operator*(float value) const
{
	return Vector3{ x * value, y * value , z * value };
}


Vector3 Vector3::operator/(const Vector3& vec) const
{
	return Vector3{ x / vec.x, y / vec.y , z / vec.z };
}


Vector3 Vector3::operator/(float value) const
{
	return Vector3{ x / value, y / value , z / value };
}


Vector3& Vector3::operator+=(const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;

	return *this;
}


Vector3& Vector3::operator-=(const Vector3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;

	return *this;
}


Vector3& Vector3::operator*=(const Vector3& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;

	return *this;
}


Vector3& Vector3::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}


Vector3& Vector3::operator/=(const Vector3& vec)
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.y;

	return *this;
}


Vector3& Vector3::operator/=(float value)
{
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

Vector3& Vector3::operator*=(const Vector2f& vec)
{
	return *this *= Vector3(vec, 1);
}


std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
	return os;
}
