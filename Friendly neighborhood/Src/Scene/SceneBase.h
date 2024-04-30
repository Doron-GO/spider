#pragma once
#include <memory>

class Controller;

class SceneBase
{
public:
	SceneBase(void);
	virtual ~SceneBase(void);

	// ����������
	virtual void Init(void);

	// �X�V�X�e�b�v
	virtual void Update(void);

	// �`�揈��
	virtual void Draw(void);

	// �������
	virtual void Release(void);

protected:

	//�R���g���[���[
	//std::unique_ptr<Controller> controller_;

};

