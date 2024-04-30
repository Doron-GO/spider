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
	phase_ = &Player::UpdateGround;
	VECTOR gravityDir_ = { 0.0f,-1.0f,0.0f };
	gravity_ = VScale(gravityDir_, 3500.0f);
	gMag_ = Magnitude(gravity_);

}

Player::~Player(void)
{
}

void Player::Init(void)
{
		//controller_ = std::make_unique<Keybord>();
	tttt = 0;
	//���f�������i�[
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//���f���̐ݒ�
	//transform_.scl = { 0.5f,0.5f ,0.5f };
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
	sectionPos[0] = 25600.0f;
	sectionPos[1] = 51071.0f;
	isSwingFlag_ = false;
	// �A�j���[�V�����̐ݒ�
	AnimationInit();
	isSwingJump_ = false;
	camera_ =  SceneManager::GetInstance().GetCamera();
	isFall_ = false;
	MV1SetUseZBuffer(transform_.modelId, true);
	MV1SetWriteZBuffer(transform_.modelId, true);

}

void Player::Update(float delta, VECTOR dir,VECTOR gra,VECTOR endp, VECTOR Billpos)
{	
	endPos_2 = endp;
	billPos_ = Billpos;
	gravityNorm_ = Normalized(gravity_);		//�d�͂̐��K���x�N�g��
	//swingGravityNorm = Normalized(swingGravity);
	swingGravity_ = transform_.GetForward();
	//controller_->Update();
	(this->*phase_)(delta);
	//CalcGravityPow();
	//ProcessJump();
	Collision();
	transform_.Update();
	//endPos_= swingPoint_.SetSwingPoint(transform_.pos, 1);
	animationController_->Update();
	transform_.quaRot = Quaternion{ 1.0f,0.0f,0.0f,0.0f };
	transform_.quaRot = transform_.quaRot.Mult(pendulumRotY_);
	CheckSection();
	MV1RefreshCollInfo(transform_.modelId);
}

void Player::UpdatePendulum(float delta)
{
	SwingDraw();
	Swing(delta);
	//Collision();
	//transform_.Update();
}

void Player::UpdateGround(float delta)
{
	ProcessMove();
	CalcGravityPow();
	ProcessJump();
	//Collision();
	SetEndPpos(endPos_2);
	//�Փ˔���
}

void Player::Flying(float delta)
{
	SetEndPpos(endPos_2);
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), true, 2.0f, 17.0f);
	animationController_->SetEndLoop(12.0f, 17.0f, 3.0f);
	auto speed = 4.0f;
	CalcGravityPow();
	ProcessJump();
	ProcessMove();
	//Collision();

	auto& ins = InputManager::GetInstance();
	bool isHit = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT);
	if (isHit)
	{
		isJump_ = true;
		SetSwingParam(transform_.pos, endPos_);
	}
}


bool Player::SetSwingParam(VECTOR pos ,VECTOR end)
{
	stringV_ = VSub(transform_.pos,endPos_);//�x�_���琎�ւ̂̃x�N�g��

	VECTOR GroundV = VSub(VECTOR{ 0.0f,100.0f,0.0f }, endPos_);
	
	auto stleng = Magnitude(VECTOR{ 0.0f,stringV_.y,0.0f });
	auto GroundLen = Magnitude(GroundV);
	if (abs(stleng) >= abs(GroundLen))
	{
		return false;
	}
	length_ = Magnitude(stringV_);				//�q���̒���

	//transform_.pos = pos;
	//���̂��

	//�����ɕς��Ă���
	auto jk = Normalized(stringV_);
	VECTOR Fro;
	Fro= SceneManager::GetInstance().GetCamera()->GetDir();
	Fro = VNorm(Fro);
	Fro.y = 0.2f;
	//swingGravityNorm = Normalized(Fro);
	swingGravityNorm_ = VNorm(swingGravity_);

	auto x2 = Dot(swingGravityNorm_, stringV_);//�d�̓x�N�g���Ɛ�����d�͎��܂ł̃x�N�g���������_�Ǝx�_�̑傫��
	swingYnorm_ = stringV_ - swingGravityNorm_ * x2;//������d�͎��܂ł̃x�N�g��
	auto y2 = Magnitude(swingYnorm_);				//������d�͎��܂ł̐��K���ς݃x�N�g��
	swingYnorm_=Normalized(swingYnorm_);
	theta_ = atan2f(y2, x2);				//�x�_�̋ߐړ�ӂ̂��ꂼ��̑傫�����g�p���Ċp�x���o��
	auto gravity_ = VScale(swingGravityNorm_, 3500.0f);
	
	gMag_ = Magnitude(gravity_);
	omega_ = -0.5f;									//�p���x��0�ŏ�����

	transform_.Update();
	isSwingFlag_ = true;
	phase_ = &Player::UpdatePendulum;
	return true;

}

void Player::SwingStart()
{
	isJump_ = false;
	SetSwingParam(transform_.pos, endPos_);

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
	//if (!isStarted_)
	//{
	//	return;
	//}
	auto& ins = InputManager::GetInstance();

	auto coe = -gMag_ / length_;				//k�̌W��

	//�����Q=�N�b�^�@�̌v�Z
	auto k1a = delta * coe * sin(theta_);
	auto m1a = delta * omega_;

	auto k2a = delta * coe * sin(theta_ + m1a / 2.0f);
	auto m2a = delta * (omega_ + k1a / 2.0f);

	auto k3a = delta * coe * sin(theta_ + m2a / 2.0f);
	auto m3a = delta * (omega_ + k2a / 2.0f);

	auto k4a = delta * coe * sin(theta_ + m3a);
	auto m4a = delta * (omega_ + k3a);

	omega_ += (k1a + 2.0f * k2a + 2.0f * k3a + k4a) /5.0f;		//�p���x�̉��Z
	theta_ += (m1a + 2.0f * m2a + 2.0f * m3a + m4a) / 5.0f;		//���p�x�̉��Z

	auto stv = Normalized(stringV_);
	
	if (isSwingFlag_==true) 
	{
		//transform_.pos = endPos_ + length_ * cos(theta_) * Normalized(swingGravityNorm) + length_ * sin(theta_) * swingYnorm_;
		auto l = endPos_ + length_ * cos(theta_) * Normalized(swingGravityNorm_) + length_ * sin(theta_) * swingYnorm_;
		
		movePow_ = VSub(l, transform_.pos);
	}
	//transform_.pos = endPos_ + length_ * cos(theta_) * Normalized(gravity_) + length_ * sin(theta_) * yNorm_;


	animationController_->Play(static_cast<int>(ANIM_TYPE::SWING), true, 50.0f, 62.0f);
	animationController_->SetEndLoop(50.0f, 55.0f, 3.0f);


	//�J���������Ă�������ɑ΂��Ă̑O�㍶�E�̏d�͂��X����
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();
	if (ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)||
		CheckHitKey(KEY_INPUT_SPACE))
	{
		movePow_.y = 0.0f;
		movePow_.x *= 1.5f;
		movePow_.z *= 1.5f;
		isJump_ = true;
		phase_ = &Player::Flying;
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
	//DrawFormatString(0, 52, 0xffffff, "movedPos���W:%f,%f,%f",
	//	movedPos_.x, movedPos_.y, movedPos_.z);

	//DrawFormatString(50, 530, 0xffffff, "mJumpPow=%f", jumpPow_.y);
	//DrawFormatString(50, 470, 0xffffff, "step:%f", stepJump_);
	//DrawFormatString(50, 500, 0xffffff, "stepswing:%f", stepSwingump_);
	//DrawFormatString(50, 200, 0xffffff, "%f,%f,%f", endPos_.x, endPos_.y, endPos_.z);
	//DrawFormatString(50, 220, 0xffffff, "%d", tttt);
	//DrawFormatString(50, 550, 0xffffff, "movePow_:%f,%f,%f", movePow_.x, movePow_.y, movePow_.z);
	auto fw = VAdd(transform_.pos, VScale(transform_.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, fw, 0x00ff00);
	auto pa = VAdd(transform_.pos, VScale(quaPaddir.GetForward(), 60.0f));
	DrawLine3D(transform_.pos, pa, 0x00ff00);


	auto  lol = VECTOR{ 1.0f,1400.0f,0.2f };
	//swingGravity = VECTOR{lol.x,-0.4f,lol.z };

	auto kj = VAdd(endPos_, VScale(swingGravityNorm_, 600.0f));
	auto kj2 = VAdd(transform_.pos, VScale(swingGravityNorm_, 600.0f));
	DrawLine3D(endPos_, kj, 0x00ff00);
	DrawLine3D(transform_.pos, kj2, 0x00ff00);
	VECTOR klk = { 4200.0f,2200.0f,1200.0f };
	DrawLine3D(endPos_, transform_.pos, 0xff00ff);//��������x�_�܂ł̃��C��������

	DrawSphere3D(klk, 60.0f, 20, 0xff0000, 0xffffff, true);
	//VECTOR bill{ billPos_.x,500.0f,billPos_.z };
	//DrawLine3D(bill, transform_.pos, 0x0000ff);//���������ԋ߂��r���ւ�line������

}


void Player::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
	if (isSwingFlag_)
	{
		auto hand =MV1SearchFrame(transform_.modelId, "mixamorig:LeftHandIndex4");
		auto pos= MV1GetFramePosition(transform_.modelId, hand);
	   DrawLine3D(endPos_, pos, 0xffffff);//��������x�_�܂ł̃��C��������
	}
	DrawLine3D(endPos_, transform_.pos, 0xff00ff);//��������x�_�܂ł̃��C��������
}

void Player::SwingDraw(void)
{
}

int Player::CheckSection(void)
{


	if (transform_.pos.x <= sectionPos[0] && transform_.pos.z <= sectionPos[0])
	{
		//19
		tttt = 1;
	}
	if (sectionPos[0]<=transform_.pos.x&&transform_.pos.x <= sectionPos[1] &&
		transform_.pos.z <= sectionPos[0])
	{
		//18
		tttt = 2;

	}
	if (transform_.pos.x <= sectionPos[0] &&
		sectionPos[0]<=transform_.pos.z&&transform_.pos.z <= sectionPos[1])
	{
		//14
		tttt = 2;

	}
	if (sectionPos[0]<=transform_.pos.x&&transform_.pos.x <= sectionPos[1] &&
		sectionPos[0]<= transform_.pos.z && transform_.pos.z <= sectionPos[1])
	{
		//13
		tttt = 1;

	}
	return tttt;
}

void Player::SetEndPpos(VECTOR pos)
{
	endPos_ = pos;
}

VECTOR Player::GetCameraAngles(void)
{
	return camera_->GetDir();
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
	if (!isJump_)
	{
		movePow_ = VECTOR{ 0.0f,0.0f,0.0f };
	}

	Quaternion cameraRot = camera_->GetQuaRotOutX();

	float rotRad = 0;
	float speed = 40.0f;

	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	auto& ins = InputManager::GetInstance();
	const auto& stick = InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

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


	if (!AsoUtility::EqualsVZero(padDir))
	{
		if (!isJump_&&!isFall_)//�W�����v��Ԃ���Ȃ��X�e�B�b�N���͂������
		{	//�A�j���[�V������RUN��
			
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		movePow_ = VScale(goal2.GetForward(), speed);	//�ړ��ʁ@= �����@�~�@�X�s�[�h
		goalQuaRot_ = Quaternion::LookRotation(goal2.GetForward());
	}
	else
	{
		if (!isJump_ && !isFall_)
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
	bool isHit = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);
	if (isHit)
	{
		if (!isJump_)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), true, 2.0f, 17.0f);
			animationController_->SetEndLoop(12.0f, 17.0f, 3.0f);
		}
		isJump_ = true;

	}
	else
	{
		//stepJump_ = TIME_JUMP_IN;
	}
	if (isSwingJump_)
	{
		stepSwingump_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepSwingump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(VECTOR{ 0.0f,1.0f,0.0f }, POW_JUMP);
		}
	}

	if (isJump_)
	{
		stepJump_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{			
			jumpPow_ = VScale(VECTOR{ 0.0f,1.0f,0.0f }, POW_JUMP);
		}
	}
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}
	if (stepSwingump_ > TIME_JUMP_IN) 
	{
		stepSwingump_ = 0.0f;
		isSwingJump_ = false;
	}
}

void Player::CalcGravityPow()
{
	//�d�͕���
	VECTOR dirGravity = gravityNorm_;
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
					phase_ = &Player::UpdateGround;
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

	VECTOR dirGravity = VNorm(gravity_);

	VECTOR dirUpGravity = -VNorm(gravity_);

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

			//�X�Δ���p�ɏՓˏ����Ƃ��Ă���
			//mHitNormal = hit.Normal;
			//mHitPos = hit.HitPosition;

			//�Փ˂��Ă���
			float dis = 4.0f;
			phase_ = &Player::UpdateGround;

			isSwingFlag_ = false;
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, dis));
			//��R�͂̑����
			jumpPow_ = AsoUtility::VECTOR_ZERO;

			if (isJump_)
			{
				////���n���[�V����
				//mAnimationController->Play(
				//	static_cast<int>(ANIM_TYPE::JUMP), false,
				//	29.0f, 45.0f, false, true);
			}
			//�W�����v�I��
			isJump_ = false;
			isSwingJump_ = false;
			stepJump_ = 0.0f;
			isFall_ = false;
			stepSwingump_ = 0.0f;
		}
		else if(!isSwingFlag_&&!isJump_&&! isSwingJump_)
		{
			if (!isFall_)
			{
				isFall_ = true;
				animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP),true, 19.0f);
				animationController_->SetEndLoop(12.0f,18.0f , 3.0f);
			}
		}

	}
}

void Player::SetGoalRotate(float rotRad)
{
	VECTOR cameraAngle =
		SceneManager::GetInstance().GetCamera()->GetAngles();

	Quaternion axis =
		Quaternion::AngleAxis(cameraAngle.y + rotRad, AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������߂�
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//�������l
	if (angleDiff > 0.1)
	{
		//mStepRot = TIME_ROT;
	}
	//���B��������]�����߂�
	goalQuaRot_ = axis;
}

void Player::SetGoalRotate2(VECTOR vec)
{
	Quaternion axis =
		Quaternion::AngleAxis(vec.y, AsoUtility::AXIS_Y);

}

void Player::Rotate(void)
{
	pendulumRotY_ = Quaternion::Slerp(pendulumRotY_, goalQuaRot_, 0.2);
}

float Player::Magnitude(VECTOR pos) const
{
	return sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
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



