#pragma once
#include "../Object/Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 280.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(void);
	SkyDome(const Transform* syncTransform);	//�v���C���[�ɒǏ]����ꍇ�͂�����
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �v���C���[�̏��
	const Transform* syncTransform_;

	// ���
	STATE state_;

	// ���f������̊�{���
	Transform transform_;

	// ��ԑJ��
	void ChangeState(STATE state);

};
