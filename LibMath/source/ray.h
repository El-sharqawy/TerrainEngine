#pragma once

#include <glad/glad.h>
#include "vectors.h"
#include "../../LibGL/source/singleton.h"

class CRay : CSingleton<CRay>
{
public:
	CRay() = default;

	CRay(const SVector3Df& v3Start, const SVector3Df& v3Dir, float fRayRange)
	{
		assert(fRayRange > 0);
		m_v3Start = v3Start;
		m_v3Dir = v3Dir;
		m_fRayRange = fRayRange;
		m_v3Dir.normalize();
		m_v3End = m_v3Start + fRayRange * m_v3Dir;
	}

	void SetStartPoint(const SVector3Df& v3Start)
	{
		m_v3Start = v3Start;
	}

	void SetDirection(const SVector3Df& v3Dir, float fRayRange)
	{
		assert(fRayRange > 0);
		m_v3Dir = v3Dir;
		m_fRayRange = fRayRange;
		m_v3Dir.normalize();
		m_v3End = m_v3Start + fRayRange * m_v3Dir;
	}

	const SVector3Df& GetStartPoint()
	{
		return (m_v3Start);
	}

	const SVector3Df& GetDirection()
	{
		return (m_v3Dir);
	}

	const float& GetRayRange() const
	{
		return (m_fRayRange);
	}

	const SVector3Df& GetEndPoint()
	{
		return (m_v3End);
	}

	void GetStartPoint(SVector3Df* v3Start) const
	{
		*v3Start = m_v3Start;
	}

	void GetEndPoint(SVector3Df* v3EndPoint) const
	{
		*v3EndPoint = m_v3End;
	}

	void GetDirection(SVector3Df* v3Direction, float* fRayRange) const
	{
		*v3Direction = m_v3Dir;
		*fRayRange = m_fRayRange;
	}

	const CRay& operator = (const CRay& rRay)
	{
		assert(rRay.m_fRayRange >= 0);
		m_fRayRange = rRay.m_fRayRange;
		m_v3Start = rRay.m_v3Start;
		m_v3Dir = rRay.m_v3Dir;
		m_v3Dir.normalize();
		m_v3End = m_v3Start + m_fRayRange * m_v3Dir;
	}

private:
	SVector3Df m_v3Start;
	SVector3Df m_v3End;
	SVector3Df m_v3Dir;
	float m_fRayRange;
};