#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"

#define lpGravityMng GravityManager::GetInstance()

class SceneManager;
class Planet;
class Player;
class Camera;

class GravityManager
{
public:

	// 重力方向が完全に変わるまでの時間
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// 回転力
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	//重力
	static constexpr float GRAVITY_POW = 10.0f;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static GravityManager& GetInstance(void);

	void Init(void);

	void Update(void);

	// 重力方向や、正方向の計算
	void Calculate(void);

	// 操作プレイヤーをセット
	void SetPlayer(Player* player);

	const Transform& GetTransform(void) const;
	
	// 重力方向を取得
	VECTOR GetDirGravity(void) const;

	// 重力方向の反対方向を取得
	VECTOR GetDirUpGravity(void) const;

	// 重力の強さを取得
	float GetPower(void) const;

	// 惑星切り替え前の回転情報
	Quaternion GetPremQuaRot(void) const;

	// プレイヤーと足元衝突した法線
	void SetLastHitNormal(const VECTOR& normal);

	// プレイヤーと足元衝突した位置
	void SetLastHitPos(const VECTOR& pos);
	VECTOR GetLastHitPos(void) const;

protected:
	GravityManager(void);
	~GravityManager(void);

	// 静的インスタンス
	static GravityManager* Instance_;

	// プレイヤー
	Player* player_;

	//カメラ
	Camera* camera_;

	Transform transform_;

	// 重力方向
	VECTOR dirGravity_;
	VECTOR preDirGravity_;

	// 重力方向の反対方向
	VECTOR dirUpGravity_;

	// プレイヤーと足元衝突した法線
	VECTOR lastHitNormal_;

	// プレイヤーと足元衝突した位置
	VECTOR lastHitPos_;

	// 更新ステップ
	float step_;

	// 回転力
	double slerpPow_;

	//重力量
	float gravityPow_;

	// 重力方向を計算
	VECTOR CalcDirGravity(void) const;

};

