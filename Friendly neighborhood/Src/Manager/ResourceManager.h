#pragma once
#include <map>
#include <string>
#include "Resource.h"
#define lpResourceMng  ResourceManager::GetInstance()

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		SKYDOME,
		STAGE,
		PLAYER,
		PLAYER_SHADOW,
		COIN,
		START
	};


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resouceMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void);

	// 内部ロード
	Resource* _Load(SRC src);

};

