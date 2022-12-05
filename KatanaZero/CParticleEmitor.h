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

    doublepoint m_AngleRange; // �߻簢 ����
    doublepoint m_VelocityRange; // �ӵ� ����
    doublepoint m_DurationRange; // �෹�̼� ����
    doublepoint m_AttackSpeedRange; // �߻�ӵ� ����
    doublepoint m_YOffSetRange; // Y�� ����
    doublepoint m_XOffSetRange; // X�� ����
    
    double m_Timer;
    double m_TimeLimit;

    int m_Maxcount;
    int m_count;

    bool OnOff;
};

