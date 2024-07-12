#pragma once
#include<map>
#include"Common/Transform.h"
#include"../Json/json.hpp"
#include"Stage.h"
using json = nlohmann::json;


class SwingPoint
{
public:

	enum class SIDE
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT
	};

	SwingPoint();
	~SwingPoint();

	void Draw(void);
	void Load(void);

	const VECTOR SetSwingPoint(VECTOR PlayerPos,int section);
	const VECTOR GetBillPoint();
	const VECTOR SetGravity(VECTOR PlayerPos);
private:
	float Magnitude(VECTOR pos)const;


	json json_;

	//�x�_�̍��W
	std::vector<VECTOR> pos_;										
	//�x�_��񂪂�����������Ă���
	std::map<SIDE, std::vector<VECTOR>> swingPoint_;

	//20�� �r�����X�g
	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;

	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_; 
	
	int totalSwingPoint_;

	float min;
	//��ԋ߂����W�̃��X�g�ԍ�
	int minSwingPointNum_;

	//�d�͕���
	VECTOR norm_;

	//�x�_�ԍ���n���Ǝx�_�̍��W���߂��Ă���
	std::map<int, VECTOR> swingPoint2_;

	//�x�_�ԍ���n����pair���߂��Ă���@�d�͕����E���W
	std::map<int, std::pair<VECTOR,VECTOR>> swingAndNormList_;

	//�v���C���[����x�_�܂ł̋������i�[����
	std::vector<float> distance_;

	//first��distance,second�͎x�_���W
	std::pair<float, VECTOR>fainal_;

	//fainal_�������Ă���
	std::vector<std::pair<float, VECTOR>> comparison_;

	//�r���̍��W�������Ă��� ��ԋ߂��r������肷��̂Ɏg��
	std::vector<VECTOR> BillPpoint_;

	//�x�_�̍��W�������Ă���
	std::vector<VECTOR> swingPoint3_;

	//�r���ɑ΂���x�_�̃��X�g�@�r�������肵���炻�̃r���ɂ��Ă���x�_�Ō�������
	std::vector<std::vector<VECTOR>> swingList3_;

	int swingNum_;

	int billNum_;

};

