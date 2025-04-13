#pragma once

#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cmath>

class CMatrix4Df;
struct SVector3Df;

/**
 * Maximum floating point difference that is considered as equal.
 */
#define QUATERNION_EPS (1e-4)

 /**
  * Data structure to hold a quaternion.
  */
typedef struct SQuaternion
{
    float w;           /**< Scalar part */
    float x;           /**< Vector part */
    float y;           /**< Vector part */
    float z;           /**< Vector part */
} TQuaternion;

typedef TQuaternion QUAT;
typedef TQuaternion* LPQUAT;


extern void Quaternion_Set(float _w, float _v1, float _v2, float _v3, LPQUAT qOutput);
extern void Quaternion_Set(float _w, const SVector3Df& vec, LPQUAT qOutput);

extern void Quaternion_SetIdentity(LPQUAT qOutput);
extern void Quaternion_Copy(LPQUAT copyFrom, LPQUAT copyTo);
extern bool Quaternion_Equal(LPQUAT q1, LPQUAT q2);
extern void Quaternion_Print(LPQUAT quat);

extern void Quaternion_FromAxisAngle(float vAxis[3], float fAngle, LPQUAT qOutput, bool bRadian = true);
extern void Quaternion_FromAxisAngle(const SVector3Df& vAxis, float fAngle, LPQUAT qOutput, bool bRadian = true);

extern float Quaternion_ToAxisAngle(LPQUAT quat, float dOutput[3]);
extern float Quaternion_ToAxisAngle(LPQUAT quat, SVector3Df& vec3Output);
extern void Quaternion_FromXRotation(float fAngle, LPQUAT qOutput);
extern void Quaternion_FromYRotation(float fAngle, LPQUAT qOutput);
extern void Quaternion_FromZRotation(float fAngle, LPQUAT qOutput);

extern void Quaternion_FromEulerZYX(float eulerZYX[3], LPQUAT qOutput);
extern void Quaternion_FromEulerZYX(const SVector3Df& v3EulerZYX, LPQUAT qOutput);

extern void Quaternion_ToEulerZYX(LPQUAT quat, float dOutput[3]);
extern void Quaternion_ToEulerZYX(LPQUAT quat, SVector3Df& v3EulerZYX);

extern void Quaternion_Conjugate(LPQUAT quat, LPQUAT qOutput);
extern float Quaternion_Norm(LPQUAT quat);
extern void Quaternion_Normalize(LPQUAT quat, LPQUAT qOutput);
extern void Quaternion_Multiply(LPQUAT q1, LPQUAT q2, LPQUAT qResult);

extern void Quaternion_Rotate(LPQUAT quat, float v[3], float fOutput[3]);
extern void Quaternion_Rotate(LPQUAT quat, const SVector3Df& vec3, SVector3Df& vec3OutPut);

extern void Quaternion_Slerp(LPQUAT q1, LPQUAT q2, float t, LPQUAT qOutput);
extern void Quaternion_ToMatrix(LPQUAT q, CMatrix4Df mat);
extern void Quaternion_RotateVector(const LPQUAT quat, const float v[3], float result[3]);
extern void Quaternion_RotateVector(const LPQUAT quat, const SVector3Df& vec3, SVector3Df& vec3OutPut);