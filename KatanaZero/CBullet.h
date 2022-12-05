#pragma once
#include "CObject.h"
class CBullet :
    public CObject
{
public:
    CBullet();
    virtual ~CBullet();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    virtual bool Collide(CObject* other);
    virtual bool COlliding(CObject* other);

    void SetVelocity(doublepoint v) { Velocity = v; }
    doublepoint& GetVelocity() { return Velocity; }

    void SetKillPlayer(bool b) { KillPlayer = b; }
    bool& GetKillPlayer() { return KillPlayer; }

    void SetValid(bool b) { Valid = b; }
    bool& GetValid() { return Valid; }

private:
    doublepoint Velocity;
    bool KillPlayer;  // 참이면 플레이어를 노리는 총알 거짓이면 몬스터를 노리는 총알

    bool Valid;
    CTexture* Texture;
};

