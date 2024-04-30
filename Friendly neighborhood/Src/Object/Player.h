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

	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;	//�ʏ�ړ�

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//�W�����v��
	static constexpr float POW_JUMP = 60.0f;

	//�W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//���G�t�F�N�g�̔����Ԋu
	static constexpr float TIME_SMOKE = 0.3f;

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
	void Update(float delta,VECTOR pos, VECTOR gra, VECTOR end,VECTOR Billpos);
	void SetFollowTarget(Camera* target);
	void AddCollider(Collider* collider);
	void Draw(void);
	void SwingDraw(void);
	int CheckSection(void);
	void SetEndPpos(VECTOR pos);
	VECTOR GetCameraAngles(void);
	Transform* GetTransform(void);

	bool IsAlive();
	Quaternion quaPaddir;
private:

	void AnimationInit(void);	//�A�j���[�V�����ǂݍ���
	void DrawDebug(void);	//�f�o�b�O�\��
	void UpdatePendulum(float delta);
	void UpdateGround(float delta);
	bool SetSwingParam(VECTOR pos,VECTOR end);
	void SwingStart();
	void Swing(float delta);	//�U��q�̌v�Z���s��
	void Flying(float delta);
	void ProcessMove(void);
	void ProcessJump(void);
	void CalcGravityPow(void);
	void Collision(void);
	void CollisionCupsule(void);
	void CollisionSphere(void); //�ǂƂ̓����蔻��
	void CollisionGravity(void); 

	void SetGoalRotate(float rotRad);	//	���B�������p�x��ݒ�
	void SetGoalRotate2(VECTOR vec);	//	���B�������p�x��ݒ�
	void Rotate(void);					//���B�������p�x�܂ŁA��]������	

	float Magnitude(VECTOR pos)const;
	float Dot(const VECTOR& a, const VECTOR& b);
	VECTOR Normalized(VECTOR& v);

	void (Player::* phase_)(float delta);//�V���ȕ���̎d��

	//�t���O
	int stepGrav;
	bool isJump_;
	bool isSwingJump_;//�X�E�B���O��Ԃ���W�����v����
	bool isSwingFlag_;//�X�E�B���O��ԂɈڍs����
	bool isFall_;


	float stepJump_;
	float stepSwingump_;
	VECTOR movedPos_;	//�ړ���̍��W
	VECTOR jumpPow_;		//�W�����v��
	float jumpDot_;

	Camera* camera_;

	float sectionPos[2];
	int tttt;
	VECTOR movePow_;
	VECTOR endPos_;		//�x�_�̍��W
	VECTOR endPos_2;	//�x�_�̍��W2
	VECTOR billPos_;	//�r���̍��W
	VECTOR stringV_;		//�x�_����v���C���[�ւ̃x�N�g��
	VECTOR swingYnorm_;
	VECTOR gravity_;		//�d��
	VECTOR gravityNorm_;	//�d�͕����̐��K���x�N�g��
	VECTOR swingGravityNorm_;	
	VECTOR yNorm_;			//�����琎�̐��K���ςݐ����x�N�g��
	VECTOR swingGravity_;	//swing���̎��͕���
	VECTOR swingGravity_2;	 //swing���̎��͕���

	Quaternion goalQuaRot_;		//�ŏI�I�Ɍ�����������
	Quaternion pendulumRotY_;	//y����]

	std::vector<Collider*>colliders_;
	std::unique_ptr<Capsule>capsule_;

	SwingPoint swingPoint_;
	float xVecX, xVecY;

	float gMag_;		//�d�͂̑傫��
	float omega_;			//�p���x
	float length_;			//�R�̒���
	float theta_;

	AnimationController* animationController_;	//�v���C���[�̃A�j���[�V��������
	//std::unique_ptr<Controller> controller_;	//����
	Transform transform_;	//�v���C���[�̃��f������
	ResourceManager& resourceManager_;	//�RD���f���̓ǂݍ���
};

VECTOR operator*(const VECTOR& v, float scale);
VECTOR operator*=( VECTOR& v, float scale);
VECTOR operator*(float scale, const VECTOR& v);
VECTOR operator+(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& v);

