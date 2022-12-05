#pragma once
#include "CWall.h"
class CWall_CantClimb :
    public CWall
{
public:
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
};

