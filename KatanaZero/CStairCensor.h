#pragma once
#include "CObject.h"
class CStairCensor :
    public CObject
{
public:
    CStairCensor();
    ~CStairCensor();

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    virtual void Render(HDC _dc);

    virtual void Enter();
    virtual void Exit();
};

