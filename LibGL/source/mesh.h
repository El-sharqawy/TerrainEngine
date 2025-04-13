#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <map>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include "../../LibMath/source/vectors.h"
#include "../../LibMath/source/world_translation.h"
#include "model.h"

#define INVALID_MATERIAL 0xFFFFFFFF
#define ASSIMP_LOAD_FLAGS (aiProcess_JoinIdenticalVertices |    \
                           aiProcess_Triangulate |              \
                           aiProcess_GenSmoothNormals |         \
                           aiProcess_LimitBoneWeights |         \
                           aiProcess_SplitLargeMeshes |         \
                           aiProcess_ImproveCacheLocality |     \
                           aiProcess_RemoveRedundantMaterials | \
                           aiProcess_FindDegenerates |          \
                           aiProcess_FindInvalidData |          \
                           aiProcess_GenUVCoords |              \
						   aiProcess_FlipUVs |					\
                           aiProcess_CalcTangentSpace)

#define POSITION_LOCATION  0
#define NORMALS_LOCATION    1
#define TEX_COORDS_LOCATION 2

#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define USE_MESH_OPRIMIZER
#define ENABLE_PRINT_MESH_DATA

class CMesh : public CModel
{
public:
	CMesh() = default;
	~CMesh();

	bool LoadMesh(const std::string& stFileName);
	void Render();
	void Render(GLuint uiDrawIndex, GLuint uiPrimID);
	void Render(GLuint uiNumInstances, const CMatrix4Df* matWVP, const CMatrix4Df* matWorld);
	const TMaterial& GetMaterial();
	TPBRMaterial& GetPBRMaterial();
	void GetLeadingVertex(GLuint uiDrawIndex, GLuint uiPrimID, SVector3Df& Vertex);
	void SetPBR(bool bIsPBR);
	bool IsPBR() const;

protected:
	void Clear();
	virtual void ReserveSpace(GLuint uiNumVertices, GLuint uiNumIndices);
	virtual void InitSingleMesh(const aiMesh* pMesh);
	virtual void InitSingleMeshOptimized(GLuint uiMeshIndex, const aiMesh* pMesh);
	virtual void PopulateBuffers();
	virtual void PopulateBuffersDSA();
	virtual void PopulateBuffersNonDSA();

	typedef struct SMeshEntry
	{
		SMeshEntry()
		{
			uiBaseVertex = 0;
			uiBaseIndex = 0;
			uiNumIndices = 0;
			uiMaterialIndex = INVALID_MATERIAL;
		}

		GLuint uiBaseVertex;
		GLuint uiBaseIndex;
		GLuint uiNumIndices;
		GLuint uiMaterialIndex;
	} TMeshEntry;

	enum EBufferType
	{
		INDEX_BUFFER,
		VERTEX_BUFFER,
		WVP_MAT_BUFFER = 2,  // required only for instancing
		WORLD_MAT_BUFFER = 3,  // required only for instancing
		NUM_BUFFERS = 4
	};

	std::vector<TMeshEntry> m_vMeshes;
	std::vector<GLuint> m_vIndices;

	const aiScene* m_pScene;
	CMatrix4Df m_matGlobalInverseTransform;

	GLuint m_uiVAO;
	GLuint m_uiBuffers[NUM_BUFFERS];

private:
	bool InitFromScene(const aiScene* pScene, const std::string& stFileName);
	void ConvertVerticesAndIndices(const aiScene* pScene, GLuint& uiNumVertices, GLuint& uiNumIndices);
	void InitAllMeshes(const aiScene* pScene);
	void OptimizeMesh(GLint iMeshIndex, std::vector<TVertex>& vVertices, std::vector<GLuint>& vIndices);
	bool InitMaterials(const aiScene* pScene, const std::string& stFileName);
	void LoadTextures(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);

	void LoadDiffuseTexture(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);
	void LoadDiffuseTextureEmbeded(const aiTexture* pTexture, GLint iMaterialIndex);
	void LoadDiffuseTextureFromFile(const std::string& stDirectory, const aiString& stPath, GLint iMaterialIndex);

	void LoadSpecularTexture(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);
	void LoadSpecularTextureEmbeded(const aiTexture* pTexture, GLint iMaterialIndex);
	void LoadSpecularTextureFromFile(const std::string& stDirectory, const aiString& stPath, GLint iMaterialIndex);

	void LoadAlbedoTexture(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);
	void LoadAlbedoTextureEmbeded(const aiTexture* pTexture, GLint iMaterialIndex);
	void LoadAlbedoTextureFromFile(const std::string& stDirectory, const aiString& stPath, GLint iMaterialIndex);

	void LoadMetalnessTexture(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);
	void LoadMetalnessTextureEmbeded(const aiTexture* pTexture, GLint iMaterialIndex);
	void LoadMetalnessTextureFromFile(const std::string& stDirectory, const aiString& stPath, GLint iMaterialIndex);

	void LoadRoughnessTexture(const std::string& stDirectory, const aiMaterial* pMaterial, GLint iMaterialIndex);
	void LoadRoughnessTextureEmbeded(const aiTexture* pTexture, GLint iMaterialIndex);
	void LoadRoughnessTextureFromFile(const std::string& stDirectory, const aiString& stPath, GLint iMaterialIndex);

	void LoadColors(const aiMaterial* pMaterial, GLint iMaterialIndex);
	void SetupRenderMaterialsPBR();
	void SetupRenderMaterialsPhong(GLuint uiMeshIndex, GLuint uiMaterialIndex);

	std::vector<TMaterial> m_vMaterials;

	// Temporary space for vertex stuff before we load them into the GPU
	std::vector<TVertex> m_vVertices;

	Assimp::Importer m_Importer;
	bool m_bIsPBR;
};