#include "stdafx.h"
#include "midpoint_terrain.h"
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/imgui_impl_glfw.h"
#include "../../LibImageUI/imgui_impl_opengl3.h"

void CMidPointTerrain::CreateMidPointTerrain(GLint iTerrainSize, GLint iNumPatches, float fRoughness, float fMinHeight, float fMaxHeight)
{
	if (fRoughness < 0.0f)
	{
		sys_err("CMidPointTerrain::CreateMidPointTerrain Roughness Must be Positive!");
		return;
	}

	m_iTerrainSize = iTerrainSize;
	m_iNumPatches = iNumPatches;
	m_fRoughness = fRoughness;

	SetMinMaxHeight(fMinHeight, fMaxHeight);

	m_fHeightMapGrid.InitGrid(iTerrainSize, iTerrainSize, 0.0f);

	CreateMidPointTerrainF32(m_fRoughness);

	m_fHeightMapGrid.Normalize(fMinHeight, fMaxHeight);

	Finalize();

	sys_log("CMidPointTerrain::CreateMidPointTerrain Size %d and Patches: %d with Roughness %.0f, MinHeight: %.0f, MaxHeigh %.0f", iTerrainSize, m_iNumPatches, fRoughness, fMinHeight, fMaxHeight);
}

void CMidPointTerrain::CreateMidPointTerrainF32(float fRoughness)
{
	GLint iRectSize = CalculateNextPowerOfTwo(m_iTerrainSize);
	float fCurHeight = static_cast<float>(iRectSize) / 2.0f;
	const float fHeightReduce = std::powf(2.0f, -fRoughness);

	while (iRectSize > 0)
	{
		DiamondStep(iRectSize, fCurHeight);
		SquareStep(iRectSize, fCurHeight);

		iRectSize /= 2;
		fCurHeight *= fHeightReduce;
	}
}

void CMidPointTerrain::DiamondStep(GLint iRectSize, float fCurHeight)
{
	const GLint iHalfRectangle = iRectSize / 2;

	for (GLint y = 0; y < m_iTerrainSize; y += iRectSize)
	{
		for (GLint x = 0; x < m_iTerrainSize; x += iRectSize)
		{
			GLint iNextX = (x + iRectSize) % m_iTerrainSize;
			GLint iNextY = (y + iRectSize) % m_iTerrainSize;

			if (iNextX < x)
			{
				iNextX = m_iTerrainSize - 1;
			}

			if (iNextY < y)
			{
				iNextY = m_iTerrainSize - 1;
			}

			const float fTopLeft = m_fHeightMapGrid.Get(x, y);
			const float fTopRight = m_fHeightMapGrid.Get(iNextX, y);
			const float fBottomLeft = m_fHeightMapGrid.Get(x, iNextY);
			const float fBottomRight = m_fHeightMapGrid.Get(iNextX, iNextY);


			const GLint iMidX = (x + iHalfRectangle) % m_iTerrainSize;
			const GLint iMidY = (y + iHalfRectangle) % m_iTerrainSize;

			const float fRandValue = RandomFloatRange(-fCurHeight, fCurHeight);
			const float fMidPoint = (fTopLeft + fTopRight + fBottomLeft + fBottomRight) / 4.0f;

			m_fHeightMapGrid.Set(iMidX, iMidY, fMidPoint + fRandValue);
		}
	}
}

void CMidPointTerrain::SquareStep(GLint iRectSize, float fCurHeight)
{
	//CurTopMid = avg(PrevYCenter, CurTopLeft, CurTopRight, CurCenter)
	//CurLeftMid = avg(CurPrevXCenterm CurTopleft, CurBotLeft, CurCenter)

	const GLint iHalfRectangle = iRectSize / 2;

	for (GLint y = 0; y < m_iTerrainSize; y += iRectSize)
	{
		for (GLint x = 0; x < m_iTerrainSize; x += iRectSize)
		{
			GLint iNextX = (x + iRectSize) % m_iTerrainSize;
			GLint iNextY = (y + iRectSize) % m_iTerrainSize;

			if (iNextX < x)
			{
				iNextX = m_iTerrainSize - 1;
			}

			if (iNextY < y)
			{
				iNextY = m_iTerrainSize - 1;
			}

			const GLint iMidX = (x + iHalfRectangle) % m_iTerrainSize;
			const GLint iMidY = (y + iHalfRectangle) % m_iTerrainSize;

			const GLint iPrevMidX = (x - iHalfRectangle + m_iTerrainSize) % m_iTerrainSize;
			const GLint iPrevMidY = (y - iHalfRectangle + m_iTerrainSize) % m_iTerrainSize;

			const float fCurTopLeft = m_fHeightMapGrid.Get(x, y);
			const float fCurTopRight = m_fHeightMapGrid.Get(iNextX, y);
			const float fCurCenter = m_fHeightMapGrid.Get(iMidX, iMidY);
			const float fPrevYCenter = m_fHeightMapGrid.Get(iMidX, iPrevMidY);
			const float fCurBottomLeft = m_fHeightMapGrid.Get(x, iNextY);
			const float fPrevXCenter = m_fHeightMapGrid.Get(iPrevMidX, iMidY);


			const float fCurLeftMid = (fCurTopLeft + fCurCenter + fCurBottomLeft + fPrevXCenter) / 4.0f + RandomFloatRange(-fCurHeight, fCurHeight);
			const float fCurTopMid = (fCurTopLeft + fCurCenter + fCurTopRight + fPrevYCenter) / 4.0f + RandomFloatRange(-fCurHeight, fCurHeight);

			m_fHeightMapGrid.Set(iMidX, y, fCurTopMid);
			m_fHeightMapGrid.Set(x, iMidY, fCurLeftMid);
		}
	}
}

void CMidPointTerrain::SetGUI()
{
	ImGui::Begin("TerrainEngine UI");

	static float fMaxHeight = GetMaxHeight();
	if (ImGui::SliderFloat("Max Height", &fMaxHeight, 0.0f, 1000.0f))
	{
		SetMaxHeight(fMaxHeight);
	}

	static float fRoughness = GetRoughness();
	if (ImGui::SliderFloat("Terrain Rouguhness", &fRoughness, 0.0f, 5.0f))
	{
		SetRoughness(fRoughness);
	}

	static float Height0 = 64.0f;
	static float Height1 = 128.0f;
	static float Height2 = 192.0f;
	static float Height3 = 256.0f;

	ImGui::SliderFloat("Height0", &Height0, 0.0f, 64.0f);
	ImGui::SliderFloat("Height1", &Height1, 64.0f, 128.0f);
	ImGui::SliderFloat("Height2", &Height2, 128.0f, 192.0f);
	ImGui::SliderFloat("Height3", &Height3, 192.0f, 500.0f);

	if (ImGui::Button("Generate"))
	{
		Destroy();
		SRANDOM;
		CreateMidPointTerrain(GetSize(), GetPatchSize(), GetRoughness(), GetMinHeight(), GetMaxHeight());
		SetTexturesHeights(Height0, Height1, Height2, Height3);
	}
	ImGui::End();
}

void CMidPointTerrain::Render()
{
	// Just delegate to base terrain render
	CCamera* pCam = CCameraManager::Instance().GetCurrentCamera();
	CBaseTerrain::Render(*pCam);
}