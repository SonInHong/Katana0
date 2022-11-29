#pragma once
#include "CParticleEmitor.h"
class CDirtyWaterEmitor :
    public CParticleEmitor
{
public:
    CDirtyWaterEmitor();
    virtual ~CDirtyWaterEmitor();

    virtual void Initialize();
};

