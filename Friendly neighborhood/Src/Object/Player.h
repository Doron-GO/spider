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

	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;	//通常移動

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//ジャンプ量
	static constexpr float POW_JUMP = 60.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	//煙エフェクトの発生間隔
	static constexpr float TIME_SMOKE = 0.3f;

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

	void AnimationInit(void);	//アニメーション読み込み
	void DrawDebug(void);	//デバッグ表示
	void UpdatePendulum(float delta);
	void UpdateGround(float delta);
	bool SetSwingParam(VECTOR pos,VECTOR end);
	void SwingStart();
	void Swing(float delta);	//振り子の計算を行う
	void Flying(float delta);
	void ProcessMove(void);
	void ProcessJump(void);
	void CalcGravityPow(void);
	void Collision(void);
	void CollisionCupsule(void);
	void CollisionSphere(void); //壁との当たり判定
	void CollisionGravity(void); 

	void SetGoalRotate(float rotRad);	//	到達したい角度を設定
	void SetGoalRotate2(VECTOR vec);	//	到達したい角度を設定
	void Rotate(void);					//到達したい角度まで、回転させる	

	float Magnitude(VECTOR pos)const;
	float Dot(const VECTOR& a, const VECTOR& b);
	VECTOR Normalized(VECTOR& v);

	void (Player::* phase_)(float delta);//新たな分岐の仕方

	//フラグ
	int stepGrav;
	bool isJump_;
	bool isSwingJump_;//スウィング状態からジャンプする
	bool isSwingFlag_;//スウィング状態に移行する
	bool isFall_;


	float stepJump_;
	float stepSwingump_;
	VECTOR movedPos_;	//移動後の座標
	VECTOR jumpPow_;		//ジャンプ量
	float jumpDot_;

	Camera* camera_;

	float sectionPos[2];
	int tttt;
	VECTOR movePow_;
	VECTOR endPos_;		//支点の座標
	VECTOR endPos_2;	//支点の座標2
	VECTOR billPos_;	//ビルの座標
	VECTOR stringV_;		//支点からプレイヤーへのベクトル
	VECTOR swingYnorm_;
	VECTOR gravity_;		//重力
	VECTOR gravityNorm_;	//重力方向の正規化ベクトル
	VECTOR swingGravityNorm_;	
	VECTOR yNorm_;			//軸から錘の正規化済み垂直ベクトル
	VECTOR swingGravity_;	//swing時の呪力方向
	VECTOR swingGravity_2;	 //swing時の呪力方向

	Quaternion goalQuaRot_;		//最終的に向きたい方向
	Quaternion pendulumRotY_;	//y軸回転

	std::vector<Collider*>colliders_;
	std::unique_ptr<Capsule>capsule_;

	SwingPoint swingPoint_;
	float xVecX, xVecY;

	float gMag_;		//重力の大きさ
	float omega_;			//角速度
	float length_;			//紐の長さ
	float theta_;

	AnimationController* animationController_;	//プレイヤーのアニメーション制御
	//std::unique_ptr<Controller> controller_;	//操作
	Transform transform_;	//プレイヤーのモデル制御
	ResourceManager& resourceManager_;	//３Dモデルの読み込み
};

VECTOR operator*(const VECTOR& v, float scale);
VECTOR operator*=( VECTOR& v, float scale);
VECTOR operator*(float scale, const VECTOR& v);
VECTOR operator+(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& va, const VECTOR vb);
VECTOR operator-(const VECTOR& v);

