#pragma once

#include "A4Engine/Export.hpp"
#include <array>
#include <A4Engine/Vector2.hpp>

class A4ENGINE_API Matrix3
{
public:
	//Create an identity matrix
	Matrix3() = default;
	Matrix3(const float arr[3][3]);
	Matrix3(const Vector2f& pos, float rot, const Vector2f& sca);
	~Matrix3() = default;

	static Matrix3 Multiply(Matrix3 m, float f);
	Matrix3 Multiply(float f);

	static float Determinant(Matrix3& m);
	float Determinant(const std::array<std::array<float, 2>, 2>& mat) const;

	static Matrix3 Transpose(Matrix3 m);
	Matrix3 Transpose();

	static Matrix3 Adjugate(Matrix3 m);
	Matrix3 Adjugate();

	static Matrix3 Invert(Matrix3 m);
	Matrix3 Invert();

	Matrix3 CofactorMatrix() const;
	float Cofactor(int row, int col) const;
	float GetMinor(int row, int col) const;

	static std::array<std::array<float, 2>, 2> SubMatrix(Matrix3 m, int row, int column);
	std::array<std::array<float, 2>, 2> SubMatrix(int row, int column);


	//Translate by a vector
	static Matrix3 MatrixTranslation(const Vector2f& translation);
	void Translate(const Vector2f& translation);
	//Scale by x & y
	static Matrix3 MatrixScale(const Vector2f& scale);
	void Scale(const Vector2f& scale);
	//Multiply scale by a float
	static Matrix3 MatrixScale(float scale);
	void Scale(float scale);
	//Rotate by an angle in degrees
	static Matrix3 MatrixRotation(float rotation);
	void Rotate(float rotation);

	static Matrix3 TRS(const Vector2f& translation, float rotation, Vector2f scale);

	static Matrix3 Identity();

	Matrix3 operator*=(const Matrix3& m2);
	Matrix3 operator*(const Matrix3& m2);
	/*Vector2f operator*(const Vector2f& vec) const;
	Vector2f operator*=(const Vector2f& vec) const;*/


	void Print();

	float matrix[3][3];

private:
};