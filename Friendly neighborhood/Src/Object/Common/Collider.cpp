#include "Collider.h"

Collider::Collider(TYPE type, int modelId,int num)
{
	type_ = type;
	modelId_ = modelId;
	num_ = num;
}

Collider::~Collider(void)
{
}
