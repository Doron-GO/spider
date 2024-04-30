#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "Player.h"
#include "Coin.h"
#include "Common/AnimationController.h"
#include "../Application.h"

Coin::Coin(Transform transform)
	: resourceManager_(ResourceManager::GetInstance())
{
	transform_ = transform;
}

Coin::~Coin()
{
}

void Coin::Init(void)
{
	//モデル情報を格納
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::COIN));

	//回転の設定
	coinRotY_ = Quaternion();
	stepRot_ = 0.0f;

	//衝突判定
	isHit_ = true;
	std::string path = Application::PATH_MODEL + "Player/";

	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add(0, path + "Idle1.mv1", 20.0f);
	animationController_->Add(4, path + "odo.mv1", 30.0f);
	animationController_->Add(5, path + "joy.mv1", 30.0f);
	animationController_->Play(static_cast<int>(4),true);
	//animationController_->SetEndLoop(0.0f, 188.0f, 30.0f);

	//狼煙のエフェクト
	//coinSmoke_ = ResourceManager::GetInstance().Load(
	//	ResourceManager::SRC::COIN_SMOKE).handleId_;

}

void Coin::Update(void)
{
	ProcessRot();

	transform_.Update();
	animationController_->Update();

	//エフェクト
	PlayEffectCoinSmoke();
}

void Coin::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	//デバッグ表示
	//DrawDebug();
}

void Coin::Release(void)
{
	MV1DeleteModel(transform_.modelId);

	StopEffekseer3DEffect(coinSmoke_);
}

VECTOR Coin::GetPos(void)
{
	return transform_.pos;
}

bool Coin::GetHit(void)
{
	return isHit_;
}

void Coin::Hit(void)
{
	animationController_->Play(static_cast<int>(5), true);
	isHit_ = false;

}

void Coin::DrawDebug(void)
{
	//衝突判定球体の表示
	if (isHit_) {
		DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0xffffff, 0xffffff, false);
	}
	else {
		DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0xff0000, 0xff0000, false);
	}
}

void Coin::PlayEffectCoinSmoke(void)
{
	int playHandle = -1;
	float posY = transform_.pos.y - 50.0f;

	stepEffect_ -= SceneManager::GetInstance().GetDeltaTime();

	if (stepEffect_ < 0 && isHit_)
	{
		//エフェクトを再生する
		playHandle = PlayEffekseer3DEffect(coinSmoke_);

		//エフェクトの大きさ
		SetScalePlayingEffekseer3DEffect(playHandle, 10.0f, 50.0f, 10.0f);

		//エフェクトの位置
		SetPosPlayingEffekseer3DEffect(
			playHandle, transform_.pos.x, posY, transform_.pos.z);

		stepEffect_ = 210.0f / 60.0f;

	}

}

void Coin::ProcessRot(void)
{
	float rotPow = 0.5f;

	transform_.rot.y += AsoUtility::Deg2RadF(rotPow);

	//Turn(1.0f, AsoUtility::AXIS_Z);
}

void Coin::Turn(double deg, VECTOR axis)
{
	Quaternion tmpQ =
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(static_cast<float>(deg)), axis);

	transform_.quaRot = transform_.quaRot.Mult(tmpQ);
}
