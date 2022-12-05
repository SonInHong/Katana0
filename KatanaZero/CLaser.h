#pragma once
#include "CObject.h"
#include "CTexture.h"

class CLaser :
    public CObject
{
public:
    CLaser();
    virtual ~CLaser();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);

    bool& GetOnOff() { return OnOff; }
    void SetOnOff(bool b) { OnOff = b; }

private:
    CTexture* Texture;
    bool OnOff;
    bool CollisionOn;
};

