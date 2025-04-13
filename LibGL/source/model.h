#pragma once

#include "../../LibMath/source/vectors.h"
#include "../../LibMath/source/world_translation.h"
#include "texture.h"

typedef struct SVertex
{
	SVector3Df v3Pos;
	SVector3Df v3Normals;
	SVector2Df v2Texture;

	SVertex() = default;

	SVertex(const SVector3Df& vPos, const SVector3Df& vNormals, const SVector2Df& vTexture)
	{
		v3Pos = vPos;
		v3Normals = vNormals;
		v2Texture = vTexture;
	}
} TVertex;

typedef struct SPBRMaterial
{
	float m_fRoughness;
	bool m_bIsMetal;
	SVector3Df m_v3Color;
	CTexture* m_pAlbedo;
	CTexture* m_pRoughness;
	CTexture* m_pMetallic;
	CTexture* m_pNormalMap;

	SPBRMaterial()
	{
		m_fRoughness = 0.0f;
		m_bIsMetal = false;
		m_v3Color = SVector3Df(0.0f, 0.0f, 0.0f);
		m_pAlbedo = nullptr;
		m_pRoughness = nullptr;
		m_pMetallic = nullptr;
		m_pNormalMap = nullptr;
	}

} TPBRMaterial;

typedef struct SMaterial
{
	std::string m_stName;
	TPBRMaterial m_sPBRMaterial;

	SVector4Df m_v4AmbientColor;
	SVector4Df m_v4DiffuseColor;
	SVector4Df m_v4SpecularColor;

	CTexture* m_pDiffuseMap;
	CTexture* m_pSpecularMap;

	float m_fTransparency;
	float m_fAlpha;

	SMaterial()
	{
		m_stName = "Material";
		m_sPBRMaterial = {};
		m_v4AmbientColor = SVector4Df(0.0f, 0.0f, 0.0f, 0.0f);
		m_v4DiffuseColor = SVector4Df(0.0f, 0.0f, 0.0f, 0.0f);
		m_v4SpecularColor = SVector4Df(0.0f, 0.0f, 0.0f, 0.0f);

		m_pDiffuseMap = nullptr;
		m_pSpecularMap = nullptr;

		m_fTransparency = 1.0f;
		m_fAlpha = 0.0f;
	}

	~SMaterial()
	{
		if (m_pDiffuseMap)
		{
			delete m_pDiffuseMap;
		}

		if (m_pSpecularMap)
		{
			delete m_pSpecularMap;
		}
	}

} TMaterial;

class CModel
{
public:
	CModel() = default;
	~CModel() = default;

	void SetScale(const float& fScale);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetPosition(const SVector3Df& v3Pos);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetRotation(const SVector3Df& v3Rot);
	
	SVector3Df GetPosition() const;
	SVector3Df GetRotation() const;
	float GetScale() const;
	CWorldTranslation& GetWorldTranslation();
	CMatrix4Df GetWorldMatrix();

protected:
	CWorldTranslation m_WorldTranslation;
};