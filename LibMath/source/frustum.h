#pragma once

#include "matrix.h"

struct SFrustum
{
	SVector4Df v4NearTopLeft;
	SVector4Df v4NearBottomLeft;
	SVector4Df v4NearTopRight;
	SVector4Df v4NearBottomRight;

	SVector4Df v4FarTopLeft;
	SVector4Df v4FarBottomLeft;
	SVector4Df v4FarTopRight;
	SVector4Df v4FarBottomRight;

	SFrustum() = default;

	void transform(const CMatrix4Df& mat)
	{
		v4NearTopLeft = mat * v4NearTopLeft;
		v4NearBottomLeft = mat * v4NearBottomLeft;
		v4NearTopRight = mat * v4NearTopRight;
		v4NearBottomRight = mat * v4NearBottomRight;

		v4FarTopLeft = mat * v4FarTopLeft;
		v4FarBottomLeft = mat * v4FarBottomLeft;
		v4FarTopRight = mat * v4FarTopRight;
		v4FarBottomRight = mat * v4FarBottomRight;
	}

	void print() const
	{
		printf("v4NearTopLeft "); v4NearTopLeft.print();
		printf("v4NearBottomLeft "); v4NearBottomLeft.print();
		printf("v4NearTopRight "); v4NearTopRight.print();
		printf("v4NearBottomRight "); v4NearBottomRight.print();

		printf("v4FarTopLeft "); v4FarTopLeft.print();
		printf("v4FarBottomLeft "); v4FarBottomLeft.print();
		printf("v4FarTopRight "); v4FarTopRight.print();
		printf("v4FarBottomRight "); v4FarBottomRight.print();

	}
};

struct SFrustumCulling
{
	SFrustumCulling(const CMatrix4Df& matViewProj)
	{
		Update(matViewProj);
	}

	void Update(const CMatrix4Df& matViewProj)
	{
		matViewProj.CalculateClipPlanes(m_v4LeftClipPlane, m_v4RightClipPlane, m_v4BottomClipPlane, m_v4TopClipPlane, m_v4NearClipPlane, m_v4FarClipPlane);
	}

	bool IsPointInsideViewFrustum(const SVector3Df& v3Point) const
	{
		SVector4Df v4Point(v3Point, 1.0f);

		bool bIsInside = (
			(m_v4LeftClipPlane.dot(v4Point) >= 0) &&
			(m_v4RightClipPlane.dot(v4Point) <= 0) &&
			// (m_v4BottomClipPlane.dot(v4Point) >= 0) &&
			// (m_v4TopClipPlane.dot(v4Point) <= 0)&&
			(m_v4NearClipPlane.dot(v4Point) >= 0) &&
			(m_v4FarClipPlane.dot(v4Point) <= 0));

		return (bIsInside);
	}

private:
	SVector4Df m_v4LeftClipPlane;
	SVector4Df m_v4RightClipPlane;
	SVector4Df m_v4TopClipPlane;
	SVector4Df m_v4BottomClipPlane;
	SVector4Df m_v4NearClipPlane;
	SVector4Df m_v4FarClipPlane;
};