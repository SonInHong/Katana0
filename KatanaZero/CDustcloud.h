#pragma once
#include "CParticle.h"
class CDustcloud :
    public CParticle
{
public:
    CDustcloud();
    ~CDustcloud();

    virtual void Shoot();
    virtual void Render(HDC _dc);
};

