#pragma once

#include "../../LibMath/source/stdafx.h"
#include "singleton.h"
#include <map>

enum ECameraDirections
{
	DIRECTION_FORWARD,
	DIRECTION_BACKWARD,
	DIRECTION_RIGHT,
	DIRECTION_LEFT,
};

enum ECameraStates
{
	CAMERA_STATE_NORMAL,
	CAMERA_STATE_LOCKED,
	CAMERA_STATE_CANTGOFORWARD,
	CAMERA_STATE_CANTGOBACKWARD,
	CAMERA_STATE_CANTGORIGHT,
	CAMERA_STATE_CANTGOLEFT,
	CAMERA_STATE_CANTGOUP,
	CAMERA_STATE_CANTGODOWN,
};

class CCamera
{
public:
	CCamera();
	CCamera(GLint iWidth, GLint iHeight);
	CCamera(const SPersProjInfo& persProj, const SVector3Df& v3Pos = SVector3Df(0.0f, 0.0f, 0.0f), const SVector3Df& v3Target = SVector3Df(0.0f, 0.0f, 1.0f), const SVector3Df& v3Up = SVector3Df(0.0f, 1.0f, 0.0f));
	CCamera(const SOrthoProjInfo& orthoProj, const SVector3Df& v3Pos = SVector3Df(0.0f, 0.0f, 0.0f), const SVector3Df& v3Target = SVector3Df(0.0f, 0.0f, 1.0f), const SVector3Df& v3Up = SVector3Df(0.0f, 1.0f, 0.0f));

	void ProcessKeyboardInput(ECameraDirections eDirection, float deltaTime);
	void ProcessMouseMovement(float fOffsetX, float fOffsetY);
	void ProcessMouseScroll(float fOffsetY);

	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const SVector3Df& v3Pos);

	void SetTarget(const float x, const float y, const float z);
	void SetTarget(const SVector3Df& v3Target);

	void SetUp(const float x, const float y, const float z);
	void SetUp(const SVector3Df& v3Up);

	void OnRender();

	void Print() const { printf("Pos: "); m_v3Pos.print(); printf("Target: "); m_v3Target.print(); }

	const float GetSpeed() const;
	void SetSpeed(const float fVal);

	const float GetMaxSpeed() const;
	void SetMaxSpeed(const float fVal);

	const float GetSensitivity() const;
	void SetSensitivity(const float fVal);

	bool IsLocked() const;
	void SetLock(const bool bVal);

	const SVector3Df& GetPosition() const;
	const SVector3Df& GetTarget() const;
	const SVector3Df& GetUp() const;
	const CMatrix4Df& GetProjectionMat() const;
	const SPersProjInfo& GetPersProjInfo() const;

	CMatrix4Df GetMatrix() const;
	CMatrix4Df GetViewProjMatrix() const;
	CMatrix4Df GetViewMatrix() const;
	CMatrix4Df GetViewPortMatrix() const;
	CMatrix4Df GetBillBoardMatrix() const;
	CMatrix4Df GetViewMatrixInverse() const;

	GLint GetWindowWidth() const;
	GLint GetWindowHeight() const;

	void UpdateBillBoardMatrix();
	void UpdateViewVector();

protected:
	void InitInternal();

	void InitCamera(const SPersProjInfo& persProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up);
	void InitCamera(const SOrthoProjInfo& orthoProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up);

	void OnUpdate();


private:
	SVector3Df m_v3Pos; // m_v3Eye
	SVector3Df m_v3Target;
	SVector3Df m_v3Up;

	SVector3Df m_v3View;

	GLint m_iWindowW; // Window Width
	GLint m_iWindowH; // Window Height

	float m_fSpeed;
	float m_fMaxSpeed;
	float m_fSensitivity;

	float m_fAngleH; // Horizontal Angle (Yaw)
	float m_fAngleV; // Vertical Angle (Pitch)

	float m_fZoom;

	bool m_bOnUpperEdge;
	bool m_bOnLowerEdge;
	bool m_bOnLeftEdge;
	bool m_bOnRightEdge;
	bool m_bRotateOnEdges;
	bool m_bIsPersProjInfo;
	bool m_bIsLocked;

	SVector2Df m_v2MousePos;

	SPersProjInfo m_sPersProjInfo;
	CMatrix4Df m_matProj;
	CMatrix4Df m_matBillBoard;
	CMatrix4Df m_matViewInverse;

	ECameraStates m_eCamState;
};

class CCameraManager : public CSingleton<CCameraManager>
{
public:
	CCameraManager();
	virtual ~CCameraManager();

	bool AddCamera(uint8_t CamNum);
	bool RemoveCamera(uint8_t CamNum);

	void SetCurrentCamera(uint8_t CamNum);
	CCamera* GetCurrentCamera();

protected:
	enum ECameraNum
	{
		CAMERA_DEFAULT,
		CAMERA_PRESPECTIVE,
		CAMERA_ORTHO,
	};

private:
	typedef std::map<uint8_t, CCamera*> TCameraMap;

	TCameraMap m_mCamMap;
	CCamera* m_pCurrCamera;
	CCamera* m_pPrevCam;
};