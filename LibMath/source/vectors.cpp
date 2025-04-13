#include "stdafx.h"
#include "vectors.h"
#include "glm/gtc/matrix_transform.hpp"

/**
 * Constructs an SVector2Di object with both x and y components initialized to the same value.
 *
 * @param iVal The initial value for both x and y components.
 */
SVector2Di::SVector2Di(int iVal)
{
	x = y = iVal;
}

/**
 * Constructs an SVector2Di object with specified x and y components.
 *
 * @param _x The initial value for the x component.
 * @param _y The initial value for the y component.
 */
SVector2Di::SVector2Di(int _x, int _y)
{
	x = _x;
	y = _y;
}

/**
 * Constructs an SVector2Di object by copying the components from a glm::ivec2 vector.
 *
 * @param vec The glm::ivec2 vector to copy from.
 */
SVector2Di::SVector2Di(const glm::ivec2& vec)
{
	x = vec.x;
	y = vec.y;
}

/**
 * Adds two SVector2Di objects component-wise.
 *
 * @param vec The SVector2Di object to add.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator+(const SVector2Di& vec)
{
	return (SVector2Di(x + vec.x, y + vec.y));
}

/**
 * Subtracts one SVector2Di object from another component-wise.
 *
 * @param vec The SVector2Di object to subtract.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator-(const SVector2Di& vec)
{
	return (SVector2Di(x - vec.x, y - vec.y));
}

/**
 * Multiplies two SVector2Di objects component-wise.
 *
 * @param vec The SVector2Di object to multiply.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator*(const SVector2Di& vec)
{
	return (SVector2Di(x * vec.x, y * vec.y));
}

/**
 * Divides one SVector2Di object by another component-wise.
 *
 * @param vec The SVector2Di object to divide by.
 * @return The resulting SVector2Di object.
 * @throws std::domain_error If either component of the divisor is zero.
 */
SVector2Di SVector2Di::operator/(const SVector2Di& vec)
{
	assert(vec.x != 0 && vec.y != 0);
	return (SVector2Di(x / vec.x, y / vec.y));
}

/**
 * Adds a scalar value to both components of an SVector2Di object.
 *
 * @param iVal The scalar value to add.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator+(const int iVal)
{
	return (SVector2Di(x + iVal, y + iVal));
}

/**
 * Subtracts a scalar value from both components of an SVector2Di object.
 *
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator-(const int iVal)
{
	return (SVector2Di(x - iVal, y - iVal));
}

/**
 * Multiplies both components of an SVector2Di object by a scalar value.
 *
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector2Di object.
 */
SVector2Di SVector2Di::operator*(const int iVal)
{
	return (SVector2Di(x * iVal, y * iVal));
}

/**
 * Divides both components of an SVector2Di object by a scalar value.
 *
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector2Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Di SVector2Di::operator/(const int iVal)
{
	assert(iVal != 0);
	return (SVector2Di(x / iVal, y / iVal));
}

/**
 * Adds another SVector2Di object to this one, modifying this object in-place.
 *
 * @param vec The SVector2Di object to add.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator+=(const SVector2Di& vec)
{
	x += vec.x;
	y += vec.y;
	return (*this);
}

/**
 * Subtracts another SVector2Di object from this one, modifying this object in-place.
 *
 * @param vec The SVector2Di object to subtract.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator-=(const SVector2Di& vec)
{
	x -= vec.x;
	y -= vec.y;
	return (*this);
}

/**
 * Multiplies this SVector2Di object by another one, modifying this object in-place.
 *
 * @param vec The SVector2Di object to multiply by.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator*=(const SVector2Di& vec)
{
	x *= vec.x;
	y *= vec.y;
	return (*this);
}

/**
 * Divides this SVector2Di object by another one, modifying this object in-place.
 *
 * @param vec The SVector2Di object to divide by.
 * @return A reference to this modified SVector2Di object.
 * @throws std::domain_error If either component of the divisor is zero.
 */
SVector2Di& SVector2Di::operator/=(const SVector2Di& vec)
{
	assert(vec.x != 0 && vec.y != 0);
	x /= vec.x;
	y /= vec.y;
	return (*this);
}

/**
 * Adds a scalar value to both components of this SVector2Di object, modifying it in-place.
 *
 * @param iVal The scalar value to add.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator+=(const int iVal)
{
	x += iVal;
	y += iVal;
	return (*this);
}

/**
 * Subtracts a scalar value from both components of this SVector2Di object, modifying it in-place.
 *
 * @param iVal The scalar value to subtract.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator-=(const int iVal)
{
	x -= iVal;
	y -= iVal;
	return (*this);
}

/**
 * Multiplies both components of this SVector2Di object by a scalar value, modifying it in-place.
 *
 * @param iVal The scalar value to multiply by.
 * @return A reference to this modified SVector2Di object.
 */
SVector2Di& SVector2Di::operator*=(const int iVal)
{
	x *= iVal;
	y *= iVal;
	return (*this);
}

/**
 * Divides both components of this SVector2Di object by a scalar value, modifying it in-place.
 *
 * @param iVal The scalar value to divide by.
 * @return A reference to this modified SVector2Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Di& SVector2Di::operator/=(const int iVal)
{
	assert(iVal != 0);
	x /= iVal;
	y /= iVal;
	return (*this);
}

/**
 * Compares two SVector2Di objects for equality.
 *
 * @param vec The SVector2Di object to compare to.
 * @return true if the two objects are equal, false otherwise.
 */
bool SVector2Di::operator == (const SVector2Di& vec)
{
	return (x == vec.x && y == vec.y);
}

/**
 * Compares two SVector2Di objects for inequality.
 *
 * @param vec The SVector2Di object to compare to.
 * @return true if the two objects are not equal, false otherwise.
 */
bool SVector2Di::operator != (const SVector2Di& vec)
{
	return (!(*this == vec));
}

/**
 * Provides a const pointer to the underlying integer array.
 *
 * This allows direct access to the x and y components as an integer array.
 *
 * @return A const pointer to the integer array.
 */
SVector2Di::operator const int* () const
{
	return (&(x));
}

/**
 * Prints the x and y components of the SVector2Di object to the console.
 *
 * @param bEndl If true, a newline character is printed after the values.
 */
void SVector2Di::print(bool bEndl) const
{
	printf("(%d, %d)", x, y);
	if (bEndl)
	{
		printf("\n");
	}
}

/**
 * Calculates the length (magnitude) of the SVector2Di object.
 *
 * @return The length of the vector.
 */
int SVector2Di::length() const
{
	float fVals = static_cast<float>(x * x + y * y);
	int iLen = static_cast<int>(sqrtf(fVals));
	return (iLen);
}

/**
 * Normalizes the SVector2Di object, making its length 1.
 *
 * @return A reference to this modified SVector2Di object.
 * @throws std::domain_error If the vector is zero.
 */
SVector2Di& SVector2Di::normalize()
{
	int iLen = length();
	assert(iLen != 0);
	x /= iLen;
	y /= iLen;
	return (*this);
}

/**
 * Returns a pointer to the underlying integer array.
 *
 * This allows direct access to the x and y components as an integer array.
 *
 * @return A pointer to the integer array.
 */
int* SVector2Di::data()
{
	return &(x);
}

/**
 * Checks if both components of the SVector2Di object are zero.
 *
 * @return true if both components are zero, false otherwise.
 */
bool SVector2Di::IsZero() const
{
	return ((x + y) == 0);
}

/**
 * Sets both components of the SVector2Di object to the specified value.
 *
 * @param iVal The value to set both components to.
 */
void SVector2Di::SetAll(int iVal)
{
	x = y = iVal;
}

/**
 * Sets both components of the SVector2Di object to zero.
 */
void SVector2Di::SetToZero()
{
	SetAll(0);
}

/**
 * Converts the SVector2Di object to a glm::ivec2 object.
 *
 * @return The equivalent glm::ivec2 object.
 */
glm::ivec2 SVector2Di::ToGLM() const
{
	return (glm::ivec2(x, y));
}

/**
 * Adds a scalar value to both components of an SVector2Di object.
 *
 * @param vec The SVector2Di object to add to.
 * @param iVal The scalar value to add.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator+(const SVector2Di& vec, int iVal)
{
	SVector2Di Result(vec.x + iVal, vec.y + iVal);
	return (Result);
}

/**
 * Subtracts a scalar value from both components of an SVector2Di object.
 *
 * @param vec The SVector2Di object to subtract from.
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator-(const SVector2Di& vec, int iVal)
{
	SVector2Di Result(vec.x - iVal, vec.y - iVal);
	return (Result);
}

/**
 * Multiplies both components of an SVector2Di object by a scalar value.
 *
 * @param vec The SVector2Di object to multiply.
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator*(const SVector2Di& vec, int iVal)
{
	SVector2Di Result(vec.x * iVal, vec.y * iVal);
	return (Result);
}

/**
 * Divides both components of an SVector2Di object by a scalar value.
 *
 * @param vec The SVector2Di object to divide.
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector2Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Di operator/(const SVector2Di& vec, int iVal)
{
	assert(iVal != 0);
	SVector2Di Result(vec.x / iVal, vec.y / iVal);
	return (Result);
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/**
 * Constructs an SVector2Df object with both x and y components initialized to the same value.
 *
 * @param fVal The initial value for both x and y components.
 */
SVector2Df::SVector2Df(float fVal)
{
	x = y = fVal;
}

/**
 * Constructs an SVector2Df object with specified x and y components.
 *
 * @param _x The initial value for the x component.
 * @param _y The initial value for the y component.
 */
SVector2Df::SVector2Df(float _x, float _y)
{
	x = _x;
	y = _y;
}

/**
 * Constructs an SVector2Df object by copying the components from a glm::vec2 vector.
 *
 * @param vec The glm::vec2 vector to copy from.
 */
SVector2Df::SVector2Df(const glm::vec2& vec)
{
	x = vec.x;
	y = vec.y;
}

/**
 * Adds two SVector2Df objects component-wise.
 *
 * @param vec The SVector2Df object to add.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator+(const SVector2Df& vec)
{
	return (SVector2Df(x + vec.x, y + vec.y));
}

/**
 * Subtracts one SVector2Df object from another component-wise.
 *
 * @param vec The SVector2Df object to subtract.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator-(const SVector2Df& vec)
{
	return (SVector2Df(x - vec.x, y - vec.y));
}

/**
 * Multiplies two SVector2Df objects component-wise.
 *
 * @param vec The SVector2Df object to multiply.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator*(const SVector2Df& vec)
{
	return (SVector2Df(x * vec.x, y * vec.y));
}

/**
 * Divides one SVector2Df object by another component-wise.
 *
 * @param vec The SVector2Df object to divide by.
 * @return The resulting SVector2Df object.
 * @throws std::domain_error If either component of the divisor is zero.
 */
SVector2Df SVector2Df::operator/(const SVector2Df& vec)
{
	assert(vec.x != 0.0f && vec.y != 0.0f);
	return (SVector2Df(x / vec.x, y / vec.y));
}

/**
 * Adds a scalar value to both components of an SVector2Df object.
 *
 * @param fVal The scalar value to add.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator+(const float fVal)
{
	return (SVector2Df(x + fVal, y + fVal));
}

/**
 * Subtracts a scalar value from both components of an SVector2Df object.
 *
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator-(const float fVal)
{
	return (SVector2Df(x - fVal, y - fVal));
}

/**
 * Multiplies both components of an SVector2Df object by a scalar value.
 *
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector2Df object.
 */
SVector2Df SVector2Df::operator*(const float fVal)
{
	return (SVector2Df(x * fVal, y * fVal));
}

/**
 * Divides both components of an SVector2Df object by a scalar value.
 *
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector2Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Df SVector2Df::operator/(const float fVal)
{
	assert(fVal != 0.0f);
	return (SVector2Df(x / fVal, y / fVal));
}

/**
 * Adds another SVector2Df object to this one, modifying this object in-place.
 *
 * @param vec The SVector2Df object to add.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator+=(const SVector2Df& vec)
{
	x += vec.x;
	y += vec.y;
	return (*this);
}

/**
 * Subtracts another SVector2Df object from this one, modifying this object in-place.
 *
 * @param vec The SVector2Df object to subtract.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator-=(const SVector2Df& vec)
{
	x -= vec.x;
	y -= vec.y;
	return (*this);
}

/**
 * Multiplies this SVector2Df object by another one, modifying this object in-place.
 *
 * @param vec The SVector2Df object to multiply by.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator*=(const SVector2Df& vec)
{
	x *= vec.x;
	y *= vec.y;
	return (*this);
}

/**
 * Divides this SVector2Df object by another one, modifying this object in-place.
 *
 * @param vec The SVector2Df object to divide by.
 * @return A reference to this modified SVector2Df object.
 * @throws std::domain_error If either component of the divisor is zero.
 */
SVector2Df& SVector2Df::operator/=(const SVector2Df& vec)
{
	assert(vec.x != 0.0f && vec.y != 0.0f);
	x /= vec.x;
	y /= vec.y;
	return (*this);
}

/**
 * Adds a scalar value to both components of this SVector2Df object, modifying it in-place.
 *
 * @param fVal The scalar value to add.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator+=(const float fVal)
{
	x += fVal;
	y += fVal;
	return (*this);
}

/**
 * Subtracts a scalar value from both components of this SVector2Df object, modifying it in-place.
 *
 * @param fVal The scalar value to subtract.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator-=(const float fVal)
{
	x -= fVal;
	y -= fVal;
	return (*this);
}

/**
 * Multiplies both components of this SVector2Df object by a scalar value, modifying it in-place.
 *
 * @param fVal The scalar value to multiply by.
 * @return A reference to this modified SVector2Df object.
 */
SVector2Df& SVector2Df::operator*=(const float fVal)
{
	x *= fVal;
	y *= fVal;
	return (*this);
}

/**
 * Divides both components of this SVector2Df object by a scalar value, modifying it in-place.
 *
 * @param fVal The scalar value to divide by.
 * @return A reference to this modified SVector2Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Df& SVector2Df::operator/=(const float fVal)
{
	assert(fVal != 0.0f);
	x /= fVal;
	y /= fVal;
	return (*this);
}

/**
 * Compares two SVector2Df objects for equality.
 *
 * @param vec The SVector2Df object to compare to.
 * @return true if the two objects are equal, false otherwise.
 */
bool SVector2Df::operator==(const SVector2Df& vec)
{
	return (x == vec.x && y == vec.y);
}

/**
 * Compares two SVector2Df objects for inequality.
 *
 * @param vec The SVector2Df object to compare to.
 * @return true if the two objects are not equal, false otherwise.
 */
bool SVector2Df::operator!=(const SVector2Df& vec)
{
	return (!(*this == vec));
}

/**
 * Provides a const pointer to the underlying float array.
 *
 * This allows direct access to the x and y components as a float array.
 *
 * @return A const pointer to the float array.
 */
SVector2Df::operator const float* () const
{
	return (&(x));
}

/**
 * Prints the x and y components of the SVector2Df object to the console.
 *
 * @param bEndl If true, a newline character is printed after the values.
 */
void SVector2Df::print(bool bEndl) const
{
	printf("(%f, %f)", x, y);
	if (bEndl)
	{
		printf("\n");
	}
}

/**
 * Calculates the length (magnitude) of the SVector2Df object.
 *
 * @return The length of the vector.
 */
float SVector2Df::length() const
{
	float fLen = sqrtf(x * x + y * y); // Corrected the calculation
	return (fLen);
}

/**
 * Normalizes the SVector2Df object, making its length 1.
 *
 * @return A reference to this modified SVector2Df object.
 * @throws std::domain_error If the vector is zero.
 */
SVector2Df& SVector2Df::normalize()
{
	float fLen = length();
	assert(fLen != 0.0f);
	x /= fLen;
	y /= fLen;
	return (*this);
}

/**
 * Returns a pointer to the underlying float array.
 *
 * This allows direct access to the x and y components as a float array.
 *
 * @return A pointer to the float array.
 */
float* SVector2Df::data()
{
	return &(x);
}

/**
 * Checks if both components of the SVector2Df object are zero.
 *
 * @return true if both components are zero, false otherwise.
 */
bool SVector2Df::IsZero() const
{
	return ((x == 0.0f) && (y == 0.0f)); // Corrected the condition
}

/**
 * Sets both components of the SVector2Df object to the specified value.
 *
 * @param fVal The value to set both components to.
 */
void SVector2Df::SetAll(float fVal)
{
	x = y = fVal;
}

/**
 * Sets both components of the SVector2Df object to zero.
 */
void SVector2Df::SetToZero()
{
	SetAll(0.0f);
}

/**
 * Converts the SVector2Df object to a glm::vec2 object.
 *
 * @return The equivalent glm::vec2 object.
 */
glm::vec2 SVector2Df::ToGLM() const
{
	return (glm::vec2(x, y));
}

/**
 * Adds a scalar value to both components of an SVector2Df object.
 *
 * @param vec The SVector2Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator+(const SVector2Df& vec, float fVal)
{
	SVector2Df Result(vec.x + fVal, vec.y + fVal);
	return (Result);
}

/**
 * Subtracts a scalar value from both components of an SVector2Df object.
 *
 * @param vec The SVector2Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator-(const SVector2Df& vec, float fVal)
{
	SVector2Df Result(vec.x - fVal, vec.y - fVal);
	return (Result);
}

/**
 * Multiplies both components of an SVector2Df object by a scalar value.
 *
 * @param vec The SVector2Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator*(const SVector2Df& vec, float fVal)
{
	SVector2Df Result(vec.x * fVal, vec.y * fVal);
	return (Result);
}

/**
 * Divides both components of an SVector2Df object by a scalar value.
 *
 * @param vec The SVector2Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector2Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Df operator/(const SVector2Df& vec, float fVal)
{
	assert(fVal != 0.0f);
	SVector2Df Result(vec.x / fVal, vec.y / fVal);
	return (Result);
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/**
 * Constructs an SVector3Di object with all components initialized to the same value.
 *
 * @param iVal The initial value for all components.
 */
SVector3Di::SVector3Di(int iVal)
{
	x = y = z = iVal;
}

/**
 * Constructs an SVector3Di object with specified x, y, and z components.
 *
 * @param _x The initial value for the x component.
 * @param _y The initial value for the y component.
 * @param _z The initial value for the z component.  
 */
SVector3Di::SVector3Di(int _x, int _y, int _z)
{
	x = _x;
	y = _y;
	z = _z;
}

/**
 * Constructs an SVector3Di object by copying the components from an integer array.
 *
 * @param pVec A pointer to the integer array containing the x, y, and z components.
 */
SVector3Di::SVector3Di(const int* pVec)
{
	if (!pVec)
	{
		x = y = z = 0;
		return;
	}

	x = pVec[0];
	y = pVec[1];
	z = pVec[2];
}

/**
 * Constructs an SVector3Di object by copying the x and y components from an SVector2Di object,
 * setting the z component to 0.
 *
 * @param vec The SVector2Di object to copy from.
 */
SVector3Di::SVector3Di(const SVector2Di& vec)
{
	x = vec.x;
	y = vec.y;
	z = 0;
}

/**
 * Constructs an SVector3Di object by copying the components from a glm::ivec3 vector.
 *
 * @param vec The glm::ivec3 vector to copy from.
 */
SVector3Di::SVector3Di(const glm::ivec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

/**
 * Adds two SVector3Di objects component-wise.
 *
 * @param vec The SVector3Di object to add.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator+(const SVector3Di& vec)
{
	return (SVector3Di(x + vec.x, y + vec.y, z + vec.z));
}

/**
 * Subtracts one SVector3Di object from another component-wise.
 *
 * @param vec The SVector3Di object to subtract.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator-(const SVector3Di& vec)
{
	return (SVector3Di(x - vec.x, y - vec.y, z - vec.z));
}

/**
 * Multiplies two SVector3Di objects component-wise.
 *
 * @param vec The SVector3Di object to multiply.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator*(const SVector3Di& vec)
{
	return (SVector3Di(x * vec.x, y * vec.y, z * vec.z));
}

/**
 * Divides one SVector3Di object by another component-wise.
 *
 * @param vec The SVector3Di object to divide by.
 * @return The resulting SVector3Di object.
 * @throws std::domain_error If either component of the divisor is zero.
 */
SVector3Di SVector3Di::operator/(const SVector3Di& vec)
{
	assert(vec.x != 0 && vec.y != 0);
	return (SVector3Di(x / vec.x, y / vec.y, z / vec.z));
}

/**
 * Adds a scalar value to all components of an SVector3Di object.
 *
 * @param iVal The scalar value to add.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator+(const int iVal)
{
	return (SVector3Di(x + iVal, y + iVal, z + iVal));
}

/**
 * Subtracts a scalar value from all components of an SVector3Di object.
 *
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator-(const int iVal)
{
	return (SVector3Di(x - iVal, y - iVal, z - iVal));
}

/**
 * Multiplies all components of an SVector3Di object by a scalar value.
 *
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector3Di object.
 */
SVector3Di SVector3Di::operator*(const int iVal)
{
	return (SVector3Di(x * iVal, y * iVal, z * iVal));
}

/**
 * Divides all components of an SVector3Di object by a scalar value.
 *
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector3Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Di SVector3Di::operator/(const int iVal)
{
	assert(iVal != 0);
	return (SVector3Di(x / iVal, y / iVal, z / iVal));
}

/**
 * Adds another SVector3Di object to this one, modifying this object in-place.
 *
 * @param vec The SVector3Di object to add.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator+=(const SVector3Di& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return (*this);
}

/**
 * Subtracts another SVector3Di object from this one, modifying this object in-place.
 *
 * @param vec The SVector3Di object to subtract.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator-=(const SVector3Di& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return (*this);
}

/**
 * Multiplies this SVector3Di object by another one, modifying this object in-place.
 *
 * @param vec The SVector3Di object to multiply by.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator*=(const SVector3Di& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return (*this);
}

/**
 * Divides this SVector3Di object by another one, modifying this object in-place.
 *
 * @param vec The SVector3Di object to divide by.
 * @return A reference to this modified SVector3Di object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector3Di& SVector3Di::operator/=(const SVector3Di& vec)
{
	assert(vec.x != 0 && vec.y != 0 && vec.z != 0);
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return (*this);
}

/**
 * Adds a scalar value to all components of this SVector3Di object, modifying it in-place.
 *
 * @param iVal The scalar value to add.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator+=(const int iVal)
{
	x += iVal;
	y += iVal;
	z += iVal;
	return (*this);
}

/**
 * Subtracts a scalar value from all components of this SVector3Di object, modifying it in-place.
 *
 * @param iVal The scalar value to subtract.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator-=(const int iVal)
{
	x -= iVal;
	y -= iVal;
	z -= iVal;
	return (*this);
}

/**
 * Multiplies all components of this SVector3Di object by a scalar value, modifying it in-place.
 *
 * @param iVal The scalar value to multiply by.
 * @return A reference to this modified SVector3Di object.
 */
SVector3Di& SVector3Di::operator*=(const int iVal)
{
	x *= iVal;
	y *= iVal;
	z *= iVal;
	return (*this);
}

/**
 * Divides all components of this SVector3Di object by a scalar value, modifying it in-place.
 *
 * @param iVal The scalar value to divide by.
 * @return A reference to this modified SVector3Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Di& SVector3Di::operator/=(const int iVal)
{
	assert(iVal != 0);
	x /= iVal;
	y /= iVal;
	z /= iVal;
	return (*this);
}

/**
 * Compares two SVector3Di objects for equality.
 *
 * @param vec The SVector3Di object to compare to.
 * @return true if the two objects are equal, false otherwise.
 */
bool SVector3Di::operator == (const SVector3Di& vec)
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

/**
 * Compares two SVector3Di objects for inequality.
 *
 * @param vec The SVector3Di object to compare to.
 * @return true if the two objects are not equal, false otherwise.
 */
bool SVector3Di::operator != (const SVector3Di& vec)
{
	return (!(*this == vec));
}

/**
 * Provides a const pointer to the underlying integer array.
 *
 * This allows direct access to the x, y, and z components as an integer array.
 *
 * @return A const pointer to the integer array.
 */
SVector3Di::operator const int* () const
{
	return (&(x));
}

/**
 * Prints the x, y, and z components of the SVector3Di object to the console.
 *
 * @param bEndl If true, a newline character is printed after the values.
 */
void SVector3Di::print(bool bEndl) const
{
	printf("(%d, %d, %d)", x, y, z);
	if (bEndl)
	{
		printf("\n");
	}
}

/**
 * Calculates the length (magnitude) of the SVector3Di object.
 *
 * @return The length of the vector.
 */
int SVector3Di::length() const
{
	float fVals = static_cast<float>(x * x + y * y + z * z);
	int iLen = static_cast<int>(sqrtf(fVals));
	return (iLen);
}

/**
 * Normalizes the SVector3Di object, making its length 1.
 *
 * @return A reference to this modified SVector3Di object.
 * @throws std::domain_error If the vector is zero.
 */
SVector3Di& SVector3Di::normalize()
{
	int iLen = length();
	assert(iLen != 0);
	x /= iLen;
	y /= iLen;
	z /= iLen;
	return (*this);
}

/**
 * Calculates the dot product of two SVector3Di objects.
 *
 * @param vec The SVector3Di object to calculate the dot product with.
 * @return The dot product of the two vectors.
 */
int SVector3Di::dot(const SVector3Di& vec) const
{
	int iRet = x * vec.x + y * vec.y + z * vec.z;
	return (iRet);
}

/**
 * Calculates the cross product of two SVector3Di objects.
 *
 * @param vec The SVector3Di object to calculate the cross product with.
 * @return The cross product of the two vectors.
 */
SVector3Di SVector3Di::cross(const SVector3Di& vec) const
{
	const int _x = y * vec.z - z * vec.y;
	const int _y = z * vec.x - x * vec.z;
	const int _z = x * vec.y - y * vec.x;
	return SVector3Di(_x, _y, _z);
}

/**
 * Calculates the Euclidean distance between two SVector3Di objects.
 *
 * @param vec The SVector3Di object to calculate the distance to.
 * @return The distance between the two vectors.
 */
int SVector3Di::distance(const SVector3Di& vec) const
{
	int delta_x = x - vec.x;
	int delta_y = y - vec.y;
	int delta_z = z - vec.z;

	float fVals = static_cast<float>(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
	int distance = static_cast<int>(sqrtf(fVals));
	return (distance);
}

/**
 * Negates all components of the SVector3Di object.
 *
 * @return The negated SVector3Di object.
 */
SVector3Di SVector3Di::negate() const
{
	SVector3Di result(-x, -y, -z);
	return (result);
}

/**
 * Returns a pointer to the underlying integer array.
 *
 * This allows direct access to the x, y, and z components as an integer array.
 *
 * @return A pointer to the integer array.
 */
int* SVector3Di::data()
{
	return &(x);
}

/**
 * Initializes the components of the SVector3Di object with random values within specified ranges.
 *
 * @param minVal The minimum value for each component.
 * @param maxVal The maximum value for each component.
 */
void SVector3Di::InitRandom(const SVector3Di& minVal, const SVector3Di& maxVal)
{
	x = RandomIntegerRange(minVal.x, maxVal.x);
	y = RandomIntegerRange(minVal.y, maxVal.y);
	z = RandomIntegerRange(minVal.z, maxVal.z);
}

/**
 * Initializes the components of the SVector3Di object with random values within a specified range.
 *
 * @param minVal The minimum value for each component.
 * @param maxVal The maximum value for each component.
 */
void SVector3Di::InitRandom(int minVal, int maxVal)
{
	x = RandomIntegerRange(minVal, maxVal);
	y = RandomIntegerRange(minVal, maxVal);
	z = RandomIntegerRange(minVal, maxVal);
}

/**
 * Initializes the SVector3Di object using spherical coordinates.
 *
 * @param Radius The radius of the sphere.
 * @param Pitch The pitch angle in degrees.
 * @param Heading The heading angle in degrees.
 */
void SVector3Di::InitBySphericalCoords(int Radius, int Pitch, int Heading)
{
	x = Radius * static_cast<int>(std::cosf(ToRadian(Pitch))) * static_cast<int>(std::sinf(ToRadian(Heading)));
	y = -Radius * static_cast<int>(std::sinf(ToRadian(Pitch)));
	z = Radius * static_cast<int>(std::cosf(ToRadian(Pitch))) * static_cast<int>(std::cosf(ToRadian(Heading)));
}

/**
 * Checks if all components of the SVector3Di object are zero.
 *
 * @return true if all components are zero, false otherwise.
 */
bool SVector3Di::IsZero() const
{
	return ((x + y + z) == 0);
}

/**
 * Sets all components of the SVector3Di object to the specified value.
 *
 * @param iVal The value to set all components to.
 */
void SVector3Di::SetAll(int iVal)
{
	x = y = z = iVal;
}

/**
 * Sets all components of the SVector3Di object to zero.
 */
void SVector3Di::SetToZero()
{
	SetAll(0);
}

/**
 * Converts the SVector3Di object to a glm::ivec3 object.
 *
 * @return The equivalent glm::ivec3 object.
 */
glm::ivec3 SVector3Di::ToGLM() const
{
	return (glm::ivec3(x, y, z));
}

/**
 * Adds a scalar value to all components of an SVector3Di object.
 *
 * @param vec The SVector3Di object to add to.
 * @param iVal The scalar value to add.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator+(const SVector3Di& vec, int iVal)
{
	SVector3Di Result(vec.x + iVal, vec.y + iVal, vec.z + iVal);
	return (Result);
}

/**
 * Subtracts a scalar value from all components of an SVector3Di object.
 *
 * @param vec The SVector3Di object to subtract from.
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator-(const SVector3Di& vec, int iVal)
{
	SVector3Di Result(vec.x - iVal, vec.y - iVal, vec.z - iVal);
	return (Result);
}

/**
 * Multiplies all components of an SVector3Di object by a scalar value.
 *
 * @param vec The SVector3Di object to multiply.
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator*(const SVector3Di& vec, int iVal)
{
	SVector3Di Result(vec.x * iVal, vec.y * iVal, vec.z * iVal);
	return (Result);
}

/**
 * Divides all components of an SVector3Di object by a scalar value.
 *
 * @param vec The SVector3Di object to divide.
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector3Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Di operator/(const SVector3Di& vec, int iVal)
{
	assert(iVal != 0);
	SVector3Di Result(vec.x / iVal, vec.y / iVal, vec.z / iVal);
	return (Result);
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/**
 * Constructs an SVector3Df object with all components set to the same value.
 *
 * @param fVal The value to set all components to.
 */
SVector3Df::SVector3Df(const float fVal)
{
	x = y = z = fVal;
}

/**
 * Constructs an SVector3Df object with specified values for each component.
 *
 * @param _x The value for the x component.
 * @param _y The value for the y component.
 * @param _z The value for the z component.  
 */
SVector3Df::SVector3Df(const float _x, const float _y, const float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

/**
 * Constructs an SVector3Df object from a std::array of floats, with optional safety check.
 *
 * @param arr The std::array of floats to construct from.
 * @param bSafety If true, performs a bounds check on the array indices (throws exception for out-of-bounds access).
 */
SVector3Df::SVector3Df(const std::array<float, 3>& arr, bool bSafety)
{
	if (!bSafety)
	{
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}
	else
	{
		x = arr.at(0);
		y = arr.at(1);
		z = arr.at(2);
	}
}

/**
 * Constructs an SVector3Df object from a pointer to a float array.
 *
 * @param pVec The pointer to the float array.
 * @throws std::runtime_error If the pointer is null.
 */
SVector3Df::SVector3Df(const float* pVec)
{
	if (!pVec)
	{
		x = y = z = 0.0f;
		return;
	}

	x = pVec[0];
	y = pVec[1];
	z = pVec[2];
}

/**
 * Constructs an SVector3Df object from an SVector2Df object, setting the z component to zero.
 *
 * @param vec The SVector2Df object to construct from.
 */
SVector3Df::SVector3Df(const SVector2Df& vec)
{
	x = vec.x;
	y = vec.y;
	z = 0.0f;
}

/**
 * Constructs an SVector3Df object from a glm::vec3 object.
 *
 * @param vec The glm::vec3 object to construct from.
 */
SVector3Df::SVector3Df(const glm::vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

/**
 * Constructs an SVector3Df object from a SVector3Df object.
 *
 * @param vec The glm::vec3 object to construct from.
 */
SVector3Df::SVector3Df(const SVector3Df& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

/**
 * Constructs an SVector3Df object from a SVector4Df object.
 *
 * @param vec The glm::vec3 object to construct from.
 */
SVector3Df::SVector3Df(const SVector4Df& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

/**
 * Constructs an SVector3Df object from a glm::vec4 object.
 *
 * @param vec The glm::vec3 object to construct from.
 */
SVector3Df::SVector3Df(const glm::vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

/**
 * Adds two SVector3Df objects component-wise.
 *
 * @param vec The SVector3Df object to add.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator+(const SVector3Df& vec)
{
	return (SVector3Df(x + vec.x, y + vec.y, z + vec.z));
}

/**
 * Subtracts one SVector3Df object from another component-wise.
 *
 * @param vec The SVector3Df object to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator-(const SVector3Df& vec)
{
	return (SVector3Df(x - vec.x, y - vec.y, z - vec.z));
}

/**
 * Multiplies two SVector3Df objects component-wise.
 *
 * @param vec The SVector3Df object to multiply.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator*(const SVector3Df& vec)
{
	return (SVector3Df(x * vec.x, y * vec.y, z * vec.z));
}

/**
 * Divides one SVector3Df object by another component-wise.
 *
 * @param vec The SVector3Df object to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector3Df SVector3Df::operator/(const SVector3Df& vec)
{
	assert(vec.x != 0 && vec.y != 0);
	return (SVector3Df(x / vec.x, y / vec.y, z / vec.z));
}

/**
 * Adds a scalar value to all components of an SVector3Df object.
 *
 * @param fVal The scalar value to add.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator+(const float fVal)
{
	return (SVector3Df(x + fVal, y + fVal, z + fVal));
}

/**
 * Subtracts a scalar value from all components of an SVector3Df object.
 *
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator-(const float fVal)
{
	return (SVector3Df(x - fVal, y - fVal, z - fVal));
}

/**
 * Subtracts a scalar value from all components of an SVector3Df object.
 *
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator-() const
{
	return (SVector3Df(-x, -y, -z));
}

/**
 * Multiplies all components of an SVector3Df object by a scalar value.
 *
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector3Df object.
 */
SVector3Df SVector3Df::operator*(const float fVal)
{
	return (SVector3Df(x * fVal, y * fVal, z * fVal));
}

/**
 * Divides all components of an SVector3Df object by a scalar value.
 *
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Df SVector3Df::operator/(const float fVal)
{
	assert(fVal != 0.0f);
	return (SVector3Df(x / fVal, y / fVal, z / fVal));
}

/**
 * Adds another SVector3Df object to this one, modifying this object in-place.
 *
 * @param vec The SVector3Df object to add.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator+=(const SVector3Df& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return (*this);
}

/**
 * Subtracts another SVector3Df object from this one, modifying this object in-place.
 *
 * @param vec The SVector3Df object to subtract.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator-=(const SVector3Df& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return (*this);
}

/**
 * Multiplies this SVector3Df object by another one, modifying this object in-place.
 *
 * @param vec The SVector3Df object to multiply by.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator*=(const SVector3Df& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return (*this);
}

/**
 * Divides this SVector3Df object by another one, modifying this object in-place.
 *
 * @param vec The SVector3Df object to divide by.
 * @return A reference to this modified SVector3Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector3Df& SVector3Df::operator/=(const SVector3Df& vec)
{
	assert(vec.x != 0.0f && vec.y != 0.0f && vec.z != 0.0f);
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return (*this);
}

/**
 * Adds a scalar value to all components of this SVector3Df object, modifying it in-place.
 *
 * @param fVal The scalar value to add.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator+=(const float fVal)
{
	x += fVal;
	y += fVal;
	z += fVal;
	return (*this);
}

/**
 * Subtracts a scalar value from all components of this SVector3Df object, modifying it in-place.
 *
 * @param fVal The scalar value to subtract.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator-=(const float fVal)
{
	x -= fVal;
	y -= fVal;
	z -= fVal;
	return (*this);
}

/**
 * Multiplies all components of this SVector3Df object by a scalar value, modifying it in-place.
 *
 * @param fVal The scalar value to multiply by.
 * @return A reference to this modified SVector3Df object.
 */
SVector3Df& SVector3Df::operator*=(const float fVal)
{
	x *= fVal;
	y *= fVal;
	z *= fVal;
	return (*this);
}

/**
 * Divides all components of this SVector3Df object by a scalar value, modifying it in-place.
 *
 * @param fVal The scalar value to divide by.
 * @return A reference to this modified SVector3Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Df& SVector3Df::operator/=(const float fVal)
{
	assert(fVal != 0);
	x /= fVal;
	y /= fVal;
	z /= fVal;
	return (*this);
}

/**
 * Compares two SVector3Df objects for equality.
 *
 * @param vec The SVector3Df object to compare to.
 * @return true if the two objects are equal, false otherwise.
 */
bool SVector3Df::operator == (const SVector3Df& vec)
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

/**
 * Compares two SVector3Df objects for inequality.
 *
 * @param vec The SVector3Df object to compare to.
 * @return true if the two objects are not equal, false otherwise.
 */
bool SVector3Df::operator != (const SVector3Df& vec)
{
	return (!(*this == vec));
}

/**
 * Provides a const pointer to the underlying float array.
 *
 * This allows direct access to the x, y, and z components as a float array.
 *
 * @return A const pointer to the float array.
 */
SVector3Df::operator const float* () const
{
	return (&(x));
}

/**
 * Prints the x, y, and z components of the SVector3Df object to the console.
 *
 * @param bEndl If true, a newline character is printed after the values.
 */
void SVector3Df::print(bool bEndl) const
{
	printf("(%f, %f, %f)", x, y, z);
	if (bEndl)
	{
		printf("\n");
	}
}

/**
 * Calculates the length (magnitude) of the SVector3Df object.
 *
 * @return The length of the vector.
 */
float SVector3Df::length() const
{
	float fLen = std::sqrtf(x * x + y * y + z * z); // works
	return (fLen);
}

/**
 * Normalizes the SVector3Df object, making its length 1.
 *
 * @return A reference to this modified SVector3Df object.
 * @throws std::domain_error If the vector is zero.
 */
SVector3Df& SVector3Df::normalize()
{
	float fLen = length();
	assert(fLen != 0.0f);
	x /= fLen;
	y /= fLen;
	z /= fLen;
	return (*this);
}

/**
 * Check if the vector can be normalized
 *
 * @return true if the Vector can be normalized, otherwise false
 */
bool SVector3Df::CanNormalize() const
{
	if (length() > 0.0f)
		return (true);

	return (false);
}

/**
 * Calculates the dot product of two SVector3Df objects.
 *
 * @param vec The SVector3Df object to calculate the dot product with.
 * @return The dot product of the two vectors.
 */
float SVector3Df::dot(const SVector3Df& vec) const
{
	float fRet = x * vec.x + y * vec.y + z * vec.z;
	return (fRet);
}

/**
 * Calculates the cross product of two SVector3Df objects.
 *
 * @param vec The SVector3Df object to calculate the cross product with.
 * @return The cross product of the two vectors.
 */
SVector3Df SVector3Df::cross(const SVector3Df& vec) const
{
	const float _x = y * vec.z - z * vec.y;
	const float _y = z * vec.x - x * vec.z;
	const float _z = x * vec.y - y * vec.x;
	return SVector3Df(_x, _y, _z);
}

/**
 * Rotates the vector using quaternion by giving angle and vector.
 *
 * @param fAngle The Angle of Rotation.
 * @param vec The vector to rotate around, could represent an Axis
 */
void SVector3Df::rotate(const float fAngle, const SVector3Df& vec)
{
	// Convert the vector to a quaternion with w = 0
	QUAT vectorQuat;
	Quaternion_Set(0.0, x, y, z, &vectorQuat);

	// Create the quaternion from the axis and angle
	QUAT rotationQuat;
	Quaternion_FromAxisAngle(vec, fAngle, &rotationQuat); // Convert angle to radians

	// Compute the conjugate of the rotation quaternion
	QUAT conjugateQuat;
	Quaternion_Conjugate(&rotationQuat, &conjugateQuat);

	// Perform the quaternion multiplication: q * v * q^-1
	QUAT tempQuat;
	Quaternion_Multiply(&rotationQuat, &vectorQuat, &tempQuat);  // q * v
	QUAT resultQuat;
	Quaternion_Multiply(&tempQuat, &conjugateQuat, &resultQuat); // q * v * q^-1

	// Extract the rotated vector from the resulting quaternion
	x = resultQuat.x;
	y = resultQuat.y;
	z = resultQuat.z;
}

/**
 * Calculates the Euclidean distance between two SVector3Df objects.
 *
 * @param vec The SVector3Df object to calculate the distance to.
 * @return The distance between the two vectors.
 */
float SVector3Df::distance(const SVector3Df& vec) const
{
	float delta_x = x - vec.x;
	float delta_y = y - vec.y;
	float delta_z = z - vec.z;

	float distance = sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
	return (distance);
}

/**
 * Negates all components of the SVector3Df object.
 *
 * @return The negated SVector3Df object.
 */
SVector3Df SVector3Df::negate() const
{
	SVector3Df result(-x, -y, -z);
	return (result);
}

/**
 * Returns a pointer to the underlying float array.
 *
 * This allows direct access to the x, y, and z components as a float array.
 *
 * @return A pointer to the float array.
 */
float* SVector3Df::data()
{
	return &(x);
}

/**
 * Initializes the components of the SVector3Df object with random values within specified ranges.
 *
 * @param minVal The minimum value for each component.
 * @param maxVal The maximum value for each component.
 */
void SVector3Df::InitRandom(const SVector3Df& minVal, const SVector3Df& maxVal)
{
	x = RandomFloatRange(minVal.x, maxVal.x);
	y = RandomFloatRange(minVal.y, maxVal.y);
	z = RandomFloatRange(minVal.z, maxVal.z);
}

/**
 * Initializes the components of the SVector3Df object with random values within a specified range.
 *
 * @param minVal The minimum value for each component.
 * @param maxVal The maximum value for each component.
 */
void SVector3Df::InitRandom(float minVal, float maxVal)
{
	x = RandomFloatRange(minVal, maxVal);
	y = RandomFloatRange(minVal, maxVal);
	z = RandomFloatRange(minVal, maxVal);
}

/**
 * Initializes the SVector3Df object using spherical coordinates.
 *
 * @param Radius The radius of the sphere.
 * @param Pitch The pitch angle in degrees.
 * @param Heading The heading angle in degrees.
 */
void SVector3Df::InitBySphericalCoords(float Radius, float Pitch, float Heading)
{
	x = Radius * std::cosf(ToRadian(Pitch)) * std::sinf(ToRadian(Heading));
	y = -Radius * std::sinf(ToRadian(Pitch));
	z = Radius * std::cosf(ToRadian(Pitch)) * std::cosf(ToRadian(Heading));
}

/**
 * Checks if all components of the SVector3Df object are zero.
 *
 * @return true if all components are zero, false otherwise.
 */
bool SVector3Df::IsZero() const
{
	return ((x + y + z) == 0.0f);
}

/**
 * Sets all components of the SVector3Df object to the specified value.
 *
 * @param fVal The value to set all components to.
 */
void SVector3Df::SetAll(float fVal)
{
	x = y = z = fVal;
}

/**
 * Sets all components of the SVector3Df object to zero.
 */
void SVector3Df::SetToZero()
{
	SetAll(0.0f);
}

/**
 * Converts the SVector3Df object to a glm::vec3 object.
 *
 * @return The equivalent glm::vec3 object.
 */
glm::vec3 SVector3Df::ToGLM() const
{
	return (glm::vec3(x, y, z));
}

/**
 * Adds a scalar value to all components of an SVector3Df object.
 *
 * @param vec The SVector3Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator+(const SVector3Df& vec, float fVal)
{
	SVector3Df Result(vec.x + fVal, vec.y + fVal, vec.z + fVal);
	return (Result);
}

/**
 * Subtracts a scalar value from all components of an SVector3Df object.
 *
 * @param vec The SVector3Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator-(const SVector3Df& vec, float fVal)
{
	SVector3Df Result(vec.x - fVal, vec.y - fVal, vec.z - fVal);
	return (Result);
}

/**
 * Multiplies all components of an SVector3Df object by a scalar value.
 *
 * @param vec The SVector3Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator*(const SVector3Df& vec, float fVal)
{
	SVector3Df Result(vec.x * fVal, vec.y * fVal, vec.z * fVal);
	return (Result);
}

/**
 * Divides all components of an SVector3Df object by a scalar value.
 *
 * @param vec The SVector3Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Df operator/(const SVector3Df& vec, float fVal)
{
	assert(fVal != 0.0f);
	SVector3Df Result(vec.x / fVal, vec.y / fVal, vec.z / fVal);
	return (Result);
}

/**
 * Adds two SVector3Df objects component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator+(const SVector3Df& vec1, const SVector3Df& vec2)
{
	SVector3Df Result(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
	return (Result);
}

/**
 * Subtracts one SVector3Df object from another component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator-(const SVector3Df& vec1, const SVector3Df& vec2)
{
	SVector3Df Result(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
	return (Result);
}

/**
 * Multiplies two SVector3Df objects component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to multiply.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator*(const SVector3Df& vec1, const SVector3Df& vec2)
{
	SVector3Df Result(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
	return (Result);
}

/**
 * Divides one SVector3Df object by another component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector3Df operator/(const SVector3Df& vec1, const SVector3Df& vec2)
{
	assert(vec2.x != 0.0f && vec2.y != 0.0f && vec2.z != 0.0f);
	SVector3Df Result(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z);
	return (Result);
}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/

/**
 * SVector4Df constructor that initializes all components to a single value.
 *
 * @param fVal The value to initialize all components to.
 */
SVector4Df::SVector4Df(const float fVal)
{
	x = y = z = w = fVal;
}

/**
 * SVector4Df constructor that initializes each component with a separate value.
 *
 * @param _x The value for the x component.
 * @param _y The value for the y component.
 * @param _z The value for the z component.
 * @param _w The value for the w component. (scalar)
 */
SVector4Df::SVector4Df(const float _x, const float _y, const float _z, const float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

/**
 * SVector4Df constructor that initializes from a std::array, with optional safety checks.
 *
 * If `bSafety` is true, the constructor uses `arr.at(i)` for bounds checking.
 * Otherwise, it assumes valid indexing and uses `arr[i]`.
 *
 * @param arr The std::array to copy from.
 * @param bSafety Flag to enable bounds checking.
 */
SVector4Df::SVector4Df(const std::array<float, 4>& arr, bool bSafety)
{
	if (!bSafety)
	{
		x = arr[0];
		y = arr[1];
		z = arr[2];
		w = arr[3];
	}
	else
	{
		x = arr.at(0);
		y = arr.at(1);
		z = arr.at(2);
		w = arr.at(3);
	}
}

/**
 * SVector4Df constructor that initializes from a raw float pointer.
 *
 * If the pointer is null, all components are set to zero.
 *
 * @param pVec The pointer to a float array (length 4).
 */
SVector4Df::SVector4Df(const float* pVec)
{
	if (!pVec)
	{
		x = y = z = w = 0;
		return;
	}

	x = pVec[0];
	y = pVec[1];
	z = pVec[2];
	w = pVec[3];
}


/**
 * SVector4Df constructor that initializes from an SVector2Df and two additional values.
 *
 * @param vec The SVector2Df object to copy from.
 * @param _z The value for the z component.
 * @param _w The value for the w component.
 */
SVector4Df::SVector4Df(const SVector2Df& vec, const float _z, const float _w)
{
	x = vec.x;
	y = vec.y;
	z = _z;
	w = _w;
}

/**
 * SVector4Df constructor that initializes from an SVector3Df and one additional value.
 *
 * @param vec The SVector3Df object to copy from.
 * @param _w The value for the w component.
 */
SVector4Df::SVector4Df(const SVector3Df& vec, const float _w)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = _w;
}

/**
 * SVector4Df constructor that initializes from a glm::vec3 and a scalar value.
 *
 * @param vec The glm::vec3 object to copy from.
 * @param _w The value for the w component.
 */
SVector4Df::SVector4Df(const glm::vec3& vec, const float _w)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = _w;
}

/**
 * SVector4Df copy constructor.
 *
 * @param vec The SVector4Df object to copy.
 */
SVector4Df::SVector4Df(const SVector4Df& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

/**
 * SVector4Df constructor that initializes from a glm::vec4 object.
 *
 * @param vec The glm::vec4 object to copy from.
 */
SVector4Df::SVector4Df(const glm::vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

/**
 * Compares two SVector4Df objects for equality.
 *
 * @param vec The SVector4Df object to compare to.
 * @return true if the two objects are equal, false otherwise.
 */
bool SVector4Df::operator == (const SVector4Df& vec)
{
	return (x == vec.x && y == vec.y && z == vec.z && w == vec.w);
}

/**
 * Compares two SVector4Df objects for inequality.
 *
 * @param vec The SVector4Df object to compare to.
 * @return true if the two objects are not equal, false otherwise.
 */
bool SVector4Df::operator != (const SVector4Df& vec)
{
	return (!(*this == vec));
}

/**
 * Provides a const pointer to the underlying float array.
 *
 * This allows direct access to the x, y, z, and w components as a float array.
 *
 * @return A const pointer to the float array.
 */
SVector4Df::operator const float* () const
{
	return (&(x));
}

/**
 * Prints the x, y, z, and w components of the SVector4Df object to the console.
 *
 * @param bEndl If true, a newline character is printed after the values.
 */
void SVector4Df::print(bool bEndl) const
{
	printf("(%f, %f, %f, %f)", x, y, z, w);
	if (bEndl)
	{
		printf("\n");
	}
}

/**
 * Calculates the length (magnitude) of the SVector4Df object.
 *
 * @return The length of the vector.
 */
float SVector4Df::length() const
{
	float fLen = sqrtf(x * x + y * y + z * z + w * w);
	return (fLen);
}

/**
 * Normalizes the SVector4Df object, making its length 1.
 *
 * @return A reference to this modified SVector4Df object.
 * @throws std::domain_error If the vector is zero.
 */
SVector4Df& SVector4Df::normalize()
{
	float fLen = length();
	assert(fLen != 0.0f);
	x /= fLen;
	y /= fLen;
	z /= fLen;
	w /= fLen;
	return (*this);
}

/**
 * Calculates the dot product of two SVector4Df objects.
 *
 * @param vec The SVector4Df object to calculate the dot product with.
 * @return The dot product of the two vectors.
 */
float SVector4Df::dot(const SVector4Df& vec) const
{
	float fRet = x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	return (fRet);
}

/**
 * Returns a pointer to the underlying float array.
 *
 * This allows direct access to the x, y, z, and w components as a float array.
 *
 * @return A pointer to the float array.
 */
float* SVector4Df::data()
{
	return (&(x));
}

/**
 * Checks if all components of the SVector4Df object are zero.
 *
 * @return true if all components are zero, false otherwise.
 */
bool SVector4Df::IsZero() const
{
	return ((x + y + z + w) == 0.0f);
}

/**
 * Sets all components of the SVector4Df object to the specified value.
 *
 * @param fVal The value to set all components to.
 */
void SVector4Df::SetAll(const float fVal)
{
	x = y = z = w = fVal;
}


/**
 * Sets all components of the SVector4Df object to zero.
 */
void SVector4Df::SetToZero()
{
	SetAll(0.0f);
}

/**
 * Converts the SVector4Df object to a glm::vec4 object.
 *
 * @return The equivalent glm::vec4 object.
 */
glm::vec4 SVector4Df::ToGLM() const
{
	return (glm::vec4(x, y, z, w));
}

/**
 * Converts the SVector4Df object to an SVector3Df object by discarding the w component.
 *
 * @return The equivalent SVector3Df object.
 */
SVector3Df SVector4Df::To3D() const
{
	return SVector3Df(x, y, z);
}

/**
 * Adds a scalar value to all components of an SVector4Df object.
 *
 * @param vec The SVector4Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator+(const SVector4Df& vec, float fVal)
{
	SVector4Df Result(vec.x + fVal, vec.y + fVal, vec.z + fVal, vec.w + fVal);
	return (Result);
}

/**
 * Subtracts a scalar value from all components of an SVector4Df object.
 *
 * @param vec The SVector4Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator-(const SVector4Df& vec, float fVal)
{
	SVector4Df Result(vec.x - fVal, vec.y - fVal, vec.z - fVal, vec.w - fVal);
	return (Result);
}

/**
 * Multiplies all components of an SVector4Df object by a scalar value.
 *
 * @param vec The SVector4Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator*(const SVector4Df& vec, float fVal)
{
	SVector4Df Result(vec.x * fVal, vec.y * fVal, vec.z * fVal, vec.w * fVal);
	return (Result);
}

/**
 * Divides all components of an SVector4Df object by a scalar value.
 *
 * @param vec The SVector4Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector4Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector4Df operator/(const SVector4Df& vec, float fVal)
{
	assert(fVal != 0.0f);
	SVector4Df Result(vec.x / fVal, vec.y / fVal, vec.z / fVal, vec.w / fVal);
	return (Result);
}

/**
 * Adds two SVector4Df objects component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator+(const SVector4Df& vec1, const SVector4Df& vec2)
{
	SVector4Df Result(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);
	return (Result);
}

/**
 * Subtracts one SVector4Df object from another component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to subtract.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator-(const SVector4Df& vec1, const SVector4Df& vec2)
{
	SVector4Df Result(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w);
	return (Result);
}

/**
 * Multiplies two SVector4Df objects component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to multiply.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator*(const SVector4Df& vec1, const SVector4Df& vec2)
{
	SVector4Df Result(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w);
	return (Result);
}

/**
 * Divides one SVector4Df object by another component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to divide by.
 * @return The resulting SVector4Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector4Df operator/(const SVector4Df& vec1, const SVector4Df& vec2)
{
	assert(vec2.x != 0.0f && vec2.y != 0.0f && vec2.z != 0.0f && vec2.w != 0.0f);
	SVector4Df Result(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z, vec1.w / vec2.w);
	return (Result);

}

