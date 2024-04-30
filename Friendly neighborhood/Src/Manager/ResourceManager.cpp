#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	Resource res;

	// �^�C�g���摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "title.png");
	resouceMap_.emplace(SRC::TITLE, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "START.png");
	resouceMap_.emplace(SRC::START, res);

	// PushSpace�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PushSpace.png");
	resouceMap_.emplace(SRC::PUSH_SPACE, res);

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resouceMap_.emplace(SRC::SKYDOME, res);

	//�X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/B34.mv1");
	resouceMap_.emplace(SRC::STAGE, res);

	//�v���C���[
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Gwen_model1.mv1");
	resouceMap_.emplace(SRC::PLAYER, res);

	//�R�C��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Gwen_model1.mv1");
	resouceMap_.emplace(SRC::COIN, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete instance_;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resouceMap_.find(src);
	if (rPair == resouceMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
