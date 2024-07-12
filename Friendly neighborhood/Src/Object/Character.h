#pragma once
#include "Common/Transform.h"

class ResourceManager;
class AnimationController;
class Player;

class Character
{
public:
	//衝突判定球の半径
	static constexpr float COL_RADIUS = 50.0f;

	Character(Transform transform);
	~Character();

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	//情報取得
	VECTOR GetPos(void);

	//当たり判定
	bool GetHit(void);

	//情報格納
	void Hit(void);

private:

	//デバッグ表示
	void DrawDebug(void);

	ResourceManager& resourceManager_;

	//情報管理
	Transform transform_;

	//回転関連
	Quaternion coinRotY_;
	float stepRot_;

	//衝突判定
	bool isHit_;
	AnimationController* animationController_;	//プレイヤーのアニメーション制御

};