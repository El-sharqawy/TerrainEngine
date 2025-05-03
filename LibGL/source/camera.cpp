#include "stdafx.h"
#include "camera.h"
#include <algorithm>

static float MARGIN = 40.0f;
static float EDGE_STEP = 0.01f;

CCamera::CCamera()
{
	TPersProjInfo PersProjInfo = { 45.0f, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0.1f, 5000.0f };

	// For NonTerrain:
	//const SVector3Df v3Pos = SVector3Df(0.0f, 0.0f, 0.0f);
	//const SVector3Df v3Target = SVector3Df(0.0f, 0.0f, 1.0f);

	// For Terrain:
	const SVector3Df v3Pos = SVector3Df(100.0f, 220.0f, 400.0f);
	const SVector3Df v3Target = SVector3Df(0.0f, 0.0f, 1.0f);

	const SVector3Df v3Up = SVector3Df(0.0f, 1.0f, 0.0f);
	InitCamera(PersProjInfo, v3Pos, v3Target, v3Up);
}

CCamera::CCamera(GLint iWidth, GLint iHeight)
{
	m_v3Pos = SVector3Df(0.0f, 0.0f, 0.0f);
	m_v3Target = SVector3Df(0.0f, 0.0f, 1.0f);
	m_v3Up = SVector3Df(0.0f, 1.0f, 0.0f);
	m_fSpeed = 250.0f;
	m_fSensitivity = 0.1f;
	m_fZoom = 45.0f;

	m_iWindowW = iWidth;
	m_iWindowH = iHeight;

	InitInternal();
}

CCamera::CCamera(const SPersProjInfo& persProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up)
{
	InitCamera(persProj, v3Pos, v3Target, v3Up);
}

CCamera::CCamera(const SOrthoProjInfo& orthoProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up)
{
	InitCamera(orthoProj, v3Pos, v3Target, v3Up);
}

void CCamera::InitCamera(const SPersProjInfo& persProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up)
{
	m_v3Pos = v3Pos;

	m_v3Target = v3Target;
	m_v3Target.normalize();

	m_v3Up = v3Up;
	m_v3Up.normalize();

	m_fSpeed = 250.0f;
	m_fSensitivity = 0.1f;
	m_fZoom = 45.0f;

	m_sPersProjInfo = persProj;
	m_matProj.InitPersProjTransform(persProj);
	m_iWindowW = static_cast<GLuint>(persProj.Width);
	m_iWindowH = static_cast<GLuint>(persProj.Height);

	m_v3View = (m_v3Target - m_v3Pos).normalize();

	m_bIsPersProjInfo = true;

	InitInternal();
}

void CCamera::InitCamera(const SOrthoProjInfo& orthoProj, const SVector3Df& v3Pos, const SVector3Df& v3Target, const SVector3Df& v3Up)
{
	m_v3Pos = v3Pos;

	m_v3Target = v3Target;
	m_v3Target.normalize();

	m_v3Up = v3Up;
	m_v3Up.normalize();

	m_fSpeed = 250.0f;
	m_fSensitivity = 0.05f;
	m_fZoom = 45.0f;

	m_matProj.InitOrthoProjTransform(orthoProj);
	m_iWindowW = static_cast<GLuint>(orthoProj.fWidth);
	m_iWindowH = static_cast<GLuint>(orthoProj.fHeight);

	m_bIsPersProjInfo = false;

	InitInternal();
}

void CCamera::InitInternal()
{
	SVector3Df HTarget(m_v3Target.x, 0.0f, m_v3Target.z);
	HTarget.normalize();

	// this is commented because atan2 Does the job for us.
	/*const float fAngle = ToDegree(std::asin(std::abs(HTarget.z)));

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			m_fAngleH = 360.0f - fAngle;
		}
		else
		{
			m_fAngleH = 180.0f - fAngle;
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_fAngleH = fAngle;
		}
		else
		{
			m_fAngleH = 180.0f - fAngle;
		}
	}*/

	m_fAngleH = -ToDegree(std::atan2(m_v3Target.z, m_v3Target.x));
	m_fAngleV = -ToDegree(std::asin(m_v3Target.y));

	m_bOnUpperEdge = false;;
	m_bOnLowerEdge = false;
	m_bOnLeftEdge = false;
	m_bOnRightEdge = false;
	m_bRotateOnEdges = false;
	m_bIsLocked = false;

	m_eCamState = CAMERA_STATE_NORMAL;

	m_fMaxSpeed = 1000.0f;

	m_v2MousePos = SVector2Df(m_iWindowW / 2.0f, m_iWindowH / 2.0f);

	OnUpdate();
}

void CCamera::SetPosition(const float x, const float y, const float z)
{
	m_v3Pos.x = x;
	m_v3Pos.y = y;
	m_v3Pos.z = z;
}

void CCamera::SetPosition(const SVector3Df& v3Pos)
{
	m_v3Pos = v3Pos;
}

const SVector3Df& CCamera::GetPosition() const
{
	return (m_v3Pos);
}

void CCamera::SetTarget(const float x, const float y, const float z)
{
	m_v3Target.x = x;
	m_v3Target.y = y;
	m_v3Target.z = z;
}

void CCamera::SetTarget(const SVector3Df& v3Target)
{
	m_v3Target = v3Target;
}

const SVector3Df& CCamera::GetTarget() const
{
	return (m_v3Target);
}

void CCamera::SetUp(const float x, const float y, const float z)
{
	m_v3Up.x = x;
	m_v3Up.y = y;
	m_v3Up.z = z;
}

void CCamera::SetUp(const SVector3Df& v3Up)
{
	m_v3Up = v3Up;
}

const SVector3Df& CCamera::GetUp() const
{
	return (m_v3Up);
}

const SVector3Df& CCamera::GetDirection() const
{
	return (m_v3View);
}

CMatrix4Df CCamera::GetMatrix() const
{
	CMatrix4Df CameraTransformation{};
	CameraTransformation.InitCameraTransform(m_v3Pos, m_v3Target, m_v3Up);

	return (CameraTransformation);
}

CMatrix4Df CCamera::GetViewProjMatrix() const
{
	CMatrix4Df viewMat = GetMatrix();
	CMatrix4Df projectionMat = GetProjectionMat();
	CMatrix4Df ViewProjectionMat = projectionMat * viewMat;
	return (ViewProjectionMat);
}

CMatrix4Df CCamera::GetViewMatrix() const
{
	return (GetMatrix());
}

CMatrix4Df CCamera::GetBillBoardMatrix() const
{
	return (m_matBillBoard);
}

CMatrix4Df CCamera::GetViewMatrixInverse() const
{
	return (m_matViewInverse);
}

void CCamera::ProcessKeyboardInput(ECameraDirections eDirection, float deltaTime)
{
	if (IsLocked())
	{
		return;
	}

	const float fVelocity = m_fSpeed * deltaTime;

	switch (eDirection)
	{
	case DIRECTION_FORWARD:
		m_v3Pos += m_v3Target * fVelocity;
		break;
		
	case DIRECTION_BACKWARD:
		m_v3Pos -= m_v3Target * fVelocity;
		break;

	case DIRECTION_RIGHT:
		SVector3Df v3Right = m_v3Up.cross(m_v3Target);
		v3Right.normalize();
		m_v3Pos += v3Right * fVelocity;
		break;

	case DIRECTION_LEFT:
		SVector3Df v3Left = m_v3Target.cross(m_v3Up);
		v3Left.normalize();
		m_v3Pos += v3Left * fVelocity;
		break;
	}
}

void CCamera::ProcessMouseMovement(float fOffsetX, float fOffsetY)
{
	if (IsLocked())
	{
		return;
	}

	float fDeltaX = fOffsetX - m_v2MousePos.x;
	float fDeltaY = fOffsetY - m_v2MousePos.y;

	m_v2MousePos.x = fOffsetX;
	m_v2MousePos.y = fOffsetY;

	fDeltaX /= 2;
	fDeltaY /= 2;

	m_fAngleH -= fDeltaX * m_fSensitivity;
	m_fAngleV -= fDeltaY * m_fSensitivity;

	if (fOffsetX <= MARGIN)
	{
		m_bOnLeftEdge = true;
		m_bOnRightEdge = false;
	}
	else if (fOffsetX >= (m_iWindowW - MARGIN))
	{
		m_bOnRightEdge = true;
		m_bOnLeftEdge = false;
	}
	else
	{
		m_bOnLeftEdge = false;
		m_bOnRightEdge = false;
	}

	if (fOffsetY <= MARGIN)
	{
		m_bOnUpperEdge = true;
		m_bOnLowerEdge = false;
	}
	else if (fOffsetY >= (m_iWindowH - MARGIN))
	{
		m_bOnLowerEdge = true;
		m_bOnUpperEdge = false;
	}
	else
	{
		m_bOnUpperEdge = false;
		m_bOnLowerEdge = false;
	}

	OnUpdate();
}

void CCamera::ProcessMouseScroll(float fOffsetY)
{
	if (IsLocked())
	{
		return;
	}

	m_fZoom -= fOffsetY;
	if (m_fZoom < 1.0f)
	{
		m_fZoom = 1.0f;
	}
	if (m_fZoom > 90.0f)
	{
		m_fZoom = 90.0f;
	}

	if (m_bIsPersProjInfo)
	{
		m_sPersProjInfo.FOV = m_fZoom;
		m_matProj.InitPersProjTransform(m_sPersProjInfo);
	}

}

void CCamera::OnRender()
{
	if (IsLocked())
	{
		return;
	}

	bool bShouldUpdate = false;

	if (m_bRotateOnEdges)
	{
		if (m_bOnLeftEdge)
		{
			m_fAngleH += EDGE_STEP;
			bShouldUpdate = true;
		}
		else if (m_bOnRightEdge)
		{
			m_fAngleH -= EDGE_STEP;
			bShouldUpdate = true;
		}

		if (m_bOnUpperEdge)
		{
			if (m_fAngleV > -90.0f)
			{
				m_fAngleV += EDGE_STEP;
				bShouldUpdate = true;
			}
		}
		else if (m_bOnLowerEdge)
		{
			if (m_fAngleV < 90.0f)
			{
				m_fAngleV -= EDGE_STEP;
				bShouldUpdate = true;
			}
		}
	}

	if (bShouldUpdate)
	{
		OnUpdate();
	}
}

void CCamera::OnUpdate()
{
	bool bUseQuaternion = true;

	if (bUseQuaternion)
	{
		SVector3Df v3YAxis(0.0f, 1.0f, 0.0f);

		// Rotate the view vector by the horizontal angle around the vertical axis
		SVector3Df v3View(1.0f, 0.0f, 0.0f);
		v3View.rotate(m_fAngleH, v3YAxis);
		v3View.normalize();

		// Rotate the view vector by the vertical angle around the horizontal axis
		SVector3Df v3U = v3YAxis.cross(v3View);
		v3U.normalize();
		v3View.rotate(m_fAngleV, v3U);

		m_v3Target = v3View;
		m_v3Target.normalize();

		m_v3Up = m_v3Target.cross(v3U);
		m_v3Up.normalize();
	}
	else
	{
		// Clamp vertical angle to avoid gimbal lock and extreme rotations
		m_fAngleV = std::clamp(m_fAngleV, -89.0f, 89.0f);

		// Convert yaw (horizontal) and pitch (vertical) to radians
		float yaw = ToRadian(m_fAngleH);
		float pitch = ToRadian(m_fAngleV);

		// Calculate the new forward vector
		m_v3Target.x = std::cos(pitch) * std::cos(yaw);
		m_v3Target.y = std::sin(pitch);
		m_v3Target.z = std::cos(pitch) * std::sin(yaw);

		// Normalize the target vector only if it is not zero
		m_v3Target.normalize();

		// Recalculate the right vector (perpendicular to world up and target)
		SVector3Df worldUp(0.0f, 1.0f, 0.0f);
		SVector3Df v3Right = worldUp.cross(m_v3Target);

		// Normalize the right vector only if it is not zero
		v3Right.normalize();

		// Recalculate the up vector (perpendicular to target and right)
		m_v3Up = m_v3Target.cross(v3Right);

		// Normalize the up vector only if it is not zero
		m_v3Up.normalize();

		// Update the camera's view matrix, etc.
	}

	UpdateViewVector();
	UpdateBillBoardMatrix();
}

void CCamera::UpdateViewVector()
{
	m_v3View = m_v3Target.normalize();
}

void CCamera::UpdateBillBoardMatrix()
{
	// Get the billboard transformation matrix
	CMatrix4Df view = GetViewMatrix();
	// Extract the rotation part (upper-left 3x3 matrix) from the view matrix:
	CMatrix3Df viewRotation = CMatrix3Df(view);
	// For an orthonormal matrix, the inverse is its transpose:
	CMatrix3Df billboardRotation = viewRotation.Transpose();
	// Convert back to a 4x4 matrix with no translation:
	m_matBillBoard = billboardRotation;
	m_matBillBoard.mat4[3][0] = 0.0f;
	m_matBillBoard.mat4[3][1] = 0.0f;
	m_matBillBoard.mat4[3][2] = 0.0f;
	m_matBillBoard.mat4[3][3] = 1.0f;

	m_matViewInverse = GetViewMatrix().Inverse();
}

const float CCamera::GetSpeed() const
{
	return (m_fSpeed);
}

void CCamera::SetSpeed(const float fVal)
{
	m_fSpeed = fVal;
}

const float CCamera::GetMaxSpeed() const
{
	return (m_fMaxSpeed);
}

void CCamera::SetMaxSpeed(const float fVal)
{
	m_fMaxSpeed = fVal;
}

const float CCamera::GetSensitivity() const
{
	return (m_fSensitivity);
}

void CCamera::SetSensitivity(const float fVal)
{
	m_fSensitivity = fVal;
}

const CMatrix4Df& CCamera::GetProjectionMat() const
{
	return (m_matProj);
}

const SPersProjInfo& CCamera::GetPersProjInfo() const
{
	return (m_sPersProjInfo);
}

CMatrix4Df CCamera::GetViewPortMatrix() const
{
	float HalfW = GetWindowWidth() / 2.0f;
	float HalfH = GetWindowHeight() / 2.0f;

	CMatrix4Df Viewport{};

	Viewport[0][0] = HalfW; Viewport[0][1] = 0.0f; Viewport[0][2] = 0.0f; Viewport[0][3] = HalfW;
	Viewport[1][0] = 0.0f; Viewport[1][1] = HalfH; Viewport[1][2] = 0.0f; Viewport[1][3] = HalfH;
	Viewport[2][0] = 0.0f; Viewport[2][1] = 0.0f; Viewport[2][2] = 1.0f; Viewport[2][3] = 0.0f;
	Viewport[3][0] = 0.0f; Viewport[3][1] = 0.0f; Viewport[3][2] = 0.0f; Viewport[3][3] = 1.0f;


	return Viewport;
}

GLint CCamera::GetWindowWidth() const
{
	return (m_iWindowW);
}

GLint CCamera::GetWindowHeight() const
{
	return (m_iWindowH);
}

bool CCamera::IsLocked() const
{
	return (m_bIsLocked);
}

void CCamera::SetLock(const bool bVal)
{
	m_bIsLocked = bVal;
	m_eCamState = bVal ? CAMERA_STATE_NORMAL : CAMERA_STATE_LOCKED;
}

/////////// Camera Manager /////////////////
CCameraManager::CCameraManager()
{
	m_pCurrCamera = nullptr;
	m_pPrevCam = nullptr;

	AddCamera(CAMERA_PRESPECTIVE);
	SetCurrentCamera(CAMERA_PRESPECTIVE);
}

bool CCameraManager::AddCamera(uint8_t CamNum)
{
	if (m_mCamMap.end() != m_mCamMap.find(CamNum))
	{
		return false; // Cam Already Added
	}

	m_mCamMap.insert(TCameraMap::value_type(CamNum, new CCamera));
	return true;
}

bool CCameraManager::RemoveCamera(uint8_t CamNum)
{
	TCameraMap::iterator itor = m_mCamMap.find(CamNum);
	if (m_mCamMap.end() == itor)
	{
		return false;
	}

	m_mCamMap.erase(CamNum);
	return true;
}

void CCameraManager::SetCurrentCamera(uint8_t CamNum)
{
	if (m_pCurrCamera != m_mCamMap[CamNum])
	{
		m_pPrevCam = m_pCurrCamera;
	}
	m_pCurrCamera = m_mCamMap[CamNum];
}

CCamera* CCameraManager::GetCurrentCamera()
{
	assert(m_pCurrCamera != nullptr);
	return (m_pCurrCamera);
}

CCameraManager::~CCameraManager()
{
	for (auto& it : m_mCamMap)
	{
		delete it.second;
		it.second = nullptr;
	}
	m_mCamMap.clear();
}