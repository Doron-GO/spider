#pragma once
#include <DxLib.h>
class Transform;

class Capsule
{

public :

	// デバッグ時の簡易カプセル表示の色
	static constexpr int COLOR = 0xffffff;

	// コンストラクタ
	// カプセルをつけるオブジェクトを指定する
	Capsule(Transform* parent);

	// デストラクタ
	~Capsule(void);

	// 描画
	void Draw(void);

	// 親Transformからの相対位置をセット
	void SetRelativePosTop(const VECTOR& pos);
	void SetRelativePosDown(const VECTOR& pos);

	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;
	VECTOR GetRelativePos(const VECTOR& pos) const;

	// 半径
	float GetRadius(void) const;
	void SetRadius(float radius);

	// 高さ
	float GetHeight(void) const;

	// カプセルの中心座標
	VECTOR GetCenter(void) const;

	// コピー
	Capsule Copy(Transform* base);

private :
	//親Transfrom
	Transform* transformParent_;

	// 相対位置(上側)
	VECTOR relativePosTop_;

	// 相対位置(下側)
	VECTOR relativePosDown_;

	// 半径
	float radius_;

};

