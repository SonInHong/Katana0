#pragma once
#include "CObject.h"

class CEffectMgr;

class CEffect :
    public CObject
{
public:
    CEffect();
    virtual ~CEffect();

    virtual void Play(doublepoint pos, doublepoint resize);
    virtual void Pause();

    virtual void Enter();
    virtual void Exit();

    virtual void Render(HDC _dc);
    friend CEffectMgr;
protected:
    bool Valid;
};

