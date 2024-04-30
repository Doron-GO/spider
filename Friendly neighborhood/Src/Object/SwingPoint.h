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

	std::vector<VECTOR> pos_;										//支点の座標
	std::map<SIDE, std::vector<VECTOR>> swingPoint_;				//支点情報がたくさん入っている
	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;//20個 ビルリスト
	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_; 
//------------------------------------------------------------------------------------------------------
	
	int totalSwingPoint_;
	float min;
	int minSwingPointNum_;								//一番近い座標のリスト番号
	VECTOR norm_;										//重力方向
	std::map<int, VECTOR> swingPoint2_;					//支点番号を渡すと支点の座標が戻ってくる
	std::map<int, std::pair<VECTOR,VECTOR>> swingAndNormList_;//支点番号を渡すとpairが戻ってくる　重力方向・座標


	std::vector<float> distance_;						//プレイヤーから支点までの距離を格納する
	std::pair<float, VECTOR>fainal_;					//firstはdistance,secondは支点座標
	std::vector<std::pair<float, VECTOR>> comparison_;	//fainal_が入っている
//--------------------------------------------------------------------------------------------------------------
	std::vector<VECTOR> BillPpoint_;					//ビルの座標が入っている 一番近いビルを特定するのに使う
	std::vector<VECTOR> swingPoint3_;					//支点の座標が入っている
	std::vector<std::vector<VECTOR>> swingList3_;		//ビルに対する支点のリスト　ビルが決定したらそのビルについている支点で検索する
	int swingNum_;
	int BillNum_;

};

