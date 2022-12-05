#pragma once
#include "CParticle.h"

class CSmokeEmitor;

class CSmoke :
    public CParticle
{
public:
    enum class Smoke_State
    {
        Emitting,
        Emitted,
    };

    CSmoke();
    virtual ~CSmoke();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    void setAccel(doublepoint b) { Accelation = b; }

    friend CSmokeEmitor;

private:
    Smoke_State State;

    doublepoint Accelation;
    CTexture* Texture;
};

