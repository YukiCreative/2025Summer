#include "Camera.h"
#include <DxLib.h>
#include "Geometry.h"
#include <EffekseerForDXLib.h>

namespace
{
	constexpr float kInitCameraNear = 60;
	constexpr float kInitCameraFar = 10000;
	constexpr float kCameraRotateSensitivity = 1.0f;
	constexpr float kLerpStrength = 0.1f;
	constexpr float kDistanceLerpStrength = 0.07f;
	constexpr float kInitFovDegrees = 80.0f * Geometry::kDegToRad;
	constexpr float kChangeFoVSpeed = 0.1f;
	// 見下ろせる最大値
	constexpr float kCameraVRotPlusThreshold = 0.3f;
	// 見上げられる最大値
	constexpr float kCameraVRotMinusThreshold = 1.0f;

	const Vector3 kInitTargetToCamera = Vector3{ 0, 3, 5 }.GetNormalize();
	constexpr float kInitCameraDistance = 400.0f;
	constexpr float kCameraDrag = 0.15f;
}

Camera::Camera() :
	m_targetToCamera(kInitTargetToCamera),
	m_targetDistance(kInitCameraDistance),
	m_lerpedTargetDistance(kInitCameraDistance),
	m_targetFoV(kInitFovDegrees),
	m_FoV(kInitFovDegrees),
	m_state(&Camera::UpdateUseDirectionAndDistance)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	SetCameraNearFar(kInitCameraNear, kInitCameraFar);
	SetupCamera_Perspective(m_targetFoV);
}

void Camera::Update()
{
	(this->*m_state)();

	Rotate();

	// ライトをカメラの向きから照らす
	SetLightDirection(GetCameraDir());
}

void Camera::Draw_Debug() const
{
	Vector3 cameraPos = GetCameraPosition();
	DrawFormatString(300, 0, 0xffffff, "カメラの現在位置X:%f,Y:%f,Z:%f", cameraPos.x, cameraPos.y, cameraPos.z);
	DrawFormatString(300, 15, 0xffffff, "カメラの現在回転V:%f,H:%f,T:%f", GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());

	// 回転軸を手に入れる
	const MATRIX camMat = MTranspose(GetCameraViewMatrix());
	const Vector3 rightAxis = VTransformSR({ 1,0,0 }, camMat);
	DrawLine3D(m_targetPos + rightAxis,  m_targetPos + rightAxis*200,0xffff00);
	const Vector3 strPos = ConvWorldPosToScreenPos(m_targetPos + rightAxis * 200);
	DrawStringF(strPos.x, strPos.y, "←カメラの垂直回転軸", 0xffffff);
}

void Camera::SetTargetPos(const Vector3& targetPos)
{
	m_targetPos = targetPos;
}

void Camera::SetLerpPos(const Vector3& pos)
{
	m_targetCameraPos = pos;
}

void Camera::RotateCameraUpVecY(const float rad)
{
	m_cameraVel.x += rad;
}

void Camera::RotateCameraV(const float rad)
{
	m_cameraVel.y += rad;
}

Vector3 Camera::GetCameraDir() const
{
	return (-m_targetToCamera).GetNormalize();
}

Vector3 Camera::GetCameraDirXZ() const
{
	const Vector3 temp = GetCameraDir();
	return {temp.x, 0, temp.z};
}

Vector3 Camera::RotateVecToCameraDirXZ(const Vector3& vec, const Vector3& vec2)
{
	const MATRIX camMat = MGetRotVec2(vec2, GetCameraDirXZ());
	return VTransformSR(vec, camMat);
}

void Camera::SetTargetFoV(const float deg)
{
	m_targetFoV = deg * Geometry::kDegToRad;
}

void Camera::ChangeStateDD(const Vector3& targetPos)
{
	m_state = &Camera::UpdateUseDirectionAndDistance;

	// Distanceをデフォルトに
	m_targetDistance = kInitCameraDistance;

	// 今の位置からだんだん戻っていく感じに
	m_lerpedTargetDistance = (m_targetCameraPos - m_targetPos).Magnitude();

	m_targetPos = targetPos;
}

void Camera::ChangeStateDP()
{
	m_state = &Camera::UpdateUseDirectlyPosition;

	// lerpedCameraPosを今の位置に
	m_lerpedCameraPos = m_lerpedTargetPos + m_targetToCamera * m_targetDistance;
}

void Camera::UpdateUseDirectionAndDistance()
{
	// Effekseerのカメラと同期？する
	Effekseer_Sync3DSetting();

	m_lerpedTargetDistance = std::lerp(m_lerpedTargetDistance, m_targetDistance, kDistanceLerpStrength);

	// 注視点をlerp
	m_lerpedTargetPos.LerpMyself(m_targetPos, kLerpStrength);
	// DxLibのカメラに反映
	SetCameraPositionAndTarget_UpVecY(m_lerpedTargetPos + m_targetToCamera * m_lerpedTargetDistance, m_lerpedTargetPos);

	m_FoV = std::lerp(m_FoV, m_targetFoV, kChangeFoVSpeed);
	SetupCamera_Perspective(m_FoV);
}

void Camera::UpdateUseDirectlyPosition()
{
	// Effekseerのカメラと同期？する
	Effekseer_Sync3DSetting();

	// カメラの位置をlerp
	m_lerpedCameraPos.LerpMyself(m_targetCameraPos, kLerpStrength);
	// 注視点もlerp
	m_lerpedTargetPos.LerpMyself(m_targetPos, kLerpStrength);
	// DxLibのカメラに反映(位置を直接)
	SetCameraPositionAndTarget_UpVecY(m_lerpedCameraPos, m_lerpedTargetPos);

	const Vector3 targetToPos = m_lerpedCameraPos - m_targetPos;

	// DDで使う奴も書き換えちゃう
	// じゃあ状態分けた意味ないじゃん
	m_targetDistance = targetToPos.Magnitude();
	m_targetToCamera = targetToPos.GetNormalize();

	m_FoV = std::lerp(m_FoV, m_targetFoV, kChangeFoVSpeed);
	SetupCamera_Perspective(m_FoV);
}

void Camera::Rotate()
{
	// カメラ速度減衰
	m_cameraVel -= m_cameraVel * kCameraDrag;

	// 水平回転
	m_targetToCamera = VTransformSR(m_targetToCamera, MGetRotY(m_cameraVel.x));

	// 垂直回転
	// まず回転軸を手に入れる
	const MATRIX camMat = MTranspose(GetCameraViewMatrix());
	// カメラにとってのX軸　を取得している
	const Vector3 rightAxis = { camMat.m[0][0],camMat.m[0][1],camMat.m[0][2] };

	// 一定以上急な角度にならないようにする
	// 回転しようとしている向きが、角度を元に戻す方向なら許す
	const Vector3 unitTargetToCamera = m_targetToCamera.GetNormalize();
	const Vector3 unitTargetToCameraXZ = { unitTargetToCamera.x, 0, unitTargetToCamera.z };
	const float dot = unitTargetToCamera.Dot(unitTargetToCameraXZ);

	if ((dot < kCameraVRotPlusThreshold  && m_cameraVel.y > 0 && m_targetToCamera.y > 0) ||
		(dot < kCameraVRotMinusThreshold && m_cameraVel.y < 0 && m_targetToCamera.y < 0)
		)
	{
		m_cameraVel.y = 0;
		return;
	}

	// 回転軸にそって回転
	m_targetToCamera = VTransform(m_targetToCamera, MGetRotAxis(rightAxis, m_cameraVel.y));
}
