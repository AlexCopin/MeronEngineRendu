#pragma once
#include "A4Engine/Export.hpp"
#include "A4Engine/Vector2.hpp"

//Vector3 contains only float values
class Vector3
{
public:
	Vector3() = default; // valeur non-initialisée
	Vector3(float x, float y, float z) :
		x(x), y(y), z(z) {}
	Vector3(Vector2<float> vec, float z) :
		x(vec.x), y(vec.y), z(z) {}

	Vector3 operator+(const Vector3& vec) const;
	Vector3 operator-(const Vector3& vec) const;
	Vector3 operator*(const Vector3& vec) const;
	Vector3 operator*(float value) const;
	Vector3 operator/(const Vector3& vec) const;
	Vector3 operator/(float value) const;

	Vector3& operator+=(const Vector3& vec);
	Vector3& operator-=(const Vector3& vec);
	Vector3& operator*=(const Vector3& vec);
	Vector3& operator*=(float value);
	Vector3& operator/=(const Vector3& vec);
	Vector3& operator/=(float value);

	float x, y, z;
private:
};
