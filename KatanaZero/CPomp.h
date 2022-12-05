#pragma once
#include "CMonster.h"
class CPomp :
    public CMonster
{
public:
    CPomp();
    ~CPomp();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void Slash(int dir);

    void CheckAttackStop();
};

