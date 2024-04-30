#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Common/Flame.h"
#include "../Input/Controller.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/StagePiece.h"
#include "../Object/Player.h"
#include "GameScene.h"
#include "../Object/Common/Grid.h"
#include "../Object/SwingPoint.h"
#include "../Object/Coin.h"

GameScene::GameScene(void)
	:SceneBase()
{
	player_ = nullptr;
}

void GameScene::Init(void)
{
	//SetUseASyncLoadFlag(TRUE);
	isLoaded = false;
	//ステージ
	stage_ = std::make_unique<Stage>();
	//プレイヤー
	player_ = new Player();
	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	// グリッド線
	grid_ = std::make_unique<Grid>();
	swi_ =std::make_unique<SwingPoint>();

	stage_->Init();
	swi_->Load();
	player_->Init();

	//コイン
	MakeCoin();
	for (const auto c : coins_)
	{
		c->Init();
	}

	isHitNum_ = 0;



	//一時的	
	
	//for (int i =1; i<21;i++ )
	//{ 
	//	auto p = stage_->GetPiece();
	//	auto oo = p[static_cast<Stage::STAGE_NUM>(i-1)];
	//	player_->AddCollider(oo->GetTransform()->collider);
	//}

	for (int i = 1; i <= 1; i++)
	{
		auto p = stage_->GetPiece();
		auto oo = p[static_cast<Stage::STAGE_NUM>(i - 1)];
		player_->AddCollider(oo->GetTransform()->collider);
	}

	skyDome_->Init();
	grid_->Init();
	//カメラの設定
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());
	isLoaded_ = true;

	//loadThread_ = std::thread(&GameScene::Load, this);
	//Load();
	//SetUseASyncLoadFlag(FALSE);

}

void GameScene::Update(void)
{
	
	//controller_->Update();
	// シーン遷移
	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
	//	!contData[OldFlame][static_cast<int>(InputID::Btn3)])

	player_->CheckSection();
	auto& ins = InputManager::GetInstance();
	if(isHitNum_==coins_.size())
	{
		for (int cnt = 0; cnt < 100; cnt++)
		{
			continue;
		}
		lpSceneMng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if(player_->IsAlive())
	{
		lpSceneMng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	auto section= swi_->SetSwingPoint(player_->GetTransform()->pos,player_->CheckSection(),player_->GetCameraAngles());
	auto gravity= swi_->SetGravity(player_->GetTransform()->pos);
	auto bill= swi_->GetBillPoint();
	player_->Update(lpSceneMng.GetDeltaTime(), lpSceneMng.GetCamera()->GetDir(), gravity, section, bill);
	skyDome_->Update();

	stage_->Update();
	// コイン
	for (const auto c : coins_)
	{
		c->Update();
	}

	IsHitCoinPlayer();
}

void GameScene::Draw(void)
{

	// コイン
	for (const auto c : coins_)
	{
		c->Draw();
	}

	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	swi_->Draw();
	//grid_->Draw();
	// デバッグ表示
	//DrawDebug();

}

void GameScene::DrawDebug(void)
{
}

void GameScene::Release(void)
{	
	delete player_;
	for (const auto c : coins_)
	{
		c->Release();
		delete c;
	}
	coins_.clear();

}

void GameScene::Load(void)
{



	//ステージ
	//stage_ = std::make_unique<Stage>();
	stage_->Init();

	//プレイヤー
	//player_ = new Player();
	player_->Init();
	//player_->AddCollider(stage_->GetTransform()->collider);

	// スカイドーム
	//skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	// グリッド線
	//grid_ = std::make_unique<Grid>();
	grid_->Init();
	//カメラの設定
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());

	isLoaded_ = true;

}
void GameScene::MakeCoin(void)
{
	Transform trans;
	Coin* coin;

	trans.pos = { 10167, 648, 25656 };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);

	trans.pos = { 12065, 4094, 20288 };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);

	//trans.pos = { 28646, 1206, 42177 };
	//trans.scl = { 1.0f,1.0f, 1.0f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(0.0f),
	//	AsoUtility::Deg2RadF(0.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);
	//coin = new Coin(trans);
	//coin->Init();
	//coins_.push_back(coin);

}

void GameScene::IsHitCoinPlayer(void)
{

	// コインとプレイヤーの衝突判定
	auto p = player_->GetTransform();

	for (const auto c : coins_)
	{
		//表示されているとき
		if (c->GetHit())
		{
			//衝突判定
			auto hit = MV1CollCheck_Sphere(p->modelId, -1, c->GetPos(), c->COL_RADIUS);
			MV1CollResultPolyDimTerminate(hit);

			//ひとつでも衝突していたら
			if (hit.HitNum > 0)
			{
				c->Hit();
				isHitNum_++;
				break;
			}
		}
	}
}