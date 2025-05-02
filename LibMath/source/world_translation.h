#pragma once

#include "matrix.h"

class CWorldTranslation
{
public:
	CWorldTranslation();

	void SetScale(const GLfloat fScale);
	void SetPosition(const GLfloat fPosX, const GLfloat fPosY, const GLfloat fPosZ);
	void SetPosition(const SVector3Df& v3Pos);
	void SetRotation(const GLfloat fRotX, const GLfloat fRotY, const GLfloat fRotZ);
	void SetRotation(const SVector3Df& v3Rot);
	void Rotate(const GLfloat fRotX, const GLfloat fRotY, const GLfloat fRotZ);
	void Rotate(const SVector3Df& v3Rot);

	GLfloat GetScale() const;
	SVector3Df GetPosition() const;
	SVector3Df GetRotation() const;
	CMatrix4Df GetMatrix() const;

	CMatrix4Df GetReversedTranslationMatrix() const;
	CMatrix4Df GetReversedRotationMatrix() const;

	SVector3Df WorldPosToLocalPos(const SVector3Df& v3WorldPos) const;
	SVector3Df WorldDirToLocalDir(const SVector3Df& v3WorldDir) const;

private:
	GLfloat m_fScale;
	SVector3Df m_vPosition;
	SVector3Df m_vRotation;
};