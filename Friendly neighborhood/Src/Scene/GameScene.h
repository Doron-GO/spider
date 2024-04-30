#pragma once
#include "SceneBase.h"
class SceneManager;
class AnimationController;
class Controller;
class Stage;
class SkyDome;
class Player;
class Grid;
class SwingPoint;
class Coin;

class GameScene 
	: public SceneBase
{

public:

	GameScene(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;
	void Load(void);
private:
	//�X�e�[�W
	std::unique_ptr<Stage> stage_;
	AnimationController* animationController_;	//�v���C���[�̃A�j���[�V��������

	//�O���b�h��
	std::unique_ptr<Grid> grid_;
	bool isLoaded_;

	//�R�C���̏�񐶐�
	void MakeCoin(void);

	//�R�C���ƃv���C���[�̏Փ˔���
	void IsHitCoinPlayer(void);

	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;
	//�w�i
	std::unique_ptr<SkyDome> skyDome_;

	// �R�C��
	std::vector<Coin*> coins_;

	//�v���C���[
	Player* player_;

	int isHitNum_;

};
