#include <string>
#include "../Application.h"
#include "../common/Flame.h"
#include "../Utility/AsoUtility.h"
#include "../Input/Keybord.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Transform.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Player.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Player::Player(void) : resourceManager_(ResourceManager::GetInstance())
{
	_update = &Player::StandardUpdate;
	gMag_ = SWING_GRAVITY; //�d�͂̑傫��
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	//section_ = 0;
	//���f�������i�[
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//���f���̐ݒ�
	transform_.scl = { 1.0f,1.0f ,1.0f };
	transform_.pos = { 7028.0f, 220.0f, 5467.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();
	
	capsule_ = std::make_unique<Capsule>(&transform_);
	capsule_->SetRelativePosTop({ 0.0f,65.0f,0.0f });
	capsule_->SetRelativePosDown({ 0.0f,28.0f,0.0f });
	capsule_->SetRadius(20.0f);


	endPos_ = { 4200.0f,2200.0f,1200.0f };

	isSwingFlag_ = false;
	// �A�j���[�V�����̐ݒ�
	AnimationInit();
	isSwingJump_ = false;
	camera_ =  SceneManager::GetInstance().GetCamera();
	isFall_ = false;
	MV1SetUseZBuffer(transform_.modelId, true);
	MV1SetWriteZBuffer(transform_.modelId, true);

}

void Player::Update(float delta, VECTOR dir,VECTOR gra,VECTOR endp)
{	
	setEndPos_ = endp;
	gravityNorm_ = VNorm(GRAVITY);		//�d�͂̐��K���x�N�g��
	swingGravity_ = transform_.GetForward();
	(this->*_update)(delta);
	Collision();
	transform_.Update();
	animationController_->Update();
	transform_.quaRot = Quaternion{ 1.0f,0.0f,0.0f,0.0f };
	transform_.quaRot = transform_.quaRot.Mult(pendulumRotY_);
	MV1RefreshCollInfo(transform_.modelId);
}

void Player::UpdateSwing(float delta)
{
	SwingDraw();
	Swing(delta);
}

void Player::StandardUpdate(float delta)
{
	ProcessMove();
	CalcGravityPow();
	ProcessJump();
	SetEndPpos(setEndPos_);
}

void Player::FallUpdate(float delta)
{
	SetEndPpos(setEndPos_);
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), true, 2.0f, 17.0f);
	animationController_->SetEndLoop(12.0f, 17.0f, 3.0f);
	auto speed = 4.0f;
	CalcGravityPow();
	ProcessJump();
	ProcessSwingJump();
	ProcessMove();

	auto& ins = InputManager::GetInstance();
	bool isHit = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);
	if (isHit)
	{
		isNormalJump_ = true;
		SetSwingParam(transform_.pos, endPos_);
	}
}


bool Player::SetSwingParam(VECTOR pos ,VECTOR end)
{
	//�x�_���琎�ւ̂̃x�N�g��
	stringV_ = VSub(transform_.pos,endPos_);

	//�q���̒���
	length_ = Magnitude(stringV_);

	//�d�͂̐��K���ς݃x�N�g��
	swingGravityNorm_ = VNorm(swingGravity_);

	float x = Dot(swingGravityNorm_, stringV_);//�d�̓x�N�g����,������d�͎��܂ł̃x�N�g���������_�Ǝx�_�̑傫��
	swingYnorm_ = stringV_ - swingGravityNorm_ * x;//������d�͎��܂ł̃x�N�g��

	float y = Magnitude(swingYnorm_);		//�x�_���琎�ւ̃x�N�g���̑傫��
	swingYnorm_=Normalized(swingYnorm_);//������d�͎��܂ł̐��K���ς݃x�N�g��

	theta_ = atan2f(y, x);				//�x�_�̋ߐړ�ӂ̂��ꂼ��̑傫�����g�p���Ċp�x���o��
	omega_ = -0.5f;	//�����x

	transform_.Update();
	isSwingFlag_ = true;
	_update = &Player::UpdateSwing;
	return true;
}

void Player::SwingStart()
{
	isNormalJump_ = false;
	SetSwingParam(transform_.pos, endPos_);

}

void Player::ProcessSwingJump(void)
{
	if (isSwingJump_)
	{
		stepSwingJump_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepSwingJump_ < TIME_SWING_JUMP)
		{
			jumpPow_ = VScale(VECTOR{ 0.0f,1.0f,0.0f }, POW_JUMP);
		}
	}
	if (stepSwingJump_ > TIME_SWING_JUMP)
	{
		stepSwingJump_ = 0.0f;
		isSwingJump_ = false;
	}
}

void Player::SetFollowTarget(Camera* target)
{
	camera_= target;

}

void Player::AddCollider(Collider* collider)
{
	colliders_.emplace_back(collider);
}

void Player::Swing(float delta)
{
	InputManager& ins = InputManager::GetInstance();

	float coe = -gMag_ / length_;	//�}�C�i�X(�d�͂̑傫��)���Ђ��̒����Ŋ���Fk�̌W��
	stringV_ = VSub(transform_.pos, endPos_);//�x�_���琎�ւ̂̃x�N�g��
	//�����Q=�N�b�^�@�̌v�Z��
	
	float k1a = delta * coe * sin(theta_);
	float m1a = delta * omega_;

	float k2a = delta * coe * sin(theta_ + m1a / 2.0f);
	float m2a = delta * (omega_ + k1a / 2.0f);

	float k3a = delta * coe * sin(theta_ + m2a / 2.0f);
	float m3a = delta * (omega_ + k2a / 2.0f);

	float k4a = delta * coe * sin(theta_ + m3a);
	float m4a = delta * (omega_ + k3a);

	omega_ += (k1a + 2.0f * k2a + 2.0f * k3a + k4a) /5.0f;		//�p���x�̉��Z
	theta_ += (m1a + 2.0f * m2a + 2.0f * m3a + m4a) / 5.0f;		//���p�x�̉��Z

	if (isSwingFlag_==true) 
	{
		VECTOR swingPos =
			endPos_ + (length_ * cos(theta_)) * Normalized(swingGravityNorm_) + (length_ * sin(theta_)) * swingYnorm_;
		movePow_ = VSub(swingPos, transform_.pos);
	}

	animationController_->Play(static_cast<int>(ANIM_TYPE::SWING), true, 50.0f, 62.0f);
	animationController_->SetEndLoop(50.0f, 55.0f, 3.0f);

	//�X�E�B���O�{�^���𗣂�����W�����v����
	if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		movePow_.y = 0.0f;
		movePow_.x *= 0.5f;
		movePow_.z *= 0.5f;
		isNormalJump_ = true;
		_update = &Player::FallUpdate;
		isSwingJump_ = true;
		isSwingFlag_ = false;
		stepJump_ = 0.0f;
	}
}
void Player::DrawDebug(void)
{
	////�f�o�b�O�\��
	DrawFormatString(0, 32, 0xffffff, "�v���C���[���W:%f,%f,%f",
		transform_.pos.x, transform_.pos.y, transform_.pos.z);
	VECTOR fw = VAdd(transform_.pos, VScale(transform_.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, fw, 0x00ff00);
	VECTOR pa = VAdd(transform_.pos, VScale(quaPaddir.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, pa, 0x00ff00);

	VECTOR  lol = { 1.0f,1400.0f,0.2f };
	VECTOR kj = VAdd(endPos_, VScale(swingGravityNorm_, 600.0f));
	VECTOR kj2 = VAdd(transform_.pos, VScale(swingGravityNorm_, 600.0f));
	DrawLine3D(endPos_, kj, 0x00ff00);
	DrawLine3D(transform_.pos, kj2, 0x00ff00);
	VECTOR klk = { 4200.0f,2200.0f,1200.0f };
	DrawLine3D(endPos_, transform_.pos, 0xff00ff);//��������x�_�܂ł̃��C��������
	DrawSphere3D(klk, 60.0f, 20, 0xff0000, 0xffffff, true);
}


void Player::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
	if (isSwingFlag_)
	{
		int hand =MV1SearchFrame(transform_.modelId, "mixamorig:LeftHandIndex4");
		VECTOR pos= MV1GetFramePosition(transform_.modelId, hand);
	   DrawLine3D(endPos_, pos, 0xffffff);//��������x�_�܂ł̃��C��������
	}
	DrawLine3D(endPos_, transform_.pos, 0xff00ff);//��������x�_�܂ł̃��C��������
}

void Player::SwingDraw(void)
{
}

void Player::SetEndPpos(VECTOR pos)
{
	endPos_ = pos;
}


Transform* Player::GetTransform(void)
{
	//���f����{���̎擾
	return &transform_;
}
bool Player::IsAlive()
{
	if (transform_.pos.y<-4000.0f)
	{
		return true;
	}
	return false;
}
void Player::AnimationInit(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle1.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run1.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Gwen_Jump.mv1", 24.0f);
	animationController_->Add((int)ANIM_TYPE::SWING, path + "swing_PF.mv1", 20.0f);
	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void Player::ProcessMove()
{
	//�ړ��ʂ��[����
	if (!isNormalJump_)
	{
		movePow_ = VECTOR{ 0.0f,0.0f,0.0f };
	}

	Quaternion cameraRot = camera_->GetQuaRotOutX();
	float rotRad = 0;
	float speed = 40.0f;
	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	auto& ins = InputManager::GetInstance();
	const InputManager::JOYPAD_IN_STATE& stick = InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	stick.AKeyLX;	// �o�͒l�i�|1000�`1000�j���`�E�{
	stick.AKeyLY;	// �o�͒l�i�|1000�`1000�j��`���{
	// �ق����l�́|1�`1�Ȃ̂�1000�Ŋ���
	float padX = stick.AKeyLX / 1000.0f;
	// �㉺���]������
	float padY = stick.AKeyLY * -1.0f / 1000.0f;

	// Y���Ȃ������x�N�g��
	VECTOR padDir = {};
	padDir = { padX,0.0f,padY };

	float  rad = atan2f(padX,padY );
	// ���K��
	padDir = AsoUtility::VNormalize(padDir);
	cameraRot = cameraRot.Normalized();

	//�A�i���O�X�e�B�b�N�̓|���Ă��������Quaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad, cameraRot.GetUp());
	//�J�����̉�]�ƍ��������N�H�[�^�j�I��
	Quaternion goal2 = Quaternion::Mult(cameraRot, quaPadDir);

	//�X�e�B�b�N���͂�����΃v���C���[�𓮂���
	if (!AsoUtility::EqualsVZero(padDir))
	{
		if (!isNormalJump_&&!isFall_)//�W�����v��Ԃ���Ȃ��X�e�B�b�N���͂������
		{	//�A�j���[�V������RUN��
			
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		movePow_ = VScale(goal2.GetForward(), speed);	//�ړ��ʁ@= �����@�~�@�X�s�[�h
		goalQuaRot_ = Quaternion::LookRotation(goal2.GetForward());
	}
	else
	{
		if (!isNormalJump_ && !isFall_)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
		}
	}

	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)||
		CheckHitKey(KEY_INPUT_F))
	{
		transform_.pos.y += 200.0f;
		SwingStart();
	}
	Rotate();	//��](�S�[���ɑ΂��āA���B��������)
}

void Player::ProcessJump()
{
	auto& ins = InputManager::GetInstance();
	bool isDown = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);
	//�W�����v�{�^���������ꂽ����
	if (isDown && !isNormalJump_)
	{
		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), true, 2.0f, 17.0f);
		animationController_->SetEndLoop(12.0f, 17.0f, 3.0f);
		isNormalJump_ = true;
	}
	if (isNormalJump_)
	{
		stepJump_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{			
			jumpPow_ = VScale(VECTOR{ 0.0f,1.0f,0.0f }, POW_JUMP);
		}
	}	
	if (!isDown)
	{
		stepJump_ = TIME_JUMP_IN;
	}
}

void Player::CalcGravityPow()
{
	//�d�͕���
	VECTOR dirGravity = -gravityNorm_;
	//�d�͂̋���
	float gravityPow = 30.0f;
	//�d��				�����~��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	//�㉺�̈ړ��ʂɏd�͂�������
	jumpPow_ = VAdd(jumpPow_, gravity);
	//dot�͓���
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		//�d�͕����Ɣ��Ε����łȂ���΁C�W�����v�H��ł�����
		jumpPow_ = gravity;
	}
}

void Player::Collision()
{
		//���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);
	CollisionCupsule();
	CollisionGravity();
	CollisionSphere();
	transform_.pos = movedPos_;
}

void Player::CollisionCupsule(void)
{	//�J�v�Z�����ړ�
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = capsule_->Copy(&trans);

	for (auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(c->modelId_,-1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
		);

		//���o�����|���S���Ƃ̏Փˏ�������
		MV1CollResultPolyDimTerminate(hits);

		for (int i = 0; i < hits.HitNum; i++)
		{
			//1��1�̏Փˏ����擾����
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//�������ׂ����炷���߂ɁA�P�|���S����
				//�J�v�Z���̏Փ˔��肵����
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(),
					cap.GetPosDown(),
					cap.GetRadius(),
					hit.Position[0],//3�̒��_
					hit.Position[1],
					hit.Position[2]
				);
				if (pHit)
				{
					isSwingFlag_ = false;
					movePow_ = { 0.0f,0.0f,0.0f };
					//�Փ˂��Ă���
					float pow = 4.0f; //������Ƃ���������
					hit.Normal;		  //�Փ˂����|���S���̖@��
					int p = MV1GetFrameParent(c->modelId_, hit.FrameIndex);
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pow));
					//�J�v�Z�����ړ�������
					trans.pos = movedPos_;
					trans.Update();
					cap = capsule_->Copy(&trans);
					_update = &Player::StandardUpdate;
					continue;
				}
				//�Փ˂��Ă��Ȃ��̂Ń��[�v���f
			}
		}
	}
}

void Player::CollisionSphere(void)
{
	for (auto c : colliders_)
	{
		//�n��(�X�e�[�W���f��)�Ƃ̏Փ�				�@
		auto hits = MV1CollCheck_Sphere(c->modelId_, -1, transform_.pos, 50.0f);
		//���o�����|���S���Ƃ̏Փˏ�������
		MV1CollResultPolyDimTerminate(hits);

		for (int i = 0; i < hits.HitNum; i++)
		{
			break;
		}

	}
}

void Player::CollisionGravity(void)
{
	movedPos_ = VAdd(movedPos_, jumpPow_);

	VECTOR dirGravity = -VNorm(GRAVITY);

	VECTOR dirUpGravity = VNorm(GRAVITY);

	//�����@movePow
	float CheckPow = 10.0f;
	
	//�����̂�����Ə�̍��W
	auto gravHitUp = VAdd(movedPos_, VScale(dirUpGravity, 25.0f));
	gravHitUp = VAdd(gravHitUp, VScale(dirUpGravity, CheckPow * 2.0f));

	//�n�_
	//�����̂�����Ɖ��̍��W
	auto gravHitDown = VAdd(movedPos_, VScale(dirGravity, CheckPow));

	for (auto c : colliders_)
	{
		//�n��(�X�e�[�W���f��)�Ƃ̏Փ�				�@���n�_		���I�_
		auto hit = MV1CollCheck_Line(c->modelId_, -1, gravHitUp, gravHitDown);
		//���������������̓����蔻��
		jumpDot_ = VDot(dirGravity, VNorm(jumpPow_));

		if (hit.HitFlag > 0 && jumpDot_ > 0.9f)
		{
			//�Փ˂��Ă�����W�����v�I��

			float dis = 4.0f;
			_update = &Player::StandardUpdate;
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, dis));
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			isNormalJump_		 = false;
			isSwingFlag_ = false;
			isSwingJump_ = false;
			isFall_		 = false;
			stepSwingJump_ = 0.0f;
			stepJump_	= 0.0f;
		}
		//�W�����v��ԁA�X�C���O��Ԃ̂ǂ���ł��Ȃ������痎����Ԃɂ���
		else if(!IsJump()&&!isSwingFlag_ )
		{
			isFall_ = true;
			animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP),true, 19.0f);
			animationController_->SetEndLoop(12.0f,18.0f , 3.0f);
		}
	}
}

bool Player::IsJump(void)
{
	return (isNormalJump_ && isSwingJump_);
}


void Player::Rotate(void)
{
	pendulumRotY_ = Quaternion::Slerp(pendulumRotY_, goalQuaRot_, 0.2);
}

const float Player::Magnitude(VECTOR vec) const
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Player::Dot(const VECTOR& a, const VECTOR& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

VECTOR Player::Normalized(VECTOR& v)
{
	float mag = Magnitude(v);
	return VECTOR{ v.x / mag, v.y / mag, v.z / mag };
}

VECTOR operator*(const VECTOR& v, float scale)
{
	return{ v.x * scale, v.y * scale ,v.z * scale };
}

VECTOR operator*=( VECTOR& v, float scale)
{
	return{v.x*= scale, v.y *= scale ,v.z *= scale };
}

VECTOR operator*(float scale, const VECTOR& v)
{
	return{ v.x * scale, v.y * scale ,v.z * scale };
}

VECTOR operator+(const VECTOR& va, const VECTOR vb)
{
	return{ va.x + vb.x, va.y + vb.y ,va.z + vb.z };
}

VECTOR operator-(const VECTOR& va, const VECTOR vb)
{
	return { va.x - vb.x, va.y - vb.y,va.z - vb.z };
}

VECTOR operator-(const VECTOR& v)
{
	return { -v.x,-v.y ,-v.z };
}



