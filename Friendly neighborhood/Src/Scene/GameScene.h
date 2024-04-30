#pragma once
#include "SceneBase.h"
class SceneManager;
class AnimationController;
class Controller;
class Stage;
class SkyDome;
class Player;
class Grid;
class SwingPoint;
class Coin;

class GameScene 
	: public SceneBase
{

public:

	GameScene(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;
	void Load(void);
private:
	//ステージ
	std::unique_ptr<Stage> stage_;
	AnimationController* animationController_;	//プレイヤーのアニメーション制御

	//グリッド線
	std::unique_ptr<Grid> grid_;
	bool isLoaded_;

	//コインの情報生成
	void MakeCoin(void);

	//コインとプレイヤーの衝突判定
	void IsHitCoinPlayer(void);

	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;
	//背景
	std::unique_ptr<SkyDome> skyDome_;

	// コイン
	std::vector<Coin*> coins_;

	//プレイヤー
	Player* player_;

	int isHitNum_;

};
