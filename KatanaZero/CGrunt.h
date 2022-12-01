#pragma once
#include "CMonster.h"
class CGrunt :
    public CMonster
{
public:
    CGrunt();
    ~CGrunt();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void AdjustHurtAnimation();

    void SlashFist(int dir);
    void CheckAttackStop();

};

