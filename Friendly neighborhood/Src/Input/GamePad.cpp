#include "../common/Flame.h"
#include "Gamepad.h"

GamePad::GamePad()
{
	keyMap_.try_emplace(InputID::Up, PAD_INPUT_UP);
	keyMap_.try_emplace(InputID::Down, PAD_INPUT_DOWN);
	keyMap_.try_emplace(InputID::Left, PAD_INPUT_LEFT);
	keyMap_.try_emplace(InputID::Right, PAD_INPUT_RIGHT);
	//keyMap_.try_emplace(InputID::FrontSide, KEY_INPUT_W);
	//keyMap_.try_emplace(InputID::BackSide, KEY_INPUT_S);
	//keyMap_.try_emplace(InputID::LeftSide, KEY_INPUT_A);
	//keyMap_.try_emplace(InputID::RightSide, KEY_INPUT_D);
	keyMap_.try_emplace(InputID::Btn1, PAD_INPUT_1);
	keyMap_.try_emplace(InputID::Btn2, PAD_INPUT_2);
	keyMap_.try_emplace(InputID::Btn3, PAD_INPUT_START);
}

GamePad::~GamePad()
{
}

void GamePad::Update(void)
{

	memcpy(inputData_[OldFlame].data(), inputData_[NowFlame].data(),
		sizeof(inputData_[OldFlame][NowFlame]) * inputData_[OldFlame].size());		//現在のデータを一つ前に保存する

	auto padData_ = GetJoypadInputState(DX_INPUT_PAD1);						//パッド情報を取得して格納する

	//データ入力がされていたら1を入れる
	for (auto id : InputID())
	{
		inputData_[NowFlame][static_cast<int>(id)] = (padData_ & keyMap_[id]) > 0;
	}

}
