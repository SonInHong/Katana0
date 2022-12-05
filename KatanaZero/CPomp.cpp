#include "pch.h"
#include "CPomp.h"
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
#include "CPompStick.h"

CPomp::CPomp()
{
	RoamingDistance = 300;
	DetectAngle = 30;
	DetectRange = 600;
	EyeOffset = doublepoint{ 0,-20 };
}

CPomp::~CPomp()
{
}

void CPomp::Initialize()
{
	CMonster::Initialize();

	CreateCollider(doublepoint{ 0,10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_idle\\right", L"PompIdleRight", doublepoint{ 0,0 }, doublepoint{ 33,42 }, 8, 0.07, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_idle\\left", L"PompIdleLeft", doublepoint{ 0,0 }, doublepoint{ 33,42 }, 8, 0.07, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_walk\\right", L"PompWalkRight", doublepoint{ 0,0 }, doublepoint{ 31,43 }, 10, 0.07, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_walk\\left", L"PompWalkLeft", doublepoint{ 0,0 }, doublepoint{ 31,43 }, 10, 0.07, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_run\\right", L"PompRunRight", doublepoint{ 0,0 }, doublepoint{ 33,40 }, 10, 0.07, true, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_run\\left", L"PompRunLeft", doublepoint{ 0,0 }, doublepoint{ 33,40 }, 10, 0.07, true, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_turn\\right", L"PompTurnRight", doublepoint{ 0,0 }, doublepoint{ 37,43 }, 6, 0.1, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_turn\\left", L"PompTurnLeft", doublepoint{ 0,0 }, doublepoint{ 37,43 }, 6, 0.1, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_attack\\right", L"PompAttackRight", doublepoint{ 0,0 }, doublepoint{ 46,43 }, 6, 0.04, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_attack\\left", L"PompAttackLeft", doublepoint{ 0,0 }, doublepoint{ 46,43 }, 6, 0.04, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_hurtfly\\right", L"PompHurtflyRight", doublepoint{ 0,0 }, doublepoint{ 41,35 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_hurtfly\\left", L"PompHurtflyLeft", doublepoint{ 0,0 }, doublepoint{ 41,35 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_hurtground\\right", L"PompHurtgroundRight", doublepoint{ 0,0 }, doublepoint{ 54,42 }, 15, 0.1, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_hurtground\\left", L"PompHurtgroundLeft", doublepoint{ 0,0 }, doublepoint{ 54,42 }, 15, 0.1, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_fall\\right", L"PompFallRight", doublepoint{ 0,0 }, doublepoint{ 41,35 }, 13, 0.04, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_fall\\left", L"PompFallLeft", doublepoint{ 0,0 }, doublepoint{ 41,35 }, 13, 0.04, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_lean\\right", L"PompLeanRight", doublepoint{ 0,0 }, doublepoint{ 17,42 }, 1, 0.04, true, doublepoint{ scaleA * 5,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_lean\\left", L"PompLeanLeft", doublepoint{ 0,0 }, doublepoint{ 17,42 }, 1, 0.04, true, doublepoint{ -scaleA * 5,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_knockdown\\right", L"PompKnockDownRight", doublepoint{ 0,0 }, doublepoint{ 50,44 }, 22, 0.07, false, doublepoint{ 0,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Pomp\\spr_pomp_knockdown\\left", L"PompKnockDownLeft", doublepoint{ 0,0 }, doublepoint{ 50,44 }, 22, 0.07, false, doublepoint{ 0,-10 });

	//==============================================================================================================================================================
	//이벤트 지정
	//==============================================================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnRight")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnLeft")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnRight")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Right);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnLeft")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Left);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"PompIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompTurnLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"PompIdleLeft");
		
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompAttackRight")->m_CompleteEvent = std::bind(&CPomp::CheckAttackStop, this);  // 공격이 끝날때마다 공격상태 탈출조건 확인

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompAttackLeft")->m_CompleteEvent = std::bind(&CPomp::CheckAttackStop, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompAttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"PompIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompAttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"PompIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompKnockDownRight")->m_CompleteEvent = std::bind(&CMonster::SetMainOrder, this, Main_Order::PlayerDetected);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompKnockDownLeft")->m_CompleteEvent = std::bind(&CMonster::SetMainOrder, this, Main_Order::PlayerDetected);
	//=====================================================================================================================
	//애니메이션에 맞는 피격 효과 설정 (피)
	//=====================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtflyRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 100,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtflyLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 270,470 }, doublepoint{ scaleA * 50,scaleA * 70 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtgroundRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 170,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtgroundLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 240,370 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });



	//=====================================================================================================================
	//잔상효과
	//=====================================================================================================================
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtflyRight")->AfterImageOn(0.2, PenColor::MAGENTA);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PompHurtflyLeft")->AfterImageOn(0.2, PenColor::MAGENTA);

	
}

void CPomp::Enter()
{
	CMonster::Enter();

	//=====================================================================================================================
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleRight");
	LookDirection = Right;
}

void CPomp::Exit()
{
}

void CPomp::Update()
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
	if (MainOrder == Main_Order::PlayerDetected && diff.Norm() < 150 && diff.x * LookDirection >= 0)
	{
		if (LookDirection == Right)
			MainOrder = Main_Order::AttackRight;

		else if (LookDirection == Left)
			MainOrder = Main_Order::AttackLeft;
	}

	//======================================================================================================================

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompTurnRight"
		&& MainOrder != Main_Order::GetHurt && MainOrder != Main_Order::MonsterStun)   // 피격시가 아니라면 턴동작은 무조건 마무리
		ActionOrder = Action_Order::TurnRight;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompTurnLeft"
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
		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompHurtflyRight")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtgroundRight");

			else if (OnWall || OnDoor)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtflyLeft");
				break;
			}

			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompFallLeft");
			}

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompFallRight");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompHurtflyLeft")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtgroundLeft");

			else if (OnWall || OnDoor)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtflyRight");

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompFallRight");
			}

			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompFallLeft");
			}
		}

		// 굴러 떨어지기 시작할때

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompFallLeft")
		{
			if (!OnGround)
			{
				force.x -= scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtgroundLeft");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"PompFallRIght")
		{
			if (!OnGround)
			{
				force.x += scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtgroundRight");
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
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompLeanLeft");
		velocity.x = 0;
		LookDirection = Right;
	}
	break;

	case Action_Order::LeanRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompLeanRight");
		velocity.x = 0;
		LookDirection = Left;
	}
	break;

	case Action_Order::RunLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleLeft");
			break;
		}


		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompRunLeft");
		force.x -= scaleA * 250000;

		Run = true;
	}
	break;

	case Action_Order::RunRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompRunRight");
		force.x += scaleA * 250000;

		Run = true;

	}
	break;

	case Action_Order::TurnLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompTurnLeft");

	}
	break;

	case Action_Order::TurnRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompTurnRight");
	}
	break;

	case Action_Order::IdleRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleRight");
	}
	break;

	case Action_Order::IdleLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleLeft");
	}
	break;

	case Action_Order::AttackLeft:
	{
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompAttackLeft");
			Slash(Left);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"PompAttackLeft")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleLeft");
		}
	}
	break;

	case Action_Order::AttackRight:
	{
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompAttackRight");
			Slash(Right);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"PompAttackRight")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleRight");
		}
	}
	break;

	case Action_Order::WalkLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleLeft");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompWalkLeft");
		force.x -= scaleA * 250000;

		Walk = true;
	}
	break;

	case Action_Order::WalkRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompWalkRight");
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
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtflyLeft");

		else
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompHurtflyRight");


		LookDirection = 0;
		Hurt = true;


	}
	break;

	case Action_Order::Stun:
	{
		if(LookDirection == Left)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompKnockDownLeft");

		if (LookDirection == Right)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PompKnockDownRight");
	}
	break;

	}


	AttackTimer += TimeMgr::Create()->dt(); // 타이머는 계속 흘러간다.
}

void CPomp::Render(HDC _dc)
{
	CObject::Render(_dc);
}

void CPomp::Slash(int dir)
{
	CPompStick* stick = new CPompStick;

	stick->Direction = dir;

	stick->Owner = this;
	stick->Initialize();

	CEventMgr::Create()->Event_CreateObj(stick, GROUP_TYPE::MONSTER_PROJECTILE);
}

void CPomp::CheckAttackStop()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	doublepoint diff = (player->GetPos() - Pos);

	if (diff.Norm() > 150 || diff.x * LookDirection < 0)
		MainOrder = Main_Order::PlayerDetected;
}
