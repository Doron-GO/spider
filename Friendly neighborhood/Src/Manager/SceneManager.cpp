#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "Camera.h"
#include "GravityManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneID_ = SCENE_ID::TITLE;
	waitSceneID_ = SCENE_ID::NONE;

	// �d�͊Ǘ�������
	//GravityManager::CreateInstance();
	scene_ = new SceneBase();
	scene_->Init();

	fader_ = new Fader();
	fader_->Init();

	camera_ = new Camera();
	camera_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);	//�f�o�b�O�\��

}

void SceneManager::Init3D(void)
{

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	// �t�H�O�ݒ�
	SetFogEnable(false);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	// �d�͕����̌v�Z
	//GravityManager::GetInstance().Update();

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	// �J�����X�V�X�e�b�v
	camera_->Update();

}

void SceneManager::Draw(void)
{

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	camera_->SetBeforeDraw();

	//Effekseer�̍X�V����
	UpdateEffekseer3D();

	// �`��
	scene_->Draw();

	//Effekseer�̕`�揈��
	DrawEffekseer3D();

	// ��Ƀ|�X�g�G�t�F�N�g�p
	camera_->Draw();

	fader_->Draw();

}

void SceneManager::Release(void)
{

	scene_->Release();
	delete scene_;

	delete fader_;

	camera_->Release();
	delete camera_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneID_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return sceneID_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

SceneManager::SceneManager(void)
{

	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;

}

SceneManager::~SceneManager(void)
{
	delete instance_;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	sceneID_ = sceneId;

	// ���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneID_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			DoChangeScene(waitSceneID_);
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


