#pragma once

#include "A4Engine/Export.hpp"
#include "A4Engine/Vector3.h"
#include "A4Engine/Vector2.hpp"

class A4ENGINE_API Matrix3
{
public:
	//Create an identity matrix
	Matrix3() = default;
	Matrix3(const float arr[3][3]);
	Matrix3(Vector2f pos, Vector2f rot, Vector2f sca);

	static Matrix3 Multiply(Matrix3 m, float f);
	Matrix3 Multiply(float f);

	static float Determinant(Matrix3& m);
	float Determinant(const std::array<std::array<float, 2> , 2>& mat);

	static Matrix3 Transpose(Matrix3 m);
	Matrix3 Transpose();

	static Matrix3 Adjugate(Matrix3 m);
	Matrix3 Adjugate();

	static Matrix3 InvertByDeterminant(Matrix3 m);
	Matrix3 InvertByDeterminant();

	static const std::array<std::array<float, 2>, 2>& SubMatrix(Matrix3 m, int row, int column);
	std::array<std::array<float, 2>, 2> SubMatrix(int row, int column);

	static Matrix3 Identity();

	Matrix3 operator*=(const Matrix3& m2);
	Matrix3 operator*(const Matrix3& m2);
	Vector3 operator*(const Vector2f& vec);
	Vector3 operator*=(const Vector2f& vec);


	void Print();

	float matrix[3][3];

private:
};

std::ostream& operator<<(std::ostream& os, const Matrix3& vec);