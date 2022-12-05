#pragma once
#include "CWall_CantClimb.h"
#include "CTexture.h"

class CWallDoor :
    public CWall_CantClimb
{
public:
    CWallDoor();
    ~CWallDoor();

    virtual void Update();
    virtual void Render(HDC _dc);

    virtual void Initialize();
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    void SetOpen(bool b) { Open = b; }
    bool& GetOpen() { return Open; }

private:
    CTexture* Texture;

    bool Open;
    double Closedness;
    bool Valid;
    
};

