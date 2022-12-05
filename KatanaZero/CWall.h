#pragma once
#include "CObject.h"
#include "CArchitecture.h"
#include <map>

class CWall :
    public CArchitecture
{
public:
    CWall();
    virtual ~CWall();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

private:
    std::map<DWORD_PTR, POINT> m_CollideDir;
        
};

