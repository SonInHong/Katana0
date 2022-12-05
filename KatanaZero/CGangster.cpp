#include "pch.h"
#include "CGangster.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CFloor.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CGangsterGun.h"
#include "CBullet.h"
#include "CNeutralObjMgr.h"
#include "CEffectMgr.h"
#include "CCollider.h"

#define ShotTerm 1.5
#define AttackTerm 0.5

CGangster::CGangster()
{
	RoamingDistance = 300;
	DetectAngle = 30;
	DetectRange = 600;
	EyeOffset = doublepoint{ 0,-20 };

	ShotSpeed = ShotTerm;
	ShotTimer = ShotTerm;
	AttackSpeed = AttackTerm;
	AttackTimer = AttackTerm;
}

CGangster::~CGangster()
{
}

void CGangster::Initialize()
{
	CMonster::Initialize();

	CreateCollider(doublepoint{ 0,10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsteridle\\right", L"GangsterIdleRight", doublepoint{ 0,0 }, doublepoint{ 49,50 }, 8, 0.07, true, doublepoint{ 0,-20 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsteridle\\left", L"GangsterIdleLeft", doublepoint{ 0,0 }, doublepoint{ 49,50 }, 8, 0.07, true, doublepoint{ 0,-20 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterwalk\\right", L"GangsterWalkRight", doublepoint{ 0,0 }, doublepoint{ 34,40 }, 8, 0.07, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterwalk\\left", L"GangsterWalkLeft", doublepoint{ 0,0 }, doublepoint{ 34,40 }, 8, 0.07, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterrun\\right", L"GangsterRunRight", doublepoint{ 0,0 }, doublepoint{ 45,40 }, 10, 0.07, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterrun\\left", L"GangsterRunLeft", doublepoint{ 0,0 }, doublepoint{ 45,40 }, 10, 0.07, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterturn\\right", L"GangsterTurnRight", doublepoint{ 0,0 }, doublepoint{ 48,42 }, 6, 0.05, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterturn\\left", L"GangsterTurnLeft", doublepoint{ 0,0 }, doublepoint{ 48,42 }, 6, 0.05, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangster_attack\\right", L"GangsterAttackRight", doublepoint{ 0,0 }, doublepoint{ 48,35 }, 6, 0.05, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangster_attack\\left", L"GangsterAttackLeft", doublepoint{ 0,0 }, doublepoint{ 48,35 }, 6, 0.05, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterhurtfly\\right", L"GangsterHurtflyRight", doublepoint{ 0,0 }, doublepoint{ 38,34 }, 2, 0.07, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterhurtfly\\left", L"GangsterHurtflyLeft", doublepoint{ 0,0 }, doublepoint{ 38,34 }, 2, 0.07, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterhurtground\\right", L"GangsterHurtgroundRight", doublepoint{ 0,0 }, doublepoint{ 44,34 }, 14, 0.1, false, doublepoint{ 0,0 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterhurtground\\left", L"GangsterHurtgroundLeft", doublepoint{ 0,0 }, doublepoint{ 44,34 }, 14, 0.1, false, doublepoint{ 0,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterfall\\right", L"GangsterFallRight", doublepoint{ 0,0 }, doublepoint{ 42,41 }, 12, 0.04, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangsterfall\\left", L"GangsterFallLeft", doublepoint{ 0,0 }, doublepoint{ 42,41 }, 12, 0.04, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangster_shot\\right", L"GangsterShotRight", doublepoint{ 0,0 }, doublepoint{ 42,50 }, 5, 0.07, false, doublepoint{ 15,-25 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Gangster\\spr_gangster_shot\\left", L"GangsterShotLeft", doublepoint{ 0,0 }, doublepoint{ 42,50 }, 5, 0.07, false, doublepoint{ -15,-25 });

	
	

	//==============================================================================================================================================================
	//이벤트 지정
	//==============================================================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnRight")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnLeft")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnRight")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Right);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnLeft")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Left);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterTurnLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleLeft");

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		//->FindAnimation(L"GangsterAttackRight")->m_StartEvent = std::bind(&CGangster::SlashFist, this, Right);  // 공격

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		//->FindAnimation(L"GangsterAttackLeft")->m_StartEvent = std::bind(&CGangster::SlashFist, this, Left); // 공격

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterAttackRight")->m_CompleteEvent = std::bind(&CGangster::CheckAttackStop, this);  // 공격이 끝날때마다 공격상태 탈출조건 확인

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterAttackLeft")->m_CompleteEvent = std::bind(&CGangster::CheckAttackStop, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterAttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterAttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterShotRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterShotLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GangsterIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterShotRight")->m_CompleteEvent = std::bind(&CGangster::CheckShotStop, this);  // 발사가 끝날때마다 발사상태 탈출조건 확인

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterShotLeft")->m_CompleteEvent = std::bind(&CGangster::CheckShotStop, this);


	//=====================================================================================================================
	//애니메이션에 맞는 피격 효과 설정 (피)
	//=====================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtflyRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 100,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtflyLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 270,470 }, doublepoint{ scaleA * 50,scaleA * 70 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtgroundRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 170,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtgroundLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 240,370 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });



	//=====================================================================================================================
	//잔상효과
	//=====================================================================================================================
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtflyRight")->AfterImageOn(0.2, PenColor::MAGENTA);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GangsterHurtflyLeft")->AfterImageOn(0.2, PenColor::MAGENTA);

	//=====================================================================================================================
	
}

void CGangster::Enter()
{
	CMonster::Enter();

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
	LookDirection = Right;
}

void CGangster::Exit()
{
}

void CGangster::Update()
{
	doublepoint& velocity = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity();
	doublepoint& force = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetForce();

	bool& OnGround = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround();
	int& OnStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair();
	int& OnWall = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnWall();
	int& OnDoor = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnDoor();
	int& WallGrab = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWallGrab();
	int& Roll = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRoll();
	int& Flip = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetFlip();
	bool& Walk = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk();
	bool& Run = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun();
	bool& AttackOnOff = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttack();
	double& AttackAngle = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttackAngle();
	bool& Hurt = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt();
	bool& FallStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetFallStair();

	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	Run = false;
	Walk = false;
	FallStair = false;// !!!필수!!!

	CMonster::Update();

	if (KeyMgr::Create()->key(Key::M).pressed)
	{
		int a = 0;
	}

	if (MainOrder == Main_Order::GetBurn)
		return;

	//=======================================================================================================
	// 무브오더
	//=======================================================================================================

	switch (MoveOrder)
	{
	case Move_Order::End:
	{

	}
	break;

	case Move_Order::CannotDetectPlayer:
	{
		//플레이어를 찾다가 길이 없는 경우 로밍으로 돌아감
		if (MainOrder == Main_Order::PlayerDetected)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::IdleRight;

			if (LookDirection == Left)
				ActionOrder = Action_Order::IdleLeft;
		}

		//지금은 바로 들어가지만, 두리번거리면서 물음표를 띄우는 액션상태 추가 후 그 액션이 끝나면 로밍으로 돌아가는 것을 추가 할 예정 
	}
	break;

	case Move_Order::MoveLeft:
	{
		if (LookDirection == Right)
			ActionOrder = Action_Order::TurnLeft;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;



	}
	break;

	case Move_Order::MoveRight:
	{
		if (LookDirection == Left)
			ActionOrder = Action_Order::TurnRight;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;


	}
	break;

	case Move_Order::ClimbDownLeft:
	{

		if (dynamic_cast<CStair*>(RecentFloor) == nullptr)
		{
			std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();

			for (int i = 0; i < CN.size(); ++i)
			{
				CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);

				if (ct)
				{
					ct->IgnoreMe(this);
					IgnoreTopFloorList.push_back(ct);

				}
			}

			if (Pos.x + (Scale.x * Resize.x) / 2 < RecentFloor->GetPos().x - (RecentFloor->GetScale().x * RecentFloor->GetResize().x) / 2)
				OnStair = Right;
		}

		if (LookDirection == Right)
			ActionOrder = Action_Order::TurnLeft;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;




	}
	break;

	case Move_Order::ClimbDownRight:
	{

		if (dynamic_cast<CStair*>(RecentFloor) == nullptr)
		{
			std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();

			for (int i = 0; i < CN.size(); ++i)
			{
				CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);

				if (ct)
				{
					ct->IgnoreMe(this);
					IgnoreTopFloorList.push_back(ct);

				}
			}

			if (Pos.x - (Scale.x * Resize.x) / 2 > RecentFloor->GetPos().x + (RecentFloor->GetScale().x * RecentFloor->GetResize().x) / 2)
				OnStair = Left;
		}

		if (LookDirection == Left)
			ActionOrder = Action_Order::TurnRight;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;




	}
	break;

	case Move_Order::Stay:
	{


		// 예외처리에서 수행

	}
	break;

	case Move_Order::MoveToTopFloorRight:
	{
		std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();
		for (int i = 0; i < CN.size(); ++i)
		{
			CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);
			if (ct)
			{
				ct->RecogniseMe(this);
				RecogniseTopFloorList.push_back(ct);
			}

		}

		if (LookDirection == Left)
			ActionOrder = Action_Order::TurnRight;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;


	}
	break;

	case Move_Order::MoveToTopFloorLeft:
	{
		std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();
		for (int i = 0; i < CN.size(); ++i)
		{
			CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);
			if (ct)
			{
				ct->RecogniseMe(this);
				RecogniseTopFloorList.push_back(ct);
			}

		}

		if (LookDirection == Right)
			ActionOrder = Action_Order::TurnLeft;

		else
			ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;

	}
	break;

	}
	//============================================================================
	//예외처리
	//============================================================================

	if (MainOrder == Main_Order::PlayerDetected && MoveOrder == Move_Order::Stay) // 스테이 플레이어 찾아가기
	{
		if (player->GetPos().x > Pos.x)
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = Action_Order::RunRight;

		}


		if (player->GetPos().x < Pos.x)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = Action_Order::RunLeft;

		}
	}

	if (MainOrder == Main_Order::GetToRoamingPoint && MoveOrder == Move_Order::Stay) // 스테이 로밍포인트 찾아가기
	{
		if (RoamingPoint.x > Pos.x + 50)
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = Action_Order::WalkRight;

		}


		if (RoamingPoint.x < Pos.x - 50)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = Action_Order::WalkLeft;

		}

		if (abs(RoamingPoint.x - Pos.x) < RoamingDistance - 1)
			MainOrder = Main_Order::RoamAround;
	}



	doublepoint diff = (player->GetPos() - Pos);              // 공격수행 조건
	if (MainOrder == Main_Order::PlayerDetected  && diff.Norm() < 150 && diff.x * LookDirection >= 0)
	{
		if (LookDirection == Right)
			MainOrder = Main_Order::AttackRight;

		else if(LookDirection == Left)
			MainOrder = Main_Order::AttackLeft;
	}
	

	//사격 수행 조건
	if (MainOrder == Main_Order::PlayerDetected && diff.Norm() < 700 && diff.x * LookDirection >= 0 && CheckArchitectureCollision(Pos, player->GetPos()) == false)
	{
		if (LookDirection == Right)
		{
			MainOrder = Main_Order::ShotRight;
			ActionOrder = Action_Order::ShotRight;
		}
			

		else if (LookDirection == Left)
		{
			MainOrder = Main_Order::ShotLeft;
			ActionOrder = Action_Order::ShotLeft;
		}
			
	}
		
	//=======================================================================================================================================

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterTurnRight"
		&& MainOrder != Main_Order::GetHurt && MainOrder != Main_Order::MonsterStun)   // 피격시가 아니라면 턴동작은 무조건 마무리
		ActionOrder = Action_Order::TurnRight;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterTurnLeft"
		&& MainOrder != Main_Order::GetHurt && MainOrder != Main_Order::MonsterStun)
		ActionOrder = Action_Order::TurnLeft;

	if (ActionOrder == Action_Order::RunLeft && (OnWall || OnDoor)) // 벽에 붙었으면 서있기
	{
		ActionOrder = Action_Order::IdleLeft;
		force.x -= scaleA * 250000;
	}

	if (ActionOrder == Action_Order::RunRight && (OnWall || OnDoor))
	{
		ActionOrder = Action_Order::IdleRight;
		force.x += scaleA * 250000;
	}

	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkRight && (OnWall || OnDoor)) // 로밍 중 벽 만나면 뒤돌기
	{
		ActionOrder = Action_Order::TurnLeft;
	}

	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkLeft && (OnWall || OnDoor))
	{
		ActionOrder = Action_Order::TurnRight;
	}

	//피격 애니메이션

	while (1)
	{
		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterHurtflyRight")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtgroundRight");

			else if (OnWall || OnDoor)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtflyLeft");
				break;
			}

			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterFallLeft");
			}

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterFallRight");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterHurtflyLeft")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtgroundLeft");

			else if (OnWall || OnDoor)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtflyRight");

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterFallRight");
			}

			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterFallLeft");
			}
		}

		// 굴러 떨어지기 시작할때

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterFallLeft")
		{
			if (!OnGround)
			{
				force.x -= scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtgroundLeft");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GangsterFallRIght")
		{
			if (!OnGround)
			{
				force.x += scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtgroundRight");
			}
		}

		break;
	}


	//===========================================================================================
	// 액션 오더 수행
	//===========================================================================================

	switch (ActionOrder)
	{
	case Action_Order::End:
	{

	}
	break;

	case Action_Order::LeanLeft:
	{
		
	}
	break;

	case Action_Order::LeanRight:
	{
		
	}
	break;

	case Action_Order::RunLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleLeft");
			break;
		}


		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterRunLeft");
		force.x -= scaleA * 250000;

		Run = true;
	}
	break;

	case Action_Order::RunRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterRunRight");
		force.x += scaleA * 250000;

		Run = true;

	}
	break;

	case Action_Order::TurnLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterTurnLeft");

	}
	break;

	case Action_Order::TurnRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterTurnRight");
	}
	break;

	case Action_Order::IdleRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
	}
	break;

	case Action_Order::IdleLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleLeft");
	}
	break;

	case Action_Order::AttackLeft:
	{				
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterAttackLeft");
			SlashFist(Left);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GangsterAttackLeft")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleLeft");
		}
	}
	break;

	case Action_Order::AttackRight:
	{
		
		
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterAttackRight");
			SlashFist(Right);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GangsterAttackRight")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
		}

	}
	break;

	case Action_Order::ShotLeft:
	{	
		if (ShotTimer > ShotSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterShotLeft");
			ShootGun();
			ShotTimer = 0;

			CEffectMgr::Create()->GunSparkLeft.Play(Pos + doublepoint{ -110,-25 }, doublepoint{ 2,2 });
		}
		
		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GangsterShotLeft")
		{
			CheckShotStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleLeft");
		}
	}
	break;

	case Action_Order::ShotRight:
	{
		if (ShotTimer > ShotSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterShotRight");
			ShootGun();
			ShotTimer = 0;

			CEffectMgr::Create()->GunSparkRight.Play(Pos + doublepoint{ 110,-25 },doublepoint{2,2});
		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GangsterShotRight")
		{
			CheckShotStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
		}
			
	}
	break;

	case Action_Order::WalkLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleLeft");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterWalkLeft");
		force.x -= scaleA * 250000;

		Walk = true;
	}
	break;

	case Action_Order::WalkRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterWalkRight");
		force.x += scaleA * 250000;

		Walk = true;
	}
	break;

	case Action_Order::HurtGround:
	{
		velocity.x = scaleA * 1000 * cos(HurtAngle);

		OnGround = false;
		OnStair = 0;
		MainOrder = Main_Order::End;
		LookDirection = 0;

	}
	break;

	case Action_Order::HurtFly:
	{
		velocity.x = scaleA * 1000 * cos(HurtAngle);
		velocity.y = scaleA * 1000 * sin(HurtAngle);
		OnGround = false;
		OnStair = 0;
		MainOrder = Main_Order::End;


		if (cos(HurtAngle) < 0)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtflyLeft");

		else
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GangsterHurtflyRight");


		LookDirection = 0;
		Hurt = true;


	}
	break;

	}

	ShotTimer += TimeMgr::Create()->dt();
	AttackTimer += TimeMgr::Create()->dt();
}

void CGangster::Render(HDC _dc)
{
	CObject::Render(_dc);
}

void CGangster::SlashFist(int dir)
{
	CGangsterGun* fist = new CGangsterGun;

	fist->Direction = dir;

	fist->Owner = this;
	fist->Initialize();

	CEventMgr::Create()->Event_CreateObj(fist, GROUP_TYPE::MONSTER_PROJECTILE);
}

#define bulletVelocity 1500

void CGangster::ShootGun()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);

	doublepoint vel = (dynamic_cast<CCollider*>(player->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos() - (Pos + doublepoint{ (double)LookDirection * 60,-15 })).Normalize() * bulletVelocity;

	CNeutralObjMgr::Create()->ShootBullet(Pos + doublepoint{(double)LookDirection*60,-15}, vel, true);

	

}

void CGangster::CheckAttackStop()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	doublepoint diff = (player->GetPos() - Pos);

	if (diff.Norm() > 150 || diff.x * LookDirection < 0)
		MainOrder = Main_Order::PlayerDetected;
}

void CGangster::CheckShotStop()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	doublepoint diff = (player->GetPos() - Pos);

	if (diff.Norm() > 700 || diff.x * LookDirection < 0 || CheckArchitectureCollision(Pos, player->GetPos()) == true)
	{
	
		MainOrder = Main_Order::PlayerDetected;
	}
		

	if (diff.Norm() < 150 && diff.x * LookDirection >= 0)
	{
		if (LookDirection == Right)
			MainOrder = Main_Order::AttackRight;

		else if (LookDirection == Left)
			MainOrder = Main_Order::AttackLeft;

	}

	

}
