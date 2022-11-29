#pragma once
#include "CParticle.h"
class CDirtyWater :
    public CParticle
{
public:

    CDirtyWater();
    virtual ~CDirtyWater();

    virtual void Shoot();
};

