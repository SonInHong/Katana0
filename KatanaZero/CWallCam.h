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
    double Angle;  // ���� ���� �ִ� ����
    double AngleRange; //�÷��̾� ���� ��ä���� ����
    doublepoint DetectAngleRange; // �� ����
    doublepoint RoamingAngleRange; // �θ����Ÿ��� ����
    double Speed;
    int dir;

    WallCamState State;

    CTexture* Texture_Green[8];
    CTexture* Texture_Red[8];

    CWallDoor* ConnectDoor; // ����� �����̴���

    bool PlayerInRange();

};

