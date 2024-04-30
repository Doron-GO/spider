#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include"../Input/InputManager.h"


class SceneManager;
class Transform;
class Collider;
class Controller;

class Camera
{

public:

	// FIXED_POINT : カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// FOLLOW : カメラ位置(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 100.0f, -800.0f };

	// FOLLOW : 注視点(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 100.0f, 500.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);
	
	// 追従対象との調整座標
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera(void);
	~Camera();

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);
	void Release(void);

	void SetTransform(const Transform* transform);

	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;
	VECTOR GetTargetPos(void) const;
	Quaternion GetQuaRot(void) const;
	Quaternion GetQuaRotOutX(void) const;
	VECTOR GetDir(void) const;
	VECTOR GetDirvec(VECTOR vec) ;

	void ChangeMode(MODE mode);

private:
	//カメラを動かす
	void ProcessRot(void);

	InputManager::JOYPAD_NO padNumber_;


	//カメラの初期設定
	void SetDefault(void);

	// Transformの位置と同期を取る
	void SyncTransform(void);

	//コントローラー
	std::unique_ptr<Controller> controller_;

	// カメラが追従対象とするTransform
	const Transform* syncTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion quaRotOutX_;

	// カメラ角度
	Quaternion quaRot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

};

