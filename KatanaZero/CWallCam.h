#pragma once
#include "CObject.h"
#include "CWallDoor.h"

class CWallCam :
    public CObject
{
public:
    CWallCam();
    virtual ~CWallCam();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();

    virtual void Update();
    virtual void Render(HDC _dc);

    double& GetAngle() { return Angle; }
    void SetAngle(double a) { Angle = a; }
    double& GetAngleRange() { return AngleRange; }
    void SetAngleRange(double a) { AngleRange = a; }
    doublepoint& GetDetectAngleRange() { return DetectAngleRange; }
    void SetDetectAngleRange(doublepoint a) { DetectAngleRange = a; }
    doublepoint& GetRoamingAngleRange() { return RoamingAngleRange;  }
    void SetRoamingAngleRange(doublepoint d) { RoamingAngleRange = d; }

    void SetConnectDoor(CWallDoor* d) { ConnectDoor = d; }
    void SetState(WallCamState s) { State = s; }

private:
    double Angle;  // 지금 보고 있는 각도
    double AngleRange; //플레이어 감지 부채꼴의 각도
    doublepoint DetectAngleRange; // 고개 각도
    doublepoint RoamingAngleRange; // 두리번거리는 각도
    double Speed;
    int dir;

    WallCamState State;

    CTexture* Texture_Green[8];
    CTexture* Texture_Red[8];

    CWallDoor* ConnectDoor; // 연결된 슬라이더벽

    bool PlayerInRange();

};

