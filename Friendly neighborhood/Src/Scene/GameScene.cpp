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
#include "../Object/Character.h"

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
	MakeCharacter();
	for (const auto c : character_)
	{
		c->Init();
	}
	isHitNum_ = 0;
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
}

void GameScene::Update(void)
{	
	auto& ins = InputManager::GetInstance();
	if(isHitNum_==character_.size())
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
	auto swingPos = swi_->SetSwingPoint(player_->GetTransform()->pos);
	auto gravity= swi_->SetGravity(player_->GetTransform()->pos);
	player_->Update(lpSceneMng.GetDeltaTime(), lpSceneMng.GetCamera()->GetDir(), gravity, swingPos);
	skyDome_->Update();
	stage_->Update();
	// キャラクター
	for (const auto c : character_)
	{
		c->Update();
	}
	IsHitCharcterPlayer();
}

void GameScene::Draw(void)
{
	for (const auto c : character_)
	{
		c->Draw();
	}
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	swi_->Draw();
}

void GameScene::DrawDebug(void)
{
}

void GameScene::Release(void)
{	
	delete player_;
	for (const auto c : character_)
	{
		c->Release();
		delete c;
	}
	character_.clear();
}

void GameScene::Load(void)
{

	stage_->Init();
	player_->Init();
	skyDome_->Init();
	grid_->Init();
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());
	isLoaded_ = true;
}

void GameScene::MakeCharacter(void)
{
	Transform trans;
	Character* character;
	trans.pos = { 10167, 648, 25656 };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	character = new Character(trans);
	character->Init();
	character_.push_back(character);

	trans.pos = { 12065, 4094, 20288 };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	character = new Character(trans);
	character->Init();
	character_.push_back(character);
}

void GameScene::IsHitCharcterPlayer(void)
{

	// コインとプレイヤーの衝突判定
	auto p = player_->GetTransform();

	for (const auto c : character_)
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