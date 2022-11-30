#pragma once
#include "CObject.h"
#include "CTexture.h"


class CTopFloor;
class CFloor;
class CSword;
class CBloodEmitor;

class CMonster :
    public CObject
{
public:
    CMonster();
    ~CMonster();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SetLookDirection(int b) { LookDirection = b; }
    void AdjustRoamingFloor(doublepoint p);
    void SetRoamingPoint(doublepoint p) { RoamingPoint = p; }
    void SetHurtAngle(double a) { HurtAngle = a; }
    void SetMainOrder(Main_Order p) { MainOrder = p; }

    void SetBloodEmitor(bool b);
    void SetBloodEmitorOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);
    void SetBloodEmitormaxOption(int maxcount, doublepoint resize, doublepoint anglerange
        , doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange);

    bool& GetDead() { return Dead; }
    bool& GetBurn() { return Burn; }

    bool PlayerDetection();

    friend CSword;

protected:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    long double AttackTimer;
    bool AttackTimerSwitch;

    //������
    Main_Order MainOrder;
    Move_Order MoveOrder;
    Action_Order ActionOrder;

    //�ι� ���� ������
    doublepoint RoamingPoint;
    double RoamingDistance;
    CFloor* RoamingFloor;

    //�þ� ���� ������
    double DetectRange;
    double DetectAngle;
    doublepoint EyeOffset; //������
    double DetectOthersDeathRange;

    //�ǰ� ���� ������
    double HurtAngle;
    bool Burn;
    bool Dead;
    
    //������ ���� ����
    bool LaserExist;

    //���� �̹���
    CBloodEmitor* BloodEmitor;

    void MoveLeft();
    void MoveRight();
    void ClimbDownRight();
    void ClimbDownLeft();
    void Stay();

    std::vector<CTopFloor*> IgnoreTopFloorList;
    std::vector<CTopFloor*> RecogniseTopFloorList;
};

