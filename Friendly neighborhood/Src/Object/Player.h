#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
#include"../Object/Stage.h"
#include "../Object/Common/Transform.h"
#include"SwingPoint.h"

class AnimationController;
class ResourceManager;
class GravityManager;
class Quaternion;
class Collider;
class Capsule;
class Controller;
class Camera;

class Player
{
public:

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		JUMP,
		SWING,
		ODO
	};

	Player(void);
	~Player(void);

	void Init(void);
	void Update(float delta,VECTOR pos, VECTOR gra, VECTOR end);
	void SetFollowTarget(Camera* target);
	void AddCollider(Collider* collider);
	void Draw(void);
	void SwingDraw(void);
	void SetEndPpos(VECTOR pos);

	Transform* GetTransform(void);
	bool IsAlive();

	Quaternion quaPaddir;
private:

	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;	//�ʏ�ړ�

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//�W�����v��
	static constexpr float POW_JUMP = 60.0f;

	//�W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//�X�C���O�W�����v����
	static constexpr float TIME_SWING_JUMP = 0.5f;
	//�X�E�B���O���d��
	static constexpr float SWING_GRAVITY = 3500.0f;

	void (Player::* _update)(float delta);//�V���ȕ���̎d��

//�t���O
	int stepGrav;

	//�W�����v��Ԃ��ǂ����𔻒�
	bool isNormalJump_;

	//�X�E�B���O��Ԃ���W�����v����
	bool isSwingJump_;

	//�X�E�B���O��ԂɈڍs����
	bool isSwingFlag_;

	//������ԉ��𔻒�
	bool isFall_;

	//�W�����v�͉��Z�p
	float stepJump_;

	//�X�C���O�W�����v�͉��Z�p
	float stepSwingJump_;

	//�ړ���̍��W
	VECTOR movedPos_;	

	//�W�����v��
	VECTOR jumpPow_;

	//�v���C���[���W�����v�͂��d�͂ɏ����Ă��邩�𔻒�
	float jumpDot_;

	//�v���C���[�̈ړ���
	VECTOR movePow_;

	Camera* camera_;

	//�x�_�̍��W
	VECTOR endPos_;		

	//�Z�b�g�p�x�_�̍��W
	VECTOR setEndPos_;	

	//�x�_����v���C���[�ւ̃x�N�g��
	VECTOR stringV_;		

	//������d�͎��܂ł̃x�N�g��
	VECTOR swingYnorm_;

	static constexpr VECTOR GRAVITY = { 0.0f,3500.0f,0.0f };

	//�d�͕����̐��K���x�N�g��
	VECTOR gravityNorm_;

	//swing���̏d�͕���
	VECTOR swingGravity_;

	//�X�C���O�̏d�̓x�N�g���𐳋K����������
	VECTOR swingGravityNorm_;

	//�����琎�̐��K���ςݐ����x�N�g��
	VECTOR yNorm_;
	
	//�ŏI�I�Ɍ�����������
	Quaternion goalQuaRot_;

	//y����]
	Quaternion pendulumRotY_;

	//�v���C���[�Ƃ̓����蔻�����郂�m����
	std::vector<Collider*>colliders_;

	//�J�v�Z���ł̓����蔻��
	std::unique_ptr<Capsule>capsule_;

	//�v���C���[�̃A�j���[�V��������
	AnimationController* animationController_;

	//�v���C���[�̃��f������
	Transform transform_;

	//�RD���f���̓ǂݍ���
	ResourceManager& resourceManager_;

	//�d�͂̑傫��
	float gMag_;

	//�p���x
	float omega_;

	//�R�̒���
	float length_;

	//�p�x
	float theta_;


	//�A�j���[�V�����ǂݍ���
	void AnimationInit(void);	

	void DrawDebug(void);	//�f�o�b�O�\��

	//�X�C���O���̃A�b�v�f�[�g
	void UpdateSwing(float delta);

	//�������
	void FallUpdate(float delta);

	//�ʏ�̃A�b�v�f�[�g(�X�C���O��ԈȊO)
	void StandardUpdate(float delta);

	//�X�C���O�J�n�O�̃p�����[�^��ݒ�
	bool SetSwingParam(VECTOR pos,VECTOR end);
	
	//�X�C���O��Ԃɂ���֐�
	void SwingStart();

	//�X�C���O�W�����v���s���֐�
	void ProcessSwingJump(void);

	//�U��q�̌v�Z���s��
	void Swing(float delta);	

	//�X�e�B�b�N�̓��͂�����Ƃ��Ƀv���C���[���ړ�������֐�
	void ProcessMove(void);

	//�{�^���̓��͂ɉ����ăW�����v���J�n����֐�
	void ProcessJump(void);
	
	//�d�͂����Z����֐�
	void CalcGravityPow(void);
	
	//�����蔻����܂Ƃ߂��֐�
	void Collision(void);

	//���f���ƃJ�v�Z���Ƃ̓����蔻��
	void CollisionCupsule(void);

	//�ǂƂ̓����蔻��
	void CollisionSphere(void); 

	//�n�ʂƂ̓����蔻��
	void CollisionGravity(void); 

	//�W�����v��Ԃ��ǂ���(�X�C���O�W�����v���܂�)
	bool IsJump(void);

	//���B�������p�x�܂ŁA��]������	
	void Rotate(void);

	//�x�N�g���̑傫�����擾
	const float Magnitude(VECTOR vec)const;

	//����
	float Dot(const VECTOR& a, const VECTOR& b);

	//���K��������֐�
	VECTOR Normalized(VECTOR& v);

};

VECTOR operator*(const VECTOR& v, float scale);
VECTOR operator*=( VECTOR& v, float scale);
VECTOR operator*(float scale, const VECTOR& v);
VECTOR operator+(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& v);

