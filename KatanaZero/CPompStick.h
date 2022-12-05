#pragma once
#include "CObject.h"

class CPomp;

class CPompStick :
    public CObject
{
public:
    CPompStick();
    ~CPompStick();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);

private:
    CObject* Owner;
    float Speed;
    doublepoint OffSet;
    int Direction;
    double TimeLimit;

    bool Valid;

    friend CPomp;
};

