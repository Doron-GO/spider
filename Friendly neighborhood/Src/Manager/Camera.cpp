#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "../Common/Flame.h"
#include "../Input/Controller.h"
#include "../Input/GamePad.h"
#include "../Input/KeyBord.h"
#include "SceneManager.h"
#include "GravityManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	padNumber_ = InputManager::JOYPAD_NO::PAD1;
	syncTransform_ = nullptr;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	if (GetJoypadNum())		//�p�b�h���ڑ�����Ă�����
	{
		controller_ = std::make_unique<GamePad>();
	}
	else
	{
		controller_ = std::make_unique<Keybord>();
	}

	//�����ݒ�
	ChangeMode(MODE::FOLLOW);
}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(50.0f, 60000.0f);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�Œ�J�����Ȃ̂ŉ������Ȃ�
}

void Camera::SetBeforeDrawFollow(void)
{

	targetPos_ =syncTransform_->pos;
	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncTransform();

}

void Camera::Draw()
{
	//DrawFormatString(0, 0, 0xffffff, "���W�F%f,%f,%f", pos_.x, pos_.y, pos_.z);
	//DrawFormatString(0, 20, 0xffffff, "�J�����p�x�F(%.2f, %.2f, %.2f)",

	//	angles_.x * 180.0f / DX_PI_F,
	//	angles_.y * 180.0f / DX_PI_F,
	//	angles_.z * 180.0f / DX_PI_F);

	char p = '0' + 1;
	char pp = '0' + 2;
	//DrawFormatString(300, 300, 0xffffff, "B"+1);


	//�Ǐ]���_
	//DrawSphere3D(targetPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);

}

void Camera::Release(void)
{
}

void Camera::ProcessRot(void)
{
	controller_->Update();

	float move = 10.0f;
	float rad = 0.0f;

	auto contData = controller_->GetInputData();
	auto controllerStick = InputManager::GetInstance().GetJPadInputState(padNumber_);
	auto& key = InputManager::GetInstance();
	//�J�����̉�]
	if (controllerStick.AKeyRX>0||key.IsNew(KEY_INPUT_RIGHT))
	{
		//�E��]
		angles_.y += AsoUtility::Deg2RadF(1.5f);
	}
	if (key.IsNew(KEY_INPUT_LEFT)||controllerStick.AKeyRX<0)
	{
		//����]
		angles_.y += AsoUtility::Deg2RadF(-1.5f);
	}

	if (controllerStick.AKeyRY>0|| key.IsNew(KEY_INPUT_UP))
	{
		//���]
		if (angles_.x <= LIMIT_X_UP_RAD)
		{
			angles_.x += AsoUtility::Deg2RadF(1.0f);
		}
	}
	if (controllerStick.AKeyRY < 0 || key.IsNew(KEY_INPUT_DOWN))
	{
		//����]
		if (angles_.x >= -LIMIT_X_DW_RAD)
		{
			angles_.x += AsoUtility::Deg2RadF(-1.0f);
		}
	}

}

void Camera::SetTransform(const Transform* transform)
{
	syncTransform_ = transform;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return quaRot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return quaRotOutX_;
}

VECTOR Camera::GetDir(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

VECTOR Camera::GetDirvec(VECTOR vec)
{
	return  VNorm(VSub(vec, pos_));

}

void Camera::ChangeMode(MODE mode)
{

	SetDefault();

	mode_ = mode;
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;
	targetPos_ = AsoUtility::VECTOR_ZERO;

	angles_.x = AsoUtility::Deg2RadF(10.0f);
	angles_.y = AsoUtility::Deg2RadF(90.0f);
	angles_.z = 0.0f;

	quaRot_ = Quaternion();

	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::SyncTransform(void)
{
	//auto& gIns = lpGravityMng;

	// ������̈ʒu
	VECTOR pos = syncTransform_->pos;

	// �d�͂̕�������ɏ]��
	Quaternion gRot = { 1.0f,0.0f,-1.0f,0.0f };

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	quaRotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	quaRot_ = quaRotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = quaRotOutX_.PosAxis(RELATIVE_TARGET_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = quaRot_.PosAxis(RELATIVE_CAMERA_POS_FOLLOW);
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = gRot.GetUp();
}
