#include "A4Engine/Matrix3.h"
#include <math.h>
#include <array>


Matrix3::Matrix3(const float arr[3][3])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix[i][j] = arr[i][j];
}

Matrix3::Matrix3(Vector2f pos, Vector2f rot, Vector2f sca)
{
	matrix[0][0] = pos.x;
	matrix[1][0] = pos.y;
	matrix[2][0] = 0;
	matrix[0][1] = rot.x;
	matrix[1][1] = rot.y;
	matrix[2][1] = 0;
	matrix[0][2] = sca.x;
	matrix[1][2] = sca.y;
	matrix[2][2] = 1;
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

const std::array<std::array<float, 2>, 2>& Matrix3::SubMatrix(Matrix3 m, int row, int column)
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

float Matrix3::Determinant(const std::array<std::array<float, 2>, 2>& mat)
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
	Matrix3 newMatrix;
	Matrix3 matrixTransposed = m;
	int power = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newMatrix.matrix[i][j] = pow(-1, power) * m.Determinant(SubMatrix(matrixTransposed, i, j));
			power++;
		}

		power = (i % 2 != 0) ? 0 : 1;
	}
	matrixTransposed = Transpose(matrixTransposed);
	return newMatrix;
}

Matrix3 Matrix3::Adjugate()
{
	return Adjugate(*this);
}

Matrix3 Matrix3::InvertByDeterminant(Matrix3 m)
{
	Matrix3 newMatrix = Matrix3();
	float det = Determinant(m);
	if (det == 0) return newMatrix;
	newMatrix = m.Adjugate();
	newMatrix = Multiply(newMatrix, 1 / det);
	return newMatrix;
}
Matrix3 Matrix3::InvertByDeterminant()
{
	return InvertByDeterminant(*this);
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
			returnMatrix.matrix[i][j] = matrix[i][j] * m2.matrix[i][j];

	return returnMatrix;
}
Matrix3 Matrix3::operator*(const Matrix3& m2)
{
	return *this *= m2;
}

Vector3 Matrix3::operator*=(const Vector2f& vec)
{
	float arr[3];
	float tempArr[3]{ vec.x, vec.y, 1};

	for(int i = 0; i < 3; ++i)
	{
		float sum = 0;
		for(int j = 0; j < 3; ++j)
		{
			sum += matrix[i][j] * tempArr[j];
		}
		arr[i] = sum;
	}
	Vector3 returnVec(arr[0], arr[1], arr[2]);
	return returnVec;
}
void Matrix3::Print()
{
	std::cout << "Matrix3(" << std::endl
		<< "{" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << "}" << std::endl
		<< "{" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << "}" << std::endl
		<< "{" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << "}" << ")" << std::endl;
}
Vector3 Matrix3::operator*(const Vector2f& vec)
{
	return *this *= vec;
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
