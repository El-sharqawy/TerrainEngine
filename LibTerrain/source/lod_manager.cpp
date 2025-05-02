#include "stdafx.h"
#include "lod_manager.h"

CLodManager::CLodManager() : m_gMap()
{
	m_iMaxLod = 0;
	m_iPatchSize = 0;
	m_iNumPatchesX = 0;
	m_iNumPatchesZ = 0;
	m_fWorldScale = 0.0f;
}

GLint CLodManager::InitLodManager(GLint iPatchSize, GLint iNumPatchesX, GLint iNumPatchesZ, float fWorldScale)
{
	m_iPatchSize = iPatchSize;
	m_iNumPatchesX = iNumPatchesX;
	m_iNumPatchesZ = iNumPatchesZ;
	m_fWorldScale = fWorldScale;

	CalcMaxLOD();

	TPatchLod Zero{};
	m_gMap.InitGrid(iNumPatchesX, iNumPatchesZ, Zero);
	m_vRegions.resize(m_iMaxLod + 1);

	CalcLodRegions();

	return (m_iMaxLod);
}

void CLodManager::Update()
{
	const CCamera* camera = CCameraManager::Instance().GetCurrentCamera();
	const SVector3Df& vCameraPos = camera->GetPosition();

	UpdateLodMapPassOne(vCameraPos);
	UpdateLodMapPassTwo(vCameraPos);
}

void CLodManager::Update(const SVector3Df& vCameraPos)
{
	UpdateLodMapPassOne(vCameraPos);
	UpdateLodMapPassTwo(vCameraPos);
}

const TPatchLod& CLodManager::GetPatchLod(GLint iPatchX, GLint iPatchZ) const
{
	return (m_gMap.Get(iPatchX, iPatchZ));
}

void CLodManager::PrintLodMap() const
{
	for (GLint iLodMapZ = m_iNumPatchesZ - 1; iLodMapZ >= 0; iLodMapZ--)
	{
		for (GLint iLodMapX = 0; iLodMapX < m_iNumPatchesX; iLodMapX++)
		{
			sys_log("PrintLodMap: %d", m_gMap.Get(iLodMapX, iLodMapZ).iCore);
		}
	}
}

void CLodManager::CalcLodRegions()
{
	GLint iSum = 0;
	
	for (GLint i = 0; i <= m_iMaxLod; i++)
	{
		iSum += (i + 1);
	}

	const CCamera* camera = CCameraManager::Instance().GetCurrentCamera();

	const float m_fControlLod = 0.6f;	// 0.5-0.7 good values
	const float farZ = camera->GetPersProjInfo().zFar * m_fControlLod;
	const float fX = farZ / static_cast<float>(iSum);

	GLint iTemp = 0;

	for (GLint i = 0; i <= m_iMaxLod; i++)
	{
		GLint iCurRange = static_cast<GLint>(fX * (i + 1));
		m_vRegions[i] = iTemp + iCurRange;
		iTemp += iCurRange;
#if defined(_DEBUG)
		sys_log("CalcLodRegions: Index: %d, Val: %d", i, m_vRegions[i]);
#endif
	}
}

bool CLodManager::CalcMaxLOD()
{
	const GLint iNumSegments = m_iPatchSize - 1;
	if (std::ceilf(std::log2f(static_cast<float>(iNumSegments))) != std::floorf(std::log2f(static_cast<float>(iNumSegments))))
	{
		sys_err("The number of vertices in the patch minus one must be a power of two");
		sys_err("%f %f", std::ceilf(std::log2f(static_cast<float>(iNumSegments))), std::floorf(std::log2f(static_cast<float>(iNumSegments))));
		return false;
	}

	const GLint iPatchSizeLog2 = static_cast<GLint>(std::log2f(static_cast<float>(iNumSegments)));

#if defined(_DEBUG)
	sys_log("CalcMaxLOD: log2 of patch size %d is %d", m_iPatchSize, iPatchSizeLog2);
#endif

	m_iMaxLod = iPatchSizeLog2 - 1;
	return (true);
}

void CLodManager::UpdateLodMapPassOne(const SVector3Df& vCameraPos)
{
	const GLint iCenterStep = m_iPatchSize / 2;

	for (GLint iLodMapZ = 0; iLodMapZ < m_iNumPatchesZ; iLodMapZ++)
	{
		for (GLint iLodMapX = 0; iLodMapX < m_iNumPatchesX; iLodMapX++)
		{
			const GLint iX = iLodMapX * (m_iPatchSize - 1) + iCenterStep;
			const GLint iZ = iLodMapZ * (m_iPatchSize - 1) + iCenterStep;

			const SVector3Df v3PatchCenter = SVector3Df(static_cast<float>(iX) * m_fWorldScale, 0.0f, static_cast<float>(iZ) * m_fWorldScale);

			const float fDistanceToCamera = vCameraPos.distance(v3PatchCenter);

			const GLint iCoreLOD = DistanceToLod(fDistanceToCamera);

			TPatchLod* pPatchLOD = m_gMap.GetAddr(iLodMapX, iLodMapZ);
			pPatchLOD->iCore = iCoreLOD;
		}

	}
}

void CLodManager::UpdateLodMapPassTwo(const SVector3Df& vCameraPos)
{
	const GLint iCenterStep = m_iPatchSize / 2;

	for (GLint iLodMapZ = 0; iLodMapZ < m_iNumPatchesZ; iLodMapZ++)
	{
		for (GLint iLodMapX = 0; iLodMapX < m_iNumPatchesX; iLodMapX++)
		{
			GLint iCoreLOD = m_gMap.Get(iLodMapX, iLodMapZ).iCore;

			GLint IndexLeft = iLodMapX;
			GLint IndexRight = iLodMapX;

			GLint IndexTop = iLodMapZ;
			GLint IndexBottom = iLodMapZ;

			if (iLodMapX > 0)
			{
				IndexLeft--;

				if (m_gMap.Get(IndexLeft, iLodMapZ).iCore > iCoreLOD)
				{
					m_gMap.At(iLodMapX, iLodMapZ).iLeft = 1;
				}
				else
				{
					m_gMap.At(iLodMapX, iLodMapZ).iLeft = 0;
				}
			}

			if (iLodMapX < m_iNumPatchesX - 1)
			{
				IndexRight++;

				if (m_gMap.Get(IndexRight, iLodMapZ).iCore > iCoreLOD)
				{
					m_gMap.At(iLodMapX, iLodMapZ).iRight = 1;
				}
				else
				{
					m_gMap.At(iLodMapX, iLodMapZ).iRight = 0;
				}

			}

			if (iLodMapZ > 0)
			{
				IndexBottom--;

				if (m_gMap.Get(iLodMapX, IndexBottom).iCore > iCoreLOD)
				{
					m_gMap.At(iLodMapX, iLodMapZ).iBottom = 1;
				}
				else
				{
					m_gMap.At(iLodMapX, iLodMapZ).iBottom = 0;
				}
			}

			if (iLodMapZ < m_iNumPatchesZ - 1)
			{
				IndexTop++;

				if (m_gMap.Get(iLodMapX, IndexTop).iCore > iCoreLOD)
				{
					m_gMap.At(iLodMapX, iLodMapZ).iTop = 1;
				}
				else
				{
					m_gMap.At(iLodMapX, iLodMapZ).iTop = 0;
				}
			}
		}
	}
}

GLint CLodManager::DistanceToLod(float fDistance)
{
	GLint iLod = m_iMaxLod;

	for (GLint i = 0; i <= m_iMaxLod; i++)
	{
		if (fDistance < m_vRegions[i])
		{
			iLod = i;
			break;
		}
	}

	return (iLod);
}