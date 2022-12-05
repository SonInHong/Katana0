#pragma once
#include "CObject.h"
class ThrowingObj :
    public CObject
{
public:
    virtual void Enter();
    virtual void Exit();
    virtual void Throw(doublepoint Pos, doublepoint Velocity) = 0;
};

