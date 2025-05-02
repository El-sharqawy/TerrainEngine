#pragma once

#include <glad/glad.h>
#include "../../LibMath/source/stdafx.h"
#include "../../LibMath/source/grid.h" // Include the header file where CGrid is defined

typedef struct SPatchLod
{
	GLint iCore;
	GLint iLeft;
	GLint iRight;
	GLint iTop;
	GLint iBottom;
	SPatchLod()
	{
		iCore = iLeft = iRight = iTop = iBottom = 0;
	}
} TPatchLod;

typedef struct SSingleLodInfo
{
	GLint iStart;
	GLint iCount;
	SSingleLodInfo()
	{
		iStart = iCount = 0;
	}
} TSingleLodInfo;

constexpr GLint LEFT = 2;
constexpr GLint RIGHT = 2;
constexpr GLint TOP = 2;
constexpr GLint BOTTOM = 2;

typedef struct SLodInfo
{
	TSingleLodInfo LodInfo[LEFT][RIGHT][TOP][BOTTOM];
} TLodInfo;

class CLodManager : public CSingleton<CLodManager>
//class CLodManager
{
public:
	CLodManager();
	~CLodManager() = default;

	GLint InitLodManager(GLint iPatchSize, GLint iNumPatchesX, GLint iNumPatchesZ, float fWorldScale);
	void Update();
	void Update(const SVector3Df& vCameraPos);

	const TPatchLod& GetPatchLod(GLint iPatchX, GLint iPatchZ) const;

	void PrintLodMap() const;

private:
	void CalcLodRegions();
	bool CalcMaxLOD();
	
	void UpdateLodMapPassOne(const SVector3Df& vCameraPos);
	void UpdateLodMapPassTwo(const SVector3Df& vCameraPos);

	GLint DistanceToLod(float fDistance);

private:
	GLint m_iMaxLod;
	GLint m_iPatchSize;
	GLint m_iNumPatchesX;
	GLint m_iNumPatchesZ;
	float m_fWorldScale;

	CGrid<TPatchLod> m_gMap;
	std::vector<GLint> m_vRegions;
};