#pragma once
#include "SceneBase.h"
class SceneManager;
class AnimationController;
class Controller;
class Stage;
class Character;
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

	//プレイヤーのアニメーション制御
	AnimationController* animationController_;	

	//グリッド線
	std::unique_ptr<Grid> grid_;

	bool isLoaded_;

	//探すキャラクターの情報生成
	void MakeCharacter(void);

	//探すキャラクターとプレイヤーの衝突判定
	void IsHitCharcterPlayer(void);

	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;

	//背景
	std::unique_ptr<SkyDome> skyDome_;

	// 三人の自分の見た目をした人間
	std::vector<Character*> character_;

	//プレイヤー
	Player* player_;

	int isHitNum_;

};
