#pragma once

#if defined(_WIN64) || defined(_WIN32)
	#ifndef _USE_MATH_DEFINES
	// For some reason this works on in Debug build of Visual Studio but not in Release build.
	// The solution people suggested was to define this as a preprocessor macro in the project.
		#define _USE_MATH_DEFINES 
	#endif
#endif

#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "vectors.h"
#include <iostream>

class CMatrix3Df;

/**
 * CMatrix4Df: A 4x4 matrix class.
 *
 * This class represents a 4x4 matrix, commonly used in linear algebra and 3D graphics.
 * It provides various constructors for initialization and supports common matrix operations.
 *
 * Members:
 *   - mat4: A 4x4 array of floating-point values representing the matrix elements.
 */
class CMatrix4Df
{
public:
	float mat4[4][4];

	/**
	 * Default constructor, initializes all elements to zero.
	 */
	CMatrix4Df() = default;

	/**
	 * Constructor that initializes the matrix with specific values.
	 *
	 * @param a00, a01, ..., a33: Individual matrix elements.
	 */
	CMatrix4Df(const float a00, const float a01, const float a02, const float a03,
		const float a10, const float a11, const float a12, const float a13,
		const float a20, const float a21, const float a22, const float a23,
		const float a30, const float a31, const float a32, const float a33)
	{
		mat4[0][0] = a00; mat4[0][1] = a01; mat4[0][2] = a02; mat4[0][3] = a03;
		mat4[1][0] = a10; mat4[1][1] = a11; mat4[1][2] = a12; mat4[1][3] = a13;
		mat4[2][0] = a20; mat4[2][1] = a21; mat4[2][2] = a22; mat4[2][3] = a23;
		mat4[3][0] = a30; mat4[3][1] = a31; mat4[3][2] = a32; mat4[3][3] = a33;
	}
	
	/**
	 * Constructor that initializes the matrix from a 4x4 Assimp matrix.
	 *
	 * @param AssimpMatrix: A 4x4 Assimp matrix to copy from.
	 */
	CMatrix4Df(const aiMatrix4x4& AssimpMatrix)
	{
		mat4[0][0] = AssimpMatrix.a1; mat4[0][1] = AssimpMatrix.a2; mat4[0][2] = AssimpMatrix.a3; mat4[0][3] = AssimpMatrix.a4;
		mat4[1][0] = AssimpMatrix.b1; mat4[1][1] = AssimpMatrix.b2; mat4[1][2] = AssimpMatrix.b3; mat4[1][3] = AssimpMatrix.b4;
		mat4[2][0] = AssimpMatrix.c1; mat4[2][1] = AssimpMatrix.c2; mat4[2][2] = AssimpMatrix.c3; mat4[2][3] = AssimpMatrix.c4;
		mat4[3][0] = AssimpMatrix.d1; mat4[3][1] = AssimpMatrix.d2; mat4[3][2] = AssimpMatrix.d3; mat4[3][3] = AssimpMatrix.d4;
	}

	/**
	 * Constructor that initializes the matrix from a 3x3 Assimp matrix.
	 *
	 * @param AssimpMatrix: A 3x3 Assimp matrix to copy from.
	 */
	CMatrix4Df(const aiMatrix3x3& AssimpMatrix)
	{
		mat4[0][0] = AssimpMatrix.a1; mat4[0][1] = AssimpMatrix.a2; mat4[0][2] = AssimpMatrix.a3; mat4[0][3] = 0.0f;
		mat4[1][0] = AssimpMatrix.b1; mat4[1][1] = AssimpMatrix.b2; mat4[1][2] = AssimpMatrix.b3; mat4[1][3] = 0.0f;
		mat4[2][0] = AssimpMatrix.c1; mat4[2][1] = AssimpMatrix.c2; mat4[2][2] = AssimpMatrix.c3; mat4[2][3] = 0.0f;
		mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
	}

	/**
	 * Constructor that initializes the matrix from a 3x3 CMatrix3Df matrix.
	 *
	 * @param AssimpMatrix: A 3x3 CMatrix3Df matrix to copy from.
	 */
	CMatrix4Df(const CMatrix3Df& AssimpMatrix);

	/**
	 * Constructor that initializes the matrix from a GLM matrix.
	 *
	 * @param glmMat: A GLM matrix to copy from.
	 */
	CMatrix4Df(const glm::mat4& glmMat)
	{
		mat4[0][0] = glmMat[0][0]; mat4[0][1] = glmMat[0][1]; mat4[0][2] = glmMat[0][2]; mat4[0][3] = glmMat[0][3];
		mat4[1][0] = glmMat[1][0]; mat4[1][1] = glmMat[1][1]; mat4[1][2] = glmMat[1][2]; mat4[1][3] = glmMat[1][3];
		mat4[2][0] = glmMat[2][0]; mat4[2][1] = glmMat[2][1]; mat4[2][2] = glmMat[2][2]; mat4[2][3] = glmMat[2][3];
		mat4[3][0] = glmMat[3][0]; mat4[3][1] = glmMat[3][1]; mat4[3][2] = glmMat[3][2]; mat4[3][3] = glmMat[3][3];
	}

	/**
	 * Copy constructor.
	 *
	 * @param cMat: The matrix to copy.
	 */
	CMatrix4Df(const CMatrix4Df& cMat)
	{
		mat4[0][0] = cMat.mat4[0][0]; mat4[0][1] = cMat.mat4[0][1]; mat4[0][2] = cMat.mat4[0][2]; mat4[0][3] = cMat.mat4[0][3];
		mat4[1][0] = cMat.mat4[1][0]; mat4[1][1] = cMat.mat4[1][1]; mat4[1][2] = cMat.mat4[1][2]; mat4[1][3] = cMat.mat4[1][3];
		mat4[2][0] = cMat.mat4[2][0]; mat4[2][1] = cMat.mat4[2][1]; mat4[2][2] = cMat.mat4[2][2]; mat4[2][3] = cMat.mat4[2][3];
		mat4[3][0] = cMat.mat4[3][0]; mat4[3][1] = cMat.mat4[3][1]; mat4[3][2] = cMat.mat4[3][2]; mat4[3][3] = cMat.mat4[3][3];
	}

	/**
	 * Copy constructor.
	 *
	 * @param vec1: A 4D Vector to copy from first row.
	 */
	CMatrix4Df(const SVector4Df& vec1, const SVector4Df& vec2, const SVector4Df& vec3, const SVector4Df& vec4)
	{
		mat4[0][0] = vec1.x; mat4[0][1] = vec1.y; mat4[0][2] = vec1.z; mat4[0][3] = vec1.w;
		mat4[1][0] = vec2.x; mat4[1][1] = vec2.y; mat4[1][2] = vec2.z; mat4[1][3] = vec2.w;
		mat4[2][0] = vec3.x; mat4[2][1] = vec3.y; mat4[2][2] = vec3.z; mat4[2][3] = vec3.w;
		mat4[3][0] = vec4.x; mat4[3][1] = vec4.y; mat4[3][2] = vec4.z; mat4[3][3] = vec4.w;
	}

	/**
	 * Overload the `[]` operator to access rows of the matrix.
	 *
	 * @param row: Row index (0-3).
	 * @return Reference to the array representing the row.
	 */
	float* operator[](size_t row)
	{
		if (row >= 4)
			throw std::out_of_range("Row index out of bounds");
		return mat4[row];
	}

	/**
	 * Multiplies two CMatrix4Df matrices.
	 *
	 * This operator overloads the multiplication operator for CMatrix4Df objects,
	 * performing matrix multiplication.
	 * 
	 * -Important: the correct order of multiplication is Translation * (Rotation * (Scale * Position)) by order to get the final transform
	 *
	 * @param rightMat: The right-hand side matrix.
	 *
	 * @return The product of the two matrices.
	 */
	CMatrix4Df operator*(const CMatrix4Df& rightMat)
	{
		CMatrix4Df newMat{};
		for (int8_t i = 0; i < 4; i++)
		{
			for (int8_t j = 0; j < 4; j++)
			{
				newMat.mat4[i][j] = mat4[i][0] * rightMat.mat4[0][j] + mat4[i][1] * rightMat.mat4[1][j] + mat4[i][2] * rightMat.mat4[2][j] + mat4[i][3] * rightMat.mat4[3][j];
			}
		}
		return (newMat);

	}

	CMatrix4Df operator=(const glm::mat4& glmMat)
	{
		mat4[0][0] = glmMat[0][0]; mat4[0][1] = glmMat[0][1]; mat4[0][2] = glmMat[0][2]; mat4[0][3] = glmMat[0][3];
		mat4[1][0] = glmMat[1][0]; mat4[1][1] = glmMat[1][1]; mat4[1][2] = glmMat[1][2]; mat4[1][3] = glmMat[1][3];
		mat4[2][0] = glmMat[2][0]; mat4[2][1] = glmMat[2][1]; mat4[2][2] = glmMat[2][2]; mat4[2][3] = glmMat[2][3];
		mat4[3][0] = glmMat[3][0]; mat4[3][1] = glmMat[3][1]; mat4[3][2] = glmMat[3][2]; mat4[3][3] = glmMat[3][3];
	}

	/**
	 * Multiplies a CMatrix4Df and an SVector4Df.
	 *
	 * This operator overloads the multiplication operator for CMatrix4Df and SVector4Df objects,
	 * performing matrix-vector multiplication.
	 *
	 * @param vec: The vector to multiply.
	 *
	 * @return The product of the matrix and the vector.
	 */
	SVector4Df operator*(const SVector4Df& vec)
	{
		SVector4Df newVec{};

		newVec.x = mat4[0][0] * vec.x + mat4[0][1] * vec.y + mat4[0][2] * vec.z + mat4[0][3] * vec.w;
		newVec.y = mat4[1][0] * vec.x + mat4[1][1] * vec.y + mat4[1][2] * vec.z + mat4[1][3] * vec.w;
		newVec.z = mat4[2][0] * vec.x + mat4[2][1] * vec.y + mat4[2][2] * vec.z + mat4[2][3] * vec.w;
		newVec.w = mat4[3][0] * vec.x + mat4[3][1] * vec.y + mat4[3][2] * vec.z + mat4[3][3] * vec.w;
		return (newVec);
	}

	/**
	 * Multiplies a CMatrix4Df and an SVector4Df const.
	 *
	 * This operator overloads the multiplication operator for CMatrix4Df and SVector4Df objects,
	 * performing matrix-vector multiplication.
	 *
	 * @param vec: The vector to multiply.
	 *
	 * @return The product of the matrix and the vector.
	 */
	SVector4Df operator*(const SVector4Df& vec) const
	{
		SVector4Df newVec{};

		newVec.x = mat4[0][0] * vec.x + mat4[0][1] * vec.y + mat4[0][2] * vec.z + mat4[0][3] * vec.w;
		newVec.y = mat4[1][0] * vec.x + mat4[1][1] * vec.y + mat4[1][2] * vec.z + mat4[1][3] * vec.w;
		newVec.z = mat4[2][0] * vec.x + mat4[2][1] * vec.y + mat4[2][2] * vec.z + mat4[2][3] * vec.w;
		newVec.w = mat4[3][0] * vec.x + mat4[3][1] * vec.y + mat4[3][2] * vec.z + mat4[3][3] * vec.w;
		return (newVec);
	}

	CMatrix4Df operator*(float scalar)
	{
		CMatrix4Df result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.mat4[i][j] = mat4[i][j] * scalar;
			}
		}
		return result;
	}

	/**
	 * Provides a pointer to the underlying matrix data.
	 *
	 * This operator overload allows direct access to the matrix elements as a float array.
	 *
	 * @return A pointer to the first element of the matrix.
	 */
	operator const float* () const
	{
		return (&(mat4[0][0]));
	}

	/**
	 * Prints the matrix elements to the console.
	 *
	 * This function prints the matrix elements in a readable format.
	 */
	void print() const
	{
		for (int8_t i = 0; i < 4; i++)
		{
			printf("%f - %f - %f - %f", mat4[i][0], mat4[i][1], mat4[i][2], mat4[i][3]);
		}
	}

	/**
	 * Accesses the underlying data of the matrix.
	 *
	 * - This function provides a pointer to the first element of the
	 * private `mat4` 4x4 matrix, allowing direct manipulation of the matrix data.
	 *
	 * @return A pointer to the first element of the `mat4` matrix.
	 */
	float* data()
	{
		return (&(mat4[0][0]));
	}

	/**
	 * returns a reference to the private `mat4` matrix, It allows read-only access to the 4x4 matrix stored in the class
	 *
	 * @return A constant reference to the `mat4` matrix.
	 */
	const float(&GetMatrix() const)[4][4]
	{
		return mat4;
	}

	/**
	 * Takes a 4x4 array of floats and updates the private `mat4` matrix with the provided values.
	 *
	 * @param values: A 4x4 array containing the new values for the matrix.
	 */
	void SetMatrix(const float values[4][4])
	{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				mat4[i][j] = values[i][j];
			}
		}
	}

	/**
	 * Transposes the matrix.
	 *
	 * This function returns a new matrix that is the transpose of the current matrix.
	 *
	 * @return The transposed matrix.
	 */
	CMatrix4Df Transpose() const
	{
		CMatrix4Df newMat{};

		for (int8_t i = 0; i < 4; i++)
		{
			for (int8_t j = 0; j < 4; j++)
			{
				newMat.mat4[i][j] = mat4[j][i];
			}
		}

		return (newMat);
	}

	/**
	 * Calculates the determinant of the matrix.
	 *
	 * This function calculates the determinant of the 4x4 matrix using a direct formula.
	 *
	 * @return The determinant of the matrix.
	 */
	float Determinant() const
	{
		float fDet = 
			  mat4[0][0] * mat4[1][1] * mat4[2][2] * mat4[3][3] - mat4[0][0] * mat4[1][1] * mat4[2][3] * mat4[3][2] + mat4[0][0] * mat4[1][2] * mat4[2][3] * mat4[3][1] - mat4[0][0] * mat4[1][2] * mat4[2][1] * mat4[3][3]
			+ mat4[0][0] * mat4[1][3] * mat4[2][1] * mat4[3][2] - mat4[0][0] * mat4[1][3] * mat4[2][2] * mat4[3][1] - mat4[0][1] * mat4[1][2] * mat4[2][3] * mat4[3][0] + mat4[0][1] * mat4[1][2] * mat4[2][0] * mat4[3][3]
			- mat4[0][1] * mat4[1][3] * mat4[2][0] * mat4[3][2] + mat4[0][1] * mat4[1][3] * mat4[2][2] * mat4[3][0] - mat4[0][1] * mat4[1][0] * mat4[2][2] * mat4[3][3] + mat4[0][1] * mat4[1][0] * mat4[2][3] * mat4[3][2]
			+ mat4[0][2] * mat4[1][3] * mat4[2][0] * mat4[3][1] - mat4[0][2] * mat4[1][3] * mat4[2][1] * mat4[3][0] + mat4[0][2] * mat4[1][0] * mat4[2][1] * mat4[3][3] - mat4[0][2] * mat4[1][0] * mat4[2][3] * mat4[3][1]
			+ mat4[0][2] * mat4[1][1] * mat4[2][3] * mat4[3][0] - mat4[0][2] * mat4[1][1] * mat4[2][0] * mat4[3][3] - mat4[0][3] * mat4[1][0] * mat4[2][1] * mat4[3][2] + mat4[0][3] * mat4[1][0] * mat4[2][2] * mat4[3][1]
			- mat4[0][3] * mat4[1][1] * mat4[2][2] * mat4[3][0] + mat4[0][3] * mat4[1][1] * mat4[2][0] * mat4[3][2] - mat4[0][3] * mat4[1][2] * mat4[2][0] * mat4[3][2] + mat4[0][3] * mat4[1][2] * mat4[2][1] * mat4[3][0];

		return (fDet);
	}

	/**
	 * Calculates the determinant of a 3x3 submatrix, Calcualted as Same as GLM Function.
	 *
	 * This function calculates the determinant of a 3x3 submatrix, which is a crucial
	 * step in the cofactor expansion method for computing the determinant of a 4x4 matrix.
	 *
	 * @return The determinant of the 3x3 submatrix.
	 */
	float DeterminantSub() const
	{
		float SubFactor00 = mat4[2][2] * mat4[3][3] - mat4[3][2] * mat4[2][3];
		float SubFactor01 = mat4[2][1] * mat4[3][3] - mat4[3][1] * mat4[2][3];
		float SubFactor02 = mat4[2][1] * mat4[3][2] - mat4[3][1] * mat4[2][2];
		float SubFactor03 = mat4[2][0] * mat4[3][3] - mat4[3][0] * mat4[2][3];
		float SubFactor04 = mat4[2][0] * mat4[3][2] - mat4[3][0] * mat4[2][2];
		float SubFactor05 = mat4[2][0] * mat4[3][1] - mat4[3][0] * mat4[2][1];

		SVector4Df DetCof(
			+ (mat4[1][1] * SubFactor00 - mat4[1][2] * SubFactor01 + mat4[1][3] * SubFactor02),
			- (mat4[1][0] * SubFactor00 - mat4[1][2] * SubFactor03 + mat4[1][3] * SubFactor04),
			+ (mat4[1][0] * SubFactor01 - mat4[1][1] * SubFactor03 + mat4[1][3] * SubFactor05),
			- (mat4[1][0] * SubFactor02 - mat4[1][1] * SubFactor04 + mat4[1][2] * SubFactor05)
		);

		return mat4[0][0] * DetCof[0] + mat4[0][1] * DetCof[1] + mat4[0][2] * DetCof[2] + mat4[0][3] * DetCof[3];
	}

	/**
	 * Calculates the inverse of the matrix.
	 *
	 * This function calculates the inverse of the 4x4 matrix using the adjugate matrix and the determinant.
	 *
	 * @return The inverse of the matrix.
	 */
	CMatrix4Df Inverse() const
	{
		// Compute the reciprocal determinant first

		float det = Determinant();

		if (det == 0.0f)
		{
			ASSERT(det == 0.0f, "Matrix Determinant Is 0");
			return (*this);
		}

		// Calculate the inverse determinant
		float fInvDet = 1.0f / det;

		// Calculate the adjugate matrix (transpose of the cofactor matrix) (https://byjus.com/maths/inverse-matrix/)
		CMatrix4Df res{};

		// Calcualte First Element --- Inverse Is Positive
		res.mat4[0][0] = fInvDet * (mat4[1][1] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[1][2] * 
			(mat4[2][3] * mat4[3][1] - mat4[2][1] * mat4[3][3]) + mat4[1][3] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Negative
		res.mat4[0][1] = -fInvDet * (mat4[0][1] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[2][3] * mat4[3][1] - mat4[2][1] * mat4[3][3]) + mat4[0][3] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Positive
		res.mat4[0][2] = fInvDet * (mat4[0][1] * (mat4[1][2] * mat4[3][3] - mat4[1][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[3][1] - mat4[1][1] * mat4[3][3]) + mat4[0][3] * (mat4[1][1] * mat4[3][2] - mat4[1][2] * mat4[3][1]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Negative
		res.mat4[0][3] = -fInvDet * (mat4[0][1] * (mat4[1][2] * mat4[2][3] - mat4[1][3] * mat4[2][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[2][1] - mat4[1][1] * mat4[2][3]) + mat4[0][3] * (mat4[1][1] * mat4[2][2] - mat4[1][2] * mat4[2][1]));

		// Same as the FIRST just switch the [x][1] to [x][0] --- Inverse Is Negative
		res.mat4[1][0] = -fInvDet * (mat4[1][0] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[1][2] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[1][3] * (mat4[2][0] * mat4[3][2] - mat4[2][2] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Positive
		res.mat4[1][1] = fInvDet * (mat4[0][0] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[0][3] * (mat4[2][0] * mat4[3][2] - mat4[2][2] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Negative
		res.mat4[1][2] = -fInvDet * (mat4[0][0] * (mat4[1][2] * mat4[3][3] - mat4[1][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[3][0] - mat4[1][0] * mat4[3][3]) + mat4[0][3] * (mat4[1][0] * mat4[3][2] - mat4[1][2] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Positive
		res.mat4[1][3] = fInvDet * (mat4[0][0] * (mat4[1][2] * mat4[2][3] - mat4[1][3] * mat4[2][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[2][0] - mat4[1][0] * mat4[2][3]) + mat4[0][3] * (mat4[1][0] * mat4[2][2] - mat4[1][2] * mat4[2][0]));

		// Same as [1][0] ELEMENT calculation, just switch the [x][2] to [x][1] --- Inverse Is Positive
		res.mat4[2][0] = fInvDet * (mat4[1][0] * (mat4[2][1] * mat4[3][3] - mat4[2][3] * mat4[3][1]) + mat4[1][1] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[1][3] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Negative
		res.mat4[2][1] = -fInvDet * (mat4[0][0] * (mat4[2][1] * mat4[3][3] - mat4[2][3] * mat4[3][1]) + mat4[0][1] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[0][3] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Positive
		res.mat4[2][2] = fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[3][3] - mat4[1][3] * mat4[3][1]) + mat4[0][1] *
			(mat4[1][3] * mat4[3][0] - mat4[1][0] * mat4[3][3]) + mat4[0][3] * (mat4[1][0] * mat4[3][1] - mat4[1][1] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Negative
		res.mat4[2][3] = -fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[2][3] - mat4[1][3] * mat4[2][1]) + mat4[0][1] *
			(mat4[1][3] * mat4[2][0] - mat4[1][0] * mat4[2][3]) + mat4[0][3] * (mat4[1][0] * mat4[2][1] - mat4[1][1] * mat4[2][0]));

		// Same as [2][0] ELEMENT calculation, just switch the [x][3] to [x][2] --- Inverse Is Negative
		res.mat4[3][0] = -fInvDet * (mat4[1][0] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]) + mat4[1][1] *
			(mat4[2][2] * mat4[3][0] - mat4[2][0] * mat4[3][2]) + mat4[1][2] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Positive
		res.mat4[3][1] = fInvDet * (mat4[0][0] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]) + mat4[0][1] *
			(mat4[2][2] * mat4[3][0] - mat4[2][0] * mat4[3][2]) + mat4[0][2] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Negative
		res.mat4[3][2] = -fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[3][2] - mat4[1][2] * mat4[3][1]) + mat4[0][1] *
			(mat4[1][2] * mat4[3][0] - mat4[1][0] * mat4[3][2]) + mat4[0][2] * (mat4[2][0] * mat4[3][1] - mat4[1][1] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Positive
		res.mat4[3][3] = fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[2][2] - mat4[1][2] * mat4[2][1]) + mat4[0][1] *
			(mat4[1][2] * mat4[3][0] - mat4[1][0] * mat4[2][2]) + mat4[0][2] * (mat4[2][0] * mat4[2][1] - mat4[1][1] * mat4[2][0]));

		// The Pattern -> Previous Calculation (make sure of fInvDet sign) and each time make [x][n] = [x-1][n] , ex: [3][2] -> [2][2], [2][2] -> [1][2], [1][2] -> [0][2]
		return (res);
	}

	/**
	 * Calculates the inverse of the matrix, Calcualted as Same as GLM Function.
	 *
	 * This function calculates the inverse of the 4x4 matrix using the adjugate matrix and the determinant.
	 *
	 * @return The inverse of the matrix.
	 */
	CMatrix4Df InverseSub() const
	{
		float Coef00 = mat4[2][2] * mat4[3][3] - mat4[3][2] * mat4[2][3];
		float Coef02 = mat4[1][2] * mat4[3][3] - mat4[3][2] * mat4[1][3];
		float Coef03 = mat4[1][2] * mat4[2][3] - mat4[2][2] * mat4[1][3];

		float Coef04 = mat4[2][1] * mat4[3][3] - mat4[3][1] * mat4[2][3];
		float Coef06 = mat4[1][1] * mat4[3][3] - mat4[3][1] * mat4[1][3];
		float Coef07 = mat4[1][1] * mat4[2][3] - mat4[2][1] * mat4[1][3];

		float Coef08 = mat4[2][1] * mat4[3][2] - mat4[3][1] * mat4[2][3];
		float Coef10 = mat4[1][1] * mat4[3][2] - mat4[3][1] * mat4[1][2];
		float Coef11 = mat4[1][1] * mat4[2][2] - mat4[2][1] * mat4[1][2];

		float Coef12 = mat4[2][0] * mat4[3][3] - mat4[3][0] * mat4[2][3];
		float Coef14 = mat4[1][0] * mat4[3][3] - mat4[3][0] * mat4[1][3];
		float Coef15 = mat4[1][0] * mat4[2][3] - mat4[2][0] * mat4[1][3];

		float Coef16 = mat4[2][0] * mat4[3][2] - mat4[3][0] * mat4[2][2];
		float Coef18 = mat4[1][0] * mat4[3][2] - mat4[3][0] * mat4[1][2];
		float Coef19 = mat4[1][0] * mat4[2][2] - mat4[2][0] * mat4[1][2];

		float Coef20 = mat4[2][0] * mat4[3][1] - mat4[3][0] * mat4[2][1];
		float Coef22 = mat4[1][0] * mat4[3][1] - mat4[3][0] * mat4[1][1];
		float Coef23 = mat4[1][0] * mat4[2][1] - mat4[2][0] * mat4[1][1];

		SVector4Df Fac0(Coef00, Coef00, Coef02, Coef03);
		SVector4Df Fac1(Coef04, Coef04, Coef06, Coef07);
		SVector4Df Fac2(Coef08, Coef08, Coef10, Coef11);
		SVector4Df Fac3(Coef12, Coef12, Coef14, Coef15);
		SVector4Df Fac4(Coef16, Coef16, Coef18, Coef19);
		SVector4Df Fac5(Coef20, Coef20, Coef22, Coef23);


		SVector4Df Vec0(mat4[1][0], mat4[0][0], mat4[0][0], mat4[0][0]);
		SVector4Df Vec1(mat4[1][1], mat4[0][1], mat4[0][1], mat4[0][1]);
		SVector4Df Vec2(mat4[1][2], mat4[0][2], mat4[0][2], mat4[0][2]);
		SVector4Df Vec3(mat4[1][3], mat4[0][3], mat4[0][3], mat4[0][3]);

		SVector4Df Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		SVector4Df Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		SVector4Df Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		SVector4Df Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		SVector4Df SignA(+1, -1, +1, -1);
		SVector4Df SignB(-1, +1, -1, +1);

		CMatrix4Df Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		SVector4Df Row0(Inverse.mat4[0][0], Inverse.mat4[1][0], Inverse.mat4[2][0], Inverse.mat4[3][0]);

		SVector4Df Dot0(mat4[0] * Row0);

		float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		float OneOverDeterminant = 1.0f / Dot1;

		return Inverse * OneOverDeterminant;
	}

	/**
	 * InverseGJ - Calculates the inverse of a matrix using the Gauss-Jordan method.
	 *
	 * This function computes the inverse of the provided 4x4 matrix using the
	 * Gauss-Jordan elimination method. If the matrix is singular (non-invertible),
	 * the behavior is undefined or may result in a failure to calculate the inverse.
	 *
	 * The Gauss-Jordan method involves augmenting the input matrix with the identity
	 * matrix and performing row operations to reduce the input matrix to the identity
	 * matrix, simultaneously transforming the identity matrix into the inverse.
	 *
	 * @param inputMatrix: The matrix to be inverted.
	 * @return A CMatrix4Df object representing the inverse of the input matrix.
	 */
	CMatrix4Df InverseGJ(const CMatrix4Df& inputMatrix)
	{
		CMatrix4Df inverseMatrix{};
		float augmented[4][8] = { 0 };

		// Get the input matrix
		const auto& mat = inputMatrix.GetMatrix();

		// Create the augmented matrix
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				augmented[i][j] = mat[i][j]; // Copy the original matrix
			}
			augmented[i][i + 4] = 1; // Add the identity matrix
		}

		// Perform Gaussian elimination
		for (int i = 0; i < 4; i++)
		{
			// Normalize the current row
			float diagElement = augmented[i][i];
			for (int j = 0; j < 8; j++)
			{
				augmented[i][j] /= diagElement;
			}

			// Eliminate other rows
			for (int k = 0; k < 4; k++)
			{
				if (k != i)
				{
					float factor = augmented[k][i];
					for (int j = 0; j < 8; j++)
					{
						augmented[k][j] -= factor * augmented[i][j];
					}
				}
			}
		}

		// Extract the inverse matrix
		float result[4][4] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result[i][j] = augmented[i][j + 4];
			}
		}

		// Set the result to the inverse matrix object
		inverseMatrix.SetMatrix(result);
		return inverseMatrix;
	}


	/**
	 * Initializes the matrix to zero using memset.
	 *
	 * This function efficiently sets all elements of the matrix to zero using the `memset` function.
	 */
	void InitMemZero()
	{
		arr_mem_zero_ref(mat4);
	}

	/**
	 * InitZero: Initializes the matrix to zero element-wise.
	 *
	 * This function sets all elements of the matrix to zero using a nested loop.
	 */
	void InitZero()
	{
		for (int8_t i = 0; i < 4; i++)
		{
			for (int8_t j = 0; j < 4; j++)
			{
				mat4[i][j] = 0.0f;
			}
		}
	}

	/**
	 * Initializes the matrix with a specified number.
	 *
	 * This function initializes the matrix elements by the specified `fNum` value.
	 *
	 * @param fNum: The value for each matrix element.
	 */
	void InitNum(const float fNum)
	{
		for (int8_t i = 0; i < 4; i++)
		{
			for (int8_t j = 0; j < 4; j++)
			{
				mat4[i][j] = fNum;
			}
		}
	}

	/**
	 * Initializes the matrix to the identity matrix.
	 *
	 * This function sets the matrix to the identity matrix, which has ones on the
	 * diagonal and zeros elsewhere.
	 */
	void InitIdentity()
	{
		mat4[0][0] = 1.0f; mat4[0][1] = 0.0f; mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
		mat4[1][0] = 0.0f; mat4[1][1] = 1.0f; mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
		mat4[2][0] = 0.0f; mat4[2][1] = 0.0f; mat4[2][2] = 1.0f; mat4[2][3] = 0.0f;
		mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
	}

	/**
	 * Initializes a scaling transformation matrix.
	 *
	 * This function sets up a 4x4 transformation matrix that applies scaling
	 * transformations along the X, Y, and Z axes. The scaling values determine
	 * how much the object is enlarged or shrunk in each respective axis.
	 *
	 * @param fScaleX: Scaling factor along the X-axis.
	 * @param fScaleY: Scaling factor along the Y-axis.
	 * @param fScaleZ: Scaling factor along the Z-axis.
	 */
	void InitScaleTransform(const float fScaleX, const float fScaleY, const float fScaleZ);

	/**
	 * Initializes a uniform scaling transformation matrix.
	 *
	 * This function sets up a 4x4 transformation matrix that applies uniform
	 * scaling along all three axes (X, Y, and Z) using the specified scale factor.
	 * It delegates the call to the function that accepts individual scaling
	 * factors for each axis.
	 *
	 * @param fScale: Uniform scaling factor applied to all axes.
	 */
	void InitScaleTransform(const float fScale);

	/**
	 * Initializes a scaling transformation matrix using a vector.
	 *
	 * This function sets up a 4x4 transformation matrix that applies scaling
	 * transformations along the X, Y, and Z axes using the values provided
	 * in the input vector. It delegates the call to the function that accepts
	 * individual scaling factors.
	 *
	 * @param vScale: A 3D vector containing scaling factors for the X, Y, and Z axes.
	 */
	void InitScaleTransform(const SVector3Df& vScale);

	/**
	 * Initializes a scaling transformation matrix using a glm vector.
	 *
	 * This function sets up a 4x4 transformation matrix that applies scaling
	 * transformations along the X, Y, and Z axes using the values provided
	 * in the input vector. It delegates the call to the function that accepts
	 * individual scaling factors.
	 *
	 * @param vScale: A glm::vec3 containing scaling factors for the X, Y, and Z axes.
	 */
	void InitScaleTransform(const glm::vec3& vScale);

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * @param RotateX: The angle of rotation around the X-axis in degrees.
	 * @param RotateY: The angle of rotation around the Y-axis in degrees.
	 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 */
	void InitRotateTransform(const float fRotateX, const float fRotateY, const float fRotateZ);

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * @param vRotation: The 3D Vector holding rotation degrees.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 */
	void InitRotateTransform(const SVector3Df& vRotation);

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 * 
	 * @param vRotation: The glm::vec3 holding rotation degrees.
	 */
	void InitRotateTransform(const glm::vec3& vRotation);

	/**
	 * Initializes the matrix for a rotation transformation using the XYZ Euler rotation sequence.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rx * ry * rz. This order applies the
	 * rotations in the sequence: Z-axis, Y-axis, then X-axis, relative to the
	 * global coordinate system.
	 * 
	 * @param RotateX: The angle of rotation around the X-axis in degrees.
	 * @param RotateY: The angle of rotation around the Y-axis in degrees.
	 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
	 *
	 */
	void InitRotateTransformZYX(const float fRotateX, const float fRotateY, const float fRotateZ);

	/**
	 * Initializes the matrix for a rotation transformation using a quaternion.
	 *
	 * Quaternions provide a compact and efficient way to represent 3D rotations,
	 * avoiding issues like gimbal lock that occur with Euler angles.
	 *
	 * The resulting matrix applies the rotation described by the quaternion
	 * when multiplied by a vector or another matrix.
	 *
	 * Note: The function assumes the matrix `mat4` is a 4x4 matrix, and it resets
	 *       the translation components to zero, leaving the rotation part intact.
	 *
	 * @param sQuat: The quaternion representing the rotation to be applied from GLM.
	 */
	 void InitRotateTransform(const SQuaternion& sQuat);

	/**
	 * Initializes the matrix for a rotation transformation using a quaternion.	
	 *
	 * Quaternions provide a compact and efficient way to represent 3D rotations,
	 * avoiding issues like gimbal lock that occur with Euler angles.
	 *
	 * The resulting matrix applies the rotation described by the quaternion
	 * when multiplied by a vector or another matrix.
	 *
	 * Note: The function assumes the matrix `mat4` is a 4x4 matrix, and it resets
	 *       the translation components to zero, leaving the rotation part intact.
	 * 
	 * @param sQuat: The quaternion representing the rotation to be applied from GLM.
	 */
	void InitRotateTransform(const glm::quat& sQuat);

	/**
	 * Initializes the rotation transformation matrix based on a specified direction.
	 * 
	 * This function aligns the camera's forward direction (`vDir`) with the
	 * specified direction while keeping the up direction constant as (0, 1, 0).
	 *
	 * @param vDir: Direction vector to align the transformation with.
	 */
	void InitRotationFromDir(const SVector3Df& vDir);

	/**
	 * Initializes the rotation transformation matrix based on a specified direction.
	 *
	 * This function aligns the camera's forward direction (`vDir`) with the
	 * specified direction while keeping the up direction constant as (0, 1, 0).
	 * 
	 * @param vDir: Direction vector to align the transformation with.
	 */
	void InitRotationFromDir(const glm::vec3& vDir);

	/**
	 * Initializes the matrix for translation.
	 *
	 * This function sets up a 4x4 matrix that applies a translation
	 * transformation. The resulting matrix moves objects in 3D space along
	 * the specified axes.
	 *
	 * @param x: The translation offset along the X-axis.
	 * @param y: The translation offset along the Y-axis.
	 * @param z: The translation offset along the Z-axis.
	 */
	void InitTranslationTransform(const float x, const float y, const float z);

	/**
	 * Initializes the matrix for translation.
	 *
	 * This function sets up a 4x4 matrix that applies a translation
	 * transformation. The resulting matrix moves objects in 3D space along
	 * the specified axes.
	 *
	 * @param vTranslate: The translation offset 3D Vector GLM.
	 */
	void InitTranslationTransform(const SVector3Df& vDir);

	/**
	 * Initializes the matrix for translation.
	 *
	 * This function sets up a 4x4 matrix that applies a translation
	 * transformation. The resulting matrix moves objects in 3D space along
	 * the specified axes.
	 *
	 * @param vTranslate: The translation offset 3D Vector GLM.
	 */
	void InitTranslationTransform(const glm::vec3& vDir);

	/**
	 * Initializes the camera transformation matrix
	 *
	 * This function sets up the camera transformation matrix based on
	 * the specified target and up direction. The result is a matrix
	 * that transforms coordinates into the camera's coordinate space.
	 * 
	 * @param vTarget: Vector representing the target direction (camera's forward direction)
	 * @param vUp: Vector representing the up direction of the camera
	 */
	void InitCameraTransform(const SVector3Df& vTarget, const SVector3Df& vUp);

	/**
	 * Initializes the camera transformation matrix
	 *
	 * This function sets up the camera transformation matrix based on
	 * the specified target and up direction. The result is a matrix
	 * that transforms coordinates into the camera's coordinate space.
	 * 
	 * @param vTarget: Vector representing the target direction (camera's forward direction)
	 * @param vUp: Vector representing the up direction of the camera
	 */
	void InitCameraTransform(const glm::vec3& vTarget, const glm::vec3& vUp);

	/**
	 * Initializes a camera transformation matrix.
	 *
	 * This function combines a translation and rotation transformation
	 * to position and orient a camera in 3D space. The camera is first
	 * translated to the origin relative to its position (`vPos`), and
	 * then it is rotated to face the target (`vTarget`) with the specified
	 * upward direction (`vUp`). The resulting transformation matrix
	 * is the product of the rotation and translation matrices.
	 *
	 * @param vPos The position of the camera in world space.
	 * @param vTarget The target point the camera should face.
	 * @param vUp The upward direction vector for the camera's orientation.
	 */
	void InitCameraTransform(const SVector3Df& vPos, const SVector3Df& vTarget, const SVector3Df& vUp);

	/**
	 * Initializes a camera transformation matrix.
	 *
	 * This function combines a translation and rotation transformation
	 * to position and orient a camera in 3D space. The camera is first
	 * translated to the origin relative to its position (`vPos`), and
	 * then it is rotated to face the target (`vTarget`) with the specified
	 * upward direction (`vUp`). The resulting transformation matrix
	 * is the product of the rotation and translation matrices.
	 *
	 * @param vPos The position of the camera in world space as GLM.
	 * @param vTarget The target point the camera should face as GLM.
	 * @param vUp The upward direction vector for the camera's orientation as GLM.
	 */
	void InitCameraTransform(const glm::vec3& vPos, const glm::vec3& vTarget, const glm::vec3& vUp);

	/**
	 * Initializes a perspective projection transformation matrix.
	 *
	 * This function sets up a perspective projection matrix based on the
	 * given projection parameters. It supports both left-handed and right-handed
	 * coordinate systems.
	 *
	 * @param sPersProj: The projection parameters, encapsulated in an SPersProjInfo structure.
	 * @param bUseGLM A boolean indicating whether to use GLM functions for the transformation.
	 * @param bLeftHanded A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitPersProjTransform(const SPersProjInfo& sPersProj, const bool bUseGLM = false, const bool bLeftHanded = false);

	/**
	 * Initializes an orthographic projection matrix.
	 *
	 * This function constructs a 4x4 orthographic projection matrix for use
	 * in right-handed 3D graphics systems. The matrix maps the orthographic
	 * volume specified by the planes (l, r, b, t, n, f) into normalized
	 * device coordinates (NDC), where x and y range from -1 to 1, and z
	 * ranges from -1 to 1.
	 *
	 * @param sOrthoProj: A struct containing the orthographic projection parameters:
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitOrthoProjTransform(const SOrthoProjInfo& sOrthoProj, const bool bLeftHanded = false);

	/**
	 * Extracts clipping planes from the current matrix.
	 *
	 * This function calculates the six frustum planes of a perspective or
	 * orthographic projection matrix. It utilizes the rows of the matrix
	 * to compute the planes by summing or subtracting the respective row
	 * vectors. The resulting vectors define the planes in the form:
	 * Ax + By + Cz + D = 0.
	 *
	 * This is useful for tasks such as frustum culling or collision detection
	 * in 3D graphics.
	 *
	 * @param vLeft: Output vector representing the left clipping plane.
	 * @param vRight: Output vector representing the right clipping plane.
	 * @param vBottom: Output vector representing the bottom clipping plane.
	 * @param vTop: Output vector representing the top clipping plane.
	 * @param vNear: Output vector representing the near clipping plan.
	 * @param vFar: Output vector representing the far clipping plane.
	 */
	void CalculateClipPlanes(SVector4Df& vLeft, SVector4Df& vRight, SVector4Df& vBottom, SVector4Df& vTop, SVector4Df& vNear, SVector4Df& vFar) const;

	/**
	 * Extracts clipping planes from the current matrix.
	 *
	 * This function calculates the six frustum planes of a perspective or
	 * orthographic projection matrix. It utilizes the rows of the matrix
	 * to compute the planes by summing or subtracting the respective row
	 * vectors. The resulting vectors define the planes in the form:
	 * Ax + By + Cz + D = 0.
	 *
	 * This is useful for tasks such as frustum culling or collision detection
	 * in 3D graphics.
	 *
	 * @param vLeft: Output vector representing the left clipping plane GLM version.
	 * @param vRight: Output vector representing the right clipping plane GLM version.
	 * @param vBottom: Output vector representing the bottom clipping plane GLM version.
	 * @paramvTop: Output vector representing the top clipping plane GLM version.
	 * @param vNear: Output vector representing the near clipping plane GLM version.
	 * @param vFar: Output vector representing the far clipping plane GLM version.
	 */
	void CalculateClipPlanes(glm::vec4& vLeft, glm::vec4& vRight, glm::vec4& vBottom, glm::vec4& vTop, glm::vec4& vNear, glm::vec4& vFar);

private:
	/**
	 * Initializes the matrix for a rotation around the X-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the X-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotX: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationX(const float fRotX, const bool bLeftHanded = false);

	/**
	 * Initializes the matrix for a rotation around the Y-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the Y-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotY: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationY(const float fRotY, const bool bLeftHanded = false);

	/**
	 * Initializes the matrix for a rotation around the Z-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the Z-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotZ: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationZ(const float fRotZ, const bool bLeftHanded = false);
};

/* Idea of "ANY TYPE" Matrix .. would it work? */
template <typename T>
class CMatrix4D
{
public:
	/**
	 * Default constructor, initializes all elements to zero.
	 */
	CMatrix4D() = default;


	/**
	 * Constructor that initializes the matrix with specific values.
	 *
	 * @param a00, a01, ..., a33: Individual matrix elements.
	 */
	CMatrix4D(const T a00, const T a01, const T a02, const T a03,
		const T a10, const T a11, const T a12, const T a13,
		const T a20, const T a21, const T a22, const T a23,
		const T a30, const T a31, const T a32, const T a33)
	{
		mat4[0][0] = a00; mat4[0][1] = a01; mat4[0][2] = a02; mat4[0][3] = a03;
		mat4[1][0] = a10; mat4[1][1] = a11; mat4[1][2] = a12; mat4[1][3] = a13;
		mat4[2][0] = a20; mat4[2][1] = a21; mat4[2][2] = a22; mat4[2][3] = a23;
		mat4[3][0] = a30; mat4[3][1] = a31; mat4[3][2] = a32; mat4[3][3] = a33;
	}

	/**
	 * Constructor that initializes the matrix from a GLM matrix.
	 *
	 * @param glmMat: A GLM matrix to copy from.
	 */
	CMatrix4D(const glm::mat4& glmMat)
	{
		mat4[0][0] = glmMat[0][0]; mat4[0][1] = glmMat[0][1]; mat4[0][2] = glmMat[0][2]; mat4[0][3] = glmMat[0][3];
		mat4[1][0] = glmMat[1][0]; mat4[1][1] = glmMat[1][1]; mat4[1][2] = glmMat[1][2]; mat4[1][3] = glmMat[1][3];
		mat4[2][0] = glmMat[2][0]; mat4[2][1] = glmMat[2][1]; mat4[2][2] = glmMat[2][2]; mat4[2][3] = glmMat[2][3];
		mat4[3][0] = glmMat[3][0]; mat4[3][1] = glmMat[3][1]; mat4[3][2] = glmMat[3][2]; mat4[3][3] = glmMat[3][3];
	}

	/**
	 * Copy constructor.
	 *
	 * @param cMat: The matrix to copy.
	 */
	CMatrix4D(const CMatrix4Df& cMat)
	{
		mat4[0][0] = cMat.mat4[0][0]; mat4[0][1] = cMat.mat4[0][1]; mat4[0][2] = cMat.mat4[0][2]; mat4[0][3] = cMat.mat4[0][3];
		mat4[1][0] = cMat.mat4[1][0]; mat4[1][1] = cMat.mat4[1][1]; mat4[1][2] = cMat.mat4[1][2]; mat4[1][3] = cMat.mat4[1][3];
		mat4[2][0] = cMat.mat4[2][0]; mat4[2][1] = cMat.mat4[2][1]; mat4[2][2] = cMat.mat4[2][2]; mat4[2][3] = cMat.mat4[2][3];
		mat4[3][0] = cMat.mat4[3][0]; mat4[3][1] = cMat.mat4[3][1]; mat4[3][2] = cMat.mat4[3][2]; mat4[3][3] = cMat.mat4[3][3];
	}

	/**
	 * Copy constructor.
	 *
	 * @param vec1: A 4D Vector to copy from first row.
	 */
	CMatrix4D(const SVector4Df& vec1, const SVector4Df& vec2, const SVector4Df& vec3, const SVector4Df& vec4)
	{
		mat4[0][0] = vec1.x; mat4[0][1] = vec1.y; mat4[0][2] = vec1.z; mat4[0][3] = vec1.w;
		mat4[1][0] = vec2.x; mat4[1][1] = vec2.y; mat4[1][2] = vec2.z; mat4[1][3] = vec2.w;
		mat4[2][0] = vec3.x; mat4[2][1] = vec3.y; mat4[2][2] = vec3.z; mat4[2][3] = vec3.w;
		mat4[3][0] = vec4.x; mat4[3][1] = vec4.y; mat4[3][2] = vec4.z; mat4[3][3] = vec4.w;
	}

	/**
	 * Overload the `[]` operator to access rows of the matrix.
	 *
	 * @param row: Row index (0-3).
	 * @return Reference to the array representing the row.
	 */
	T* operator[](size_t row)
	{
		if (row >= 4)
			throw std::out_of_range("Row index out of bounds");
		return mat4[row];
	}

	/**
	 * Access an element of the matrix (non-const version).
	 */
	T& operator()(size_t row, size_t col)
	{
		if (row >= 4 || col >= 4)
			throw std::out_of_range("Matrix index out of bounds");
		return mat4[row][col];
	}

	/**
	 * Access an element of the matrix (const version).
	 */
	const T& operator()(size_t row, size_t col) const
	{
		if (row >= 4 || col >= 4)
			throw std::out_of_range("Matrix index out of bounds");
		return mat4[row][col];
	}

	/**
	 * Print the matrix (for debugging purposes).
	 */
	void print() const
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				std::cout << mat4[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	/**
	 * Calculates the inverse of the matrix.
	 *
	 * This function calculates the inverse of the 4x4 matrix using the adjugate matrix and the determinant.
	 *
	 * @return The inverse of the matrix.
	 */
	CMatrix4D Inverse() const
	{
		// Compute the reciprocal determinant first

		float det = Determinant();

		if (det == 0.0f)
		{
			ASSERT(det == 0.0f, "Matrix Determinant Is 0");
			return (*this);
		}

		// Calculate the inverse determinant
		float fInvDet = 1.0f / det;

		// Calculate the adjugate matrix (transpose of the cofactor matrix) (https://byjus.com/maths/inverse-matrix/)
		CMatrix4D res{};

		// Calcualte First Element --- Inverse Is Positive
		res.mat4[0][0] = fInvDet * (mat4[1][1] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[1][2] *
			(mat4[2][3] * mat4[3][1] - mat4[2][1] * mat4[3][3]) + mat4[1][3] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Negative
		res.mat4[0][1] = -fInvDet * (mat4[0][1] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[2][3] * mat4[3][1] - mat4[2][1] * mat4[3][3]) + mat4[0][3] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Positive
		res.mat4[0][2] = fInvDet * (mat4[0][1] * (mat4[1][2] * mat4[3][3] - mat4[1][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[3][1] - mat4[1][1] * mat4[3][3]) + mat4[0][3] * (mat4[1][1] * mat4[3][2] - mat4[1][2] * mat4[3][1]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Negative
		res.mat4[0][3] = -fInvDet * (mat4[0][1] * (mat4[1][2] * mat4[2][3] - mat4[1][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[2][1] - mat4[1][1] * mat4[2][3]) + mat4[0][3] * (mat4[1][1] * mat4[2][2] - mat4[1][2] * mat4[2][1]));

		// Same as the FIRST just switch the [x][1] to [x][0] --- Inverse Is Negative
		res.mat4[1][0] = -fInvDet * (mat4[1][0] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[1][2] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[1][3] * (mat4[2][0] * mat4[3][2] - mat4[2][2] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Positive
		res.mat4[1][1] = fInvDet * (mat4[0][0] * (mat4[2][2] * mat4[3][3] - mat4[2][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[0][3] * (mat4[2][0] * mat4[3][2] - mat4[2][2] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Negative
		res.mat4[1][2] = -fInvDet * (mat4[0][0] * (mat4[1][2] * mat4[3][3] - mat4[1][3] * mat4[3][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[3][0] - mat4[1][0] * mat4[3][3]) + mat4[0][3] * (mat4[1][0] * mat4[3][2] - mat4[1][2] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Positive
		res.mat4[1][3] = fInvDet * (mat4[0][0] * (mat4[1][2] * mat4[2][3] - mat4[1][3] * mat4[2][2]) + mat4[0][2] *
			(mat4[1][3] * mat4[2][0] - mat4[1][0] * mat4[2][3]) + mat4[0][3] * (mat4[1][0] * mat4[2][2] - mat4[1][2] * mat4[2][0]));

		// Same as [1][0] ELEMENT calculation, just switch the [x][2] to [x][1] --- Inverse Is Positive
		res.mat4[2][0] = fInvDet * (mat4[1][0] * (mat4[2][1] * mat4[3][3] - mat4[2][3] * mat4[3][1]) + mat4[1][1] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[1][3] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Negative
		res.mat4[2][1] = -fInvDet * (mat4[0][0] * (mat4[2][1] * mat4[3][3] - mat4[2][3] * mat4[3][1]) + mat4[0][1] *
			(mat4[2][3] * mat4[3][0] - mat4[2][0] * mat4[3][3]) + mat4[0][3] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Positive
		res.mat4[2][2] = fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[3][3] - mat4[1][3] * mat4[3][1]) + mat4[0][1] *
			(mat4[1][3] * mat4[3][0] - mat4[1][0] * mat4[3][3]) + mat4[0][3] * (mat4[1][0] * mat4[3][1] - mat4[1][1] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Negative
		res.mat4[2][3] = -fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[2][3] - mat4[1][3] * mat4[2][1]) + mat4[0][1] *
			(mat4[1][3] * mat4[2][0] - mat4[1][0] * mat4[2][3]) + mat4[0][3] * (mat4[1][0] * mat4[2][1] - mat4[1][1] * mat4[2][0]));

		// Same as [2][0] ELEMENT calculation, just switch the [x][3] to [x][2] --- Inverse Is Negative
		res.mat4[3][0] = -fInvDet * (mat4[1][0] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]) + mat4[1][1] *
			(mat4[2][2] * mat4[3][0] - mat4[2][0] * mat4[3][2]) + mat4[1][2] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [1][x] to [0][x] --- Inverse Is Positive
		res.mat4[3][1] = fInvDet * (mat4[0][0] * (mat4[2][1] * mat4[3][2] - mat4[2][2] * mat4[3][1]) + mat4[0][1] *
			(mat4[2][2] * mat4[3][0] - mat4[2][0] * mat4[3][2]) + mat4[0][2] * (mat4[2][0] * mat4[3][1] - mat4[2][1] * mat4[3][0]));

		// Same as the previous just switch the [2][x] to [1][x] --- Inverse Is Negative
		res.mat4[3][2] = -fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[3][2] - mat4[1][2] * mat4[3][1]) + mat4[0][1] *
			(mat4[1][2] * mat4[3][0] - mat4[1][0] * mat4[3][2]) + mat4[0][2] * (mat4[2][0] * mat4[3][1] - mat4[1][1] * mat4[3][0]));

		// Same as the previous just switch the [3][x] to [2][x] --- Inverse Is Positive
		res.mat4[3][3] = fInvDet * (mat4[0][0] * (mat4[1][1] * mat4[2][2] - mat4[1][2] * mat4[2][1]) + mat4[0][1] *
			(mat4[1][2] * mat4[3][0] - mat4[1][0] * mat4[2][2]) + mat4[0][2] * (mat4[2][0] * mat4[2][1] - mat4[1][1] * mat4[2][0]));

		// The Pattern -> Previous Calculation (make sure of fInvDet sign) and each time make [x][n] = [x-1][n] , ex: [3][2] -> [2][2], [2][2] -> [1][2], [1][2] -> [0][2]
		return (res);
	}

	/**
	 * Calculates the determinant of the matrix.
	 *
	 * This function calculates the determinant of the 4x4 matrix using a direct formula.
	 *
	 * @return The determinant of the matrix.
	 */
	float Determinant() const
	{
		float fDet =
			mat4[0][0] * mat4[1][1] * mat4[2][2] * mat4[3][3] - mat4[0][0] * mat4[1][1] * mat4[2][3] * mat4[3][2] + mat4[0][0] * mat4[1][2] * mat4[2][3] * mat4[3][1] - mat4[0][0] * mat4[1][2] * mat4[2][1] * mat4[3][3]
			+ mat4[0][0] * mat4[1][3] * mat4[2][1] * mat4[3][2] - mat4[0][0] * mat4[1][3] * mat4[2][2] * mat4[3][1] - mat4[0][1] * mat4[1][2] * mat4[2][3] * mat4[3][0] + mat4[0][1] * mat4[1][2] * mat4[2][0] * mat4[3][3]
			- mat4[0][1] * mat4[1][3] * mat4[2][0] * mat4[3][2] + mat4[0][1] * mat4[1][3] * mat4[2][2] * mat4[3][0] - mat4[0][1] * mat4[1][0] * mat4[2][2] * mat4[3][3] + mat4[0][1] * mat4[1][0] * mat4[2][3] * mat4[3][2]
			+ mat4[0][2] * mat4[1][3] * mat4[2][0] * mat4[3][1] - mat4[0][2] * mat4[1][3] * mat4[2][1] * mat4[3][0] + mat4[0][2] * mat4[1][0] * mat4[2][1] * mat4[3][3] - mat4[0][2] * mat4[1][0] * mat4[2][3] * mat4[3][1]
			+ mat4[0][2] * mat4[1][1] * mat4[2][3] * mat4[3][0] - mat4[0][2] * mat4[1][1] * mat4[2][0] * mat4[3][3] - mat4[0][3] * mat4[1][0] * mat4[2][1] * mat4[3][2] + mat4[0][3] * mat4[1][0] * mat4[2][2] * mat4[3][1]
			- mat4[0][3] * mat4[1][1] * mat4[2][2] * mat4[3][0] + mat4[0][3] * mat4[1][1] * mat4[2][0] * mat4[3][2] - mat4[0][3] * mat4[1][2] * mat4[2][0] * mat4[3][2] + mat4[0][3] * mat4[1][2] * mat4[2][1] * mat4[3][0];

		return (fDet);
	}

private:
	T mat4[4][4] = {};
};


/**
 * CMatrix3Df: A 3x3 matrix class.
 *
 * This class represents a 3x3 matrix, commonly used in linear algebra and 3D graphics.
 * It provides various constructors for initialization and supports common matrix operations.
 *
 * Members:
 *   - mat4: A 3x3 array of floating-point values representing the matrix elements.
 */
class CMatrix3Df
{
public:
	float mat3[3][3];

	/**
	 * Default constructor, initializes all elements to zero.
	 */
	CMatrix3Df() = default;

	/**
	 * Constructor that initializes the matrix with specific values.
	 *
	 * @param a00, a01, ..., a33: Individual matrix elements.
	 */
	CMatrix3Df(const float a00, const float a01, const float a02,
		const float a10, const float a11, const float a12,
		const float a20, const float a21, const float a22)
	{
		mat3[0][0] = a00; mat3[0][1] = a01; mat3[0][2] = a02;
		mat3[1][0] = a10; mat3[1][1] = a11; mat3[1][2] = a12;
		mat3[2][0] = a20; mat3[2][1] = a21; mat3[2][2] = a22;
	}

	/**
	 * Constructor that initializes the matrix from a 3x3 Assimp matrix.
	 *
	 * @param AssimpMatrix: A 3x3 Assimp matrix to copy from.
	 */
	CMatrix3Df(const aiMatrix3x3& AssimpMatrix)
	{
		mat3[0][0] = AssimpMatrix.a1; mat3[0][1] = AssimpMatrix.a2; mat3[0][2] = AssimpMatrix.a3;
		mat3[1][0] = AssimpMatrix.b1; mat3[1][1] = AssimpMatrix.b2; mat3[1][2] = AssimpMatrix.b3;
		mat3[2][0] = AssimpMatrix.c1; mat3[2][1] = AssimpMatrix.c2; mat3[2][2] = AssimpMatrix.c3;
	}

	/**
	 * Constructor that initializes the matrix from a GLM matrix.
	 *
	 * @param glmMat: A GLM matrix to copy from.
	 */
	CMatrix3Df(const glm::mat3& glmMat)
	{
		mat3[0][0] = glmMat[0][0]; mat3[0][1] = glmMat[0][1]; mat3[0][2] = glmMat[0][2];
		mat3[1][0] = glmMat[1][0]; mat3[1][1] = glmMat[1][1]; mat3[1][2] = glmMat[1][2];
		mat3[2][0] = glmMat[2][0]; mat3[2][1] = glmMat[2][1]; mat3[2][2] = glmMat[2][2];
	}

	/**
	 * Copy constructor.
	 *
	 * @param cMat: The 3x3 matrix to copy.
	 */
	CMatrix3Df(const CMatrix3Df& cMat)
	{
		mat3[0][0] = cMat.mat3[0][0]; mat3[0][1] = cMat.mat3[0][1]; mat3[0][2] = cMat.mat3[0][2];
		mat3[1][0] = cMat.mat3[1][0]; mat3[1][1] = cMat.mat3[1][1]; mat3[1][2] = cMat.mat3[1][2];
		mat3[2][0] = cMat.mat3[2][0]; mat3[2][1] = cMat.mat3[2][1]; mat3[2][2] = cMat.mat3[2][2];
	}

	/**
 * Copy constructor.
 *
 * @param cMat: The 3x3 matrix to copy.
 */
	CMatrix3Df(const CMatrix4Df& cMat)
	{
		mat3[0][0] = cMat.mat4[0][0]; mat3[0][1] = cMat.mat4[0][1]; mat3[0][2] = cMat.mat4[0][2];
		mat3[1][0] = cMat.mat4[1][0]; mat3[1][1] = cMat.mat4[1][1]; mat3[1][2] = cMat.mat4[1][2];
		mat3[2][0] = cMat.mat4[2][0]; mat3[2][1] = cMat.mat4[2][1]; mat3[2][2] = cMat.mat4[2][2];
	}

	/**
	 * Copy constructor.
	 *
	 * @param vec1: A 4D Vector to copy from first row.
	 */
	CMatrix3Df(const SVector3Df& vec1, const SVector3Df& vec2, const SVector3Df& vec3)
	{
		mat3[0][0] = vec1.x; mat3[0][1] = vec1.y; mat3[0][2] = vec1.z;
		mat3[1][0] = vec2.x; mat3[1][1] = vec2.y; mat3[1][2] = vec2.z;
		mat3[2][0] = vec3.x; mat3[2][1] = vec3.y; mat3[2][2] = vec3.z;
	}

	/**
	 * Multiplies two CMatrix3Df matrices.
	 *
	 * This operator overloads the multiplication operator for CMatrix3Df objects,
	 * performing matrix multiplication.
	 *
	 * -Important: the correct order of multiplication is Translation * (Rotation * (Scale * Position)) by order to get the final transform
	 *
	 * @param rightMat: The right-hand side matrix.
	 *
	 * @return The product of the two matrices.
	 */
	CMatrix3Df operator*(const CMatrix3Df& rightMat)
	{
		CMatrix3Df newMat{};
		for (int8_t i = 0; i < 3; i++)
		{
			for (int8_t j = 0; j < 3; j++)
			{
				newMat.mat3[i][j] = mat3[i][0] * rightMat.mat3[0][j] + mat3[i][1] * rightMat.mat3[1][j] + mat3[i][2] * rightMat.mat3[2][j];
			}
		}
		return (newMat);

	}

	/**
	 * Multiplies a CMatrix4Df and an SVector4Df.
	 *
	 * This operator overloads the multiplication operator for CMatrix4Df and SVector4Df objects,
	 * performing matrix-vector multiplication.
	 *
	 * @param vec: The vector to multiply.
	 *
	 * @return The product of the matrix and the vector.
	 */
	SVector3Df operator*(const SVector3Df& vec) const
	{
		SVector3Df newVec{};

		newVec.x = mat3[0][0] * vec.x + mat3[0][1] * vec.y + mat3[0][2] * vec.z;
		newVec.y = mat3[1][0] * vec.x + mat3[1][1] * vec.y + mat3[1][2] * vec.z;
		newVec.z = mat3[2][0] * vec.x + mat3[2][1] * vec.y + mat3[2][2] * vec.z;
		return (newVec);
	}

	/**
	 * Transposes the matrix.
	 *
	 * This function returns a new matrix that is the transpose of the current matrix.
	 *
	 * @return The transposed matrix.
	 */
	CMatrix3Df Transpose() const
	{
		CMatrix3Df newMat{};

		for (int8_t i = 0; i < 3; i++)
		{
			for (int8_t j = 0; j < 3; j++)
			{
				newMat.mat3[i][j] = mat3[j][i];
			}
		}

		return (newMat);
	}

	/**
	 * Prints the matrix elements to the console.
	 *
	 * This function prints the matrix elements in a readable format.
	 */
	void print() const
	{
		for (int8_t i = 0; i < 3; i++)
		{
			printf("%f - %f - %f", mat3[i][0], mat3[i][1], mat3[i][2]);
		}
	}

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * @param RotateX: The angle of rotation around the X-axis in degrees.
	 * @param RotateY: The angle of rotation around the Y-axis in degrees.
	 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 */
	void InitRotateTransform(const float fRotateX, const float fRotateY, const float fRotateZ);

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * @param vRotation: The 3D Vector holding rotation degrees.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 */
	void InitRotateTransform(const SVector3Df& vRotation);

	/**
	 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rz * ry * rx. This order applies the
	 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
	 * local coordinate system.
	 *
	 * @param vRotation: The glm::vec3 holding rotation degrees.
	 */
	void InitRotateTransform(const glm::vec3& vRotation);

	/**
	 * Initializes the matrix for a rotation transformation using the XYZ Euler rotation sequence.
	 *
	 * This function creates individual rotation matrices for the X, Y, and Z axes
	 * and combines them in the order rx * ry * rz. This order applies the
	 * rotations in the sequence: Z-axis, Y-axis, then X-axis, relative to the
	 * global coordinate system.
	 *
	 * @param RotateX: The angle of rotation around the X-axis in degrees.
	 * @param RotateY: The angle of rotation around the Y-axis in degrees.
	 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
	 *
	 */
	void InitRotateTransformZYX(const float fRotateX, const float fRotateY, const float fRotateZ);

private:
	/**
	 * Initializes the matrix for a rotation around the X-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the X-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotX: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationX(const float fRotX, const bool bLeftHanded = false);

	/**
	 * Initializes the matrix for a rotation around the Y-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the Y-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotY: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationY(const float fRotY, const bool bLeftHanded = false);

	/**
	 * Initializes the matrix for a rotation around the Z-axis.
	 *
	 * This function sets up a 4x4 transformation matrix for a rotation
	 * around the Z-axis. The matrix can be configured for either left-handed
	 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
	 *
	 * @param fRotZ: The angle of rotation in radians.
	 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
	 */
	void InitRotationZ(const float fRotZ, const bool bLeftHanded = false);

};