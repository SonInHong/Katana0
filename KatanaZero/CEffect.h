#pragma once
#include "CObject.h"
class CEffect :
    public CObject
{
public:
    CEffect();
    virtual ~CEffect();

    virtual void Play(doublepoint pos, doublepoint resize);
    virtual void Pause();

    virtual void Render(HDC _dc);

protected:
    bool Valid;
};

