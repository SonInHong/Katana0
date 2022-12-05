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
#include "CCollider.h"
#include "CParticleMgr.h"


CMonster::CMonster()
	:Texture(nullptr)
	, IgnoreTopFloorList{}
	, RecogniseTopFloorList{}
	, MoveOrder(Move_Order::End)
	, ActionOrder(Action_Order::End)
	, RoamingPoint{}
	, RoamingDistance(0)
	, RoamingFloor(nullptr)
	, EyeOffset{}
	, HurtAngle(0)
	,Burn(false)
	,Dead(false)
	, LaserExist(false)
	, DetectOthersDeathRange(300*scaleA)
	,ShotSpeed(2)
	,ShotTimer(2) //�߻��� ����Ʈ 2��
	,AttackSpeed(1) // ���� �� ����Ʈ 1��
	,AttackTimer(1)
	,DetectCoff(1)
	, DetectOthersDeathCoff(1)
{
	MainOrder = Main_Order::GetToRoamingPoint;
}

CMonster::~CMonster()
{

}

void CMonster::Initialize()
{
	CreateAnimator(); // ���� �ִϸ��̼�
	CreateAnimator(); // Į ����Ʈ
	CreateAnimator(); // UI ����Ʈ�� �ִϸ��̼�
	CreateRigidBody();		
	

	CAnimal::Initialize();


	
}

void CMonster::Enter()
{
	CAnimal::Enter();

	

	AdjustRoamingFloor(RoamingPoint);
	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->ScaleUpdate();
}

void CMonster::Exit()
{
}

void CMonster::Update()
{	
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	
	//�������̸� �þ߹����� ���δ�
	if (player->GetHiding() || Hiding)
	{
		DetectOthersDeathCoff = 0;
		DetectCoff = 0.2;
	}
		

	else
		DetectCoff = 1;

	//���� ���� �� �׼� ���� �ʱ�ȭ
	MoveOrder = Move_Order::End;
	ActionOrder = Action_Order::End;

	//�ȴ»��� �ٴ»��� �ʱ�ȭ
	dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk() = false;
	dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun() = false;

	//==================================================================================================

	//������ ���� ����
	doublepoint P1 = Pos;
	doublepoint P2 = Pos + doublepoint{ (double)LookDirection * 150,0 };

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
	
	//==================================================================================================

	if (MainOrder == Main_Order::LeanLeft)
	{
		ActionOrder = Action_Order::LeanLeft;
			
		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
		}

				
	}

	if (MainOrder == Main_Order::LeanRight)
	{
		
		ActionOrder = Action_Order::LeanRight;

		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
		}

	}

	if (MainOrder == Main_Order::IdleRight)
	{
		LookDirection = Right;
		ActionOrder = Action_Order::IdleRight;

		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
		}
	}
	
	if (MainOrder == Main_Order::IdleLeft)
	{
		LookDirection = Left;
		ActionOrder = Action_Order::IdleLeft;

		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
		}
	}

	if (MainOrder == Main_Order::RoamAround)
	{

		if (LookDirection == Right)
			ActionOrder = Action_Order::WalkRight;

		if (LookDirection == Left)
			ActionOrder = Action_Order::WalkLeft;

		// �ι� ����Ʈ�� �������� ����� ���� ��ȯ
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

		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
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
			int a = 0; //������
		}

		std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, RoamingFloor);

		if (path.size() == 0)
		{
			//���� ����
			MoveOrder = Move_Order::End;
		}

		if (path.size() == 1)
		{
			//������
			MoveOrder = Move_Order::Stay;
		}

		if (path.size() > 1)
		{
			MoveOrder = path[1].HowToMove;
		}
		
		if (PlayerDetection())
		{
			MainOrder = Main_Order::PlayerDetected;
			ActionOrder = Action_Order::End;
		}
		
		
		
	}
	
	//=======================================================================================


	if (MainOrder == Main_Order::PlayerDetected) // �÷��̾ �����Ǿ����� ã�Ƽ� ���󰣴�
	{
		IgnoreTopFloorList.clear();
		RecogniseTopFloorList.clear();

		//������ ���� ����

		if (KeyMgr::Create()->key(Key::M).pressed)
		{
			std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, player->GetRecentFloor());
			int a = 0; //������
		}

		std::vector<CFloor::ConnectedNode> path = CNodeMgr::FindPath(RecentFloor, player->GetRecentFloor());

		if (path.size() == 0)
		{
			//���� ����
			MoveOrder = Move_Order::CannotDetectPlayer;
		}

		if (path.size() == 1)
		{
			//������
			MoveOrder = Move_Order::Stay;
		}

		if (path.size() > 1)
		{
			MoveOrder = path[1].HowToMove;
		}
	}

	if (MainOrder == Main_Order::AttackLeft) 
	{
		ActionOrder = Action_Order::AttackLeft;
	
	}

	if (MainOrder == Main_Order::AttackRight) 
	{
		ActionOrder = Action_Order::AttackRight;

	}

	if (MainOrder == Main_Order::ShotLeft)
	{
		ActionOrder = Action_Order::ShotLeft;

	}

	if (MainOrder == Main_Order::ShotRight)
	{
		ActionOrder = Action_Order::ShotRight;

	}

	//=================================================================================================== �ǰ�
	
	if (MainOrder == Main_Order::GetHurt) // �ǰݽ� 
	{
		double b = TimeMgr::Create()->CheckTimer();
		int a = 0;
		
		if (Dead == false)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (CParticleMgr::Create()->BloodEmitor[i]->GetOnOff() == false)
				{
					BloodEmitor = CParticleMgr::Create()->BloodEmitor[i];
					BloodEmitor->SetOwner(this);
					BloodEmitor->SetOffset(doublepoint{ 0,0 });
					BloodEmitor->SetOnOff(false);

					break;
				}
			}
		}
			

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

		if (lasoremitorcount < 1)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (CParticleMgr::Create()->LaserParticleEmitor[i]->GetOnOff() == false)
				{
					LaserparticleEmitor = CParticleMgr::Create()->LaserParticleEmitor[i];
					LaserparticleEmitor->SetOwner(this);
					LaserparticleEmitor->SetOffset(doublepoint{ 0,-20 });
					LaserparticleEmitor->SetOnOff(false);
					break;
				}
			}

			SetLaserParticleOption(doublepoint{ 1,1 }, doublepoint{ 190,350 }, doublepoint{ 70,500 }, doublepoint{ 0.3,1 }, doublepoint{ 0.001,0.01 }, doublepoint{ -10,10 }, doublepoint{ 0,0 });
		}
			

		lasoremitorcount++;
	}

	if (MainOrder == Main_Order::Dead)
	{
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().y = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetGravity(false);

		Dead = true;
		SetBloodEmitor(false);
	}

	if (MainOrder == Main_Order::MonsterStun)
	{
		ActionOrder = Action_Order::Stun;
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	CAnimal::Update();
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



bool CMonster::PlayerDetection()
{
	// �÷��̾� ���� ����
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);

	doublepoint eyeposition = Pos + doublepoint{ EyeOffset.x * LookDirection, EyeOffset.y };

	doublepoint playerpos[3] = { player->GetPos()             // �÷��̾��� ������ �Ӹ� �� �������� �ҷ��ͼ�
	, player->GetPos() + doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3}
	,player->GetPos() - doublepoint{ 0,player->GetScale().y * player->GetResize().y / 3 } };



	for (int i = 0; i < 3; ++i)
	{
		doublepoint diff = (playerpos[i] - eyeposition) * LookDirection;

		if (LookDirection != 0 && diff.Norm() < DetectRange*DetectCoff && abs(diff.Angle()) < DetectAngle) // �þ� ���� �ȿ� ������
		{
			
			if (CheckArchitectureCollision(eyeposition, playerpos[i]) == false)
			{
				return true;
			}

		}
	}

	// �ٸ� ������ ������ �����Ѵ�.

	std::vector<CObject*> monsters = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (int i = 0; i < monsters.size(); ++i)
	{
		CMonster* p = dynamic_cast<CMonster*>(monsters[i]);

		if (p)
		{
			if (p->Dead && abs(p->GetPos().x-Pos.x) < DetectOthersDeathRange* DetectOthersDeathCoff && abs(p->GetPos().y-Pos.y) < 100 && CheckArchitectureCollision(p->GetPos(),Pos) == false)
				return true;
		}
	}

	return false;
}

bool CMonster::CheckArchitectureCollision(doublepoint A, doublepoint B)
{
	bool check = false;

	std::vector<CObject*> floors = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR);
	for (int j = 0; j < floors.size(); ++j)
	{
		doublepoint pos = ((CCollider*)floors[j]->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
		doublepoint scale = ((CCollider*)floors[j]->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();

		// ��� �������� �ҷ��ͼ� �÷��̾� ����, ���������� ������ ���� �´���� �׽�Ʈ �Ѵ�.

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
			check = check || CollisionMath::CheckifLineCollide(A1, A2, A, B);
			continue;
		}

		else
			check = check || CollisionMath::CheckLineBoxCollision(pos, scale, A, B);
	}


	return check;
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
