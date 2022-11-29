#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CJumpCloudLeft.h"
#include "CJumpCloudRight.h"


class CFloor;
class CDustcloudEmitor;
class CJumpCloud;
class CLandCloud;
class CBloodEmitor;
class CSword;

class CPlayer :
    public CObject
{
public:
    CPlayer();
    ~CPlayer();

    void GetInput();
    void ManageEffector();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SlashSword(double _Angle);
    void BreakGround();
    void SmashDoor();

    void SetRollDustcloud(bool b);
    void SetRollDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);
   
    void SetRunDustcloud(bool b);
    void SetRunDustcloudOption(int maxcount, doublepoint resize, doublepoint anglerange, 
        doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetWallDustcloud(bool b);
    void SetWallDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);
    
    void SetBloodEmitor(bool b);
    void SetBloodEmitorOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);
    void SetBloodEmitormaxOption(int maxcount, doublepoint resize, doublepoint anglerange
        , doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetHurtAngle(double b) { HurtAngle = b; }
    void SetMainOrder(Main_Order m) { MainOrder = m; }
    Main_Order& GetMainOrder() { return MainOrder; }

    int GetRoll() { return Rolling; }

private:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    long double AttackTimer;
    bool AttackTimerSwitch;

    CDustcloudEmitor* RollDustcloud;
    CDustcloudEmitor* RunDustcloud;
    CDustcloudEmitor* WallDustcloud;
    

    CBloodEmitor* BloodEmitor;

    CJumpCloud* JumpCloud;
    CLandCloud* LandCloud;
    CJumpCloudLeft* JumpCloudLeft;
    CJumpCloudRight* JumpCloudRight;

    bool burn;
    bool PlayerDead;
    double HurtAngle;
    int Rolling;

    CSword* PlayerSword;

    Main_Order MainOrder;

    
    

   
    
};

