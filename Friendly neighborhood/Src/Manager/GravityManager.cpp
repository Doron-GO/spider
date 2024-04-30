#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "../Object/Player.h"
#include "../Manager/Camera.h"
#include "../Manager/SceneManager.h"
#include "GravityManager.h"

GravityManager* GravityManager::Instance_ = nullptr;

GravityManager::GravityManager(void)
{
	player_ = nullptr;
	camera_ = nullptr;
	preDirGravity_ = AsoUtility::DIR_D;
	dirGravity_ = AsoUtility::DIR_D;
	dirUpGravity_ = AsoUtility::DIR_U;
	lastHitNormal_ = AsoUtility::DIR_U;
	lastHitPos_ = AsoUtility::VECTOR_ZERO;
	slerpPow_ = 0.0f;
	step_ = 0.0f;
}

GravityManager::~GravityManager(void)
{
	delete Instance_;
}

void GravityManager::CreateInstance(void)
{
	if (Instance_ == nullptr)
	{
		Instance_ = new GravityManager();
	}
	Instance_->Init();
}

GravityManager& GravityManager::GetInstance(void)
{
	return *Instance_;
}

void GravityManager::Init(void)
{

	dirGravity_ = AsoUtility::DIR_D;
	preDirGravity_ = AsoUtility::VECTOR_ZERO;
	step_ = -1.0f;
	slerpPow_ = DEFAULT_POW_SLERP;
	gravityPow_ = GRAVITY_POW;

	transform_.quaRot = Quaternion();
	transform_.Update();
}

void GravityManager::Update(void)
{

	// 重力方向や、正方向の計算
	Calculate();

}

void GravityManager::Calculate(void)
{
	//重力方向
	dirGravity_ = AsoUtility::DIR_D;

	//重力の判定方向
	dirUpGravity_ = VScale(dirGravity_, -1.0f);

	//二つのベクトル間の回転量を求める
	VECTOR up = transform_.GetUp();
	Quaternion toRot = Quaternion::FromToRotation(up, dirUpGravity_);

	//到達したい回転
	transform_.quaRot = 
		Quaternion::Slerp(transform_.quaRot, toRot.Mult(transform_.quaRot), slerpPow_);

}

void GravityManager::SetPlayer(Player* player)
{
	player_ = player;
}

const Transform& GravityManager::GetTransform(void) const
{
	return transform_;
}

VECTOR GravityManager::GetDirGravity(void) const
{
	return dirGravity_;
}

VECTOR GravityManager::GetDirUpGravity(void) const
{
	return dirUpGravity_;
}

float GravityManager::GetPower(void) const
{
	return gravityPow_;
}

Quaternion GravityManager::GetPremQuaRot(void) const
{
	return preDirGravity_;
}

void GravityManager::SetLastHitNormal(const VECTOR& normal)
{
	lastHitNormal_ = normal;
}

void GravityManager::SetLastHitPos(const VECTOR& pos)
{
	lastHitPos_ = pos;
}

VECTOR GravityManager::GetLastHitPos(void) const
{
	return lastHitPos_;
}
