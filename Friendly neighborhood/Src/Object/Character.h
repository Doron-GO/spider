#pragma once
#include "Common/Transform.h"

class ResourceManager;
class AnimationController;
class Player;

class Character
{
public:
	//�Փ˔��苅�̔��a
	static constexpr float COL_RADIUS = 50.0f;

	Character(Transform transform);
	~Character();

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	//���擾
	VECTOR GetPos(void);

	//�����蔻��
	bool GetHit(void);

	//���i�[
	void Hit(void);

private:

	//�f�o�b�O�\��
	void DrawDebug(void);

	ResourceManager& resourceManager_;

	//���Ǘ�
	Transform transform_;

	//��]�֘A
	Quaternion coinRotY_;
	float stepRot_;

	//�Փ˔���
	bool isHit_;
	AnimationController* animationController_;	//�v���C���[�̃A�j���[�V��������

};