#pragma once
class Collider
{
public :

	// �Փˎ��
	enum class TYPE
	{
		STAGE
	};

	// �R���X�g���N�^
	Collider(TYPE type, int modelId,int num);

	// �f�X�g���N�^
	~Collider(void);

	// �Փˎ��
	TYPE type_;

	// ���f���̃n���h��ID
	int modelId_;

	// �������ꂽ�X�e�[�W�̉��Ԗڂ�
	int num_;
};

