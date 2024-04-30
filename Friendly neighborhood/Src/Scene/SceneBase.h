#pragma once
#include <memory>

class Controller;

class SceneBase
{
public:
	SceneBase(void);
	virtual ~SceneBase(void);

	// 初期化処理
	virtual void Init(void);

	// 更新ステップ
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	virtual void Release(void);

protected:

	//コントローラー
	//std::unique_ptr<Controller> controller_;

};

