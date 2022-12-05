#pragma once
#include "CObject.h"
#include "CParticle.h"
#include <queue>

class CParticleEmitor :
    public CObject
{
public:
    CParticleEmitor();
    ~CParticleEmitor();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();

   

    void SetOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
    {
        m_AngleRange = anglerange; m_VelocityRange = velocityrange; m_DurationRange = durationrange; m_AttackSpeedRange = attackspeedrange;  m_XOffSetRange = XoffRange;
        m_YOffSetRange = YoffRange; Resize = resize; m_count = 0; m_Maxcount = 10000;
    }

    void SetOption(int maxcount, doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
    {
        m_AngleRange = anglerange; m_VelocityRange = velocityrange; m_DurationRange = durationrange; m_AttackSpeedRange = attackspeedrange;  m_XOffSetRange = XoffRange;
        m_YOffSetRange = YoffRange; Resize = resize; m_count = 0; m_Maxcount = maxcount;
    }

    void SetOnOff(bool b) { OnOff = b; m_count = 0; }
    bool& GetOnOff() { return OnOff; }

protected:
    std::vector<CParticle*> m_Particles;
    int m_PoolSize;

    doublepoint m_AngleRange; // 발사각 범위
    doublepoint m_VelocityRange; // 속도 범위
    doublepoint m_DurationRange; // 듀레이션 범위
    doublepoint m_AttackSpeedRange; // 발사속도 범위
    doublepoint m_YOffSetRange; // Y축 범위
    doublepoint m_XOffSetRange; // X축 범위
    
    double m_Timer;
    double m_TimeLimit;

    int m_Maxcount;
    int m_count;

    bool OnOff;
};

