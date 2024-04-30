#include "SwingPoint.h"
#include<fstream>
#include<DxLib.h>
#include <stdlib.h>
#include<string>
#include"../Utility/AsoUtility.h"


SwingPoint::SwingPoint()
{
	norm_ = VECTOR{ 0.0f,0.0f,0.0f };
	min = 9999999.0f;
	BillNum_ = 0;
	totalSwingPoint_ = 0;
}

SwingPoint::~SwingPoint()
{
} 

void SwingPoint::Draw(void)
{
	//DrawFormatString(50, 400, 0xffffff, " min %f", min);
	//DrawFormatString(50, 440, 0xffffff, "testpoint %f,%f,%f", testPoint_[minNum].x, testPoint_[minNum].y, testPoint_[minNum].z);
	
	//for (auto point : testPoint_)
	//{
	//	DrawSphere3D(point.second, 50.0f, 10.0f, 10.0f, 0xff0000, true);
	//}

	for (auto section : sectionList_[static_cast<Stage::STAGE_NUM>(0)])
	{
		for (auto bulidings : section.second)
		{
			for (auto swingPoint : bulidings.second)
			{			
				DrawSphere3D(swingPoint, 70.0f, 10, 10, 0xff0000, true);
			}
		}
	}
	for (auto bill : BillPpoint_)
	{
		VECTOR b = { bill.x,500.0f,bill.z };
		DrawSphere3D(b, 70.0f, 10, 10, 0x000000, true);
	}
}

void SwingPoint::Load(void)
{
	std::ifstream f("Src/Json/Data.json");
	if (!f)
	{
		auto l = 0.0f;
	}
 	json_ = json::parse(f);
	int TotalSectionNum = json_["TotalSectionNum"].get<int>();
	for (int TSN = 1; TSN <= TotalSectionNum; TSN++)
	{
		std::string tsn = std::to_string(TSN);
		std::string Section = "Section" + tsn;
		auto Section_ = json_[Section];
		int totalBldgNum = Section_["TotalBldgNum"].get<int>();
		BuildingList_.clear();
		for (int idx = 1; idx <= totalBldgNum; idx++)
		{
			std::string st = std::to_string(idx);
			std::string BldgNum = "Bldg" + st;
			auto Bldg = Section_[BldgNum];
			//int VecNum = Bldg["VECTORTotalNum"].get<int>();
			int PointTotalNum = Bldg["PointTotalNum"];
			std::string PointTotalNumSt = std::to_string(PointTotalNum);
			auto SideNum = Bldg["SideNum"].get<int>();
			for (int idx2 = 1; idx2 <= SideNum; idx2++)
			{
				std::string num = std::to_string(idx2);
				std::string Side = "Side" + num;
				auto sideObj = Bldg[Side];
				int VecNum = sideObj["VECTORTotalNum"].get<int>();
				auto norm = sideObj["Norm"];
				norm_ = { norm["x"].get<float>(),norm["y"].get<float>() ,norm["z"].get<float>() };
				std::vector<VECTOR> p;

				for (int Ptl = 1; Ptl <= VecNum; Ptl++)
				{
					std::string num2 = std::to_string(Ptl);
					auto pos = Bldg[Side][num2]["VECTOR"];
					p.push_back(
						VECTOR{ pos["x"].get<float>(),
						pos["y"].get<float>(),
						pos["z"].get<float>() });
					//pos_ = p;
					if (Ptl == VecNum)
					{
						swingPoint_[static_cast<SIDE>(idx2 - 1)] = p;
					}
				}
			}
			BuildingList_[idx] = swingPoint_;
		}
		sectionList_[static_cast<Stage::STAGE_NUM>(TSN-1)]= BuildingList_;
	}

	auto SwingPoint = json_["SwingPoint"];
	 totalSwingPoint_ = SwingPoint["Total"].get<int>();
	for (int i = 1; i <= totalSwingPoint_; i++)
	{
		std::string num3 = std::to_string(i);
		VECTOR f = { SwingPoint[num3]["VECTOR"]["x"].get<float>(),SwingPoint[num3]["VECTOR"]["y"].get<float>(),SwingPoint[num3]["VECTOR"]["z"].get<float>()};
		swingPoint2_[i] = f;
	}

	//ビルリストの中に支点情報を入れる
	auto SwingPoint2 = json_["SwingPoint2"];
	auto totalBNum = SwingPoint2["TotalBNum"].get<int>();
	for (int i = 1; i <= totalBNum; i++)
	{
		swingPoint3_.clear();
		std::string num3 = std::to_string(i);
		auto pointList = SwingPoint2[num3];
		auto total = SwingPoint2[num3]["Total"];
		for (int d = 1; d <= total;d++) 
		{
			std::string num = std::to_string(d);
			VECTOR f = { pointList[num]["VECTOR"]["x"].get<float>(),pointList[num]["VECTOR"]["y"].get<float>(),pointList[num]["VECTOR"]["z"].get<float>()};
			swingPoint3_.push_back(f);
		}
		swingList3_.push_back(swingPoint3_);
	}

	auto BillPoint = json_["BillPoint"];
	auto BillTotalNum = BillPoint["Total"].get<int>();
	for (int idx = 0; idx < BillTotalNum;idx++)
	{
		std::string num3 = std::to_string(idx+1);
		VECTOR f = { BillPoint[num3]["VECTOR"]["x"].get<float>(),BillPoint[num3]["VECTOR"]["y"].get<float>(),BillPoint[num3]["VECTOR"]["z"].get<float>() };
		BillPpoint_.push_back(f);	
	}

}

const VECTOR SwingPoint::SetSwingPoint(VECTOR pos, int section, VECTOR Angle)
{
	min = 9999999.0f;
	auto pop = pos;
	auto angle= Angle;
	//pop.y = 0.0f;
	distance_.clear();
	comparison_.clear();
	//for (int i = 1; i <= total; i++)
	//{
	//	auto tesP = testPoint_[i];
	//	tesP.y = 0.0f;
	//	auto p =VSub(pop, tesP);
	//	float pp = abs(p.x) +abs( p.z);
	//	distance_.push_back(pp);
	//}

	//for (auto section : sectionList_[static_cast<Stage::STAGE_NUM>(0)])
	//{
	//	for (auto bulidings : section.second)
	//	{
	//		for (auto swingPoint : bulidings.second)
	//		{
	//			 tesP = swingPoint;
	//			//tesP.y = 0.0f;
	//			auto p = VSub(pop, tesP);
	//			float pp = abs(p.x) + abs(p.z) +abs(p.y);
	//			distans_2 = abs(p.x) + abs(p.z) +abs(p.y);
	//			distance_.push_back(pp);
	//			fainal_.first = distans_2;
	//			fainal_.second = tesP;
	//			comparison_.push_back(fainal_);
	//		}
	//	}
	//}
	//for (int f = 0; f < comparison_.size(); f++)
	//{
	//	if (comparison_[f].first <= min)
	//	{
	//		min = comparison_[f].first;
	//		minSwingPointNum_ = f;
	//	}
	//}


	//自分から近いビルから
	for (int idx =0; idx< BillPpoint_.size();idx++)
	{
		auto tesP = BillPpoint_[idx];
		tesP.y = 0.0f;
		auto p =VSub(pop, tesP);
		float pp = abs(p.x) +abs( p.z);
		distance_.push_back(pp);
	}
	BillNum_ =0;
	for (int idx = 0; idx < distance_.size(); idx++)
	{
		if (distance_[idx] <= min)
		{
			min = distance_[idx];
			BillNum_ = idx;
		}
	}

	distance_.clear();
	min = 9999999.0f;
	for (int idx = 0; idx < swingList3_[BillNum_].size(); idx++)
	{
		auto Point = swingList3_[BillNum_];
		auto p = VSub(pop, Point[idx]);
		float pp = abs(p.x) + abs(p.z);
		distance_.push_back(pp);
	}
	swingNum_ = 0;
	for (int idx = 0; idx < distance_.size(); idx++)
	{
		if (distance_[idx] <= min)
		{
			min = distance_[idx];
			swingNum_ = idx;
		}
	}
	return swingList3_[BillNum_][swingNum_];


	//視野
	//auto BuildingList = sectionList_[static_cast<Stage::STAGE_NUM>(0)];
	//for (auto bldg : BuildingList)
	//{
	//	for(auto side : bldg.second)
	//	{
	//		for (auto fulcrum :side.second)
	//		{
	//			auto diffX = pop.x - fulcrum.x;
	//			auto diffZ = pop.z - fulcrum.z;
	//			auto  distanceB = hypotf(diffX, diffZ);
	//			if (distanceB <= 5000)
	//			{
	//				float  rad = atan2(diffX, diffZ);
	//				float pp = AsoUtility::Rad2DegF(rad - angle.y);
	//				if (abs(pp) <= 90.0f)
	//				{
	//					// 検知したあ
	//					return fulcrum;
	//				}
	//			}
	//		}
	//	}
	//}

	//auto swingSide = BuildingList[2];
	//auto swingPointOptions = swingSide[static_cast<SIDE>(section+1)];
	//VECTOR swingPoint = swingPointOptions[2];
	

}

const VECTOR SwingPoint::GetBillPoint()
{
	return BillPpoint_[BillNum_];
}

const VECTOR SwingPoint::SetGravity(VECTOR PlayerPos)
{
	float z;
	float x;
	auto tz = abs(swingPoint2_[minSwingPointNum_].z);
	auto tx = abs(swingPoint2_[minSwingPointNum_].x);
	auto px = abs(PlayerPos.x);
	auto pz = abs(PlayerPos.z);
	if (tz <= pz)
	{
		z = 1.0f;
	}
	else {
		z = -1.0f;
	}
	if (tx <= px)
	{
		x = 1.0f;
	} 
	else {
		x = -1.0f;
	}
	return VECTOR{-0.219f,0.5f,-0.975f};
}

float SwingPoint::Magnitude(VECTOR pos) const
{
	 return sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);	
}

