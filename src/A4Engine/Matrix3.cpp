#include "A4Engine/Matrix3.h"
#include "A4Engine/Vector2.hpp"
#include <math.h>


Matrix3::Matrix3(const float arr[3][3])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix[i][j] = arr[i][j];
}

Matrix3::Matrix3(const Vector2f& pos, float rotation, const Vector2f& sca)
{
	Matrix3 m = TRS(pos, rotation, sca);
	*this = std::move(m);
}

Matrix3 Matrix3::Multiply(Matrix3 m, float f)
{
	float newArray[3][3];
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			newArray[i][j] = m.matrix[i][j];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			newArray[i][j] *= f;

	return Matrix3(newArray);
}

Matrix3 Matrix3::Multiply(float f)
{
	return Multiply(*this, f);
}

std::array<std::array<float, 2>, 2> Matrix3::SubMatrix(Matrix3 m, int row, int column)
{
	return m.SubMatrix(row, column);
}

std::array<std::array<float, 2>, 2> Matrix3::SubMatrix(int row, int column)
{
	std::array<std::array<float, 2>, 2> newMatrix;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 3; i++)
	{
		if (i == row)
			continue;
			for (int j = 0; j < 3; j++)
			{
				if (j == column)
					continue;
				newMatrix[x][y] = matrix[i][j];
				y++;
			}

			x++;
			y = 0;
	}
	return newMatrix;
}


float Matrix3::Determinant(Matrix3& m)
{
	float det = 0;
	/*int factor = 1;

	float t1 = m.matrix[0][0] * m.matrix[1][1];
	float t2 = m.matrix[0][0] * m.matrix[2][1];
	float t3 = m.matrix[1][0] * m.matrix[0][1];
	float t4 = m.matrix[2][0] * m.matrix[0][1];
	float t5 = m.matrix[1][0] * m.matrix[0][2];
	float t6 = m.matrix[2][0] * m.matrix[0][2];
	det = (t1 * m.matrix[2][2] - t2 * m.matrix[1][2] - t3 * m.matrix[2][2] + t4 * m.matrix[1][2] + t5 * m.matrix[2][1] - t6 * m.matrix[1][1]);*/

	det = 0;
	int power = 0;
	for (int i = 0; i < 3; i++)
	{
		det += (float)pow(-1, power) * (m.matrix[i][0] * m.Determinant(m.SubMatrix(i, 0)));
		power++;
	}

	return det;
}

float Matrix3::Determinant(const std::array<std::array<float, 2>, 2>& mat) const
{
	float result = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	return result;
}

Matrix3 Matrix3::Transpose(Matrix3 m)
{
	float array[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			array[i][j] = m.matrix[j][i];

	return Matrix3(array);
}
Matrix3 Matrix3::Transpose()
{
	return Transpose(*this);
}

//Static
Matrix3 Matrix3::Adjugate(Matrix3 m)
{
	//Separated in functions (seen with Dougidoug)
	/*Matrix3 newMatrix;
	int power = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newMatrix.matrix[i][j] = pow(-1, power) * m.Determinant(SubMatrix(m, i, j));
			power++;
		}

		power = (i % 2 != 0) ? 0 : 1;
	}
	newMatrix = Transpose(m);*/
	return CofactorMatrix(m).Transpose();
}

Matrix3 Matrix3::Adjugate()
{
	return Adjugate(*this);
}

Matrix3 Matrix3::Invert(Matrix3 m)
{
	Matrix3 newMatrix = Matrix3();
	float det = Determinant(m);
	if (det == 0) return newMatrix;
	newMatrix = m.Adjugate();
	return  Multiply(newMatrix, 1 / det);
}
Matrix3 Matrix3::Invert()
{
	return Invert(*this);
}

Matrix3 Matrix3::CofactorMatrix(Matrix3 m)
{
	return m.CofactorMatrix();
}

Matrix3 Matrix3::CofactorMatrix() const
{
	Matrix3 result(matrix);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			result.matrix[i][j] = Cofactor(i, j);

	return result;
}

float Matrix3::Cofactor(int row, int col) const
{
	auto minor = 0.f;
	minor = GetMinor(row, col);
	return (row + col) % 2 == 0 ? minor : -minor;
}

float Matrix3::GetMinor(int row, int col) const
{
	return Determinant(SubMatrix(matrix, row, col));
}


Matrix3 Matrix3::MatrixTranslation(const Vector2f& translation)
{
	float arr[3][3]{
	  { 1, 0, translation.x},
	  { 0, 1, translation.y },
	  { 0 , 0, 1 } };
	return Matrix3(arr);
}

void Matrix3::Translate(const Vector2f& translation)
{
	*this *= MatrixTranslation(translation);
}

Matrix3 Matrix3::MatrixScale(const Vector2f& scale)
{
	float arr[3][3]{
	  { scale.x, 0, 0 },
	  { 0, scale.y, 0 },
	  { 0, 0, 1 } };
	return Matrix3(arr);
}

void Matrix3::Scale(const Vector2f& scale)
{
	*this *= MatrixScale(scale);
}

Matrix3 Matrix3::MatrixScale(float scale)
{
	float arr[3][3]{
	  { scale, 0, 0 },
	  { 0, scale, 0 },
	  { 0, 0, 1 } };
	return Matrix3(arr);
}

void Matrix3::Scale(float scale)
{
	*this *= MatrixScale(scale);
}

Matrix3 Matrix3::MatrixRotation(float rotation)
{
	auto a = rotation * Deg2Rad;
	auto cos = std::cos(a);
	auto sin = std::sin(a);
	float arr[3][3]{
	  { cos, (-sin), 0 },
	  { sin, cos, 0 },
	  { 0, 0, 1 }};
	return Matrix3(arr);
}

void Matrix3::Rotate(float rotation)
{
	*this *= MatrixRotation(rotation);
}

Matrix3 Matrix3::TRS(const Vector2f& translation, float rotation, Vector2f scale)
{
	return MatrixTranslation(translation) * MatrixRotation(rotation) * MatrixScale(scale);
}

Matrix3 Matrix3::SRT(const Vector2f& translation, float rotation, Vector2f scale)
{
	return  MatrixScale(scale) * MatrixRotation(rotation) * MatrixTranslation(translation);
}

Matrix3 Matrix3::Identity()
{
	Matrix3 matrix;
	for (int i = 0; i < 3; ++i)
		matrix.matrix[i][i] = 1;

	return matrix;
}
Matrix3 Matrix3::operator*=(const Matrix3& m2)
{
	Matrix3 returnMatrix = Matrix3();
	for (int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
		{
			float somme = 0;
			for (int k = 0; k < 3; k++)
			{
				somme += matrix[i][k] * m2.matrix[k][j];
			}
			returnMatrix.matrix[i][j] = somme;
		}

	return returnMatrix;
}
Matrix3 Matrix3::operator*(const Matrix3& m2)
{
	return *this *= m2;
}

Vector2f Matrix3::operator*=(const Vector2f& vec) const
{
	float arr[3]{};
	float tempArr[3]{ vec.x, vec.y, 1};


	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			arr[i] += matrix[i][j] * tempArr[j];

	return Vector2f(arr[0], arr[1]);
}
Vector2f Matrix3::operator*(const Vector2f& vec) const
{
	return *this *= vec;
}

void Matrix3::Print()
{
	std::cout << "Matrix3(" << std::endl
		<< "{" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << "}" << std::endl
		<< "{" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << "}" << std::endl
		<< "{" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << "}" << ")" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Matrix3& mat)
{
	os << "Matrix3(" << std::endl
		<< "{" << mat.matrix[0][0] << ", " << mat.matrix[0][1] << ", " << mat.matrix[0][2] << "}" << std::endl
		<< "{" << mat.matrix[1][0] << ", " << mat.matrix[1][1] << ", " << mat.matrix[1][2] << "}" << std::endl
		<< "{" << mat.matrix[2][0] << ", " << mat.matrix[2][1] << ", " << mat.matrix[2][2] << "}" << std::endl
		<< ")";
	return os;
}
