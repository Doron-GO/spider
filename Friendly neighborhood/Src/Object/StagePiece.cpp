#include <DxLib.h>
#include "StagePiece.h"
#include "Common/Transform.h"

StagePiece::StagePiece(Stage::STAGE_NUM num, Transform transform)
{
	num_ = num;
	transform_ = transform;
	//MV1SetUseZBuffer(transform_.modelId, false);
	//MV1SetWriteZBuffer(transform_.modelId, true);

}

StagePiece::~StagePiece(void)
{
}

void StagePiece::Draw(void)
{
	//MV1SetOpacityRate(transform_.modelId, 0.3f);
	MV1DrawModel(transform_.modelId);

}

Transform* StagePiece::GetTransform(void)
{
	return &transform_;
}
