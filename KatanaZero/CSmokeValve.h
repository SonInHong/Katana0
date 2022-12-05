#pragma once
#include "CWall_CantClimb.h"
#include "CSmokeEmitor.h"

class CSmokeValve :
    public CWall_CantClimb
{
public:
    CSmokeValve();
    virtual ~CSmokeValve();

    virtual void Initialize();
    virtual void Enter();
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    void SetOpen(bool b) { Open = b; }
    bool& GetOpen() { return Open; }

private:
    CTexture* Texture_Open;
    CTexture* Texture_Closed;

    CSmokeEmitor* SmokeEmitor;

    bool Open;
};

