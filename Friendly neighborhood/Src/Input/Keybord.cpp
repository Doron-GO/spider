#include "InputID.h"
#include "../common/Flame.h"
#include "Keybord.h"

Keybord::Keybord()
{
	memset(keyData_, 0, sizeof(keyData_));

	//InPutIDを格納する		
	//		try_emplace:マップのなかに入れる　
	keyMap_.try_emplace(InputID::Up, KEY_INPUT_UP);
	keyMap_.try_emplace(InputID::Down, KEY_INPUT_DOWN);
	keyMap_.try_emplace(InputID::Left, KEY_INPUT_LEFT);
	keyMap_.try_emplace(InputID::Right, KEY_INPUT_RIGHT);
	keyMap_.try_emplace(InputID::FrontSide, KEY_INPUT_W);
	keyMap_.try_emplace(InputID::BackSide, KEY_INPUT_S);
	keyMap_.try_emplace(InputID::LeftSide, KEY_INPUT_A);
	keyMap_.try_emplace(InputID::RightSide, KEY_INPUT_D);
	keyMap_.try_emplace(InputID::Btn1, KEY_INPUT_Z);
	keyMap_.try_emplace(InputID::Btn2, KEY_INPUT_X);
	keyMap_.try_emplace(InputID::Btn3, KEY_INPUT_SPACE);

}

Keybord::~Keybord()
{
}

void Keybord::Update(void)
{
	memcpy(inputData_[OldFlame].data(), inputData_[NowFlame].data(),
		sizeof(inputData_[OldFlame][NowFlame]) * inputData_[OldFlame].size());		
										//現在のデータを一つ前に保存する

	GetHitKeyStateAll(keyData_);		//キー情報を取得して格納する
	for (auto id : InputID())
	{
		inputData_[0][static_cast<int>(id)] = keyData_[keyMap_[id]];
	}

}
