#pragma once

#include "matrix.h"

class CWorldTranslation
{
public:
	CWorldTranslation();

	void SetScale(const float fScale);
	void SetPosition(const float fPosX, const float fPosY, const float fPosZ);
	void SetPosition(const SVector3Df& v3Pos);
	void SetRotation(const float fRotX, const float fRotY, const float fRotZ);
	void SetRotation(const SVector3Df& v3Rot);
	void Rotate(const float fRotX, const float fRotY, const float fRotZ);
	void Rotate(const SVector3Df& v3Rot);

	float GetScale() const;
	SVector3Df GetPosition() const;
	SVector3Df GetRotation() const;
	CMatrix4Df GetMatrix() const;

	CMatrix4Df GetReversedTranslationMatrix() const;
	CMatrix4Df GetReversedRotationMatrix() const;

	SVector3Df WorldPosToLocalPos(const SVector3Df& v3WorldPos) const;
	SVector3Df WorldDirToLocalDir(const SVector3Df& v3WorldDir) const;

private:
	float m_fScale;
	SVector3Df m_vPosition;
	SVector3Df m_vRotation;
};