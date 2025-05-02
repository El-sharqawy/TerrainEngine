#pragma once

#include <glad/glad.h>
#include <memory>
#include "shader.h"
#include "../../LibMath/source/ray.h"
#include "../../LibTerrain/source/terrain.h"
#include "../../LibTerrain/source/object.h"

class CScreen : public CSingleton<CScreen>, public CObject
{
public:
	CScreen(GLint iVerticesNum = 1024);
	~CScreen();

	void RenderLine3d(float sx, float sy, float sz, float ex, float ey, float ez);
	void RenderLine3d(const SVector3Df& v3StartPoint, const SVector3Df& v3EndPoint);

	void RenderLine2d(float sx, float sz, float ex, float ez, float y);
	void RenderLine2d(const SVector2Df& v2StartPoint, const SVector2Df& v2EndPoint, float y);

	void RenderCircle2d(float fx, float fy, float fz, float fRadius, int iStep, bool bHorizontal);
	void RenderCircle3d(float fx, float fy, float fz, float fRadius, int iStep);

	void RenderLinedBox3d(float sx, float sy, float sz, float ex, float ey, float ez);
	void RenderLinedSquare3d(float sx, float sy, float sz, float ex, float ey, float ez);

	void RenderBox3d(float sx, float sy, float sz, float ex, float ey, float ez);
	void RenderSquare3d(float sx, float sy, float sz, float ex, float ey, float ez);

	void Init();

	const SVector4Df& GetDiffuseColor();
	void SetDiffuseColor(const SVector4Df& v4DiffuseColor);
	void SetDiffuseColor(float r, float g, float b, float a);

	void SetCursorPosition(GLint iX, GLint iY, GLint hRes, GLint vRes);

	bool GetCursorXYPosition(float* px, float* py);
	bool GetCursorZPosition(float* pz);
	void GetPickingPosition(float t, float* x, float* y, float* z);
	bool GetCursorPosition(float* px, float* py, float* pz);
	bool GetCursorPosition(SVector3Df* v3Pos);

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update();

	static CRay& GetCRay()
	{
		return ms_Ray;
	}

	SVector3Df GetRayFromScreenCenter();
 
	virtual void Render() {}
	virtual void SetGUI() {}

protected:
	typedef struct SScreenVertex
	{
		SVector3Df v3Pos;
		SVector4Df v4Color;
	} TScreenVertex;

	void UpdateVertexBuffer(const TScreenVertex* vertices, size_t vertexCount);

public: // terrrain
	bool RaycastHeightmap(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint);
	bool RaycastHeightmapFast(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint);

	void ApplyTerrainBrush(EBrushType eBrushType);

	void SetEditingMode(bool bActive);
	bool GetEditingMode() const;

	void SetBrushRadius(float fVal);
	float GetBrushRadius() const;
	void SetBrushStrength(float fVal);
	float GetBrushStrength() const;

	void SetTextureNum(GLint iTexNum);
	GLint GetTextureNum() const;

	SVector3Df GetIntersectionPoint() const;

private:
	GLuint m_iVAO; // Vertex Array Object
	GLuint m_iVBO; // Vertex Buffer Object
	GLuint m_iIdxBuf; // Index Buffer
	GLint m_iVertexCapacity;

	std::unique_ptr<CShader> m_pLineShader;
	GLboolean m_bIsInitialized;
	SVector4Df m_v4DiffColor;

	CMatrix4Df m_matView;
	CMatrix4Df m_matInverseView;

	static CRay ms_Ray;
	SVector3Df ms_v3PickRayOrigin;
	SVector3Df ms_v3PickRayDir;

	// Terrain Edit Methods
	GLint m_iTerrainWidth;
	GLint m_iTerrainDepth;
	bool m_bEditingMode;
	bool m_bTerrainBrush;
	bool m_bTerrainTextureBrush;

	SVector3Df m_v3InterSectionPoint;
	GLfloat m_fBrushRadius;
	GLfloat m_fBrushStrength;
	GLint m_iTerrainTexNum;

	GLboolean m_bTerrainRayIntersection;
};
