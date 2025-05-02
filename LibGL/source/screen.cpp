#include "stdafx.h"
#include "screen.h"
#include "window.h"

CRay CScreen::ms_Ray;


/*
 * - Always Remember, Y is for the Vertical Axis not Z.
 */
CScreen::CScreen(GLint iVerticesNum)
{
	m_bIsInitialized = true;
	m_iVertexCapacity = iVerticesNum;

	m_pLineShader = std::make_unique<CShader>("LineShader");

	glGenVertexArrays(1, &m_iVAO);
	glBindVertexArray(m_iVAO);

	glGenBuffers(1, &m_iVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);

	glGenBuffers(1, &m_iIdxBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TScreenVertex) * m_iVertexCapacity, nullptr, GL_STATIC_DRAW);

	const GLint POS_LOC = 0;
	const GLint	COL_LOC = 1;
	size_t NumFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(TScreenVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
	glEnableVertexAttribArray(COL_LOC);
	glVertexAttribPointer(COL_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(TScreenVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 4;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TODO: Initialize shaderProgram with a basic line shader
	m_pLineShader->AttachShader("shaders/line_shader.vert");
	m_pLineShader->AttachShader("shaders/line_shader.frag");
	m_pLineShader->LinkPrograms();
	m_v4DiffColor = SVector4Df(0.0f, 1.0f, 0.0f, 1.0f);
	m_iVertexCapacity = 1;
	m_matView = CCameraManager::Instance().GetCurrentCamera()->GetViewMatrix();
	m_matInverseView = CCameraManager::Instance().GetCurrentCamera()->GetViewMatrixInverse();

	ms_v3PickRayOrigin = SVector3Df(0.0f);
	ms_v3PickRayDir = SVector3Df(0.0f);

	// Terrain Variables
	//m_pTerrain = nullptr;
	m_iTerrainWidth = 0;
	m_iTerrainDepth = 0;
	m_v3InterSectionPoint = SVector3Df(0.0f);
	m_bEditingMode = false;
	m_fBrushRadius = 5.0f;
	m_fBrushStrength = 5.0f;
	m_iTerrainTexNum = 0;
	m_bTerrainRayIntersection = false;
}

void CScreen::Init()
{
	if (!m_bIsInitialized)
	{
		m_pLineShader = std::make_unique<CShader>("LineShader");

		glGenVertexArrays(1, &m_iVAO);
		glBindVertexArray(m_iVAO);

		glGenBuffers(1, &m_iVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);

		glGenBuffers(1, &m_iIdxBuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);

		glBindVertexArray(m_iVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TScreenVertex) * m_iVertexCapacity, nullptr, GL_STATIC_DRAW);

		const GLint POS_LOC = 0;
		const GLint	COL_LOC = 1;
		size_t NumFloats = 0;

		glEnableVertexAttribArray(POS_LOC);
		glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(TScreenVertex), (const void*)(NumFloats * sizeof(float)));
		NumFloats += 3;
		glEnableVertexAttribArray(COL_LOC);
		glVertexAttribPointer(COL_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(TScreenVertex), (const void*)(NumFloats * sizeof(float)));
		NumFloats += 4;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// TODO: Initialize shaderProgram with a basic line shader
		m_pLineShader->AttachShader("shaders/line_shader.vert");
		m_pLineShader->AttachShader("shaders/line_shader.frag");
		m_pLineShader->LinkPrograms();
	}
}

CScreen::~CScreen()
{
	if (m_iVAO)
	{
		glDeleteVertexArrays(1, &m_iVAO);
	}
	if (m_iVBO)
	{
		glDeleteBuffers(1, &m_iVBO);
	}
	if (m_iIdxBuf)
	{
		glDeleteBuffers(1, &m_iIdxBuf);
	}
}

void CScreen::RenderLine2d(float sx, float sz, float ex, float ez, float y)
{
	RenderLine3d(sx, sz, y, ex, ez, y);
}

void CScreen::RenderLine2d(const SVector2Df& v2StartPoint, const SVector2Df& v2EndPoint, float z)
{
	RenderLine3d(v2StartPoint.x, v2StartPoint.y, z, v2StartPoint.x, v2StartPoint.y, z);
}

void CScreen::RenderLine3d(float sx, float sy, float sz, float ex, float ey, float ez)
{
	if (m_iVAO == 0)
	{
		Init();
	}

	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	SVector3Df v3StartPoint(sx, sy, sz);
	SVector3Df v3EndPoint(ex, ey, ez);

	TScreenVertex vertices[2] = {
		{{ v3StartPoint }, { m_v4DiffColor }},
		{{ v3EndPoint }, { m_v4DiffColor }}
	};

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CScreen::RenderLine3d(const SVector3Df& v3StartPoint, const SVector3Df& v3EndPoint)
{
	if (m_iVAO == 0)
	{
		Init();
	}

	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	TScreenVertex vertices[2] = {
		{{ v3StartPoint }, { m_v4DiffColor }},
		{{ v3EndPoint }, { m_v4DiffColor }}
	};

	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CScreen::RenderCircle2d(float fx, float fy, float fz, float fRadius, int iStep, bool bHorizontal)
{
	float theta = 0.0f;
	float delta = 2.0f * static_cast<float>(M_PI) / static_cast<float>(iStep);
	float x, y, z;
	std::vector<SVector3Df> points;
	points.clear();
	points.resize(iStep);

	for (GLint count = 0; count < iStep; count++)
	{
		if (bHorizontal)
		{
			x = fx + fRadius * std::cosf(theta);
			y = fy;
			z = fz + fRadius * std::sinf(theta);
		}
		else
		{
			x = fx + fRadius * std::cosf(theta);
			y = fy + fRadius * std::sinf(theta);
			z = fz;
		}

		points[count] = SVector3Df(x, y, z);
		theta += delta;
	}

	for (GLint count = 0; count < iStep - 1; count++)
	{
		RenderLine3d(points[count].x, points[count].y, points[count].z, points[count + 1].x, points[count + 1].y, points[count + 1].z);
	}
	RenderLine3d(points[iStep - 1].x, points[iStep - 1].y, points[iStep - 1].z, points[0].x, points[0].y, points[0].z);
}

void CScreen::RenderCircle3d(float fx, float fy, float fz, float fRadius, int iStep)
{
	std::vector<SVector3Df> points;
	points.clear();
	points.resize(iStep);

	float theta = 0.0f;
	float delta = 2.0f * static_cast<float>(M_PI) / static_cast<float>(iStep);

	CMatrix4Df billBoardMat = CCameraManager::Instance().GetCurrentCamera()->GetBillBoardMatrix();

	// Generate the circle points
	for (GLint count = 0; count < iStep; count++)
	{
		SVector3Df point(fRadius * std::cosf(theta), fRadius * std::sinf(theta), 0.0f);
		points[count] = SVector3Df(billBoardMat * SVector4Df(point, 1.0f)); // Transform to camera-aligned space
		theta += delta;
	}

	// Render the circle using connected line segments
	for (GLint count = 0; count < iStep - 1; count++)
	{
		RenderLine3d(fx + points[count].x, fy + points[count].y, fz + points[count].z,
			fx + points[count + 1].x, fy + points[count + 1].y, fz + points[count + 1].z);
	}

	// Connect the last point to the first to close the circle
	RenderLine3d(fx + points[iStep - 1].x, fy + points[iStep - 1].y, fz + points[iStep - 1].z,
		fx + points[0].x, fy + points[0].y, fz + points[0].z);
}

void CScreen::RenderLinedBox3d(float sx, float sy, float sz, float ex, float ey, float ez)
{
	if (m_iVAO == 0)
	{
		Init();
	}

	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	// Define 8 corners of the box
	SVector3Df v0(sx, sy, sz);
	SVector3Df v1(ex, sy, sz);
	SVector3Df v2(sx, ey, sz);
	SVector3Df v3(ex, ey, sz);
	SVector3Df v4(sx, sy, ez);
	SVector3Df v5(ex, sy, ez);
	SVector3Df v6(sx, ey, ez);
	SVector3Df v7(ex, ey, ez);

	// Use m_v4DiffColor for all vertices (assumed to be set to a visible color)
	TScreenVertex vertices[8] = {
		{ v0, m_v4DiffColor },
		{ v1, m_v4DiffColor },
		{ v2, m_v4DiffColor },
		{ v3, m_v4DiffColor },
		{ v4, m_v4DiffColor },
		{ v5, m_v4DiffColor },
		{ v6, m_v4DiffColor },
		{ v7, m_v4DiffColor }
	};

	// Define indices for the 12 edges (24 indices total)
	GLuint indices[24] = {
		// Bottom face edges
		0, 1,  1, 3,  3, 2,  2, 0,
		// Top face edges
		4, 5,  5, 7,  7, 6,  6, 4,
		// Vertical edges
		0, 4,  1, 5,  2, 6,  3, 7
	};

	UpdateVertexBuffer(vertices, 8);

	// Bind the VAO and update the vertex buffer with new vertex data
	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Draw using indices; 8 indices means 4 line segments (GL_LINES uses 2 indices per line)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINES, 0, 8);

	// Unbind buffers and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CScreen::RenderLinedSquare3d(float sx, float sy, float sz, float ex, float ey, float ez)
{
	if (m_iVAO == 0)
	{
		Init();
	}

	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	// Define 4 corners of the square plane.
	// Here, we assume the plane is axis-aligned in XY and uses the z value of the first corner (sz).
	SVector3Df v0(sx, sy, sz); // bottom-left
	SVector3Df v1(ex, sy, sz); // bottom-right
	SVector3Df v2(sx, ey, sz); // top-left
	SVector3Df v3(ex, ey, sz); // top-right

	// Create a vertex array for 4 vertices using a common color (m_v4DiffColor)
	TScreenVertex vertices[4] = {
		{ v0, m_v4DiffColor },
		{ v1, m_v4DiffColor },
		{ v2, m_v4DiffColor },
		{ v3, m_v4DiffColor }
	};

	// Define indices for the square edges (4 edges, 8 indices total)
	GLuint indices[8] = {
		0, 1, // bottom edge
		1, 3, // right edge
		3, 2, // top edge
		2, 0  // left edge
	};

	UpdateVertexBuffer(vertices, 4);

	// Bind the VAO and update the vertex buffer with new vertex data
	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Draw using indices; 8 indices means 4 line segments (GL_LINES uses 2 indices per line)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Draw the square edges using the index buffer (8 indices = 4 line segments)
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

	// Unbind buffers and VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CScreen::RenderBox3d(float sx, float sy, float sz, float ex, float ey, float ez)
{
	// Ensure the VAO is initialized
	if (m_iVAO == 0)
	{
		Init();
	}

	// Use your line (or general) shader and set the view-projection matrix
	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	// Define the 8 corners of the cube
	SVector3Df v0(sx, sy, sz);
	SVector3Df v1(ex, sy, sz);
	SVector3Df v2(sx, ey, sz);
	SVector3Df v3(ex, ey, sz);
	SVector3Df v4(sx, sy, ez);
	SVector3Df v5(ex, sy, ez);
	SVector3Df v6(sx, ey, ez);
	SVector3Df v7(ex, ey, ez);

	// Create an array of vertices using a common color (m_v4DiffColor)
	TScreenVertex vertices[8] = {
		{ v0, m_v4DiffColor },
		{ v1, m_v4DiffColor },
		{ v2, m_v4DiffColor },
		{ v3, m_v4DiffColor },
		{ v4, m_v4DiffColor },
		{ v5, m_v4DiffColor },
		{ v6, m_v4DiffColor },
		{ v7, m_v4DiffColor }
	};

	// Define indices for 12 triangles (36 indices total)
	// Each face of the cube is made up of two triangles.
	GLuint indices[36] = {
		// Front face (v0,v1,v3,v2)
		0, 1, 3,
		3, 2, 0,
		// Back face (v4,v6,v7,v5)
		4, 6, 7,
		7, 5, 4,
		// Left face (v0,v2,v6,v4)
		0, 2, 6,
		6, 4, 0,
		// Right face (v1,v5,v7,v3)
		1, 5, 7,
		7, 3, 1,
		// Top face (v2,v3,v7,v6)
		2, 3, 7,
		7, 6, 2,
		// Bottom face (v0,v4,v5,v1)
		0, 4, 5,
		5, 1, 0
	};

	UpdateVertexBuffer(vertices, 8);

	// Update the vertex buffer with the vertex data
	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Update the index buffer with our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Draw the cube using triangles (36 indices = 12 triangles)
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Unbind buffers and VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CScreen::RenderSquare3d(float sx, float sy, float sz, float ex, float ey, float ez)
{
	if (m_iVAO == 0)
	{
		Init();
	}

	m_pLineShader->Use();
	m_pLineShader->setMat4("ViewMatrix", CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

	// Define 4 corners of the square plane.
	// Here, we assume the plane is axis-aligned in XY and uses the z value of the first corner (sz).
	SVector3Df v0(sx, sy, sz); // bottom-left
	SVector3Df v1(ex, sy, sz); // bottom-right
	SVector3Df v2(sx, ey, sz); // top-left
	SVector3Df v3(ex, ey, sz); // top-right

	// Create a vertex array for 4 vertices using a common color (m_v4DiffColor)
	TScreenVertex vertices[4] = {
		{ v0, m_v4DiffColor },
		{ v1, m_v4DiffColor },
		{ v2, m_v4DiffColor },
		{ v3, m_v4DiffColor }
	};

	// Define indices for the square edges (4 edges, 8 indices total)
	GLuint indices[6] = {
		// Front face (v0,v1,v3,v2)
		0, 1, 3,
		3, 2, 0,
	};

	UpdateVertexBuffer(vertices, 4);

	glDisable(GL_CULL_FACE);

	// Bind the VAO and update the vertex buffer with new vertex data
	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Draw using indices; 8 indices means 4 line segments (GL_LINES uses 2 indices per line)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Draw the cube using triangles (36 indices = 12 triangles)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind buffers and VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

void CScreen::UpdateVertexBuffer(const TScreenVertex* vertices, size_t vertexCount)
{
	if (vertexCount > m_iVertexCapacity)
	{
		// Increase capacity (e.g. double it or set to vertexCount)
		m_iVertexCapacity = static_cast<GLint>(vertexCount);
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TScreenVertex) * m_iVertexCapacity, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// Now update the buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TScreenVertex) * vertexCount, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const SVector4Df& CScreen::GetDiffuseColor()
{
	return (m_v4DiffColor);
}

void CScreen::SetDiffuseColor(const SVector4Df& v4DiffuseColor)
{
	m_v4DiffColor = v4DiffuseColor;
}

void CScreen::SetDiffuseColor(float r, float g, float b, float a)
{
	m_v4DiffColor = SVector4Df(r, g, b, a);
}

static float TriangleArea(const SVector3Df p1, const SVector3Df p2, const SVector3Df p3)
{
	/* Calculate distance between p1 and p2, returns the length of A */
	const float a = std::sqrt((p2.x - p1.x) * (p2.x - p1.x) +
		(p2.y - p1.y) * (p2.y - p1.y) +
		(p2.z - p1.z) * (p2.z - p1.z));

	/* Calculate distance between p2 and p3, returns the length of B */
	const float b = std::sqrt((p3.x - p2.x) * (p3.x - p2.x) +
		(p3.y - p2.y) * (p3.y - p2.y) +
		(p3.z - p2.z) * (p3.z - p2.z));

	/* Calculate distance between p3 and p1, returns the length of C */
	const float c = std::sqrt((p1.x - p3.x) * (p1.x - p3.x) +
		(p1.y - p3.y) * (p1.y - p3.y) +
		(p1.z - p3.z) * (p1.z - p3.z));

	/* Calcualte semi-preimeter which is the half the sum of three sides lengths */
	const float semiPremiter = (a + b + c) / 2.0f;

	/* Calcualte the Area using Heron Formula which as it looks now */
	return (std::sqrt(semiPremiter * ((semiPremiter - a) * (semiPremiter - b) * (semiPremiter - c))));
}

SVector3Df CScreen::GetRayFromScreenCenter()
{
	SSceneElements* scene = CObject::pScene;
	if (!scene)
	{
		sys_err("CSkyBox::Render: Scene Is NULL!!");
		return SVector3Df(0.0f, 0.0f, 0.0f);
	}

	float screenX = static_cast<float>(CWindow::Instance().GetWidth()) / 2.0f;
	float screenY = static_cast<float>(CWindow::Instance().GetHeight()) / 2.0f;

	// Convert screen space to Normalized Device Coordinates (NDC)
	float ndcX = (2.0f * screenX) / CWindow::Instance().GetWidth() - 1.0f;
	float ndcY = 1.0f - (2.0f * screenY) / CWindow::Instance().GetHeight(); // Flip Y for OpenGL

	SVector4Df rayClip(ndcX, ndcY, -1.0f, 1.0f);

	// Inverse projection to eye (view) space
	CMatrix4Df invProj = scene->pCamera->GetProjectionMat().Inverse();
	SVector4Df rayEye = invProj * rayClip;
	rayEye = SVector4Df(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// Inverse view to world space
	CMatrix4Df invView = scene->pCamera->GetViewMatrix().Inverse();
	SVector4Df rayWorld = invView * rayEye;

	SVector3Df rayDir = SVector3Df(rayWorld.x, rayWorld.y, rayWorld.z).normalize();

	return SVector3Df(rayDir.x, rayDir.y, rayDir.z);
}

/*bool CScreen::GetRayTerrainIntersection(const SVector3Df& rayOrigin, const SVector3Df& rayDir,
	SVector3Df& intersectionPoint)
{
	CGeoMipGrid::TVertex vert;
	float closestT = std::numeric_limits<float>::infinity();
	bool hit = false;

	GLint iDepth = GetTerrain()->GetDepth();
	GLint iWidth = GetTerrain()->GetWidth();
	GLint iPatchSize = GetTerrain()->GetPatchSize();

	for (GLint z = 0; z < iDepth - 1; z += (iPatchSize - 1))
	{
		for (GLint x = 0; x < iWidth - 1; x += (iPatchSize - 1))
		{
			GLint iBaseVertex = z * iWidth + x;
			GLint iNumIndices = GetTerrain()->GetGeoMipGrid()->GetLodInfo()[0].LodInfo[0][0][0][0].iCount;
			for (GLint i = 0; i < iNumIndices; i += 3)
			{
				GLuint uiIndex0 = iBaseVertex + m_vTerrainIndices[i];
				GLuint uiIndex1 = iBaseVertex + m_vTerrainIndices[i + 1];
				GLuint uiIndex2 = iBaseVertex + m_vTerrainIndices[i + 2];

				SVector3Df v0 = m_vTerrainVertices[uiIndex0].v3Pos;
				SVector3Df v1 = m_vTerrainVertices[uiIndex1].v3Pos;
				SVector3Df v2 = m_vTerrainVertices[uiIndex2].v3Pos;

				float t, u, v;
				if (IntersectTriangle(rayOrigin, rayDir, v0, v1, v2, &u, &v, &t))
				{
					// If the ray intersects the triangle and it's the closest intersection, update the closest point
					if (t < closestT)
					{
						closestT = t;
						intersectionPoint = rayOrigin + rayDir * t;  // Calculate intersection point
						hit = true;
					}
				}
			}
		}
	}

	return hit;
}*/

bool CScreen::RaycastHeightmap(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint)
{
	if (rayDir.y >= 0.0f)
		return false; // Only works for rays pointing downward

	// Find where the ray crosses the ground plane
	float t = -rayOrigin.y / rayDir.y;
	SVector3Df groundPoint = rayOrigin + rayDir * t;

	GLint width = CBaseTerrain::Instance().GetWidth();
	GLint depth = CBaseTerrain::Instance().GetDepth();

	float fx = groundPoint.x;
	float fz = groundPoint.z;

	if (fx < 0.0f || fx >= width - 1 || fz < 0.0f || fz >= depth - 1)
		return false; // Out of terrain bounds

	GLint ix = static_cast<GLint>(fx);
	GLint iz = static_cast<GLint>(fz);

	float localX = fx - ix;
	float localZ = fz - iz;

	// Get 4 corner heights
	float h00 = CBaseTerrain::Instance().GetVertices()[iz * width + ix].m_v3Pos.y;
	float h10 = CBaseTerrain::Instance().GetVertices()[iz * width + (ix + 1)].m_v3Pos.y;
	float h01 = CBaseTerrain::Instance().GetVertices()[(iz + 1) * width + ix].m_v3Pos.y;
	float h11 = CBaseTerrain::Instance().GetVertices()[(iz + 1) * width + (ix + 1)].m_v3Pos.y;

	// Bilinear interpolation
	float h0 = h00 + (h10 - h00) * localX;
	float h1 = h01 + (h11 - h01) * localX;
	float height = h0 + (h1 - h0) * localZ;

	if (groundPoint.y <= height)
	{
		intersectionPoint = SVector3Df(groundPoint.x, height, groundPoint.z);
		return true;
	}

	return false;
}

bool CScreen::RaycastHeightmapFast(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint)
{
	if (rayDir.y >= 0.0f)
		return false; // Only works for downward rays

	const float stepSize = 1.0f;
	const int maxSteps = 102400;    // Max distance = 1024 units

	const GLint width = m_iTerrainWidth;
	const GLint depth = m_iTerrainDepth;
	const float maxX = static_cast<float>(width - 2);
	const float maxZ = static_cast<float>(depth - 2);

	SVector3Df currentPos = rayOrigin;

	for (int step = 0; step < maxSteps; ++step)
	{
		float fx = std::clamp(currentPos.x, 0.0f, maxX);
		float fz = std::clamp(currentPos.z, 0.0f, maxZ);

		GLint ix = static_cast<GLint>(fx);
		GLint iz = static_cast<GLint>(fz);

		float localX = fx - ix;
		float localZ = fz - iz;

		const auto& v00 = CBaseTerrain::Instance().GetVertices()[iz * width + ix].m_v3Pos.y;
		const auto& v10 = CBaseTerrain::Instance().GetVertices()[iz * width + (ix + 1)].m_v3Pos.y;
		const auto& v01 = CBaseTerrain::Instance().GetVertices()[(iz + 1) * width + ix].m_v3Pos.y;
		const auto& v11 = CBaseTerrain::Instance().GetVertices()[(iz + 1) * width + (ix + 1)].m_v3Pos.y;

		float h0 = v00 + (v10 - v00) * localX;
		float h1 = v01 + (v11 - v01) * localX;
		float height = h0 + (h1 - h0) * localZ;

		if (currentPos.y <= height)
		{
			// Optional: binary refinement for smoother brush edge
			SVector3Df backtrack = currentPos - rayDir * stepSize;
			for (int j = 0; j < 4; ++j)
			{
				SVector3Df mid = (currentPos + backtrack) * 0.5f;
				float mx = std::clamp(mid.x, 0.0f, maxX);
				float mz = std::clamp(mid.z, 0.0f, maxZ);
				GLint mix = static_cast<GLint>(mx);
				GLint miz = static_cast<GLint>(mz);
				float lx = mx - mix;
				float lz = mz - miz;

				float mh0 = CBaseTerrain::Instance().GetVertices()[miz * width + mix].m_v3Pos.y +
					(CBaseTerrain::Instance().GetVertices()[miz * width + (mix + 1)].m_v3Pos.y -
						CBaseTerrain::Instance().GetVertices()[miz * width + mix].m_v3Pos.y) * lx;

				float mh1 = CBaseTerrain::Instance().GetVertices()[(miz + 1) * width + mix].m_v3Pos.y +
					(CBaseTerrain::Instance().GetVertices()[(miz + 1) * width + (mix + 1)].m_v3Pos.y -
						CBaseTerrain::Instance().GetVertices()[(miz + 1) * width + mix].m_v3Pos.y) * lx;

				float midHeight = mh0 + (mh1 - mh0) * lz;

				if (mid.y <= midHeight)
					currentPos = mid;
				else
					backtrack = mid;
			}

			intersectionPoint = currentPos;
			intersectionPoint.y = height;
			return true;
		}

		currentPos += rayDir * stepSize;

		// early out if ray is too far
		if (currentPos.x < 0.0f || currentPos.x >= width || currentPos.z < 0.0f || currentPos.z >= depth)
			return false;
	}

	return false;
}

void CScreen::SetCursorPosition(GLint iX, GLint iY, GLint hRes, GLint vRes)
{
	// Retrieve inverse view and inverse projection matrices
	auto inverseView = CCameraManager::Instance().GetCurrentCamera()->GetViewMatrixInverse();
	auto inverseProj = CCameraManager::Instance().GetCurrentCamera()->GetProjectionMat().Inverse();

	// Convert screen coordinates to normalized device coordinates (NDC) [-1,1]
	SVector3Df v3NDC;
	v3NDC.x = (2.0f * static_cast<float>(iX)) / static_cast<float>(hRes) - 1.0f;  // X range [-1, 1]
	v3NDC.y = 1.0f - (2.0f * static_cast<float>(iY)) / static_cast<float>(vRes);  // Y range [-1, 1]
	v3NDC.z = -1.0f; // Near plane (for ray direction)

	// Convert NDC to eye space (clip space -> world space)
	SVector4Df eyePos = inverseProj * SVector4Df(v3NDC.x, v3NDC.y, v3NDC.z, 1.0f);
	eyePos.x /= eyePos.w; // Perspective divide
	eyePos.y /= eyePos.w; // Perspective divide
	eyePos.z /= eyePos.w; // Perspective divide

	// Convert to world space (view space -> world space)
	auto worldDir = inverseView * SVector4Df(eyePos.x, eyePos.y, -1.0f, 0.0f); // w = 0 for direction

	// Normalize ray direction
	SVector3Df rayDir = SVector3Df(worldDir.x, worldDir.y, worldDir.z);

	ms_v3PickRayOrigin = SVector3Df(
		inverseView.mat4[0][3], // Column-major indexing
		inverseView.mat4[1][3],
		inverseView.mat4[2][3]
	);

	rayDir = -rayDir;

	// Store the ray
	ms_Ray.SetStartPoint(CCameraManager::Instance().GetCurrentCamera()->GetPosition());
	ms_Ray.SetDirection(CCameraManager::Instance().GetCurrentCamera()->GetDirection(), 51200.0f);

	//if (m_pTerrain->GetQuadList().GetRayTerrainIntersection(ms_Ray.GetStartPoint(), ms_Ray.GetEndPoint(), intersectionPoint))
	if (GetEditingMode())
	{
		if (RaycastHeightmap(ms_Ray.GetStartPoint(), ms_Ray.GetDirection(), m_v3InterSectionPoint))
		{
			CBaseTerrain::Instance().GetTerrainShader()->Use();
			CBaseTerrain::Instance().GetTerrainShader()->setVec3("u_HitPosition", m_v3InterSectionPoint);
			CBaseTerrain::Instance().GetTerrainShader()->setFloat("u_HitRadius", m_fBrushRadius);
			CBaseTerrain::Instance().GetTerrainShader()->setBool("u_HasHit", true);				// enable hit visualization
			m_bTerrainRayIntersection = true;
		}
		else
		{
			CBaseTerrain::Instance().GetTerrainShader()->Use();
			CBaseTerrain::Instance().GetTerrainShader()->setBool("u_HasHit", false);			// enable hit visualization
			m_bTerrainRayIntersection = false;
		}
	}
}

void CScreen::Update()
{
	// Render Anything
	//RenderCircle2d(m_v3InterSectionPoint.x, m_pTerrain->GetWorldHeight(m_v3InterSectionPoint.x, m_v3InterSectionPoint.z) + 5.0f, m_v3InterSectionPoint.z, 50.0f, 128, true);
}

bool CScreen::GetCursorPosition(SVector3Df* v3Pos)
{
	if (!GetCursorXYPosition(&v3Pos->x, &v3Pos->y))
	{
		return false;
	}
	if (!GetCursorZPosition(&v3Pos->z))
	{
		return false;
	}

	return true;
}

bool CScreen::GetCursorPosition(float* px, float* py, float* pz)
{
	if (!GetCursorXYPosition(px, py))
	{
		return false;
	}
	if (!GetCursorZPosition(pz))
	{
		return false;
	}

	return true;
}

bool CScreen::GetCursorXYPosition(float* px, float* py)
{
	SVector3Df eyePos = CCameraManager::Instance().GetCurrentCamera()->GetPosition();

	float fScaleFactor = 90000000.0f;

	SVector3Df posVertices[4] = {
		{ eyePos.x - fScaleFactor, eyePos.y + fScaleFactor, 0.0f },
		{ eyePos.x - fScaleFactor, eyePos.y - fScaleFactor, 0.0f },
		{ eyePos.x + fScaleFactor, eyePos.y + fScaleFactor, 0.0f },
		{ eyePos.x + fScaleFactor, eyePos.y - fScaleFactor, 0.0f }
	};

	static const int indices[6] = { 0, 2, 1, 2, 3, 1 };

	float u, v, t;
	for (int i = 0; i < 2; ++i)
	{
		if (IntersectTriangle(ms_v3PickRayOrigin, ms_v3PickRayDir,
			posVertices[indices[i * 3 + 0]],
			posVertices[indices[i * 3 + 1]],
			posVertices[indices[i * 3 + 2]],
			&u, &v, &t))
		{
			*px = ms_v3PickRayOrigin.x + ms_v3PickRayDir.x * t;
			*py = ms_v3PickRayOrigin.y + ms_v3PickRayDir.y * t;
			return true;
		}
	}
	return false;
}

bool CScreen::GetCursorZPosition(float* pz)
{
	SVector3Df eyePos = CCameraManager::Instance().GetCurrentCamera()->GetPosition();

	float fScaleFactor = 90000000.0f;

	SVector3Df posVertices[4] = {
		{ eyePos.x - fScaleFactor, 0.0f, eyePos.z + fScaleFactor },
		{ eyePos.x - fScaleFactor, 0.0f, eyePos.z - fScaleFactor },
		{ eyePos.x + fScaleFactor, 0.0f, eyePos.z + fScaleFactor },
		{ eyePos.x + fScaleFactor, 0.0f, eyePos.z - fScaleFactor }
	};

	static const int indices[6] = { 0, 2, 1, 2, 3, 1 };

	float u, v, t;
	for (int i = 0; i < 2; ++i)
	{
		if (IntersectTriangle(ms_v3PickRayOrigin, ms_v3PickRayDir,
			posVertices[indices[i * 3 + 0]],
			posVertices[indices[i * 3 + 1]],
			posVertices[indices[i * 3 + 2]],
			&u, &v, &t))
		{
			*pz = ms_v3PickRayOrigin.z + ms_v3PickRayDir.z * t;
			return true;
		}
	}
	return false;
}

void CScreen::GetPickingPosition(float t, float* x, float* y, float* z)
{
	*x = ms_v3PickRayOrigin.x + ms_v3PickRayDir.x * t;
	*y = ms_v3PickRayOrigin.y + ms_v3PickRayDir.y * t;
	*z = ms_v3PickRayOrigin.z + ms_v3PickRayDir.z * t;
}


void CScreen::ApplyTerrainBrush(EBrushType eBrushType)
{
	if (GetEditingMode() && m_bTerrainRayIntersection )
	{
		// Just pass the exact world coordinates — no grid conversion here
		if (eBrushType >= BRUSH_TYPE_UP && eBrushType <= BRUSH_TYPE_NOISE)
		{
			CBaseTerrain::Instance().GetGeoMipGrid()->ApplyTerrainBrush_World(eBrushType, m_v3InterSectionPoint.x, m_v3InterSectionPoint.z, m_fBrushRadius, m_fBrushStrength);
		}
		else
		{
			TBrushParams brush{};
			brush.v2WorldPos = SVector2Df(m_v3InterSectionPoint.x, m_v3InterSectionPoint.z);
			brush.fRadius = m_fBrushRadius;
			brush.fStrength = 1.0f;
			brush.iSelectedTexChannel = CBaseTerrain::Instance().GetGeoMipGrid()->GetCurrentTextureIndex(); // G channel (for grass, e.g.)
			CBaseTerrain::Instance().GetGeoMipGrid()->PaintSplatmap(eBrushType, brush);
		}
	}
}

void CScreen::SetEditingMode(bool bActive)
{
	m_bEditingMode = bActive;
}

bool CScreen::GetEditingMode() const
{
	return (m_bEditingMode);
}

void CScreen::SetBrushRadius(float fVal)
{
	m_fBrushRadius = fVal;
}

float CScreen::GetBrushRadius() const
{
	return (m_fBrushRadius);
}

void CScreen::SetBrushStrength(float fVal)
{
	m_fBrushStrength = fVal;
}

float CScreen::GetBrushStrength() const
{
	return (m_fBrushStrength);
}

void CScreen::SetTextureNum(GLint iTexNum)
{
	m_iTerrainTexNum = iTexNum;
}

GLint CScreen::GetTextureNum() const
{
	return (m_iTerrainTexNum);
}

SVector3Df CScreen::GetIntersectionPoint() const
{
	return (m_v3InterSectionPoint);
}