#pragma once
#include "CMonster.h"
class CPomp :
    public CMonster
{
public:
    CPomp();
    ~CPomp();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void Slash(int dir);
};

