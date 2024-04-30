#pragma once
#include "Controller.h"

class Keybord :
    public Controller
{
public:
    Keybord();
    ~Keybord();

private:
    void Update(void)override;

    char keyData_[256];					//キーボード情報

};

