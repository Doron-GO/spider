#include <DxLib.h>
#include <stdlib.h>
#include <string>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "Player.h"
#include "Stage.h"
#include"StagePiece.h"
#include<fstream>


Stage::Stage()
	:resourceManager_(ResourceManager::GetInstance())
{

}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	std::ifstream f("Src/Json/Data.json");
	if (!f)
	{
		auto l = 0.0f;
	}
	json_ = json::parse(f);
	json_ = json_["StageModel1"];
	Load();
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{

	//for (const auto stagedraw : stagePiece_)
	//{
	//	stagedraw.second->Draw();
	//}

	stagePiece_[STAGE_NUM::S1_1]->Draw();
	//stagePiece_[STAGE_NUM::S1_2]->Draw();
	//stagePiece_[STAGE_NUM::S1_3]->Draw();
	//stagePiece_[STAGE_NUM::S1_4]->Draw();

}

void Stage::Release(void) 
{
	MV1DeleteModel(transform_.modelId);
}

 std::map<Stage::STAGE_NUM, StagePiece*> Stage:: GetPiece(void)
{
	return  stagePiece_;
}


int Stage::GetModelId(void) const
{
    return transform_.modelId;
}

const Transform* Stage::GetTransform(int num)
{
	return &transform_;
}

VECTOR Stage::GetSwinPoint(void)
{
	VECTOR sPos ={ -42.4f,37.6f,-12.5f };


	return sPos;
}

void Stage::Load(void)
{
	auto max = json_["StageTotalNum"].get<int>();
	for (int model = 1; model <= 1; model++)
	{
		std::string st = std::to_string(model);
		const char* i = st.c_str();
		std::string name = json_["StageMV1"].at(i).get<std::string>();
		std::string ss = "Data/Model/" + name;

		auto modelId = MV1LoadModel(ss.c_str());
		Transform stageTrans;
		stageTrans.SetModel(modelId);
		stageTrans.scl = AsoUtility::VECTOR_ONE;
		stageTrans.quaRot = Quaternion();
		stageTrans.pos = { 0.0f, 0.0f, 0.0f };
		stageTrans.localPos = { 0.0f, -1500.0f, 0.0f };
		stageTrans.pos = VAdd(stageTrans.pos, stageTrans.localPos);
		stageTrans.Update();

		// ìñÇΩÇËîªíË(ÉRÉâÉCÉ_)çÏê¨
		stageTrans.MakeStageCollider(Collider::TYPE::STAGE,model);

		StagePiece* stagepiece = new StagePiece(static_cast<STAGE_NUM>(model-1), stageTrans);

		stagePiece_.emplace(static_cast<STAGE_NUM>(model-1), stagepiece);
	}

}


