#pragma once
#include "CObject.h"
#include "CTexture.h"

class CBackgroundObj :
    public CObject
{
public:
    CBackgroundObj();
    virtual ~CBackgroundObj();

    virtual void Render(HDC _dc);
    virtual void Enter();
    virtual void Exit();

protected:
    CTexture* Texture;
};

