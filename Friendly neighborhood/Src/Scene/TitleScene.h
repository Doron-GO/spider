#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SceneManager;
class Grid;
class SkyDome;
class AnimationController;

class TitleScene 
	: public SceneBase
{

public:
	TitleScene(void);
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	int count_;
	AnimationController* animationController_;	//�v���C���[�̃A�j���[�V��������

	// �摜
	int imgTitle_;
	int imgTitle_2;
	int imgPush_;
	int p;
	//�O���b�h��
	Transform transform_;	//�v���C���[�̃��f������

	// �w�i
	std::unique_ptr<SkyDome> skyDome_;

};
