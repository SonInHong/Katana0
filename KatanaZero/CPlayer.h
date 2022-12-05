#pragma once
#include "CObject.h"
#include "CAnimal.h"
#include "CTexture.h"
#include "CJumpCloudLeft.h"
#include "CJumpCloudRight.h"
#include "ThrowingObj.h"

class CFloor;
class CDustcloudEmitor;
class CJumpCloud;
class CLandCloud;
class CBloodEmitor;
class CSword;
class CLaserparticleEmitor;

class CPlayer :
    public CAnimal
{
public:
    CPlayer();
    ~CPlayer();

    void GetInput();
    void ManageEffector();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SlashSword(double _Angle);
    void BreakGround();
    void SmashDoor();


    void SetHurtAngle(double b) { HurtAngle = b; }
    void SetMainOrder(Main_Order m) { MainOrder = m; }
    

    int GetRoll() { return Rolling; }
    bool& GetUnbeatable() { return Unbeatable; }
    void StunPlayer() { Stunned = true; }
    void SetCantClimb(bool b) { CantClimb = b; }
    bool& GetHiding() { return Hiding; }
    void SetHiding(bool b) { Hiding = b; }

    CSword* GetSword() { return PlayerSword; }
    ThrowingObj* GetItem() { return Item; }
    void SetItem(ThrowingObj* p) { Item = p; }

    

private:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    long double AttackTimer;
    bool AttackTimerSwitch;


    bool burn;
    bool PlayerDead;
    double HurtAngle;
    int Rolling;
    bool CantClimb;
   

    bool Unbeatable;
    double StunTimer;
    bool Stunned;
   

    CSword* PlayerSword;

    double MP;
    ThrowingObj* Item;
    
    void SlowOn();
    void SlowOff();

    

    
    

   
    
};

