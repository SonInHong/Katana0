#pragma once
#include "CObject.h"
class CPlayer;

class CDoorBreaker :
    public CObject
{
public:
    CDoorBreaker();
    ~CDoorBreaker();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();

    virtual bool Collide(CObject* other);

    void SetDir(int d) { dir = d; }

private:
    CObject* Owner;
    float Speed;
    doublepoint OffSet;
    int dir;

    friend CPlayer;
};

