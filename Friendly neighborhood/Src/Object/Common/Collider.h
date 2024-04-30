#pragma once
class Collider
{
public :

	// 衝突種別
	enum class TYPE
	{
		STAGE
	};

	// コンストラクタ
	Collider(TYPE type, int modelId,int num);

	// デストラクタ
	~Collider(void);

	// 衝突種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

	// 分割されたステージの何番目か
	int num_;
};

