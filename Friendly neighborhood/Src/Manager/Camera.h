#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include"../Input/InputManager.h"


class SceneManager;
class Transform;
class Collider;
class Controller;

class Camera
{

public:

	// FIXED_POINT : �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 100.0f, -800.0f };

	// FOLLOW : �����_(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 100.0f, 500.0f };

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);
	
	// �Ǐ]�ΏۂƂ̒������W
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera(void);
	~Camera();

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);
	void Release(void);

	void SetTransform(const Transform* transform);

	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;
	VECTOR GetTargetPos(void) const;
	Quaternion GetQuaRot(void) const;
	Quaternion GetQuaRotOutX(void) const;
	VECTOR GetDir(void) const;
	VECTOR GetDirvec(VECTOR vec) ;

	void ChangeMode(MODE mode);

private:
	//�J�����𓮂���
	void ProcessRot(void);

	InputManager::JOYPAD_NO padNumber_;


	//�J�����̏����ݒ�
	void SetDefault(void);

	// Transform�̈ʒu�Ɠ��������
	void SyncTransform(void);

	//�R���g���[���[
	std::unique_ptr<Controller> controller_;

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* syncTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion quaRotOutX_;

	// �J�����p�x
	Quaternion quaRot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

};

