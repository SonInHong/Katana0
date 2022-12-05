#pragma once
#include "CObject.h"

class CDustcloudEmitor;
class CJumpCloud;
class CLandCloud;
class CBloodEmitor;
class CSword;
class CLaserparticleEmitor;

class CAnimal :
    public CObject
{
public:
    CAnimal();
    virtual ~CAnimal();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    

    void SetRollDustcloud(bool b);
    void SetRollDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetRunDustcloud(bool b);
    void SetRunDustcloudOption(int maxcount, doublepoint resize, doublepoint anglerange,
        doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetWallDustcloud(bool b);
    void SetWallDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetLaserParticle(bool b);
    void SetLaserParticleOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    void SetBloodEmitor(bool b);
    void SetBloodEmitorOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);
    void SetBloodEmitormaxOption(int maxcount, doublepoint resize, doublepoint anglerange
        , doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    CLaserparticleEmitor* GetLaserparticleEmitor() { return LaserparticleEmitor; }

    Main_Order& GetMainOrder() { return MainOrder; }

    void SetInSmoke(bool b) { InSmoke = b; }
    bool& GetInSmoke() { return InSmoke; }


protected:
    CLaserparticleEmitor* LaserparticleEmitor;
    int lasoremitorcount = 0; //레이저이미터는 딱 한번만 소환되면 되기때문에 1이 되면 더이상 호출하지 않기 위한 기능

    CBloodEmitor* BloodEmitor;

    CDustcloudEmitor* RollDustcloud;
    CDustcloudEmitor* RunDustcloud;
    CDustcloudEmitor* WallDustcloud;

    Main_Order MainOrder;


    bool InSmoke;
    bool Hiding;
};

