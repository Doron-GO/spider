#pragma once
#include "SceneBase.h"
class SceneManager;
class AnimationController;
class Controller;
class Stage;
class Character;
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

	//�v���C���[�̃A�j���[�V��������
	AnimationController* animationController_;	

	//�O���b�h��
	std::unique_ptr<Grid> grid_;

	bool isLoaded_;

	//�T���L�����N�^�[�̏�񐶐�
	void MakeCharacter(void);

	//�T���L�����N�^�[�ƃv���C���[�̏Փ˔���
	void IsHitCharcterPlayer(void);

	std::unique_ptr<SwingPoint> swi_;

	bool isLoaded ;

	//�w�i
	std::unique_ptr<SkyDome> skyDome_;

	// �O�l�̎����̌����ڂ������l��
	std::vector<Character*> character_;

	//�v���C���[
	Player* player_;

	int isHitNum_;

};
