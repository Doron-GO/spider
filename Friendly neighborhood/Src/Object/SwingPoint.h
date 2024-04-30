#pragma once
#include<map>
#include"../Json/json.hpp"
#include"Common/Transform.h"
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

	const VECTOR SetSwingPoint(VECTOR PlayerPos,int section,VECTOR Angle);
	const VECTOR GetBillPoint();
	const VECTOR SetGravity(VECTOR PlayerPos);
private:
	float Magnitude(VECTOR pos)const;


	json json_;

	std::vector<VECTOR> pos_;										//�x�_�̍��W
	std::map<SIDE, std::vector<VECTOR>> swingPoint_;				//�x�_��񂪂�����������Ă���
	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20�� �r�����X�g
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_; 
//------------------------------------------------------------------------------------------------------
	
	int totalSwingPoint_;
	float min;
	int minSwingPointNum_;								//��ԋ߂����W�̃��X�g�ԍ�
	VECTOR norm_;										//�d�͕���
	std::map<int, VECTOR> swingPoint2_;					//�x�_�ԍ���n���Ǝx�_�̍��W���߂��Ă���
	std::map<int, std::pair<VECTOR,VECTOR>> swingAndNormList_;//�x�_�ԍ���n����pair���߂��Ă���@�d�͕����E���W


	std::vector<float> distance_;						//�v���C���[����x�_�܂ł̋������i�[����
	std::pair<float, VECTOR>fainal_;					//first��distance,second�͎x�_���W
	std::vector<std::pair<float, VECTOR>> comparison_;	//fainal_�������Ă���
//--------------------------------------------------------------------------------------------------------------
	std::vector<VECTOR> BillPpoint_;					//�r���̍��W�������Ă��� ��ԋ߂��r������肷��̂Ɏg��
	std::vector<VECTOR> swingPoint3_;					//�x�_�̍��W�������Ă���
	std::vector<std::vector<VECTOR>> swingList3_;		//�r���ɑ΂���x�_�̃��X�g�@�r�������肵���炻�̃r���ɂ��Ă���x�_�Ō�������
	int swingNum_;
	int BillNum_;

};

