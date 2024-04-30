#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Flame.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
//#include "../Manager/GravityManager.h"
#include "../Input/InputManager.h"
#include "../Manager/Camera.h"
#include "../Input/Controller.h"
#include "../Object/Common/Grid.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"
#include "../Object/Common/AnimationController.h"

TitleScene::TitleScene(void)
	:SceneBase()
{
	imgPush_ = -1;
	imgTitle_ = -1;
	skyDome_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	//lpGravityMng.Init();

	ResourceManager& rm = lpResourceMng;

	// 画像読み込み
	imgTitle_ = rm.Load(ResourceManager::SRC::TITLE).handleId_;
	imgTitle_2 = rm.Load(ResourceManager::SRC::START).handleId_;
	imgPush_ = rm.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	transform_.SetModel(
		lpResourceMng.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
		transform_.scl = { 2.0f,2.0f ,2.0f };
	transform_.pos = { 0.0f, -100.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();
	std::string path = Application::PATH_MODEL + "Player/";

	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add(0, path + "Idle1.mv1", 20.0f);


	// グリッド線

	// 背景
	//skyDome_ = std::make_unique<SkyDome>();
	//skyDome_->Init();

	// 定点カメラ
 	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	count_ = 0;
}

void TitleScene::Update(void)
{
	//controller_->Update();

	count_++;
	animationController_->Play(0);


	// シーン遷移
	InputManager& ins = InputManager::GetInstance();

	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
	//	!contData[OldFlame][static_cast<int>(InputID::Btn3)])
	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
	//skyDome_->Update();

}

void TitleScene::Draw(void)
{
	//skyDome_->Draw();

	MV1DrawModel(transform_.modelId);
	//タイトル画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 0.35, 0.0, imgTitle_, true,5.0);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 550, 0.35, 0.0, imgTitle_2, true,5.0);

	if ((count_ / 30) % 2 != 0)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, imgPush_, true);
	}

	//デバッグ用
	//DrawString(0, 0, "TitleScene", 0xffffff);
}

void TitleScene::Release(void)
{
	//skyDome_->Release();
}
