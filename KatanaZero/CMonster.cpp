#include "pch.h"
#include "PenMgr.h"
#include "CMonster.h"
#include "CResourceMgr.h"
#include "CCameraMgr.h"
#include "CRigidBody.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CNodeMgr.h"
#include "KeyMgr.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CFloor.h"
#include "TimeMgr.h"
#include "CBloodEmitor.h"
#include "CollisionMath.h"
#include "CDoor.h"
#include "CAnimator.h"
#include "CStair.h"
#include "CSceneMgr.h"
#include "CLaser.h"

CMonster::CMonster()
	:Texture(nullptr)
	, IgnoreTopFloorList{}
	, RecogniseTopFloorList{}
	, MainOrder(Main_Order::GetToRoamingPoint)
	, MoveOrder(Move_Order::End)
	, ActionOrder(Action_Order::End)
	, RoamingPoint{}
	, RoamingDistance(0)
	, RoamingFloor(nullptr)
	, EyeOffset{}
	, HurtAngle(0)
	, BloodEmitor{}
	,Burn(false)
	,Dead(false)
	, LaserExist(false)
{
}

CMonster::~CMonster()
{

}

void CMonster::Initialize()
{
		
	CreateRigidBody();		
	AdjustRoamingFloor(RoamingPoint);

	BloodEmitor = new CBloodEmitor; //피
	BloodEmitor->SetOwner(this);
	BloodEmitor->SetOffset(doublepoint{ 0,0 });
	BloodEmitor->SetOnOff(false);
	BloodEmitor->Initialize();
	
}

void CMonster::Update()
{	
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	
	//메인 오더 및 액션 오더 초기화
	MoveOrder = Move_Order::End;
	ActionOrder = Action_Order::End;

	//걷는상태 뛰는상태 초기화
	dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk() = false;
	dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun() = false;

	//레이저 감지 로직
	doublepoint P1 = Pos;
	doublepoint P2 = Pos + doublepoint{ (double)LookDirection * 100,0 };

	LaserExist = false;

	std::vector<CObject*> lasers = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::TRAP);
	for (int i = 0; i < lasers.size(); ++i)
	{
		CLaser* las = dynamic_cast<CLaser*>(lasers[i]);
		if (las && las->GetOnOff())
		{
			doublepoint L1 = las->GetPos() - doublepoint{ 0,las->GetScale().y/2 };
			doublepoint L2 = las->GetPos() + doublepoint{ 0,las->GetScale().y/2 };

			LaserExist = LaserExist || CollisionMath::CheckifLineCollide(P1, P2, L1, L2);
		}
	}


	if (MainOrder == Main_Order::RoamAround)
	{

		if (LookDirection == Right)
			ActionOrder = Action_Order::WalkRight;

		if (LookDirection == Left)
			ActionOrder = Action_Order::WalkLeft;

		// 로밍 포인트를 기준으로 벗어나면 방향 전환
		if (Pos.x > RoamingPoint.x + RoamingDistance)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			if (LookDirection == Left)
				ActionOrder = Action_Order::WalkLeft;
		}

		if (Pos.x < RoamingPoint.x - RoamingDistance)
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			if (LookDirection == Right)
				ActionOrder = Action_Order::WalkRight;
		}

		doublepoint eyeposition = Pos + doublepoint{ EyeOffset.x * LookDirection, EyeOffset.y }; // 플레이어 감지 로직

		doublepoint playerpos[3] = { player->GetPos()             // 플레이어의 가슴과 머리 발 포지션을 불러와서
		, player->GetPos() + doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3}
		,player->GetPos() - doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3 } };

		

		for (int i = 0; i < 3; ++i)
		{
			doublepoint diff = (playerpos[i] - eyeposition) * LookDirection;

			if (LookDirection != 0 && diff.Norm() < DetectRange && abs(diff.Angle()) < DetectAngle) // 시야 범위 안에 있을때
			{
				bool check = false;

				std::vector<CObject*> floors = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR);
				for (int j = 0; j < floors.size(); ++j)
				{
					doublepoint pos = floors[j]->GetPos();
					doublepoint scale = floors[j]->GetScale();

					// 모든 구조물들 불러와서 플레이어 포즈, 아이포지션 사이의 선과 맞닿는지 테스트 한다.

					CDoor* p = dynamic_cast<CDoor*>(floors[j]);
					if (p)
						if (p->GetOpen())
							continue;

					CStair* s = dynamic_cast<CStair*>(floors[j]);
					if (s)
					{
						doublepoint A1, A2;
						A1 = doublepoint{ pos.x - scale.x / 2, pos.y - s->GetDir() * scale.y / 2 };
						A2 = doublepoint{ pos.x + scale.x / 2,pos.y + s->GetDir() * scale.y / 2 };
						check = check || CollisionMath::CheckifLineCollide(A1, A2, eyeposition, playerpos[i]);
						continue;
					}

					else
						check = check || CollisionMath::CheckLineBoxCollision(pos, scale, eyeposition, playerpos[i]);
				}


				if (check == false)
				{
					MainOrder = Main_Order::PlayerDetected;
					ActionOrder = Action_Order::End;
				}

			}
		}

	}

	//=======================================================================================

	if (MainOrder == Main_Order::GetToRoamingPoint)
	{
		IgnoreTopFloorList.clear();
		RecogniseTopFloorList.clear();

		if (KeyMgr::Create()->key(Key::M).pressed)
		{
			std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, RoamingFloor);
			int a = 0; //디버깅용
		}

		std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, RoamingFloor);

		if (path.size() == 0)
		{
			//길이 없음
			MoveOrder = Move_Order::End;
		}

		if (path.size() == 1)
		{
			//스테이
			MoveOrder = Move_Order::Stay;
		}

		if (path.size() > 1)
		{
			MoveOrder = path[1].HowToMove;
		}
		
		doublepoint eyeposition = Pos + doublepoint{ EyeOffset.x * LookDirection, EyeOffset.y }; // 플레이어 감지 로직

		doublepoint playerpos[3] = { player->GetPos()             // 플레이어의 가슴과 머리 발 포지션을 불러와서
		, player->GetPos() + doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3}
		,player->GetPos() - doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3 } };



		for (int i = 0; i < 3; ++i)
		{
			doublepoint diff = (playerpos[i] - eyeposition) * LookDirection;

			if (LookDirection != 0 && diff.Norm() < DetectRange && abs(diff.Angle()) < DetectAngle) // 시야 범위 안에 있을때
			{
				bool check = false;

				std::vector<CObject*> floors = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR);
				for (int j = 0; j < floors.size(); ++j)
				{
					doublepoint pos = floors[j]->GetPos();
					doublepoint scale = floors[j]->GetScale();

					// 모든 구조물들 불러와서 플레이어 포즈, 아이포지션 사이의 선과 맞닿는지 테스트 한다.

					CDoor* p = dynamic_cast<CDoor*>(floors[j]);
					if (p)
						if (p->GetOpen())
							continue;

					CStair* s = dynamic_cast<CStair*>(floors[j]);
					if (s)
					{
						doublepoint A1, A2;
						A1 = doublepoint{ pos.x - scale.x / 2, pos.y - s->GetDir() * scale.y / 2 };
						A2 = doublepoint{ pos.x + scale.x / 2,pos.y + s->GetDir() * scale.y / 2 };
						check = check || CollisionMath::CheckifLineCollide(A1, A2, eyeposition, playerpos[i]);
						continue;
					}
					
					else
						check = check || CollisionMath::CheckLineBoxCollision(pos, scale, eyeposition, playerpos[i]);
				}


				if (check == false)
				{
					MainOrder = Main_Order::PlayerDetected;
					ActionOrder = Action_Order::End;
				}

			}
		}
		
		
		

		
	}
	
	//=======================================================================================


	if (MainOrder == Main_Order::PlayerDetected) // 플레이어가 감지되었으니 찾아서 따라간다
	{
		IgnoreTopFloorList.clear();
		RecogniseTopFloorList.clear();

		//레이저 감지 로직

		if (KeyMgr::Create()->key(Key::M).pressed)
		{
			std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, player->GetRecentFloor());
			int a = 0; //디버깅용
		}

		std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, player->GetRecentFloor());

		if (path.size() == 0)
		{
			//길이 없음
			MoveOrder = Move_Order::CannotDetectPlayer;
		}

		if (path.size() == 1)
		{
			//스테이
			MoveOrder = Move_Order::Stay;
		}

		if (path.size() > 1)
		{
			MoveOrder = path[1].HowToMove;
		}
	}

	//=================================================================================================== 피격
	
	if (MainOrder == Main_Order::GetHurt) // 피격시 
	{
		double b = TimeMgr::Create()->CheckTimer();
		int a = 0;
				
			ActionOrder = Action_Order::HurtFly;
			Dead = true;
		
	}

	if (MainOrder == Main_Order::GetBurn)
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->BurnAnimation();
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().y = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetGravity(false);

		SetBloodEmitor(false);
		Burn = true;
		Dead = true;
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	BloodEmitor->Update();
}

void CMonster::Render(HDC _dc)
{
	
	CObject::Render(_dc);

	doublepoint eyeposition = Pos + doublepoint{ EyeOffset.x * LookDirection, EyeOffset.y };

	doublepoint A = CCameraMgr::Create()->CameraCoordinate(eyeposition + doublepoint{ LookDirection * DetectRange * cos(DetectAngle / 180 * M_PI) ,LookDirection * DetectRange * sin(DetectAngle / 180 * M_PI) });
	doublepoint B = CCameraMgr::Create()->CameraCoordinate(eyeposition + doublepoint{ LookDirection * DetectRange * cos(DetectAngle / 180 * M_PI) , -LookDirection * DetectRange * sin(DetectAngle / 180 * M_PI) });
	doublepoint O = CCameraMgr::Create()->CameraCoordinate(eyeposition);

	HPEN oldpen = (HPEN)SelectObject(_dc, PenMgr::Create()->GetRedPen());

	//MoveToEx(_dc, A.x, A.y, NULL);
	//LineTo(_dc, O.x, O.y);
	//LineTo(_dc, B.x, B.y);

	SelectObject(_dc,oldpen);
}

void CMonster::AdjustRoamingFloor(doublepoint p)
{
	RoamingPoint = p;

	double min = 100000;

	for (int i = 0; i < CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR).size(); ++i)
	{
		CFloor* CF = dynamic_cast<CFloor*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);
		CStair* CS = dynamic_cast<CStair*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);

		if (CS)
		{
			double y = CS->GetPos().y + (CS->GetScale().y * CS->GetResize().y) / 2;
			double x1 = CS->GetPos().x - (CS->GetScale().x * CS->GetResize().x) / 2;
			double x2 = CS->GetPos().x + (CS->GetScale().x * CS->GetResize().x) / 2;

			if (y - RoamingPoint.y > 0 && y - RoamingPoint.y < min && RoamingPoint.x > x1 && RoamingPoint.x < x2)
				RoamingFloor = CS;

		}

		else if (CF)
		{
			double y = CF->GetPos().y - (CF->GetScale().y * CF->GetResize().y) / 2;
			double x1 = CF->GetPos().x - (CF->GetScale().x * CF->GetResize().x) / 2;
			double x2 = CF->GetPos().x + (CF->GetScale().x * CF->GetResize().x) / 2;

			if (y - RoamingPoint.y > 0 && y - RoamingPoint.y < min && RoamingPoint.x > x1 && RoamingPoint.x < x2)
				RoamingFloor = CF;
		}

	}
}

void CMonster::SetBloodEmitor(bool b)
{
	BloodEmitor->SetOnOff(b);
}

void CMonster::SetBloodEmitorOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	BloodEmitor->SetOption(resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	BloodEmitor->SetOnOff(true);
}

void CMonster::SetBloodEmitormaxOption(int maxcount, doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	BloodEmitor->SetOption(maxcount,resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	BloodEmitor->SetOnOff(true);
}

void CMonster::MoveLeft()
{

}

void CMonster::MoveRight()
{
}

void CMonster::ClimbDownRight()
{
}

void CMonster::ClimbDownLeft()
{
}

void CMonster::Stay()
{
}
