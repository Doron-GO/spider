#pragma once
#include <DxLib.h>
#include <map>
#include <array>
#include"InputID.h"

class BaseScene;
class Actor;

using InputData = std::array<int, static_cast<int>(InputID::Max)>;
using InputDatas = std::array<InputData, 2>;

class Controller
{
public:
	Controller();
	~Controller();
	virtual void Update(void) = 0;
	const InputDatas& GetInputData(void) { return inputData_; }
	const bool IsInputdata(void);

protected:
	InputDatas inputData_;
	std::map<InputID, int> keyMap_;		//InputID‚ÌŠi”[æ

	//“ü—Í‚ª‚ ‚é‚©
	bool isInput_;
};

