#pragma once
#include "CParticle.h"
#include "CTexture.h"

class CLaserParticle :
    public CParticle
{
public:
    CLaserParticle();
    ~CLaserParticle();

    virtual void Shoot();
    virtual void Render(HDC _dc);

private:
    CTexture* Texture;
};

