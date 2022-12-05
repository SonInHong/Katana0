#pragma once
#include "CObject.h"
#include "CTexture.h"

class CHitEffect :
    public CObject
{
public:
    CHitEffect();
    ~CHitEffect();

    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void Render(HDC _dc);
    void Shoot(doublepoint from, doublepoint To , double velocity, double Length);

    struct info
    {
        doublepoint pos;
        double t;
    };

private:
    bool Valid;
    CTexture* Texture;

    doublepoint From;
    doublepoint To;
    doublepoint Velocity;

    double Time;
    double t;
    double Length;

    std::vector<info> infos;
};

