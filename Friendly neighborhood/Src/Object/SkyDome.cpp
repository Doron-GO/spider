#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
{
}

SkyDome::SkyDome(const Transform* syncTransform)
{
	syncTransform_ = syncTransform;
	state_ = STATE::NONE;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		lpResourceMng.LoadModelDuplicate(ResourceManager::SRC::SKYDOME));
	transform_.scl = SCALES;
	transform_.pos = { 15123.0f,0.0f,14557.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f, 
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Zバッファ無効(突き抜け対策)
	//MV1SetUseZBuffer(transform_.modelId, false);
	//MV1SetWriteZBuffer(transform_.modelId, false);

	// 状態遷移
	if (syncTransform_ == nullptr)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}

}

void SkyDome::Update(void)
{

	switch (state_)
	{
	case SkyDome::STATE::STAY:
		break;
	case SkyDome::STATE::FOLLOW:
		transform_.pos = syncTransform_->pos;
		break;
	default:
		break;
	}

	transform_.Update();

}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::Release(void)
{
}

void SkyDome::ChangeState(STATE state)
{

	state_ = state;
	switch (state_)
	{
	case SkyDome::STATE::STAY:
		break;
	case SkyDome::STATE::FOLLOW:
		transform_.pos = syncTransform_->pos;
		break;
	}

	transform_.Update();

}
