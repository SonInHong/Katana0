#pragma once
#include "CMonster.h"
class CGangster :
    public CMonster
{
public:
    CGangster();
    ~CGangster();
    
    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SlashFist(int dir);
    void ShootGun();

    void CheckAttackStop();
    void CheckShotStop();

private:
    double ShotSpeed;
    double ShotTimer;

    double AttackSpeed;
    double AttackTimer;
};

