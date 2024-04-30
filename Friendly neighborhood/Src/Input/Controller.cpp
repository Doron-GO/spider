#include "../Common/Flame.h"
#include "Controller.h"

Controller::Controller()
{
	memset(inputData_[NowFlame].data(), 0xff, inputData_[NowFlame].size() * sizeof(inputData_[NowFlame][NowFlame]));
	memset(inputData_[OldFlame].data(), 0xff, inputData_[OldFlame].size() * sizeof(inputData_[OldFlame][NowFlame]));

	isInput_ = false;
}

Controller::~Controller()
{
}

const bool Controller::IsInputdata(void)
{
	if (inputData_[NowFlame].size() >= -1)
	{
		isInput_ = true;
	}
	else {
		isInput_ = false;
	}

	return isInput_;
}
