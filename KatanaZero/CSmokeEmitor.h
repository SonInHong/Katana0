#pragma once
#include "CParticleEmitor.h"
class CSmokeEmitor :
    public CParticleEmitor
{
public:
    CSmokeEmitor();
    ~CSmokeEmitor();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SetOwner(CObject* p) { Owner = p; }
    void SetOffset(doublepoint p) { OffSet = p; }

protected:
    CObject* Owner;
    doublepoint OffSet;

};

