//------------------------------------------
// 氏名      ：氏名
//------------------------------------------
#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// 初期化失敗
		TRACE("初期化失敗");
		return -1;
	}

	// 実行
	instance.Run();

	// 解放
	instance.Release();

	if (instance.IsReleaseFail())
	{
		// 解放失敗
		TRACE("解放失敗");
		return -1;
	}

	return 0;

}
