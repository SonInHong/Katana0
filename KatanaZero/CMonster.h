#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CAnimal.h"

class CTopFloor;
class CFloor;
class CSword;
class CBloodEmitor;

class CMonster :
    public CAnimal
{
public:
    CMonster();
    ~CMonster();

    virtual void Initialize();
    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SetLookDirection(int b) { LookDirection = b; }
    void AdjustRoamingFloor(doublepoint p);
    void SetRoamingPoint(doublepoint p) { RoamingPoint = p; }
    void SetHurtAngle(double a) { HurtAngle = a; }
    void SetMainOrder(Main_Order p) { MainOrder = p; }

    bool& GetDead() { return Dead; }
    bool& GetBurn() { return Burn; }

    bool PlayerDetection();

    bool CheckArchitectureCollision(doublepoint A, doublepoint B);

    friend CSword;

protected:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    
    //오더들
    Move_Order MoveOrder;
    Action_Order ActionOrder;

    //로밍 관련 변수들
    doublepoint RoamingPoint;
    double RoamingDistance;
    CFloor* RoamingFloor;

    //시야 관련 변수들
    double DetectRange;
    double DetectAngle;
    doublepoint EyeOffset; //눈높이
    double DetectOthersDeathRange;
    double DetectCoff;
    double DetectOthersDeathCoff;

    //피격 관련 변수들
    double HurtAngle;
    bool Burn;
    bool Dead;
    
    //레이저 관련 변수
    bool LaserExist;


    void MoveLeft();
    void MoveRight();
    void ClimbDownRight();
    void ClimbDownLeft();
    void Stay();

    std::vector<CTopFloor*> IgnoreTopFloorList;
    std::vector<CTopFloor*> RecogniseTopFloorList;

    //공속 관련 변수
    double ShotSpeed;
    long double ShotTimer;

    double AttackSpeed;
    long double AttackTimer;
};

