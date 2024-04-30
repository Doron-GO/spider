#pragma once
#include "Common/Transform.h"

class ResourceManager;
class AnimationController;
class Player;

class Coin
{
public:
	//衝突判定球の半径
	static constexpr float COL_RADIUS = 50.0f;

	Coin(Transform transform);
	~Coin();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//情報取得
	VECTOR GetPos(void);
	bool GetHit(void);

	//情報格納
	void Hit(void);

private:
	//デバッグ表示
	void DrawDebug(void);

	//エフェクト
	void PlayEffectCoinSmoke(void);

	//回転
	void ProcessRot(void);
	void Turn(double deg, VECTOR axis);

	ResourceManager& resourceManager_;

	//情報管理
	Transform transform_;

	//回転関連
	Quaternion coinRotY_;
	float stepRot_;

	//衝突判定
	bool isHit_;
	AnimationController* animationController_;	//プレイヤーのアニメーション制御

	//エフェクト関連
	int coinSmoke_;
	float stepEffect_;
};

