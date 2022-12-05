#pragma once
#include "CObject.h"
#include "CArchitecture.h"
#include <map>
class CDoor :
    public CArchitecture
{
public:
    CDoor();
    virtual ~CDoor();

   

    virtual void Initialize();
    virtual void Update();
    virtual void Enter();
    virtual void Exit();

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

   

    void SetOpen(bool b) { m_Open = b; }
    bool& GetOpen() { return m_Open; }

protected:
    std::map<DWORD_PTR, POINT> m_CollideDir;
    bool m_Open;

};

