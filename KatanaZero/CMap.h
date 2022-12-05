#pragma once
#include "CObject.h"
#include "CTexture.h"

class CMap :
    public CObject
{
public:
    CMap();
    ~CMap();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);
    void Load(const std::wstring& _Path , double sizeup);
    void Copy(double d);

private:
    CTexture* Texture;
    CTexture* TextureSU;
};

