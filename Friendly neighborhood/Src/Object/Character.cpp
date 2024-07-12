#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "Player.h"
#include "Character.h"
#include "Common/AnimationController.h"
#include "../Application.h"

Character::Character(Transform transform)
	: resourceManager_(ResourceManager::GetInstance())
{
	transform_ = transform;
}

Character::~Character()
{
}

void Character::Init(void)
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
}

void Character::Update(void)
{
	transform_.Update();
	animationController_->Update();
	//エフェクト
	PlayEffectCoinSmoke();
}

void Character::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
}

void Character::Release(void)
{
	MV1DeleteModel(transform_.modelId);

}

VECTOR Character::GetPos(void)
{
	return transform_.pos;
}

bool Character::GetHit(void)
{
	return isHit_;
}

void Character::Hit(void)
{
	animationController_->Play(static_cast<int>(5), true);
	isHit_ = false;
}

void Character::DrawDebug(void)
{
	//衝突判定球体の表示
	if (isHit_) {
		DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0xffffff, 0xffffff, false);
	}
	else {
		DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0xff0000, 0xff0000, false);
	}
}



