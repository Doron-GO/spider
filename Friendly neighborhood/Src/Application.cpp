#include <DxLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"
#include "Input/InputManager.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("AGS夏");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib の初期化失敗");
		isInitFail_ = true;
		return;
	}

	InitEffekseer();

	// デバッグ機能初期化
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// リソース管理初期化
	SetUseDirectInputFlag(true);

	//InputManager::CreateInstance();
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{
	auto& inputManager = InputManager::GetInstance();

	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		_dbgStartDraw();
		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		_dbgAddDraw();

		ScreenFlip();

	}

}

void Application::Release(void)
{
	//Effekseerを終了する
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

Application::~Application(void)
{
	delete instance_;
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}
