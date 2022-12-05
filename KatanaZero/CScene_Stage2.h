#pragma once
#include "CScene.h"
class CScene_Stage2 :
    public CScene
{
public:
    CScene_Stage2();
    ~CScene_Stage2();

public:
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(HDC _dc);

private:

};

