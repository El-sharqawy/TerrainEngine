#pragma once

#include "terrain.h"

class CMidPointTerrain : public CBaseTerrain
{
public:
	CMidPointTerrain() = default;

	void CreateMidPointTerrain(GLint iTerrainSize, GLint iNumPatches, float fRoughness, float fMinHeight, float fMaxHeight);

	virtual void Render();
	virtual void SetGUI();

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update() {}

protected:
	void CreateMidPointTerrainF32(float fRoughness);
	void DiamondStep(GLint iRectSize, float fCurHeight);
	void SquareStep(GLint iRectSize, float fCurHeight);
};