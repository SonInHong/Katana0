#pragma once
#include "CParticleEmitor.h"
class CLaserparticleEmitor :
    public CParticleEmitor
{
public:
    CLaserparticleEmitor();
    ~CLaserparticleEmitor();

    virtual void Initialize();
    virtual void Update();

    void SetOwner(CObject* p) { Owner = p; }
    void SetOffset(doublepoint p) { OffSet = p; }

protected:
    CObject* Owner;
    doublepoint OffSet;
};

