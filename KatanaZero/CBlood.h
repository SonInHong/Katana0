#pragma once
#include "CParticle.h"
class CBlood :
    public CParticle
{
public:
    CBlood();
    virtual ~CBlood();

    virtual void Shoot();
    
};

