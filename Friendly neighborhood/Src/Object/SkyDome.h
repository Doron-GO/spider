#pragma once
#include "../Object/Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 280.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(void);
	SkyDome(const Transform* syncTransform);	//プレイヤーに追従する場合はこっち
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// プレイヤーの情報
	const Transform* syncTransform_;

	// 状態
	STATE state_;

	// モデル制御の基本情報
	Transform transform_;

	// 状態遷移
	void ChangeState(STATE state);

};
