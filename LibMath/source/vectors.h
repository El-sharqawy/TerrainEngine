#pragma once

/**
 * this code have been written by Osama Elsharqawy @2024
 * @cerdits in the Implementation and better understanding of 3D Mathematics to @OGLDev
 * 
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <glm/glm.hpp>
#include <array>

/**
 * SVector2Di: A 2D integer vector struct.
 *
 * This struct represents a 2D vector with integer components. It provides
 * basic vector operations such as addition, subtraction, scalar multiplication,
 * dot product, length calculation, and normalization.
 *
 * Key features:
 * - Efficient integer-based operations
 * - Accurate length and normalization calculations
 * - Support for dot product
 * - Conversion to and from GLM vectors for interoperability
 * - Clear and concise implementation adhering to Betty coding standards
 */
struct SVector2Di
{
	union {
		int x;
		int u;
	};
	union {
		int y;
		int v;
	};

	/**
	 * Default constructor, initializes all components to zero.
	 */
	SVector2Di() = default;

	/**
	 * Constructs an SVector2Di object with both x and y components initialized to the same value.
	 *
	 * @param iVal The initial value for both x and y components.
	 */
	SVector2Di(int iVal);

	/**
	 * Constructs an SVector2Di object with specified x and y components.
	 *
	 * @param _x The initial value for the x component.
	 * @param _y The initial value for the y component.
	 */
	SVector2Di(int _x, int _y);

	/**
	 * Constructs an SVector2Di object by copying the components from a glm::ivec2 vector.
	 *
	 * @param vec The glm::ivec2 vector to copy from.
	 */
	SVector2Di(const glm::ivec2& vec);

	/**
	 * Adds two SVector2Di objects component-wise.
	 *
	 * @param vec The SVector2Di object to add.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator+(const SVector2Di& vec);

	/**
	 * Subtracts one SVector2Di object from another component-wise.
	 *
	 * @param vec The SVector2Di object to subtract.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator-(const SVector2Di& vec);

	/**
	 * Multiplies two SVector2Di objects component-wise.
	 *
	 * @param vec The SVector2Di object to multiply.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator*(const SVector2Di& vec);

	/**
	 * Divides one SVector2Di object by another component-wise.
	 *
	 * @param vec The SVector2Di object to divide by.
	 * @return The resulting SVector2Di object.
	 * @throws std::domain_error If either component of the divisor is zero.
	 */
	SVector2Di operator/(const SVector2Di& vec);

	/**
	 * Adds a scalar value to both components of an SVector2Di object.
	 *
	 * @param iVal The scalar value to add.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator+(const int iVal);

	/**
	 * Subtracts a scalar value from both components of an SVector2Di object.
	 *
	 * @param iVal The scalar value to subtract.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator-(const int iVal);

	/**
	 * Multiplies both components of an SVector2Di object by a scalar value.
	 *
	 * @param iVal The scalar value to multiply by.
	 * @return The resulting SVector2Di object.
	 */
	SVector2Di operator*(const int iVal);

	/**
	 * Divides both components of an SVector2Di object by a scalar value.
	 *
	 * @param iVal The scalar value to divide by.
	 * @return The resulting SVector2Di object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector2Di operator/(const int iVal);

	/**
	 * Adds another SVector2Di object to this one, modifying this object in-place.
	 *
	 * @param vec The SVector2Di object to add.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator+=(const SVector2Di& vec);

	/**
	 * Subtracts another SVector2Di object from this one, modifying this object in-place.
	 *
	 * @param vec The SVector2Di object to subtract.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator-=(const SVector2Di& vec);

	/**
	 * Multiplies this SVector2Di object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector2Di object to multiply by.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator*=(const SVector2Di& vec);

	/**
	 * Divides this SVector2Di object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector2Di object to divide by.
	 * @return A reference to this modified SVector2Di object.
	 * @throws std::domain_error If either component of the divisor is zero.
	 */
	SVector2Di& operator/=(const SVector2Di& vec);

	/**
	 * Adds a scalar value to both components of this SVector2Di object, modifying it in-place.
	 *
	 * @param iVal The scalar value to add.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator+=(const int iVal);

	/**
	 * Subtracts a scalar value from both components of this SVector2Di object, modifying it in-place.
	 *
	 * @param iVal The scalar value to subtract.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator-=(const int iVal);

	/**
	 * Multiplies both components of this SVector2Di object by a scalar value, modifying it in-place.
	 *
	 * @param iVal The scalar value to multiply by.
	 * @return A reference to this modified SVector2Di object.
	 */
	SVector2Di& operator*=(const int iVal);

	/**
	 * Divides both components of this SVector2Di object by a scalar value, modifying it in-place.
	 *
	 * @param iVal The scalar value to divide by.
	 * @return A reference to this modified SVector2Di object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector2Di& operator/=(const int iVal);

	/**
	 * Compares two SVector2Di objects for equality.
	 *
	 * @param vec The SVector2Di object to compare to.
	 * @return true if the two objects are equal, false otherwise.
	 */
	bool operator == (const SVector2Di& vec);

	/**
	 * Compares two SVector2Di objects for inequality.
	 *
	 * @param vec The SVector2Di object to compare to.
	 * @return true if the two objects are not equal, false otherwise.
	 */
	bool operator != (const SVector2Di& vec);

	/**
	 * Provides a const pointer to the underlying integer array.
	 *
	 * This allows direct access to the x and y components as an integer array.
	 *
	 * @return A const pointer to the integer array.
	 */
	operator const int* () const;

	/**
	 * Prints the x and y components of the SVector2Di object to the console.
	 *
	 * @param bEndl If true, a newline character is printed after the values.
	 */
	void print(bool bEndl = true) const;

	/**
	 * Calculates the length (magnitude) of the SVector2Di object.
	 *
	 * @return The length of the vector.
	 */
	int length() const;

	/**
	 * Normalizes the SVector2Di object, making its length 1.
	 *
	 * @return A reference to this modified SVector2Di object.
	 * @throws std::domain_error If the vector is zero.
	 */
	SVector2Di& normalize();

	/**
	 * Returns a pointer to the underlying integer array.
	 *
	 * This allows direct access to the x and y components as an integer array.
	 *
	 * @return A pointer to the integer array.
	 */
	int* data();

	/**
	 * Checks if both components of the SVector2Di object are zero.
	 *
	 * @return true if both components are zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Sets both components of the SVector2Di object to the specified value.
	 *
	 * @param iVal The value to set both components to.
	 */
	void SetAll(int iVal);

	/**
	 * Sets both components of the SVector2Di object to zero.
	 */
	void SetToZero();

	/**
	 * Converts the SVector2Di object to a glm::ivec2 object.
	 *
	 * @return The equivalent glm::ivec2 object.
	 */
	glm::ivec2 ToGLM() const;
};

/**
 * Adds a scalar value to both components of an SVector2Di object.
 *
 * @param vec The SVector2Di object to add to.
 * @param iVal The scalar value to add.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator+(const SVector2Di& vec, int iVal);

/**
 * Subtracts a scalar value from both components of an SVector2Di object.
 *
 * @param vec The SVector2Di object to subtract from.
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator-(const SVector2Di& vec, int iVal);

/**
 * Multiplies both components of an SVector2Di object by a scalar value.
 *
 * @param vec The SVector2Di object to multiply.
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector2Di object.
 */
SVector2Di operator*(const SVector2Di& vec, int iVal);

/**
 * Divides both components of an SVector2Di object by a scalar value.
 *
 * @param vec The SVector2Di object to divide.
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector2Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Di operator/(const SVector2Di& vec, int iVal);

/**
 * SVector2Df: A 2D float vector struct.
 *
 * This struct represents a 2D vector with float components. It provides
 * basic vector operations such as addition, subtraction, scalar multiplication,
 * dot product, length calculation, and normalization.
 *
 * Key features:
 * - Efficient float-based operations
 * - Accurate length and normalization calculations
 * - Support for dot product
 * - Conversion to and from GLM vectors for interoperability
 * - Clear and concise implementation adhering to Betty coding standards
 */
struct SVector2Df
{
	union {
		float x;
		float u;
	};

	union {
		float y;
		float v;
	};

	/**
	 * Default constructor, initializes all components to zero.
	 */
	SVector2Df() = default;

	/**
	 * Constructs an SVector2Df object with both x and y components initialized to the same value.
	 *
	 * @param fVal The initial value for both x and y components.
	 */
	SVector2Df(float fVal);

	/**
	 * Constructs an SVector2Df object with specified x and y components.
	 *
	 * @param _x The initial value for the x component.
	 * @param _y The initial value for the y component.
	 */
	SVector2Df(float _x, float _y);

	/**
	 * Constructs an SVector2Df object with both x and y components initialized to the same value.
	 *
	 * @param fVal The initial value for both x and y components.
	 */
	SVector2Df(int iVal);

	/**
	 * Constructs an SVector2Df object with specified x and y components.
	 *
	 * @param _x The initial value for the x component.
	 * @param _y The initial value for the y component.
	 */
	SVector2Df(int _x, int _y);

	/**
	 * Constructs an SVector2Df object by copying the components from a glm::vec2 vector.
	 *
	 * @param vec The glm::vec2 vector to copy from.
	 */
	SVector2Df(const glm::vec2& vec);

	/**
	 * Adds two SVector2Df objects component-wise.
	 *
	 * @param vec The SVector2Df object to add.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator+(const SVector2Df& vec);

	/**
	 * Subtracts one SVector2Df object from another component-wise.
	 *
	 * @param vec The SVector2Df object to subtract.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator-(const SVector2Df& vec);

	/**
	 * Multiplies two SVector2Df objects component-wise.
	 *
	 * @param vec The SVector2Df object to multiply.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator*(const SVector2Df& vec);

	/**
	 * Divides one SVector2Df object by another component-wise.
	 *
	 * @param vec The SVector2Df object to divide by.
	 * @return The resulting SVector2Df object.
	 * @throws std::domain_error If either component of the divisor is zero.
	 */
	SVector2Df operator/(const SVector2Df& vec);

	/**
	 * Adds a scalar value to both components of an SVector2Df object.
	 *
	 * @param fVal The scalar value to add.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator+(const float fVal);

	/**
	 * Subtracts a scalar value from both components of an SVector2Df object.
	 *
	 * @param fVal The scalar value to subtract.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator-(const float fVal);

	/**
	 * Multiplies both components of an SVector2Df object by a scalar value.
	 *
	 * @param fVal The scalar value to multiply by.
	 * @return The resulting SVector2Df object.
	 */
	SVector2Df operator*(const float fVal);

	/**
	 * Divides both components of an SVector2Df object by a scalar value.
	 *
	 * @param fVal The scalar value to divide by.
	 * @return The resulting SVector2Df object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector2Df operator/(const float fVal);

	/**
	 * Adds another SVector2Df object to this one, modifying this object in-place.
	 *
	 * @param vec The SVector2Df object to add.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator+=(const SVector2Df& vec);

	/**
	 * Subtracts another SVector2Df object from this one, modifying this object in-place.
	 *
	 * @param vec The SVector2Df object to subtract.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator-=(const SVector2Df& vec);

	/**
	 * Multiplies this SVector2Df object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector2Df object to multiply by.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator*=(const SVector2Df& vec);

	/**
	 * Divides this SVector2Df object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector2Df object to divide by.
	 * @return A reference to this modified SVector2Df object.
	 * @throws std::domain_error If either component of the divisor is zero.
	 */
	SVector2Df& operator/=(const SVector2Df& vec);

	/**
	 * Adds a scalar value to both components of this SVector2Df object, modifying it in-place.
	 *
	 * @param fVal The scalar value to add.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator+=(const float fVal);

	/**
	 * Subtracts a scalar value from both components of this SVector2Df object, modifying it in-place.
	 *
	 * @param fVal The scalar value to subtract.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator-=(const float fVal);

	/**
	 * Multiplies both components of this SVector2Df object by a scalar value, modifying it in-place.
	 *
	 * @param fVal The scalar value to multiply by.
	 * @return A reference to this modified SVector2Df object.
	 */
	SVector2Df& operator*=(const float fVal);

	/**
	 * Divides both components of this SVector2Df object by a scalar value, modifying it in-place.
	 *
	 * @param fVal The scalar value to divide by.
	 * @return A reference to this modified SVector2Df object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector2Df& operator/=(const float fVal);

	/**
	 * Compares two SVector2Df objects for equality.
	 *
	 * @param vec The SVector2Df object to compare to.
	 * @return true if the two objects are equal, false otherwise.
	 */
	bool operator == (const SVector2Df& vec);

	/**
	 * Compares two SVector2Df objects for inequality.
	 *
	 * @param vec The SVector2Df object to compare to.
	 * @return true if the two objects are not equal, false otherwise.
	 */
	bool operator != (const SVector2Df& vec);

	/**
	 * Provides a const pointer to the underlying float array.
	 *
	 * This allows direct access to the x and y components as a float array.
	 *
	 * @return A const pointer to the float array.
	 */
	operator const float* () const;

	/**
	 * Prints the x and y components of the SVector2Df object to the console.
	 *
	 * @param bEndl If true, a newline character is printed after the values.
	 */
	void print(bool bEndl = true) const;

	/**
	 * Calculates the length (magnitude) of the SVector2Df object.
	 *
	 * @return The length of the vector.
	 */
	float length() const;

	/**
	 * Normalizes the SVector2Df object, making its length 1.
	 *
	 * @return A reference to this modified SVector2Df object.
	 * @throws std::domain_error If the vector is zero.
	 */
	SVector2Df& normalize();

	/**
	 * Returns a pointer to the underlying float array.
	 *
	 * This allows direct access to the x and y components as a float array.
	 *
	 * @return A pointer to the float array.
	 */
	float* data();

	/**
	 * Checks if both components of the SVector2Df object are zero.
	 *
	 * @return true if both components are zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Sets both components of the SVector2Df object to the specified value.
	 *
	 * @param fVal The value to set both components to.
	 */
	void SetAll(float fVal);

	/**
	 * Sets both components of the SVector2Df object to zero.
	 */
	void SetToZero();

	/**
	 * Converts the SVector2Df object to a glm::vec2 object.
	 *
	 * @return The equivalent glm::vec2 object.
	 */
	glm::vec2 ToGLM() const;
};

/**
 * Adds a scalar value to both components of an SVector2Df object.
 *
 * @param vec The SVector2Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator+(const SVector2Df& vec, float fVal);

/**
 * Subtracts a scalar value from both components of an SVector2Df object.
 *
 * @param vec The SVector2Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator-(const SVector2Df& vec, float fVal);

/**
 * Multiplies both components of an SVector2Df object by a scalar value.
 *
 * @param vec The SVector2Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector2Df object.
 */
SVector2Df operator*(const SVector2Df& vec, float fVal);

/**
 * Divides both components of an SVector2Df object by a scalar value.
 *
 * @param vec The SVector2Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector2Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector2Df operator/(const SVector2Df& vec, float fVal);

/**
 * SVector3Di: A 3D integer vector struct.
 *
 * This struct represents a 3D vector with integer components. It provides
 * basic vector operations such as addition, subtraction, scalar multiplication,
 * dot product, length calculation, and normalization.
 *
 * Key features:
 * - Efficient integer-based operations
 * - Accurate length and normalization calculations
 * - Support for dot product
 * - Conversion to and from GLM vectors for interoperability
 * - Clear and concise implementation adhering to Betty coding standards
 */
struct SVector3Di
{
	union {
		int x;
		int r;
	};

	union {
		int y;
		int g;
	};

	union {
		int z;
		int b;
	};

	/**
	 * Default constructor, initializes all components to zero.
	 */
	SVector3Di() = default;

	/**
	 * Constructs an SVector3Di object with all components initialized to the same value.
	 *
	 * @param iVal The initial value for all components.
	 */
	SVector3Di(int iVal);

	/**
	 * Constructs an SVector3Di object with specified x, y, and z components.
	 *
	 * @param _x The initial value for the x component.
	 * @param _y The initial value for the y component.
	 * @param _z The initial value for the z component.  
	 */
	SVector3Di(int _x, int _y, int _z);

	/**
	 * Constructs an SVector3Di object by copying the components from an integer array.
	 *
	 * @param pVec A pointer to the integer array containing the x, y, and z components.
	 */
	SVector3Di(const int* pVec);

	/**
	 * Constructs an SVector3Di object by copying the x and y components from an SVector2Di object,
	 * setting the z component to 0.
	 *
	 * @param vec The SVector2Di object to copy from.
	 */
	SVector3Di(const SVector2Di& vec);

	/**
	 * Constructs an SVector3Di object by copying the components from a glm::ivec3 vector.
	 *
	 * @param vec The glm::ivec3 vector to copy from.
	 */
	SVector3Di(const glm::ivec3& vec);

	/**
	 * Adds two SVector3Di objects component-wise.
	 *
	 * @param vec The SVector3Di object to add.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator+(const SVector3Di& vec);

	/**
	 * Subtracts one SVector3Di object from another component-wise.
	 *
	 * @param vec The SVector3Di object to subtract.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator-(const SVector3Di& vec);

	/**
	 * Multiplies two SVector3Di objects component-wise.
	 *
	 * @param vec The SVector3Di object to multiply.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator*(const SVector3Di& vec);

	/**
	 * Divides one SVector3Di object by another component-wise.
	 *
	 * @param vec The SVector3Di object to divide by.
	 * @return The resulting SVector3Di object.
	 * @throws std::domain_error If either component of the divisor is zero.
	 */
	SVector3Di operator/(const SVector3Di& vec);

	/**
	 * Adds a scalar value to all components of an SVector3Di object.
	 *
	 * @param iVal The scalar value to add.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator+(const int iVal);

	/**
	 * Subtracts a scalar value from all components of an SVector3Di object.
	 *
	 * @param iVal The scalar value to subtract.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator-(const int iVal);

	/**
	 * Multiplies all components of an SVector3Di object by a scalar value.
	 *
	 * @param iVal The scalar value to multiply by.
	 * @return The resulting SVector3Di object.
	 */
	SVector3Di operator*(const int iVal);

	/**
	 * Divides all components of an SVector3Di object by a scalar value.
	 *
	 * @param iVal The scalar value to divide by.
	 * @return The resulting SVector3Di object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector3Di operator/(const int iVal);

	/**
	 * Adds another SVector3Di object to this one, modifying this object in-place.
	 *
	 * @param vec The SVector3Di object to add.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator+=(const SVector3Di& vec);

	/**
	 * Subtracts another SVector3Di object from this one, modifying this object in-place.
	 *
	 * @param vec The SVector3Di object to subtract.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator-=(const SVector3Di& vec);

	/**
	 * Multiplies this SVector3Di object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector3Di object to multiply by.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator*=(const SVector3Di& vec);

	/**
	 * Divides this SVector3Di object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector3Di object to divide by.
	 * @return A reference to this modified SVector3Di object.
	 * @throws std::domain_error If any component of the divisor is zero.
	 */
	SVector3Di& operator/=(const SVector3Di& vec);

	/**
	 * Adds a scalar value to all components of this SVector3Di object, modifying it in-place.
	 *
	 * @param iVal The scalar value to add.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator+=(const int iVal);

	/**
	 * Subtracts a scalar value from all components of this SVector3Di object, modifying it in-place.
	 *
	 * @param iVal The scalar value to subtract.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator-=(const int iVal);

	/**
	 * Multiplies all components of this SVector3Di object by a scalar value, modifying it in-place.
	 *
	 * @param iVal The scalar value to multiply by.
	 * @return A reference to this modified SVector3Di object.
	 */
	SVector3Di& operator*=(const int iVal);

	/**
	 * Divides all components of this SVector3Di object by a scalar value, modifying it in-place.
	 *
	 * @param iVal The scalar value to divide by.
	 * @return A reference to this modified SVector3Di object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector3Di& operator/=(const int iVal);

	/**
	 * Compares two SVector3Di objects for equality.
	 *
	 * @param vec The SVector3Di object to compare to.
	 * @return true if the two objects are equal, false otherwise.
	 */
	bool operator == (const SVector3Di& vec);

	/**
	 * Compares two SVector3Di objects for inequality.
	 *
	 * @param vec The SVector3Di object to compare to.
	 * @return true if the two objects are not equal, false otherwise.
	 */
	bool operator != (const SVector3Di& vec);

	/**
	 * Provides a const pointer to the underlying integer array.
	 *
	 * This allows direct access to the x, y, and z components as an integer array.
	 *
	 * @return A const pointer to the integer array.
	 */
	operator const int* () const;

	/**
	 * Prints the x, y, and z components of the SVector3Di object to the console.
	 *
	 * @param bEndl If true, a newline character is printed after the values.
	 */
	void print(bool bEndl = true) const;

	/**
	 * Calculates the length (magnitude) of the SVector3Di object.
	 *
	 * @return The length of the vector.
	 */
	int length() const;

	/**
	 * Normalizes the SVector3Di object, making its length 1.
	 *
	 * @return A reference to this modified SVector3Di object.
	 * @throws std::domain_error If the vector is zero.
	 */
	SVector3Di& normalize();

	/**
	 * Calculates the dot product of two SVector3Di objects.
	 *
	 * @param vec The SVector3Di object to calculate the dot product with.
	 * @return The dot product of the two vectors.
	 */
	int dot(const SVector3Di& vec) const;

	/**
	 * Calculates the cross product of two SVector3Di objects.
	 *
	 * @param vec The SVector3Di object to calculate the cross product with.
	 * @return The cross product of the two vectors.
	 */
	SVector3Di cross(const SVector3Di& vec) const;

	/**
	 * Calculates the Euclidean distance between two SVector3Di objects.
	 *
	 * @param vec The SVector3Di object to calculate the distance to.
	 * @return The distance between the two vectors.
	 */
	int distance(const SVector3Di& vec) const;

	/**
	 * Negates all components of the SVector3Di object.
	 *
	 * @return The negated SVector3Di object.
	 */
	SVector3Di negate() const;

	/**
	 * Returns a pointer to the underlying integer array.
	 *
	 * This allows direct access to the x, y, and z components as an integer array.
	 *
	 * @return A pointer to the integer array.
	 */
	int* data();

	/**
	 * Initializes the components of the SVector3Di object with random values within specified ranges.
	 *
	 * @param minVal The minimum value for each component.
	 * @param maxVal The maximum value for each component.
	 */
	void InitRandom(const SVector3Di& minVal, const SVector3Di& maxVal);

	/**
	 * Initializes the components of the SVector3Di object with random values within a specified range.
	 *
	 * @param minVal The minimum value for each component.
	 * @param maxVal The maximum value for each component.
	 */
	void InitRandom(int minVal, int maxVal);

	/**
	 * Initializes the SVector3Di object using spherical coordinates.
	 *
	 * @param Radius The radius of the sphere.
	 * @param Pitch The pitch angle in degrees.
	 * @param Heading The heading angle in degrees.
	 */
	void InitBySphericalCoords(int Radius, int Pitch, int Heading);

	/**
	 * Checks if all components of the SVector3Di object are zero.
	 *
	 * @return true if all components are zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Sets all components of the SVector3Di object to the specified value.
	 *
	 * @param iVal The value to set all components to.
	 */
	void SetAll(int iVal);

	/**
	 * Sets all components of the SVector3Di object to the specified value.
	 *
	 * @param iVal The value to set all components to.
	 */
	void SetToZero();

	/**
	 * Converts the SVector3Di object to a glm::ivec3 object.
	 *
	 * @return The equivalent glm::ivec3 object.
	 */
	glm::ivec3 ToGLM() const;
};

/**
 * Adds a scalar value to all components of an SVector3Di object.
 *
 * @param vec The SVector3Di object to add to.
 * @param iVal The scalar value to add.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator+(const SVector3Di& vec, int iVal);

/**
 * Subtracts a scalar value from all components of an SVector3Di object.
 *
 * @param vec The SVector3Di object to subtract from.
 * @param iVal The scalar value to subtract.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator-(const SVector3Di& vec, int iVal);

/**
 * Multiplies all components of an SVector3Di object by a scalar value.
 *
 * @param vec The SVector3Di object to multiply.
 * @param iVal The scalar value to multiply by.
 * @return The resulting SVector3Di object.
 */
SVector3Di operator*(const SVector3Di& vec, int iVal);

/**
 * Divides all components of an SVector3Di object by a scalar value.
 *
 * @param vec The SVector3Di object to divide.
 * @param iVal The scalar value to divide by.
 * @return The resulting SVector3Di object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Di operator/(const SVector3Di& vec, int iVal);

struct SVector4Df;

/**
 * SVector3Df: A 3D float vector struct.
 *
 * This struct represents a 3D vector with float components. It provides
 * basic vector operations such as addition, subtraction, scalar multiplication,
 * dot product, length calculation, and normalization.
 *
 * Key features:
 * - Efficient float-based operations
 * - Accurate length and normalization calculations
 * - Support for dot product
 * - Conversion to and from GLM vectors for interoperability
 * - Clear and concise implementation adhering to Betty coding standards
 */
struct SVector3Df
{
	union {
		float x;
		float r;
	};

	union {
		float y;
		float g;
	};

	union {
		float z;
		float b;
	};

	/**
	 * Default constructor, initializes all components to zero.
	 */
	SVector3Df() = default;

	/**
	 * Constructs an SVector3Df object with all components set to the same value.
	 *
	 * @param fVal The value to set all components to.
	 */
	SVector3Df(const float fVal);

	/**
	 * Constructs an SVector3Df object with specified values for each component.
	 *
	 * @param _x The value for the x component.
	 * @param _y The value for the y component.
	 * @param _z The value for the z component.  
	 */
	SVector3Df(const float _x, const float _y, const float _z);

	/**
	 * Constructs an SVector3Df object from a std::array of floats, with optional safety check.
	 *
	 * @param arr The std::array of floats to construct from.
	 * @param bSafety If true, performs a bounds check on the array indices (throws exception for out-of-bounds access).
	 */
	SVector3Df(const std::array<float, 3>& arr, bool bSafety = false);

	/**
	 * Constructs an SVector3Df object from a pointer to a float array.
	 *
	 * @param pVec The pointer to the float array.
	 * @throws std::runtime_error If the pointer is null.
	 */
	SVector3Df(const float* pVec);

	/**
	 * Constructs an SVector3Df object from an SVector2Df object, setting the z component to zero.
	 *
	 * @param vec The SVector2Df object to construct from.
	 */
	SVector3Df(const SVector2Df& vec);

	/**
	 * Constructs an SVector3Df object from a glm::vec3 object.
	 *
	 * @param vec The glm::vec3 object to construct from.
	 */
	SVector3Df(const glm::vec3& vec);

	/**
	 * Constructs an SVector3Df object from a SVector3Df object.
	 *
	 * @param vec The glm::vec3 object to construct from.
	 */
	SVector3Df(const SVector3Df& vec);

	/**
	 * Constructs an SVector3Df object from a SVector4Df object.
	 *
	 * @param vec The glm::vec3 object to construct from.
	 */
	SVector3Df(const SVector4Df& vec);

	/**
	 * Constructs an SVector3Df object from a glm::vec4 object.
	 *
	 * @param vec The glm::vec3 object to construct from.
	 */
	SVector3Df(const glm::vec4& vec);

	/**
	 * Adds two SVector3Df objects component-wise.
	 *
	 * @param vec The SVector3Df object to add.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator+(const SVector3Df& vec);

	/**
	 * Subtracts one SVector3Df object from another component-wise.
	 *
	 * @param vec The SVector3Df object to subtract.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator-(const SVector3Df& vec);

	/**
	 * Multiplies two SVector3Df objects component-wise.
	 *
	 * @param vec The SVector3Df object to multiply.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator*(const SVector3Df& vec);

	/**
	 * Divides one SVector3Df object by another component-wise.
	 *
	 * @param vec The SVector3Df object to divide by.
	 * @return The resulting SVector3Df object.
	 * @throws std::domain_error If any component of the divisor is zero.
	 */
	SVector3Df operator/(const SVector3Df& vec);

	/**
	 * Adds a scalar value to all components of an SVector3Df object.
	 *
	 * @param fVal The scalar value to add.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator+(const float fVal);

	/**
	 * Subtracts a scalar value from all components of an SVector3Df object.
	 *
	 * @param fVal The scalar value to subtract.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator-(const float fVal);

	/**
	 * Subtracts a scalar value from all components of an SVector3Df object.
	 *
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator-() const;

	/**
	 * Multiplies all components of an SVector3Df object by a scalar value.
	 *
	 * @param fVal The scalar value to multiply by.
	 * @return The resulting SVector3Df object.
	 */
	SVector3Df operator*(const float fVal);

	/**
	 * Divides all components of an SVector3Df object by a scalar value.
	 *
	 * @param fVal The scalar value to divide by.
	 * @return The resulting SVector3Df object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector3Df operator/(const float fVal);

	/**
	 * Adds another SVector3Df object to this one, modifying this object in-place.
	 *
	 * @param vec The SVector3Df object to add.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator+=(const SVector3Df& vec);

	/**
	 * Subtracts another SVector3Df object from this one, modifying this object in-place.
	 *
	 * @param vec The SVector3Df object to subtract.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator-=(const SVector3Df& vec);

	/**
	 * Multiplies this SVector3Df object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector3Df object to multiply by.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator*=(const SVector3Df& vec);

	/**
	 * Divides this SVector3Df object by another one, modifying this object in-place.
	 *
	 * @param vec The SVector3Df object to divide by.
	 * @return A reference to this modified SVector3Df object.
	 * @throws std::domain_error If any component of the divisor is zero.
	 */
	SVector3Df& operator/=(const SVector3Df& vec);

	/**
	 * Adds a scalar value to all components of this SVector3Df object, modifying it in-place.
	 *
	 * @param fVal The scalar value to add.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator+=(const float fVal);

	/**
	 * Subtracts a scalar value from all components of this SVector3Df object, modifying it in-place.
	 *
	 * @param fVal The scalar value to subtract.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator-=(const float fVal);

	/**
	 * Multiplies all components of this SVector3Df object by a scalar value, modifying it in-place.
	 *
	 * @param fVal The scalar value to multiply by.
	 * @return A reference to this modified SVector3Df object.
	 */
	SVector3Df& operator*=(const float fVal);

	/**
	 * Divides all components of this SVector3Df object by a scalar value, modifying it in-place.
	 *
	 * @param fVal The scalar value to divide by.
	 * @return A reference to this modified SVector3Df object.
	 * @throws std::domain_error If the divisor is zero.
	 */
	SVector3Df& operator/=(const float fVal);

	/**
	 * Compares two SVector3Df objects for equality.
	 *
	 * @param vec The SVector3Df object to compare to.
	 * @return true if the two objects are equal, false otherwise.
	 */
	bool operator == (const SVector3Df& vec);

	/**
	 * Compares two SVector3Df objects for inequality.
	 *
	 * @param vec The SVector3Df object to compare to.
	 * @return true if the two objects are not equal, false otherwise.
	 */
	bool operator != (const SVector3Df& vec);

	/**
	 * Provides a const pointer to the underlying float array.
	 *
	 * This allows direct access to the x, y, and z components as a float array.
	 *
	 * @return A const pointer to the float array.
	 */
	operator const float* () const;

	/**
	 * Prints the x, y, and z components of the SVector3Df object to the console.
	 *
	 * @param bEndl If true, a newline character is printed after the values.
	 */
	void print(bool bEndl = true) const;

	/**
	 * Calculates the length (magnitude) of the SVector3Df object.
	 *
	 * @return The length of the vector.
	 */
	float length() const;

	/**
	 * Normalizes the SVector3Df object, making its length 1.
	 *
	 * @return A reference to this modified SVector3Df object.
	 * @throws std::domain_error If the vector is zero.
	 */
	SVector3Df& normalize();

	/**
	 * Check if the vector can be normalized
	 *
	 * @return true if the Vector can be normalized, otherwise false
	 */
	bool CanNormalize() const;


	/**
	 * Calculates the dot product of two SVector3Df objects.
	 *
	 * @param vec The SVector3Df object to calculate the dot product with.
	 * @return The dot product of the two vectors.
	 */
	float dot(const SVector3Df& vec) const;

	/**
	 * Calculates the cross product of two SVector3Df objects.
	 *
	 * @param vec The SVector3Df object to calculate the cross product with.
	 * @return The cross product of the two vectors.
	 */
	SVector3Df cross(const SVector3Df& vec) const;

	/**
	 * Rotates the vector using quaternion by giving angle and vector.
	 *
	 * @param fAngle The Angle of Rotation.
	 * @param vec The vector to rotate around, could represent an Axis
	 * @return The cross product of the two vectors.
	 */
	void rotate(const float fAngle, const SVector3Df& vec);

	/**
	 * Calculates the Euclidean distance between two SVector3Df objects.
	 *
	 * @param vec The SVector3Df object to calculate the distance to.
	 * @return The distance between the two vectors.
	 */
	float distance(const SVector3Df& vec) const;

	/**
	 * Calculates the Angle between two SVector3Df objects.
	 *
	 * @param vec The SVector3Df object to calculate the angle with in Radian.
	 * @return The angle between the two vectors.
	 */
	float angle(const SVector3Df& vec) const;

	/**
	 * Negates all components of the SVector3Df object.
	 *
	 * @return The negated SVector3Df object.
	 */
	SVector3Df negate() const;

	/**
	 * Returns a pointer to the underlying float array.
	 *
	 * This allows direct access to the x, y, and z components as a float array.
	 *
	 * @return A pointer to the float array.
	 */
	float* data();

	/**
	 * Initializes the components of the SVector3Df object with random values within specified ranges.
	 *
	 * @param minVal The minimum value for each component.
	 * @param maxVal The maximum value for each component.
	 */
	void InitRandom(const SVector3Df& minVal, const SVector3Df& maxVal);

	/**
	 * Initializes the components of the SVector3Df object with random values within a specified range.
	 *
	 * @param minVal The minimum value for each component.
	 * @param maxVal The maximum value for each component.
	 */
	void InitRandom(float minVal, float maxVal);

	/**
	 * Initializes the SVector3Df object using spherical coordinates.
	 *
	 * @param Radius The radius of the sphere.
	 * @param Pitch The pitch angle in degrees.
	 * @param Heading The heading angle in degrees.
	 */
	void InitBySphericalCoords(float Radius, float Pitch, float Heading);

	/**
	 * Checks if all components of the SVector3Df object are zero.
	 *
	 * @return true if all components are zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Sets all components of the SVector3Df object to the specified value.
	 *
	 * @param fVal The value to set all components to.
	 */
	void SetAll(float fVal);

	/**
	 * Sets all components of the SVector3Df object to zero.
	 */
	void SetToZero();

	/**
	 * Converts the SVector3Df object to a glm::vec3 object.
	 *
	 * @return The equivalent glm::vec3 object.
	 */
	glm::vec3 ToGLM() const;
};

/**
 * Adds a scalar value to all components of an SVector3Df object.
 *
 * @param vec The SVector3Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator+(const SVector3Df& vec, float fVal);

/**
 * Subtracts a scalar value from all components of an SVector3Df object.
 *
 * @param vec The SVector3Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator-(const SVector3Df& vec, float fVal);

/**
 * Multiplies all components of an SVector3Df object by a scalar value.
 *
 * @param vec The SVector3Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator*(const SVector3Df& vec, float fVal);

/**
 * Divides all components of an SVector3Df object by a scalar value.
 *
 * @param vec The SVector3Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector3Df operator/(const SVector3Df& vec, float fVal);

/**
 * Adds two SVector3Df objects component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator+(const SVector3Df& vec1, const SVector3Df& vec2);

/**
 * Subtracts one SVector3Df object from another component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to subtract.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator-(const SVector3Df& vec1, const SVector3Df& vec2);

/**
 * Multiplies two SVector3Df objects component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to multiply.
 * @return The resulting SVector3Df object.
 */
SVector3Df operator*(const SVector3Df& vec1, const SVector3Df& vec2);

/**
 * Divides one SVector3Df object by another component-wise.
 *
 * @param vec1 The first SVector3Df object.
 * @param vec2 The second SVector3Df object to divide by.
 * @return The resulting SVector3Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector3Df operator/(const SVector3Df& vec1, const SVector3Df& vec2);

/**
 * SVector4Df: A 4D float vector struct.
 *
 * This struct represents a 4D vector with float components. It provides
 * basic vector operations such as addition, subtraction, scalar multiplication,
 * dot product, length calculation, and normalization.
 *
 * Key features:
 * - Efficient float-based operations
 * - Accurate length and normalization calculations
 * - Support for dot product
 * - Conversion to and from GLM vectors for interoperability
 * - Clear and concise implementation adhering to Betty coding standards
 */
struct SVector4Df
{
	union {
		float x;
		float r;
	};

	union {
		float y;
		float g;
	};

	union {
		float z;
		float b;
	};

	union {
		float w;
		float a;
	};

	/**
	 * Default constructor, initializes all components to zero.
	 */
	SVector4Df() = default;

	/**
	 * SVector4Df constructor that initializes all components to a single value.
	 *
	 * @param fVal The value to initialize all components to.
	 */
	SVector4Df(const float fVal);

	/**
	 * SVector4Df constructor that initializes each component with a separate value.
	 *
	 * @param _x The value for the x component.
	 * @param _y The value for the y component.
	 * @param _z The value for the z component.
	 * @param _w The value for the w component. (scalar)
	 */
	SVector4Df(const float _x, const float _y, const float _z, const float _w);

	/**
	 * SVector4Df constructor that initializes from a std::array, with optional safety checks.
	 *
	 * If `bSafety` is true, the constructor uses `arr.at(i)` for bounds checking.
	 * Otherwise, it assumes valid indexing and uses `arr[i]`.
	 *
	 * @param arr The std::array to copy from.
	 * @param bSafety Flag to enable bounds checking.
	 */
	SVector4Df(const std::array<float, 4>& arr, bool bSafety = false);

	/**
	 * SVector4Df constructor that initializes from a raw float pointer.
	 *
	 * If the pointer is null, all components are set to zero.
	 *
	 * @param pVec The pointer to a float array (length 4).
	 */
	SVector4Df(const float* pVec);

	/**
	 * SVector4Df constructor that initializes from an SVector2Df and two additional values.
	 *
	 * @param vec The SVector2Df object to copy from.
	 * @param _z The value for the z component.
	 * @param _w The value for the w component.
	 */
	SVector4Df(const SVector2Df& vec, const float _z, const float _w);

	/**
	 * SVector4Df constructor that initializes from an SVector3Df and one additional value.
	 *
	 * @param vec The SVector3Df object to copy from.
	 * @param _w The value for the w component.
	 */
	SVector4Df(const SVector3Df& vec, const float _w);

	/**
	 * SVector4Df constructor that initializes from a glm::vec3 and a scalar value.
	 *
	 * @param vec The glm::vec3 object to copy from.
	 * @param _w The value for the w component.
	 */
	SVector4Df(const glm::vec3& vec, const float _w);

	/**
	 * SVector4Df copy constructor.
	 *
	 * @param vec The SVector4Df object to copy.
	 */
	SVector4Df(const SVector4Df& vec);

	/**
	 * SVector4Df constructor that initializes from a glm::vec4 object.
	 *
	 * @param vec The glm::vec4 object to copy from.
	 */
	SVector4Df(const glm::vec4& vec);


	/**
	 * Compares two SVector4Df objects for equality.
	 *
	 * @param vec The SVector4Df object to compare to.
	 * @return true if the two objects are equal, false otherwise.
	 */
	bool operator == (const SVector4Df& vec);

	/**
	 * Compares two SVector4Df objects for inequality.
	 *
	 * @param vec The SVector4Df object to compare to.
	 * @return true if the two objects are not equal, false otherwise.
	 */
	bool operator != (const SVector4Df& vec);

	float& operator[](size_t index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Invalid index");
		}
	}

	const float& operator[](size_t index) const {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Invalid index");
		}
	}


	/**
	 * Provides a const pointer to the underlying float array.
	 *
	 * This allows direct access to the x, y, z, and w components as a float array.
	 *
	 * @return A const pointer to the float array.
	 */
	operator const float* () const;

	/**
	 * Prints the x, y, z, and w components of the SVector4Df object to the console.
	 *
	 * @param bEndl If true, a newline character is printed after the values.
	 */
	void print(bool bEndl = true) const;

	/**
	 * Calculates the length (magnitude) of the SVector4Df object.
	 *
	 * @return The length of the vector.
	 */
	float length() const;

	/**
	 * Normalizes the SVector4Df object, making its length 1.
	 *
	 * @return A reference to this modified SVector4Df object.
	 * @throws std::domain_error If the vector is zero.
	 */
	SVector4Df& normalize();

	/**
	 * Calculates the dot product of two SVector4Df objects.
	 *
	 * @param vec The SVector4Df object to calculate the dot product with.
	 * @return The dot product of the two vectors.
	 */
	float dot(const SVector4Df& vec) const;


	/**
	 * Returns a pointer to the underlying float array.
	 *
	 * This allows direct access to the x, y, z, and w components as a float array.
	 *
	 * @return A pointer to the float array.
	 */
	float* data();

	/**
	 * Checks if all components of the SVector4Df object are zero.
	 *
	 * @return true if all components are zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Sets all components of the SVector4Df object to the specified value.
	 *
	 * @param fVal The value to set all components to.
	 */
	void SetAll(const float fVal);

	/**
	 * Sets all components of the SVector4Df object to zero.
	 */
	void SetToZero();

	/**
	 * Converts the SVector4Df object to a glm::vec4 object.
	 *
	 * @return The equivalent glm::vec4 object.
	 */
	glm::vec4 ToGLM() const;

	/**
	 * Converts the SVector4Df object to an SVector3Df object by discarding the w component.
	 *
	 * @return The equivalent SVector3Df object.
	 */
	SVector3Df To3D() const;
};

/**
 * Adds a scalar value to all components of an SVector4Df object.
 *
 * @param vec The SVector4Df object to add to.
 * @param fVal The scalar value to add.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator+(const SVector4Df& vec, float fVal);

/**
 * Subtracts a scalar value from all components of an SVector4Df object.
 *
 * @param vec The SVector4Df object to subtract from.
 * @param fVal The scalar value to subtract.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator-(const SVector4Df& vec, float fVal);

/**
 * Multiplies all components of an SVector4Df object by a scalar value.
 *
 * @param vec The SVector4Df object to multiply.
 * @param fVal The scalar value to multiply by.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator*(const SVector4Df& vec, float fVal);

/**
 * Divides all components of an SVector4Df object by a scalar value.
 *
 * @param vec The SVector4Df object to divide.
 * @param fVal The scalar value to divide by.
 * @return The resulting SVector4Df object.
 * @throws std::domain_error If the divisor is zero.
 */
SVector4Df operator/(const SVector4Df& vec, float fVal);

/**
 * Adds two SVector4Df objects component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator+(const SVector4Df& vec1, const SVector4Df& vec2);

/**
 * Subtracts one SVector4Df object from another component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to subtract.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator-(const SVector4Df& vec1, const SVector4Df& vec2);

/**
 * Multiplies two SVector4Df objects component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to multiply.
 * @return The resulting SVector4Df object.
 */
SVector4Df operator*(const SVector4Df& vec1, const SVector4Df& vec2);

/**
 * Divides one SVector4Df object by another component-wise.
 *
 * @param vec1 The first SVector4Df object.
 * @param vec2 The second SVector4Df object to divide by.
 * @return The resulting SVector4Df object.
 * @throws std::domain_error If any component of the divisor is zero.
 */
SVector4Df operator/(const SVector4Df& vec1, const SVector4Df& vec2);
