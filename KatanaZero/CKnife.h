#pragma once
#include "CObject.h"
#include "ThrowingObj.h"

class CPlayer;

class CKnife :
    public ThrowingObj
{
public:
    CKnife();
    ~CKnife();

    virtual void Enter();
    virtual void Exit();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual void Throw(doublepoint pos, doublepoint velocity);

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    void SetState(ThrowingObjState t) { State = t; }
    ThrowingObjState& GetState() { return State; }

    void SetVelocity(doublepoint d) { Velocity = d; }
    doublepoint& GetVelocity() { return Velocity; }
 
    void SetValid(bool b) { Valid = b; }
    bool& GetValid() { return Valid; }

    void SetOwner(CPlayer* p) { Owner = p; }

private:
    ThrowingObjState State;
    doublepoint Velocity;
    bool Valid;
    bool stick = false;


    CPlayer* Owner;

    CTexture* Texture;

    
};

