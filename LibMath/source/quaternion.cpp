#include "stdafx.h"
#include "quaternion.h"

void Quaternion_Set(float _w, float _v1, float _v2, float _v3, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	qOutput->w = _w;
	qOutput->x = _v1;
	qOutput->y = _v2;
	qOutput->z = _v3;
}

void Quaternion_Set(float _w, const SVector3Df& vec, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	qOutput->w = _w;
	qOutput->x = vec.x;
	qOutput->y = vec.y;
	qOutput->z = vec.z;
}

void Quaternion_SetIdentity(LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	Quaternion_Set(1.0, 0.0, 0.0, 0.0, qOutput);
}

void Quaternion_Copy(LPQUAT copyFrom, LPQUAT copyTo)
{
	assert(copyFrom != nullptr && copyTo != nullptr);
	Quaternion_Set(copyFrom->w, copyFrom->x, copyFrom->y, copyFrom->z, copyTo);
}

bool Quaternion_Equal(LPQUAT q1, LPQUAT q2)
{
	assert(q1 != nullptr && q2 != nullptr);
	bool bEqualW = std::fabs(q1->w - q2->w) <= QUATERNION_EPS;
	bool bEqualV0 = std::fabs(q1->x - q2->z) <= QUATERNION_EPS;
	bool bEqualV1 = std::fabs(q1->y - q2->y) <= QUATERNION_EPS;
	bool bEqualV2 = std::fabs(q1->z - q2->z) <= QUATERNION_EPS;

	return (bEqualW && bEqualV0 && bEqualV1 && bEqualV2);
}

void Quaternion_Print(LPQUAT quat)
{
	assert(quat != nullptr);
	fprintf(stdout, "(%.3f, %.3f, %.3f, %.3f)", quat->w, quat->x, quat->y, quat->z);
}

void Quaternion_FromAxisAngle(float vAxis[3], float fAngle, LPQUAT qOutput, bool bRadian)
{
	assert(qOutput != nullptr);

	// Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
	float angleRad = fAngle;

	if (bRadian)
		angleRad = -ToRadian(fAngle);

	qOutput->w = std::cosf(angleRad / 2.0f);

	//the sine of half the rotation angle
	float c = std::sinf(angleRad / 2.0f);
	qOutput->x = c * vAxis[0];
	qOutput->y = c * vAxis[1];
	qOutput->z = c * vAxis[2];
}

void Quaternion_FromAxisAngle(const SVector3Df& vAxis, float fAngle, LPQUAT qOutput, bool bRadian)
{
	assert(qOutput != nullptr);

	// Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
	float angleRad = fAngle;

	if (bRadian)
		angleRad = -ToRadian(fAngle);

	qOutput->w = std::cosf(angleRad / 2.0f);

	//the sine of half the rotation angle
	float c = std::sinf(angleRad / 2.0f);
	qOutput->x = c * vAxis.x;
	qOutput->y = c * vAxis.y;
	qOutput->z = c * vAxis.z;
}

float Quaternion_ToAxisAngle(LPQUAT quat, float fOutput[3])
{
	assert(fOutput != nullptr && quat != nullptr);
	// Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/

	float Angle = 2.0f * std::acosf(quat->w);
	float divider = std::sqrtf(1.0f - quat->w * quat->w);

	if (divider != 0.0f)
	{
		// Calculate the axis
		fOutput[0] = quat->x / divider;
		fOutput[1] = quat->y / divider;
		fOutput[2] = quat->z / divider;
	}
	else
	{
		// Arbitrary normalized axis
		fOutput[0] = 1.0f;
		fOutput[1] = 0.0f;
		fOutput[2] = 0.0f;
	}
	return (Angle);
}

float Quaternion_ToAxisAngle(LPQUAT quat, SVector3Df& vec3Output)
{
	assert(quat != nullptr);
	// Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/

	float Angle = 2.0f * std::acosf(quat->w);
	float divider = std::sqrtf(1.0f - quat->w * quat->w);

	if (divider != 0.0f)
	{
		// Calculate the axis
		vec3Output.x = quat->x / divider;
		vec3Output.y = quat->y / divider;
		vec3Output.z = quat->z / divider;
	}
	else
	{
		// Arbitrary normalized axis
		vec3Output.x = 1.0f;
		vec3Output.y = 0.0f;
		vec3Output.z = 0.0f;
	}
	return (Angle);
}

void Quaternion_FromXRotation(float fAngle, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	float Axis[3] = { 1.0, 0.0, 0.0 };
	Quaternion_FromAxisAngle(Axis, fAngle, qOutput);
}

void Quaternion_FromYRotation(float fAngle, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	float Axis[3] = { 0.0, 1.0, 0.0 };
	Quaternion_FromAxisAngle(Axis, fAngle, qOutput);
}

void Quaternion_FromZRotation(float fAngle, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	float Axis[3] = { 0.0, 0.0, 1.0 };
	Quaternion_FromAxisAngle(Axis, fAngle, qOutput);
}

void Quaternion_FromEulerZYX(float eulerZYX[3], LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	// Based on https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float cy = std::cosf(eulerZYX[2] * 0.5f);
	float sy = std::sinf(eulerZYX[2] * 0.5f);
	float cr = std::cosf(eulerZYX[0] * 0.5f);
	float sr = std::sinf(eulerZYX[0] * 0.5f);
	float cp = std::cosf(eulerZYX[1] * 0.5f);
	float sp = std::sinf(eulerZYX[1] * 0.5f);

	qOutput->w = cy * cr * cp + sy * sr * sp;
	qOutput->x = cy * sr * cp - sy * cr * sp;
	qOutput->y = cy * cr * sp + sy * sr * cp;
	qOutput->z = sy * cr * cp - cy * sr * sp;
}

void Quaternion_FromEulerZYX(const SVector3Df& v3EulerZYX, LPQUAT qOutput)
{
	assert(qOutput != nullptr);
	// Based on https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float cy = std::cosf(v3EulerZYX.z * 0.5f);
	float sy = std::sinf(v3EulerZYX.z * 0.5f);
	float cr = std::cosf(v3EulerZYX.x * 0.5f);
	float sr = std::sinf(v3EulerZYX.x * 0.5f);
	float cp = std::cosf(v3EulerZYX.y * 0.5f);
	float sp = std::sinf(v3EulerZYX.y * 0.5f);

	qOutput->w = cy * cr * cp + sy * sr * sp;
	qOutput->x = cy * sr * cp - sy * cr * sp;
	qOutput->y = cy * cr * sp + sy * sr * cp;
	qOutput->z = sy * cr * cp - cy * sr * sp;
}

void Quaternion_ToEulerZYX(LPQUAT quat, float fOutput[3])
{
	assert(quat != nullptr && fOutput != nullptr);

	// roll (x-axis rotation)
	float sinr_cosp = 2.0f * (quat->w * quat->x + quat->y * quat->z);
	float cosr_cosp = 1.0f - 2.0f * (quat->x * quat->x + quat->y * quat->y);
	fOutput[0] = std::atan2f(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = std::sqrtf(1.0f + 2.0f * (quat->w * quat->y - quat->x * quat->z));
	float cosp = std::sqrtf(1.0f - 2.0f * (quat->w * quat->y - quat->x * quat->z));
	fOutput[1] = 2.0f * std::atan2f(sinp, cosp) - static_cast<float>(M_PI) / 2.0f;

	// yaw (z-axis rotation)
	float siny_cosp = 2.0f * (quat->w * quat->z + quat->x * quat->y);
	float cosy_cosp = 1.0f - 2.0f * (quat->y * quat->y + quat->z * quat->z);
	fOutput[2] = std::atan2f(siny_cosp, cosy_cosp);
}

void Quaternion_ToEulerZYX(LPQUAT quat, SVector3Df& v3EulerZYX)
{
	assert(quat != nullptr);

	// roll (x-axis rotation)
	float sinr_cosp = 2.0f * (quat->w * quat->x + quat->y * quat->z);
	float cosr_cosp = 1.0f - 2.0f * (quat->x * quat->x + quat->y * quat->y);
	v3EulerZYX.x = std::atan2f(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = std::sqrtf(1.0f + 2.0f * (quat->w * quat->y - quat->x * quat->z));
	float cosp = std::sqrtf(1.0f - 2.0f * (quat->w * quat->y - quat->x * quat->z));
	v3EulerZYX.y = 2.0f * std::atan2f(sinp, cosp) - static_cast<float>(M_PI) / 2.0f;

	// yaw (z-axis rotation)
	float siny_cosp = 2.0f * (quat->w * quat->z + quat->x * quat->y);
	float cosy_cosp = 1.0f - 2.0f * (quat->y * quat->y + quat->z * quat->z);
	v3EulerZYX.z = std::atan2f(siny_cosp, cosy_cosp);
}

void Quaternion_Conjugate(LPQUAT quat, LPQUAT qOutput)
{
	assert(qOutput != nullptr && quat != nullptr);

	qOutput->w = quat->w;
	qOutput->x = -quat->x;
	qOutput->y = -quat->y;
	qOutput->z = -quat->z;
}

float Quaternion_Norm(LPQUAT quat)
{
	assert(quat != nullptr);
	float w2 = quat->w * quat->w;
	float x2 = quat->x * quat->x;
	float y2 = quat->y * quat->y;
	float z2 = quat->z * quat->z;

	return std::sqrt(w2 + x2 + y2 + z2);
}

void Quaternion_Normalize(LPQUAT quat, LPQUAT qOutput)
{
	assert(quat != nullptr && qOutput != nullptr);

	float len = Quaternion_Norm(quat);
	assert(len != 0.0);

	Quaternion_Set(
		quat->w / len,
		quat->x / len,
		quat->y / len,
		quat->z / len,
		qOutput
	);
}

void Quaternion_Multiply(LPQUAT q1, LPQUAT q2, LPQUAT qResult)
{
	assert(q1 != nullptr && q2 != nullptr && qResult != nullptr);

	QUAT result{};

	/*
	Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm
			a*e - b*f - c*g - d*h
		+ i (b*e + a*f + c*h- d*g)
		+ j (a*g - b*h + c*e + d*f)
		+ k (a*h + b*g - c*f + d*e)
	*/

	result.w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
	result.x = q1->x * q2->w + q1->w * q2->x + q1->y * q2->z - q1->z * q2->y;
	result.y = q1->w * q2->y - q1->x * q2->z + q1->y * q2->w + q1->z * q2->x;
	result.z = q1->w * q2->z + q1->x * q1->y - q1->y * q2->x + q1->z * q2->w;

	*qResult = result;
}

void Quaternion_Rotate(LPQUAT quat, float v[3], float fOutput[3])
{
	assert(quat != nullptr && fOutput != nullptr);

	float result[3]{};

	float ww = quat->w * quat->w;
	float xx = quat->x * quat->x;
	float yy = quat->y * quat->y;
	float zz = quat->z * quat->z;

	float wx = quat->w * quat->x;
	float wy = quat->w * quat->y;
	float wz = quat->w * quat->z;

	float xy = quat->x * quat->y;
	float xz = quat->x * quat->z;
	float yz = quat->y * quat->z;

	// Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
	// p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
	// p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
	// p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;

	result[0] = ww * v[0] + 2 * wy * v[2] - 2 * wz * v[1] + xx * v[0] + 2 * xy * v[1] + 2 * xz * v[2] - zz * v[0] - yy * v[0];
	result[1] = 2 * xy * v[0] + yy * v[1] + 2 * yz * v[2] + 2 * wz * v[0] - zz * v[1] + ww * v[1] - 2 * wx * v[2] - xx * v[1];
	result[2] = 2 * xz * v[0] + 2 * yz * v[1] + zz * v[2] - 2 * wy * v[0] - yy * v[2] + 2 * wx * v[1] - xx * v[2] + ww * v[2];

	// Copy result to output
	fOutput[0] = result[0];
	fOutput[1] = result[1];
	fOutput[2] = result[2];
}

void Quaternion_Rotate(LPQUAT quat, const SVector3Df& vec3, SVector3Df& vec3OutPut)
{
	assert(quat != nullptr);

	SVector3Df vResult{};

	float ww = quat->w * quat->w;
	float xx = quat->x * quat->x;
	float yy = quat->y * quat->y;
	float zz = quat->z * quat->z;

	float wx = quat->w * quat->x;
	float wy = quat->w * quat->y;
	float wz = quat->w * quat->z;

	float xy = quat->x * quat->y;
	float xz = quat->x * quat->z;
	float yz = quat->y * quat->z;

	// Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
	// p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
	// p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
	// p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;

	vResult.x = ww * vec3.x + 2 * wy * vec3.z - 2 * wz * vec3.y + xx * vec3.x + 2 * xy * vec3.y + 2 * xz * vec3.z - zz * vec3.x - yy * vec3.x;
	vResult.y = 2 * xy * vec3.x + yy * vec3.y + 2 * yz * vec3.z + 2 * wz * vec3.x - zz * vec3.y + ww * vec3.y - 2 * wx * vec3.z - xx * vec3.y;
	vResult.z = 2 * xz * vec3.x + 2 * yz * vec3.y + zz * vec3.z - 2 * wy * vec3.x - yy * vec3.z + 2 * wx * vec3.y - xx * vec3.z + ww * vec3.z;

	// Copy result to output
	vec3OutPut.x = vResult.x;
	vec3OutPut.y = vResult.y;
	vec3OutPut.z = vResult.z;
}

void Quaternion_Slerp(LPQUAT q1, LPQUAT q2, float t, LPQUAT qOutput)
{
	// t is interpolation factor
	QUAT result{};

	// Based on http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
	float cosHalfTheta = q1->w * q2->w + q1->x * q2->x + q1->y * q2->y + q1->z * q2->z;
	
	// if q1=q2 or q1=-q2 then theta = 0 and we can return q1
	if (std::fabs(cosHalfTheta) >= 1.0f)
	{
		Quaternion_Copy(q1, qOutput);
		return;
	}

	float halfTheta = std::acosf(cosHalfTheta);
	float sinHalfTheta = std::sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to q1 or q2
	if (std::fabs(sinHalfTheta) < QUATERNION_EPS) // fabs is floating point absolute
	{
		result.w = (q1->w * 0.5f + q2->w * 0.5f);
		result.x = (q1->x * 0.5f + q2->x * 0.5f);
		result.y = (q1->y * 0.5f + q2->y * 0.5f);
		result.z = (q1->z * 0.5f + q2->z * 0.5f);

	}
	else
	{
		// Default quaternion calculation
		float ratioA = std::sinf((1 - t) * halfTheta) / sinHalfTheta;
		float ratioB = std::sinf(t * halfTheta) / sinHalfTheta;

		result.w = (q1->w * ratioA + q2->w * ratioB);
		result.x = (q1->x * ratioA + q2->x * ratioB);
		result.y = (q1->y * ratioA + q2->y * ratioB);
		result.z = (q1->z * ratioA + q2->z * ratioB);
	}

	*qOutput = result;
}

void Quaternion_ToMatrix(LPQUAT q, CMatrix4Df mat)
{
	assert(q != nullptr);

	float w = q->w, x = q->x, y = q->y, z = q->z;

	// Column-major matrix for OpenGL
	mat.mat4[0][0] = 1 - 2 * (y * y + z * z);
	mat.mat4[0][1] = 2 * (x * y + z * w);
	mat.mat4[0][2] = 2 * (x * z - y * w);
	mat.mat4[0][3] = 0;

	mat.mat4[1][0] = 2 * (x * y - z * w);
	mat.mat4[1][1] = 1 - 2 * (x * x + z * z);
	mat.mat4[1][2] = 2 * (y * z + x * w);
	mat.mat4[1][3] = 0;

	mat.mat4[2][0] = 2 * (x * z + y * w);
	mat.mat4[2][1] = 2 * (y * z - x * w);
	mat.mat4[2][2] = 1 - 2 * (x * x + y * y);
	mat.mat4[2][3] = 0;

	mat.mat4[3][0] = 0;
	mat.mat4[3][1] = 0;
	mat.mat4[3][2] = 0;
	mat.mat4[3][3] = 1;
}

void Quaternion_RotateVector(const LPQUAT quat, const float v[3], float result[3])
{
    // Create a quaternion for the vector
    QUAT vQuat = { 0.0, v[0], v[1], v[2] };

    // Calculate the conjugate of the quaternion
    QUAT qConjugate = { quat->w, -quat->x, -quat->y, -quat->z };

    // Multiply quaternion by vector (v * q)
	QUAT temp{};
    temp.w = quat->w * vQuat.w - quat->x * vQuat.x - quat->y * vQuat.y - quat->z * vQuat.z;
    temp.x = quat->w * vQuat.x + quat->x * vQuat.w + quat->y * vQuat.z - quat->z * vQuat.y;
    temp.y = quat->w * vQuat.y - quat->x * vQuat.z + quat->y * vQuat.w + quat->z * vQuat.x;
    temp.z = quat->w * vQuat.z + quat->x * vQuat.y - quat->y * vQuat.x + quat->z * vQuat.w;

    // Multiply the result by the conjugate quaternion (q * v * q^-1)
	result[0] = temp.w * qConjugate.x + temp.x * qConjugate.w + temp.y * qConjugate.z - temp.z * qConjugate.y;
	result[1] = temp.w * qConjugate.y - temp.x * qConjugate.z + temp.y * qConjugate.w + temp.z * qConjugate.x;
	result[2] = temp.w * qConjugate.z + temp.x * qConjugate.y - temp.y * qConjugate.x + temp.z * qConjugate.w;
}

void Quaternion_RotateVector(const LPQUAT quat, const SVector3Df& vec3, SVector3Df& vec3OutPut)
{
	// Create a quaternion for the vector
	QUAT vQuat = { 0.0f, vec3.x, vec3.y, vec3.z };

	// Calculate the conjugate of the quaternion
	QUAT qConjugate = { quat->w, -quat->x, -quat->y, -quat->z };

	// Multiply quaternion by vector (v * q)
	QUAT temp{};
	temp.w = quat->w * vQuat.w - quat->x * vQuat.x - quat->y * vQuat.y - quat->z * vQuat.z;
	temp.x = quat->w * vQuat.x + quat->x * vQuat.w + quat->y * vQuat.z - quat->z * vQuat.y;
	temp.y = quat->w * vQuat.y - quat->x * vQuat.z + quat->y * vQuat.w + quat->z * vQuat.x;
	temp.z = quat->w * vQuat.z + quat->x * vQuat.y - quat->y * vQuat.x + quat->z * vQuat.w;

	// Multiply the result by the conjugate quaternion (q * v * q^-1)
	vec3OutPut.x = temp.w * qConjugate.x + temp.x * qConjugate.w + temp.y * qConjugate.z - temp.z * qConjugate.y;
	vec3OutPut.y = temp.w * qConjugate.y - temp.x * qConjugate.z + temp.y * qConjugate.w + temp.z * qConjugate.x;
	vec3OutPut.z = temp.w * qConjugate.z + temp.x * qConjugate.y - temp.y * qConjugate.x + temp.z * qConjugate.w;
}
