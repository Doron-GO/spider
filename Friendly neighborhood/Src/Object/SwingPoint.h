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

	const VECTOR SetSwingPoint(VECTOR PlayerPos);
	const VECTOR GetBillPoint();
	const VECTOR SetGravity(VECTOR PlayerPos);
private:
	float Magnitude(VECTOR pos)const;


	json json_;

	//支点の座標
	std::vector<VECTOR> pos_;										
	//支点情報がたくさん入っている
	std::map<SIDE, std::vector<VECTOR>> swingPoint_;

	//20個 ビルリスト
	std::map<int , std::map<SIDE, std::vector<VECTOR>>>BuildingList_;

	std::map<Stage::STAGE_NUM, std::map<int, std::map<SIDE, std::vector<VECTOR>>>> sectionList_; 
	
	int totalSwingPoint_;

	float min;
	//一番近い座標のリスト番号
	int minSwingPointNum_;

	//重力方向
	VECTOR norm_;

	//支点番号を渡すと支点の座標が戻ってくる
	std::map<int, VECTOR> swingPoint2_;

	//支点番号を渡すとpairが戻ってくる　重力方向・座標
	std::map<int, std::pair<VECTOR,VECTOR>> swingAndNormList_;

	//プレイヤーから支点までの距離を格納する
	std::vector<float> distance_;

	//firstはdistance,secondは支点座標
	std::pair<float, VECTOR>fainal_;

	//fainal_が入っている
	std::vector<std::pair<float, VECTOR>> comparison_;

	//ビルの座標が入っている 一番近いビルを特定するのに使う
	std::vector<VECTOR> BillPpoint_;

	//支点の座標が入っている
	std::vector<VECTOR> swingPoint3_;

	//ビルに対する支点のリスト　ビルが決定したらそのビルについている支点で検索する
	std::vector<std::vector<VECTOR>> swingList3_;

	int swingNum_;

	int billNum_;

};

