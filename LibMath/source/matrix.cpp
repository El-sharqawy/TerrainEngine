#include "stdafx.h"
#include "matrix.h"
#include <cmath>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext.hpp>

CMatrix4Df::CMatrix4Df(const CMatrix3Df& AssimpMatrix)
{
	mat4[0][0] = AssimpMatrix.mat3[0][0]; mat4[0][1] = AssimpMatrix.mat3[0][1]; mat4[0][2] = AssimpMatrix.mat3[0][3]; mat4[0][3] = 0.0f;
	mat4[1][0] = AssimpMatrix.mat3[1][0]; mat4[1][1] = AssimpMatrix.mat3[1][1]; mat4[1][2] = AssimpMatrix.mat3[1][3]; mat4[1][3] = 0.0f;
	mat4[2][0] = AssimpMatrix.mat3[2][0]; mat4[2][1] = AssimpMatrix.mat3[2][1]; mat4[2][2] = AssimpMatrix.mat3[2][3]; mat4[2][3] = 0.0f;
	mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
}

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
void CMatrix4Df::InitRotationX(const float fRotX, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around X Axis */
	if (bLeftHanded)
	{
		mat4[0][0] = 1.0f; mat4[0][1] = 0.0f; mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
		mat4[1][0] = 0.0f; mat4[1][1] = std::cosf(fRotX); mat4[1][2] = std::sinf(fRotX); mat4[1][3] = 0.0f;
		mat4[2][0] = 0.0f; mat4[2][1] = -std::sinf(fRotX); mat4[2][2] = std::cosf(fRotX); mat4[2][3] = 0.0f;
		mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
		return;
	}

	mat4[0][0] = 1.0f; mat4[0][1] = 0.0f; mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
	mat4[1][0] = 0.0f; mat4[1][1] = std::cosf(fRotX); mat4[1][2] = -std::sinf(fRotX); mat4[1][3] = 0.0f;
	mat4[2][0] = 0.0f; mat4[2][1] = std::sinf(fRotX); mat4[2][2] = std::cosf(fRotX); mat4[2][3] = 0.0f;
	mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
}

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
void CMatrix4Df::InitRotationY(const float fRotY, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around Y Axis */
	if (bLeftHanded)
	{
		mat4[0][0] = std::cosf(fRotY); mat4[0][1] = 0.0f; mat4[0][2] = -std::sinf(fRotY); mat4[0][3] = 0.0f;
		mat4[1][0] = 0.0f; mat4[1][1] = 1.0f; mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
		mat4[2][0] = std::sinf(fRotY); mat4[2][1] = 0.0f; mat4[2][2] = std::cosf(fRotY); mat4[2][3] = 0.0f;
		mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
		return;
	}

	mat4[0][0] = std::cosf(fRotY); mat4[0][1] = 0.0f; mat4[0][2] = std::sinf(fRotY); mat4[0][3] = 0.0f;
	mat4[1][0] = 0.0f; mat4[1][1] = 1.0f; mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
	mat4[2][0] = -std::sinf(fRotY); mat4[2][1] = 0.0f; mat4[2][2] = std::cosf(fRotY); mat4[2][3] = 0.0f;
	mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
}

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
void CMatrix4Df::InitRotationZ(const float fRotZ, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around Z Axis */
	if (bLeftHanded)
	{
		mat4[0][0] = std::cosf(fRotZ); mat4[0][1] = std::sinf(fRotZ); mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
		mat4[1][0] = -std::sinf(fRotZ); mat4[1][1] = std::cosf(fRotZ); mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
		mat4[2][0] = 0.0f; mat4[2][1] = 0.0f; mat4[2][2] = 1.0f; mat4[2][3] = 0.0f;
		mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
		return;
	}

	mat4[0][0] = std::cosf(fRotZ); mat4[0][1] = -std::sinf(fRotZ); mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
	mat4[1][0] = std::sinf(fRotZ); mat4[1][1] = std::cosf(fRotZ); mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
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
void CMatrix4Df::InitScaleTransform(const float fScaleX, const float fScaleY, const float fScaleZ)
{
	mat4[0][0] = fScaleX; mat4[0][1] = 0.0f; mat4[0][2] = 0.0f; mat4[0][3] = 0.0f;
	mat4[1][0] = 0.0f; mat4[1][1] = fScaleY; mat4[1][2] = 0.0f; mat4[1][3] = 0.0f;
	mat4[2][0] = 0.0f; mat4[2][1] = 0.0f; mat4[2][2] = fScaleZ; mat4[2][3] = 0.0f;
	mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
}

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
void CMatrix4Df::InitScaleTransform(const float fScale)
{
	InitScaleTransform(fScale, fScale, fScale);
}

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
void CMatrix4Df::InitScaleTransform(const SVector3Df& vScale)
{
	InitScaleTransform(vScale.x, vScale.y, vScale.z);
}

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
void CMatrix4Df::InitScaleTransform(const glm::vec3& vScale)
{
	InitScaleTransform(vScale.x, vScale.y, vScale.z);
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
void CMatrix4Df::InitRotateTransform(const float fRotateX, const float fRotateY, const float fRotateZ)
{
	CMatrix4Df matX{}, matY{}, matZ{};

	const float x = ToRadian(fRotateX);
	const float y = ToRadian(fRotateY);
	const float z = ToRadian(fRotateZ);

	matX.InitRotationX(x);
	matY.InitRotationY(y);
	matZ.InitRotationZ(z);
	
	/* ZYX Euler rotation sequence */
	*this = matZ * matY * matX;
}

/**
 * Initializes the matrix for a rotation transformation using the XYZ Euler rotation sequence.
 *
 * @param RotateX: The angle of rotation around the X-axis in degrees.
 * @param RotateY: The angle of rotation around the Y-axis in degrees.
 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
 *
 * This function creates individual rotation matrices for the X, Y, and Z axes
 * and combines them in the order rx * ry * rz. This order applies the
 * rotations in the sequence: Z-axis, Y-axis, then X-axis, relative to the
 * global coordinate system.
 */
void CMatrix4Df::InitRotateTransformZYX(const float fRotateX, const float fRotateY, const float fRotateZ)
{
	CMatrix4Df matX{}, matY{}, matZ{};

	const float x = ToRadian(fRotateX);
	const float y = ToRadian(fRotateY);
	const float z = ToRadian(fRotateZ);

	matX.InitRotationX(x);
	matY.InitRotationY(y);
	matZ.InitRotationZ(z);

	/* XYZ Euler rotation sequence */
	*this = matX * matY * matZ;
}

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
void CMatrix4Df::InitRotateTransform(const SVector3Df& vRotation)
{
	InitRotateTransform(vRotation.x, vRotation.y, vRotation.z);
}

/**
 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
 *
 * @param vRotation: The glm::vec3 holding rotation degrees.
 *
 * This function creates individual rotation matrices for the X, Y, and Z axes
 * and combines them in the order rz * ry * rx. This order applies the
 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
 * local coordinate system.
 */
void CMatrix4Df::InitRotateTransform(const glm::vec3& vRotation)
{
	InitRotateTransform(vRotation.x, vRotation.y, vRotation.z);
}

void CMatrix4Df::InitRotateTransform(const SQuaternion& sQuat)
{
	const float yy2 = 2.0f * sQuat.y * sQuat.y;
	const float xy2 = 2.0f * sQuat.x * sQuat.y;
	const float xz2 = 2.0f * sQuat.x * sQuat.z;
	const float yz2 = 2.0f * sQuat.y * sQuat.z;
	const float zz2 = 2.0f * sQuat.z * sQuat.z;
	const float wz2 = 2.0f * sQuat.w * sQuat.z;
	const float wy2 = 2.0f * sQuat.w * sQuat.y;
	const float wx2 = 2.0f * sQuat.w * sQuat.x;
	const float xx2 = 2.0f * sQuat.x * sQuat.x;

	mat4[0][0] = -yy2 - zz2 + 1.0f;
	mat4[0][1] = xy2 + wz2;
	mat4[0][2] = xz2 - wy2;
	mat4[0][3] = 0.0f;

	mat4[1][0] = xy2 - wz2;
	mat4[1][1] = -xx2 - zz2 + 1.0f;
	mat4[1][2] = yz2 + wx2;

	mat4[2][0] = xz2 + wy2;
	mat4[2][1] = yz2 - wx2;
	mat4[2][2] = -xx2 - yy2 + 1.0f;

	mat4[3][0] = 0.0f;
	mat4[3][1] = 0.0f;
	mat4[3][2] = 0.0f;
	mat4[3][3] = 1.0f;
}

/**
 * Initializes the matrix for a rotation transformation using a quaternion.
 * @param sQuat: The quaternion representing the rotation to be applied from GLM.
 *
 * This function computes a 4x4 rotation matrix based on the given quaternion.
 * Quaternions provide a compact and efficient way to represent 3D rotations,
 * avoiding issues like gimbal lock that occur with Euler angles.
 *
 * The matrix is calculated directly from the quaternion's components:
 * - @quat.x: The X component of the quaternion.
 * - @quat.y: The Y component of the quaternion.
 * - @quat.z: The Z component of the quaternion.
 * - @quat.w: The W (real) component of the quaternion.
 *
 * The resulting matrix applies the rotation described by the quaternion
 * when multiplied by a vector or another matrix.
 *
 * Note: The function assumes the matrix `mat4` is a 4x4 matrix, and it resets
 *       the translation components to zero, leaving the rotation part intact.
 */
void CMatrix4Df::InitRotateTransform(const glm::quat& sQuat)
{
	glm::mat4 mat = glm::mat4_cast(sQuat);

	CMatrix4Df res(mat);

	*this = res;
}

/**
 * Initializes the rotation transformation matrix based on a specified direction.
 * @param vDir: Direction vector to align the transformation with.
 *
 * This function aligns the camera's forward direction (`vDir`) with the
 * specified direction while keeping the up direction constant as (0, 1, 0).
 */
void CMatrix4Df::InitRotationFromDir(const SVector3Df& vDir)
{
	SVector3Df vUp(0.0f, 1.0f, 0.0f);
	InitCameraTransform(vDir, vUp);
}

/**
 * Initializes the rotation transformation matrix based on a specified direction.
 * @param vDir: Direction vector to align the transformation with.
 *
 * This function aligns the camera's forward direction (`vDir`) with the
 * specified direction while keeping the up direction constant as (0, 1, 0).
 */
void CMatrix4Df::InitRotationFromDir(const glm::vec3& vDir)
{
	glm::vec3 vUp(0.0f, 1.0f, 0.0f);
	InitCameraTransform(vDir, vUp);
}

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
void CMatrix4Df::InitTranslationTransform(const float x, const float y, const float z)
{
	mat4[0][0] = 1.0f; mat4[0][1] = 0.0f; mat4[0][2] = 0.0f; mat4[0][3] = x;
	mat4[1][0] = 0.0f; mat4[1][1] = 1.0f; mat4[1][2] = 0.0f; mat4[1][3] = y;
	mat4[2][0] = 0.0f; mat4[2][1] = 0.0f; mat4[2][2] = 1.0f; mat4[2][3] = z;
	mat4[3][0] = 0.0f; mat4[3][1] = 0.0f; mat4[3][2] = 0.0f; mat4[3][3] = 1.0f;
}

/**
 * Initializes the matrix for translation.
 *
 * This function sets up a 4x4 matrix that applies a translation
 * transformation. The resulting matrix moves objects in 3D space along
 * the specified axes.
 *
 * @param vTranslate: The translation offset 3D Vector.
 */
void CMatrix4Df::InitTranslationTransform(const SVector3Df& vTranslate)
{
	InitTranslationTransform(vTranslate.x, vTranslate.y, vTranslate.z);
}

/**
 * Initializes the matrix for translation.
 *
 * This function sets up a 4x4 matrix that applies a translation
 * transformation. The resulting matrix moves objects in 3D space along
 * the specified axes.
 *
 * @param vTranslate: The translation offset 3D Vector GLM.
 */
void CMatrix4Df::InitTranslationTransform(const glm::vec3& vTranslate)
{
	InitTranslationTransform(vTranslate.x, vTranslate.y, vTranslate.z);
}

/**
 * Initializes the camera transformation matrix
 * @param vTarget: Vector representing the target direction (camera's forward direction)
 * @param vUp: Vector representing the up direction of the camera
 *
 * This function sets up the camera transformation matrix based on
 * the specified target and up direction. The result is a matrix
 * that transforms coordinates into the camera's coordinate space.
 */
void CMatrix4Df::InitCameraTransform(const SVector3Df & vTarget, const SVector3Df & vUp)
{
	SVector3Df N = vTarget;
	N.normalize();

	SVector3Df UpNorm = vUp;
	UpNorm.normalize();

	SVector3Df U{};
	U = UpNorm.cross(N);
	U.normalize();

	SVector3Df V = N.cross(U);

	mat4[0][0] = U.x;
	mat4[0][1] = U.y;
	mat4[0][2] = U.z;
	mat4[0][3] = 0.0f;

	mat4[1][0] = V.x;
	mat4[1][1] = V.y;
	mat4[1][2] = V.z;
	mat4[1][3] = 0.0f;

	mat4[2][0] = N.x;
	mat4[2][1] = N.y;
	mat4[2][2] = N.z;
	mat4[2][3] = 0.0f;

	mat4[3][0] = 0.0f;
	mat4[3][1] = 0.0f;
	mat4[3][2] = 0.0f;
	mat4[3][3] = 1.0f;
}

/**
 * Initializes the camera transformation matrix
 * @param vTarget: Vector representing the target direction (camera's forward direction)
 * @param vUp: Vector representing the up direction of the camera
 *
 * This function sets up the camera transformation matrix based on
 * the specified target and up direction. The result is a matrix
 * that transforms coordinates into the camera's coordinate space.
 */
void CMatrix4Df::InitCameraTransform(const glm::vec3& vTarget, const glm::vec3& vUp)
{
	glm::vec3 N = vTarget;
	glm::normalize(N);

	glm::vec3 UpNorm = vUp;
	glm::normalize(UpNorm);

	glm::vec3 U = glm::cross(UpNorm, N);
	glm::normalize(U);

	glm::vec3 V = glm::cross(N, U);

	mat4[0][0] = U.x;
	mat4[0][1] = U.y;
	mat4[0][2] = U.z;
	mat4[0][3] = 0.0f;

	mat4[1][0] = V.x;
	mat4[1][1] = V.y;
	mat4[1][2] = V.z;
	mat4[1][3] = 0.0f;

	mat4[2][0] = N.x;
	mat4[2][1] = N.y;
	mat4[2][2] = N.z;
	mat4[2][3] = 0.0f;

	mat4[3][0] = 0.0f;
	mat4[3][1] = 0.0f;
	mat4[3][2] = 0.0f;
	mat4[3][3] = 1.0f;
}

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
void CMatrix4Df::InitCameraTransform(const SVector3Df& vPos, const SVector3Df& vTarget, const SVector3Df& vUp)
{
	CMatrix4Df matCameraTranslation{};
	matCameraTranslation.InitTranslationTransform(-vPos.x, -vPos.y, -vPos.z);

	CMatrix4Df matCameraRotationTranslation{};
	matCameraRotationTranslation.InitCameraTransform(vTarget, vUp);

	*this = matCameraRotationTranslation * matCameraTranslation;
}

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
void CMatrix4Df::InitCameraTransform(const glm::vec3& vPos, const glm::vec3& vTarget, const glm::vec3& vUp)
{
	CMatrix4Df matCameraTranslation{};
	matCameraTranslation.InitTranslationTransform(-vPos.x, -vPos.y, -vPos.z);

	CMatrix4Df matCameraRotationTranslation{};
	matCameraRotationTranslation.InitCameraTransform(vTarget, vUp);

	*this = matCameraRotationTranslation * matCameraTranslation;
}

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
void CMatrix4Df::InitPersProjTransform(const SPersProjInfo& sPersProj, const bool bUseGLM, const bool bLeftHanded)
{
	if (bLeftHanded)
	{
		const float fAspectRatio = sPersProj.Height / sPersProj.Width;
		const float fZRange = sPersProj.zNear - sPersProj.zFar;
		const float fTanHalfFOV = std::tanf(ToRadian(sPersProj.FOV / 2.0f));

		mat4[0][0] = 1.0f / fTanHalfFOV;
		mat4[0][1] = 0.0f;
		mat4[0][2] = 0.0f;
		mat4[0][3] = 0.0f;

		mat4[1][0] = 0.0f;
		mat4[1][1] = 1.0f / (fTanHalfFOV * fAspectRatio);
		mat4[1][2] = 0.0f;
		mat4[1][3] = 0.0f;

		mat4[2][0] = 0.0f;
		mat4[2][1] = 0.0f;
		mat4[2][2] = (-sPersProj.zNear - sPersProj.zFar) / fZRange;
		mat4[2][3] = 2.0f * sPersProj.zFar * sPersProj.zNear / fZRange;

		mat4[3][0] = 0.0f;
		mat4[3][1] = 0.0f;
		mat4[3][2] = 1.0f;
		mat4[3][3] = 0.0f;

		if (bUseGLM)
		{
			const glm::mat4 mProjection = glm::perspectiveFovLH(glm::radians(sPersProj.FOV), sPersProj.Width, sPersProj.Height, sPersProj.zNear, sPersProj.zFar);

			mat4[0][0] = mProjection[0][0]; mat4[0][1] = mProjection[0][1]; mat4[0][2] = mProjection[0][2]; mat4[0][3] = mProjection[0][3];
			mat4[1][0] = mProjection[1][0]; mat4[1][1] = mProjection[1][1]; mat4[1][2] = mProjection[1][2]; mat4[1][3] = mProjection[1][3];
			mat4[2][0] = mProjection[2][0]; mat4[2][1] = mProjection[2][1]; mat4[2][2] = mProjection[2][2]; mat4[2][3] = mProjection[2][3];
			mat4[3][0] = mProjection[3][0]; mat4[3][1] = mProjection[3][1]; mat4[3][2] = mProjection[3][2]; mat4[3][3] = mProjection[3][3];
		}

		return;
	}

	const float fAspectRatio = sPersProj.Width / sPersProj.Height;
	const float fZRange = sPersProj.zNear - sPersProj.zFar;
	const float fTanHalfFOV = std::tanf(ToRadian(sPersProj.FOV / 2.0f));

	mat4[0][0] = 1.0f / (fTanHalfFOV * fAspectRatio);
	mat4[0][1] = 0.0f;
	mat4[0][2] = 0.0f;
	mat4[0][3] = 0.0f;

	mat4[1][0] = 0.0f;
	mat4[1][1] = 1.0f / fTanHalfFOV;
	mat4[1][2] = 0.0f;
	mat4[1][3] = 0.0f;

	mat4[2][0] = 0.0f;
	mat4[2][1] = 0.0f;
	mat4[2][2] = (-sPersProj.zNear - sPersProj.zFar) / fZRange;
	mat4[2][3] = 2.0f * sPersProj.zFar * sPersProj.zNear / fZRange;

	mat4[3][0] = 0.0f;
	mat4[3][1] = 0.0f;
	mat4[3][2] = 1.0f; // Correct for right-handed (negative z-axis points into the screen)
	mat4[3][3] = 0.0f;

	if (bUseGLM)
	{
		const glm::mat4 mProjection = glm::perspectiveFovRH(glm::radians(sPersProj.FOV), sPersProj.Width, sPersProj.Height, sPersProj.zNear, sPersProj.zFar);

		mat4[0][0] = mProjection[0][0]; mat4[0][1] = mProjection[0][1]; mat4[0][2] = mProjection[0][2]; mat4[0][3] = mProjection[0][3];
		mat4[1][0] = mProjection[1][0]; mat4[1][1] = mProjection[1][1]; mat4[1][2] = mProjection[1][2]; mat4[1][3] = mProjection[1][3];
		mat4[2][0] = mProjection[2][0]; mat4[2][1] = mProjection[2][1]; mat4[2][2] = mProjection[2][2]; mat4[2][3] = mProjection[2][3];
		mat4[3][0] = mProjection[3][0]; mat4[3][1] = mProjection[3][1]; mat4[3][2] = mProjection[3][2]; mat4[3][3] = mProjection[3][3];

	}
}

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
void CMatrix4Df::InitOrthoProjTransform(const SOrthoProjInfo& sOrthoProj, const bool bLeftHanded)
{
	/* Just Make it shorter, less Ugly */
	const float fL = sOrthoProj.fLeft;
	const float fR = sOrthoProj.fRight;
	const float fB = sOrthoProj.fBottom;
	const float fT = sOrthoProj.fTop;
	const float	fN = sOrthoProj.fNearZ;
	const float fF = sOrthoProj.fFarZ;

	/* Not Sure if it's better to change make this conditions for mat4[2][2] .. for now it is like that. */
	if (bLeftHanded)
	{
		mat4[0][0] = 2.0f / (fR - fL);
		mat4[0][1] = 0.0f;
		mat4[0][2] = 0.0f;
		mat4[0][3] = -(fR + fL) / (fR - fL);

		mat4[1][0] = 0.0f;
		mat4[1][1] = 2.0f / (fT - fB);
		mat4[1][2] = 0.0f;
		mat4[1][3] = -(fT + fB) / (fT - fB);

		mat4[2][0] = 0.0f;
		mat4[2][1] = 0.0f;
		mat4[2][2] = 2.0f / (fF - fN);
		mat4[2][3] = -(fF + fN) / (fF - fN);

		mat4[3][0] = 0.0f;
		mat4[3][1] = 0.0f;
		mat4[3][2] = 0.0f;
		mat4[3][3] = 1.0f;
		return;
	}

	/* Same as bLeftHanded Except mat4[2][2] */
	mat4[0][0] = 2.0f / (fR - fL);
	mat4[0][1] = 0.0f;
	mat4[0][2] = 0.0f;
	mat4[0][3] = -(fR + fL) / (fR - fL);

	mat4[1][0] = 0.0f;
	mat4[1][1] = 2.0f / (fT - fB);
	mat4[1][2] = 0.0f;
	mat4[1][3] = -(fT + fB) / (fT - fB);

	mat4[2][0] = 0.0f;
	mat4[2][1] = 0.0f;
	mat4[2][2] = -2.0f / (fF - fN); // Change This to Minus
	mat4[2][3] = -(fF + fN) / (fF - fN);

	mat4[3][0] = 0.0f;
	mat4[3][1] = 0.0f;
	mat4[3][2] = 0.0f;
	mat4[3][3] = 1.0f;
}

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
 * @paramvTop: Output vector representing the top clipping plane.
 * @param vNear: Output vector representing the near clipping plane.
 * @param vFar: Output vector representing the far clipping plane.
 */
void CMatrix4Df::CalculateClipPlanes(SVector4Df& vLeft, SVector4Df& vRight, SVector4Df& vBottom, SVector4Df& vTop, SVector4Df& vNear, SVector4Df& vFar) const
{
	SVector4Df Row1(mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3]);
	SVector4Df Row2(mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3]);
	SVector4Df Row3(mat4[2][0], mat4[2][1], mat4[2][2], mat4[2][3]);
	SVector4Df Row4(mat4[3][0], mat4[3][1], mat4[3][2], mat4[3][3]);

	vLeft = Row1 + Row4;
	vRight = Row1 - Row4;
	vBottom = Row2 + Row4;
	vTop = Row2 - Row4;
	vNear = Row3 + Row4;
	vFar = Row3 - Row4;
}

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
void CMatrix4Df::CalculateClipPlanes(glm::vec4& vLeft, glm::vec4& vRight, glm::vec4& vBottom, glm::vec4& vTop, glm::vec4& vNear, glm::vec4& vFar)
{
	glm::vec4 Row1(mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3]);
	glm::vec4 Row2(mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3]);
	glm::vec4 Row3(mat4[2][0], mat4[2][1], mat4[2][2], mat4[2][3]);
	glm::vec4 Row4(mat4[3][0], mat4[3][1], mat4[3][2], mat4[3][3]);

	vLeft = Row1 + Row4;
	vRight = Row1 - Row4;
	vBottom = Row2 + Row4;
	vTop = Row2 - Row4;
	vNear = Row3 + Row4;
	vFar = Row3 - Row4;
}


/**
 * Initializes the matrix for a rotation around the X-axis.
 *
 * This function sets up a 3x3 transformation matrix for a rotation
 * around the X-axis. The matrix can be configured for either left-handed
 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
 *
 * @param fRotX: The angle of rotation in radians.
 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
 */
void CMatrix3Df::InitRotationX(const float fRotX, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around X Axis */
	if (bLeftHanded)
	{
		mat3[0][0] = 1.0f; mat3[0][1] = 0.0f; mat3[0][2] = 0.0f;
		mat3[1][0] = 0.0f; mat3[1][1] = std::cosf(fRotX); mat3[1][2] = std::sinf(fRotX);
		mat3[2][0] = 0.0f; mat3[2][1] = -std::sinf(fRotX); mat3[2][2] = std::cosf(fRotX);
		return;
	}

	mat3[0][0] = 1.0f; mat3[0][1] = 0.0f; mat3[0][2] = 0.0f;
	mat3[1][0] = 0.0f; mat3[1][1] = std::cosf(fRotX); mat3[1][2] = -std::sinf(fRotX);
	mat3[2][0] = 0.0f; mat3[2][1] = std::sinf(fRotX); mat3[2][2] = std::cosf(fRotX);
}

/**
 * Initializes the matrix for a rotation around the Y-axis.
 *
 * This function sets up a 3x3 transformation matrix for a rotation
 * around the Y-axis. The matrix can be configured for either left-handed
 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
 *
 * @param fRotY: The angle of rotation in radians.
 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
 */
void CMatrix3Df::InitRotationY(const float fRotY, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around Y Axis */
	if (bLeftHanded)
	{
		mat3[0][0] = std::cosf(fRotY); mat3[0][1] = 0.0f; mat3[0][2] = -std::sinf(fRotY);
		mat3[1][0] = 0.0f; mat3[1][1] = 1.0f; mat3[1][2] = 0.0f;
		mat3[2][0] = std::sinf(fRotY); mat3[2][1] = 0.0f; mat3[2][2] = std::cosf(fRotY);
		return;
	}

	mat3[0][0] = std::cosf(fRotY); mat3[0][1] = 0.0f; mat3[0][2] = std::sinf(fRotY);
	mat3[1][0] = 0.0f; mat3[1][1] = 1.0f; mat3[1][2] = 0.0f;
	mat3[2][0] = -std::sinf(fRotY); mat3[2][1] = 0.0f; mat3[2][2] = std::cosf(fRotY);
}

/**
 * Initializes the matrix for a rotation around the Z-axis.
 *
 * This function sets up a 3x3 transformation matrix for a rotation
 * around the Z-axis. The matrix can be configured for either left-handed
 * or right-handed coordinate systems, based on the `bLeftHanded` parameter.
 *
 * @param fRotZ: The angle of rotation in radians.
 * @param bLeftHanded: A boolean indicating whether to use a left-handed coordinate system if true.
 */
void CMatrix3Df::InitRotationZ(const float fRotZ, const bool bLeftHanded)
{
	/* Rotate The Matrix in Left Handed Coordinate System Around Z Axis */
	if (bLeftHanded)
	{
		mat3[0][0] = std::cosf(fRotZ); mat3[0][1] = std::sinf(fRotZ); mat3[0][2] = 0.0f;
		mat3[1][0] = -std::sinf(fRotZ); mat3[1][1] = std::cosf(fRotZ); mat3[1][2] = 0.0f;
		mat3[2][0] = 0.0f; mat3[2][1] = 0.0f; mat3[2][2] = 1.0f;
		return;
	}

	mat3[0][0] = std::cosf(fRotZ); mat3[0][1] = -std::sinf(fRotZ); mat3[0][2] = 0.0f;
	mat3[1][0] = std::sinf(fRotZ); mat3[1][1] = std::cosf(fRotZ); mat3[1][2] = 0.0f;
	mat3[2][0] = 0.0f; mat3[2][1] = 0.0f; mat3[2][2] = 1.0f;
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
void CMatrix3Df::InitRotateTransform(const float fRotateX, const float fRotateY, const float fRotateZ)
{
	CMatrix3Df matX{}, matY{}, matZ{};

	const float x = ToRadian(fRotateX);
	const float y = ToRadian(fRotateY);
	const float z = ToRadian(fRotateZ);

	matX.InitRotationX(x);
	matY.InitRotationY(y);
	matZ.InitRotationZ(z);

	/* ZYX Euler rotation sequence */
	*this = matZ * matY * matX;
}

/**
 * Initializes the matrix for a rotation transformation using the XYZ Euler rotation sequence.
 *
 * @param RotateX: The angle of rotation around the X-axis in degrees.
 * @param RotateY: The angle of rotation around the Y-axis in degrees.
 * @param RotateZ: The angle of rotation around the Z-axis in degrees.
 *
 * This function creates individual rotation matrices for the X, Y, and Z axes
 * and combines them in the order rx * ry * rz. This order applies the
 * rotations in the sequence: Z-axis, Y-axis, then X-axis, relative to the
 * global coordinate system.
 */
void CMatrix3Df::InitRotateTransformZYX(const float fRotateX, const float fRotateY, const float fRotateZ)
{
	CMatrix3Df matX{}, matY{}, matZ{};

	const float x = ToRadian(fRotateX);
	const float y = ToRadian(fRotateY);
	const float z = ToRadian(fRotateZ);

	matX.InitRotationX(x);
	matY.InitRotationY(y);
	matZ.InitRotationZ(z);

	/* XYZ Euler rotation sequence */
	*this = matX * matY * matZ;
}

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
void CMatrix3Df::InitRotateTransform(const SVector3Df& vRotation)
{
	InitRotateTransform(vRotation.x, vRotation.y, vRotation.z);
}

/**
 * Initializes the matrix for a rotation transformation using the ZYX Euler rotation sequence.
 *
 * @param vRotation: The glm::vec3 holding rotation degrees.
 *
 * This function creates individual rotation matrices for the X, Y, and Z axes
 * and combines them in the order rz * ry * rx. This order applies the
 * rotations in the sequence: X-axis, Y-axis, then Z-axis, relative to the
 * local coordinate system.
 */
void CMatrix3Df::InitRotateTransform(const glm::vec3& vRotation)
{
	InitRotateTransform(vRotation.x, vRotation.y, vRotation.z);
}
