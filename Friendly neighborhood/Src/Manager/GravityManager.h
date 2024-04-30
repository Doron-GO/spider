#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"

#define lpGravityMng GravityManager::GetInstance()

class SceneManager;
class Planet;
class Player;
class Camera;

class GravityManager
{
public:

	// �d�͕��������S�ɕς��܂ł̎���
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// ��]��
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	//�d��
	static constexpr float GRAVITY_POW = 10.0f;

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static GravityManager& GetInstance(void);

	void Init(void);

	void Update(void);

	// �d�͕�����A�������̌v�Z
	void Calculate(void);

	// ����v���C���[���Z�b�g
	void SetPlayer(Player* player);

	const Transform& GetTransform(void) const;
	
	// �d�͕������擾
	VECTOR GetDirGravity(void) const;

	// �d�͕����̔��Ε������擾
	VECTOR GetDirUpGravity(void) const;

	// �d�͂̋������擾
	float GetPower(void) const;

	// �f���؂�ւ��O�̉�]���
	Quaternion GetPremQuaRot(void) const;

	// �v���C���[�Ƒ����Փ˂����@��
	void SetLastHitNormal(const VECTOR& normal);

	// �v���C���[�Ƒ����Փ˂����ʒu
	void SetLastHitPos(const VECTOR& pos);
	VECTOR GetLastHitPos(void) const;

protected:
	GravityManager(void);
	~GravityManager(void);

	// �ÓI�C���X�^���X
	static GravityManager* Instance_;

	// �v���C���[
	Player* player_;

	//�J����
	Camera* camera_;

	Transform transform_;

	// �d�͕���
	VECTOR dirGravity_;
	VECTOR preDirGravity_;

	// �d�͕����̔��Ε���
	VECTOR dirUpGravity_;

	// �v���C���[�Ƒ����Փ˂����@��
	VECTOR lastHitNormal_;

	// �v���C���[�Ƒ����Փ˂����ʒu
	VECTOR lastHitPos_;

	// �X�V�X�e�b�v
	float step_;

	// ��]��
	double slerpPow_;

	//�d�͗�
	float gravityPow_;

	// �d�͕������v�Z
	VECTOR CalcDirGravity(void) const;

};

