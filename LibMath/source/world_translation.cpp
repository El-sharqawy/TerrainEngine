#include "stdafx.h"

CWorldTranslation::CWorldTranslation()
{
	m_fScale = 1.0f;
	m_vPosition = SVector3Df(0.0f, 0.0f, 0.0f);
	m_vRotation = SVector3Df(0.0f, 0.0f, 0.0f);
}

void CWorldTranslation::SetScale(const float fScale)
{
	m_fScale = fScale;
}

void CWorldTranslation::SetPosition(const float fPosX, const float fPosY, const float fPosZ)
{
	m_vPosition.x = fPosX;
	m_vPosition.y = fPosY;
	m_vPosition.z = fPosZ;
}

void CWorldTranslation::SetPosition(const SVector3Df& v3Pos)
{
	m_vPosition = v3Pos;
}

void CWorldTranslation::SetRotation(const float fRotX, const float fRotY, const float fRotZ)
{
	m_vRotation.x = fRotX;
	m_vRotation.y = fRotY;
	m_vRotation.z = fRotZ;
}

void CWorldTranslation::SetRotation(const SVector3Df& v3Rot)
{
	m_vRotation = v3Rot;
}

void CWorldTranslation::Rotate(const float fRotX, const float fRotY, const float fRotZ)
{
	m_vRotation.x += fRotX;
	m_vRotation.y += fRotY;
	m_vRotation.z += fRotZ;
}

void CWorldTranslation::Rotate(const SVector3Df& v3Rot)
{
	m_vRotation += v3Rot;
}

float CWorldTranslation::GetScale() const
{
	return m_fScale;

}

SVector3Df CWorldTranslation::GetPosition() const
{
	return (m_vPosition);
}

SVector3Df CWorldTranslation::GetRotation() const
{
	return (m_vRotation);

}

CMatrix4Df CWorldTranslation::GetMatrix() const
{
	CMatrix4Df matScale{}, matRotation{}, matTranslation{};

	matScale.InitScaleTransform(m_fScale, m_fScale, m_fScale);

	matRotation.InitRotateTransformZYX(m_vRotation.x, m_vRotation.y, m_vRotation.z);

	matTranslation.InitTranslationTransform(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	CMatrix4Df matWorldTransformation = matTranslation * matRotation * matScale;

	return (matWorldTransformation);
}

CMatrix4Df CWorldTranslation::GetReversedTranslationMatrix() const
{
	CMatrix4Df ReversedTranslationMatrix{};
	ReversedTranslationMatrix.InitTranslationTransform(GetPosition().negate());
	return (ReversedTranslationMatrix);

}

CMatrix4Df CWorldTranslation::GetReversedRotationMatrix() const
{
	CMatrix4Df ReversedRotationMatrix{};
	ReversedRotationMatrix.InitRotateTransformZYX(-m_vRotation.x, -m_vRotation.y, -m_vRotation.z);
	return (ReversedRotationMatrix);
}

SVector3Df CWorldTranslation::WorldPosToLocalPos(const SVector3Df& v3WorldPos) const
{
	CMatrix4Df WorldToLocalTranslation = GetReversedTranslationMatrix();
	CMatrix4Df WorldToLocalRotation = GetReversedRotationMatrix();
	CMatrix4Df WorldToLocalTransformation = WorldToLocalRotation * WorldToLocalTranslation;

	SVector4Df WorldPos4f = SVector4Df(v3WorldPos, 1.0f);
	SVector4Df LocalPos4f = WorldToLocalTransformation * WorldPos4f;
	SVector3Df LocalPos3f(LocalPos4f);
	return (LocalPos3f);
}

SVector3Df CWorldTranslation::WorldDirToLocalDir(const SVector3Df& v3WorldDir) const
{
	CMatrix3Df World(GetMatrix());

	// Inverse Local-To-World Transformation using Transpose (assuming uniform scaling)
	CMatrix3Df WorldToLocal = World.Transpose();

	SVector3Df v3LocalDirection = WorldToLocal * v3WorldDir;
	v3LocalDirection = v3LocalDirection.normalize();

	return (v3LocalDirection);
}

