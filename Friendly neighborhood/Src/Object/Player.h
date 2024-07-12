#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
#include"../Object/Stage.h"
#include "../Object/Common/Transform.h"
#include"SwingPoint.h"
class AnimationController;
class ResourceManager;
class GravityManager;
class Quaternion;
class Collider;
class Capsule;
class Controller;
class Camera;

class Player
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		JUMP,
		SWING,
		ODO
	};

	Player(void);
	~Player(void);

	void Init(void);
	void Update(float delta,VECTOR pos, VECTOR gra, VECTOR end,VECTOR Billpos);
	void SetFollowTarget(Camera* target);
	void AddCollider(Collider* collider);
	void Draw(void);
	void SwingDraw(void);
	int CheckSection(void);
	void SetEndPpos(VECTOR pos);
	VECTOR GetCameraAngles(void);
	Transform* GetTransform(void);
	bool IsAlive();

	Quaternion quaPaddir;
private:

	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;	//通常移動

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//ジャンプ量
	static constexpr float POW_JUMP = 60.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;
	//スウィング中重力
	static constexpr float SWING_GRAVITY = 3500.0f;

	void (Player::* _update)(float delta);//新たな分岐の仕方

//フラグ
	int stepGrav;

	//ジャンプ状態かどうかを判定
	bool isJump_;

	//スウィング状態からジャンプする
	bool isSwingJump_;

	//スウィング状態に移行する
	bool isSwingFlag_;

	//落下状態化を判定
	bool isFall_;

	//
	float stepJump_;

	float stepSwingump_;

	VECTOR movedPos_;	//移動後の座標

	//ジャンプ力
	VECTOR jumpPow_;

	//プレイヤーがジャンプ力が重力に勝っているかを判定
	float jumpDot_;

	//プレイヤーの移動量
	VECTOR movePow_;

	Camera* camera_;

	float sectionPos[2];

	int section_;//
	VECTOR testPos_;
	VECTOR endPos_;		//支点の座標

	VECTOR setEndPos_;	//セット用支点の座標

	VECTOR billPos_;	//ビルの座標

	VECTOR stringV_;		//支点からプレイヤーへのベクトル

	VECTOR swingYnorm_;

	//重力
	VECTOR gravity_;

	static constexpr VECTOR GRAVITY = { 0.0f,3500.0f,0.0f };

	//重力方向の正規化ベクトル
	VECTOR gravityNorm_;

	//swing時の重力方向
	VECTOR swingGravity_;

	//ス印字の重力ベクトルを正規化したもの
	VECTOR swingGravityNorm_;

	//軸から錘の正規化済み垂直ベクトル
	VECTOR yNorm_;



	//最終的に向きたい方向
	Quaternion goalQuaRot_;

	//y軸回転
	Quaternion pendulumRotY_;

	//プレイヤーとの当たり判定を取るモノたち
	std::vector<Collider*>colliders_;

	//カプセルでの当たり判定
	std::unique_ptr<Capsule>capsule_;

	//重力の大きさ
	float gMag_;

	//角速度
	float omega_;

	//紐の長さ
	float length_;

	//角度
	float theta_;

	//プレイヤーのアニメーション制御
	AnimationController* animationController_;

	//プレイヤーのモデル制御
	Transform transform_;

	//３Dモデルの読み込み
	ResourceManager& resourceManager_;

	//アニメーション読み込み
	void AnimationInit(void);	

	void DrawDebug(void);	//デバッグ表示

	//スイング時のアップデート
	void UpdateSwing(float delta);

	//落下状態
	void FallUpdate(float delta);

	//通常のアップデート(スイング状態以外)
	void StandardUpdate(float delta);

	//スイング開始前のパラメータを設定
	bool SetSwingParam(VECTOR pos,VECTOR end);
	
	//スイング状態にする関数
	void SwingStart();

	//振り子の計算を行う
	void Swing(float delta);	


	void ProcessMove(void);

	void ProcessJump(void);

	void CalcGravityPow(void);

	void Collision(void);

	void CollisionCupsule(void);

	//壁との当たり判定
	void CollisionSphere(void); 

	//地面との当たり判定
	void CollisionGravity(void); 

	//到達したい角度まで、回転させる	
	void Rotate(void);

	float Magnitude(VECTOR pos)const;
	float Dot(const VECTOR& a, const VECTOR& b);
	VECTOR Normalized(VECTOR& v);


};

VECTOR operator*(const VECTOR& v, float scale);
VECTOR operator*=( VECTOR& v, float scale);
VECTOR operator*(float scale, const VECTOR& v);
VECTOR operator+(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& v);

