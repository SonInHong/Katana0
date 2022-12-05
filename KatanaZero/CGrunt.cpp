#include "pch.h"
#include "CGrunt.h"
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
#include "CGruntFist.h"
#include "CEventMgr.h"

CGrunt::CGrunt()
{
	RoamingDistance = 300;
	DetectAngle = 30;
	DetectRange = 600;
	EyeOffset = doublepoint{ 0,-20 };
}

CGrunt::~CGrunt()
{
}

void CGrunt::Initialize()
{
	CMonster::Initialize();
	
	CreateCollider(doublepoint{ 0,10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_idle\\right", L"GruntIdleRight", doublepoint{ 0,0 }, doublepoint{ 30,36 }, 8, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_idle\\left", L"GruntIdleLeft", doublepoint{ 0,0 }, doublepoint{ 30,36 }, 8, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_walk\\right", L"GruntWalkRight", doublepoint{ 0,0 }, doublepoint{ 32,40 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_walk\\left", L"GruntWalkLeft", doublepoint{ 0,0 }, doublepoint{ 32,40 }, 10, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_run\\right", L"GruntRunRight", doublepoint{ 0,0 }, doublepoint{ 36,39 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_run\\left", L"GruntRunLeft", doublepoint{ 0,0 }, doublepoint{ 36,39 }, 10, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_turn\\right", L"GruntTurnRight", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 8, 0.1, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_turn\\left", L"GruntTurnLeft", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 8, 0.1, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_attack\\right", L"GruntAttackRight", doublepoint{ 0,0 }, doublepoint{ 44,42 }, 8, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_attack\\left", L"GruntAttackLeft", doublepoint{ 0,0 }, doublepoint{ 44,42 }, 8, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtfly\\right", L"GruntHurtflyRight", doublepoint{ 0,0 }, doublepoint{ 43,37 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtfly\\left", L"GruntHurtflyLeft", doublepoint{ 0,0 }, doublepoint{ 43,37 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtground\\right", L"GruntHurtgroundRight", doublepoint{ 0,0 }, doublepoint{ 56,41 }, 16, 0.1, false , doublepoint{0,-10});
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtground\\left", L"GruntHurtgroundLeft", doublepoint{ 0,0 }, doublepoint{ 56,41 }, 16, 0.1, false, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_fall\\right", L"GruntFallRight", doublepoint{ 0,0 }, doublepoint{ 38,32 }, 13, 0.04, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_fall\\left", L"GruntFallLeft", doublepoint{ 0,0 }, doublepoint{ 38,32 }, 13, 0.04, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_lean\\right", L"GruntLeanRight", doublepoint{ 0,0 }, doublepoint{ 20,38 }, 1, 0.04, true , doublepoint{ scaleA * 5,0});
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_lean\\left", L"GruntLeanLeft", doublepoint{ 0,0 }, doublepoint{ 20,38 }, 1, 0.04, true, doublepoint{ -scaleA * 5,0 });

	//�ָ� ����Ʈ �� �̺�Ʈ ����
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_gruntslash\\right", L"GruntFistRight", doublepoint{ 0,0 }, doublepoint{ 64,64 }, 8, 0.04, false, doublepoint{ 10,-10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_gruntslash\\left", L"GruntFistLeft", doublepoint{ 0,0 }, doublepoint{ 64,64 }, 8, 0.04, false , doublepoint{-10,-10});

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])
		->FindAnimation(L"GruntFistRight")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])
		->FindAnimation(L"GruntFistLeft")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackRight")->m_StartEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]), L"GruntFistRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackLeft")->m_StartEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]), L"GruntFistLeft");

	//==============================================================================================================================================================
	//�̺�Ʈ ����
	//==============================================================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Right);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Left);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackRight")->m_CompleteEvent = std::bind(&CGrunt::CheckAttackStop, this);  // ������ ���������� ���ݻ��� Ż������ Ȯ��

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackLeft")->m_CompleteEvent = std::bind(&CGrunt::CheckAttackStop, this);
	//=====================================================================================================================
	//�ִϸ��̼ǿ� �´� �ǰ� ȿ�� ���� (��)
	//=====================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtflyRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 100,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtflyLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitorOption, this
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 270,470 }, doublepoint{ scaleA * 50,scaleA * 70 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtgroundRight")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 170,300 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtgroundLeft")->m_StartEvent = std::bind(&CMonster::SetBloodEmitormaxOption, this, 300
			, doublepoint{ scaleA * 1,scaleA * 1 }, doublepoint{ 240,370 }, doublepoint{ scaleA * 100,scaleA * 150 }, doublepoint{ 0.3,0.7 }, doublepoint{ 0.01,0.02 }, doublepoint{ -scaleA * 5,scaleA * 5 }, doublepoint{ -scaleA * 5,scaleA * 5 });

	

	//=====================================================================================================================
	//�ܻ�ȿ��
	//=====================================================================================================================
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtflyRight")->AfterImageOn(0.2 , PenColor::MAGENTA);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntHurtflyLeft")->AfterImageOn(0.2, PenColor::MAGENTA);

	//=====================================================================================================================
	

	
}

void CGrunt::Enter()
{
	CMonster::Enter();

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
	LookDirection = Right;
}

void CGrunt::Exit()
{
}

void CGrunt::Update()
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
	FallStair = false;// !!!�ʼ�!!!

	CMonster::Update();

	if (KeyMgr::Create()->key(Key::M).pressed)
	{
		int a = 0;
	}

	if (MainOrder == Main_Order::GetBurn)
		return;
	
	//=======================================================================================================
	// �������
	//=======================================================================================================

	switch (MoveOrder)
	{
		case Move_Order::End:
		{
			
		}
		break;

		case Move_Order::CannotDetectPlayer:
		{
			//�÷��̾ ã�ٰ� ���� ���� ��� �ι����� ���ư�
			if (MainOrder == Main_Order::PlayerDetected)
			{
				if (LookDirection == Right)
					ActionOrder = Action_Order::IdleRight;

				if (LookDirection == Left)
					ActionOrder = Action_Order::IdleLeft;
			}

			//������ �ٷ� ������, �θ����Ÿ��鼭 ����ǥ�� ���� �׼ǻ��� �߰� �� �� �׼��� ������ �ι����� ���ư��� ���� �߰� �� ���� 
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

				if (Pos.x + (Scale.x*Resize.x)/2 < RecentFloor->GetPos().x - (RecentFloor->GetScale().x * RecentFloor->GetResize().x) / 2)
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
			
				
			// ����ó������ ����
			
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
	//����ó��
	//============================================================================

	if (MainOrder == Main_Order::PlayerDetected && MoveOrder == Move_Order::Stay) // ������ �÷��̾� ã�ư���
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

	if (MainOrder == Main_Order::GetToRoamingPoint && MoveOrder == Move_Order::Stay) // ������ �ι�����Ʈ ã�ư���
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

	

	doublepoint diff = (player->GetPos() - Pos);              // ���ݼ��� ����
	if (MainOrder == Main_Order::PlayerDetected && diff.Norm() < 150 && diff.x * LookDirection >= 0)
	{
		if (LookDirection == Right)
			MainOrder = Main_Order::AttackRight;

		else if (LookDirection == Left)
			MainOrder = Main_Order::AttackLeft;
	}


	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnRight"
		&& MainOrder != Main_Order::GetHurt && MainOrder != Main_Order::MonsterStun)   // �ǰݽð� �ƴ϶�� �ϵ����� ������ ������
		ActionOrder = Action_Order::TurnRight;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnLeft"
		&& MainOrder != Main_Order::GetHurt && MainOrder != Main_Order::MonsterStun)
		ActionOrder = Action_Order::TurnLeft;
		
	if (ActionOrder == Action_Order::RunLeft && (OnWall || OnDoor)) // ���� �پ����� ���ֱ�
	{
		ActionOrder = Action_Order::IdleLeft;
		force.x -= scaleA * 250000;
	}

	if (ActionOrder == Action_Order::RunRight && (OnWall || OnDoor))
	{
		ActionOrder = Action_Order::IdleRight;
		force.x += scaleA * 250000;
	}
	
	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkRight && (OnWall ||OnDoor) ) // �ι� �� �� ������ �ڵ���
	{
		ActionOrder = Action_Order::TurnLeft;
	}

	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkLeft && (OnWall || OnDoor))
	{
		ActionOrder = Action_Order::TurnRight;
	}

	//�ǰ� �ִϸ��̼�
	
	while (1)
	{
		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntHurtflyRight")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundRight");

			else if (OnWall || OnDoor)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyLeft");
				break;
			}
			
			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntFallLeft");
			}

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntFallRight");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntHurtflyLeft")
		{
			if (OnGround == true && abs(velocity.x) < 50)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundLeft");

			else if (OnWall || OnDoor)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyRight");

			else if (OnStair == Left && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntFallRight");
			}

			else if (OnStair == Right && velocity.Norm() < 50)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntFallLeft");
			}
		}

		// ���� �������� �����Ҷ�

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntFallLeft")
		{
			if (!OnGround)
			{
				force.x -= scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundLeft");
			}
		}

		if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntFallRIght")
		{
			if (!OnGround)
			{
				force.x += scaleA * 250000;
				FallStair = true;
			}

			else
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundRight");
			}
		}

		break;
	}
	
	
	//===========================================================================================
	// �׼� ���� ����
	//===========================================================================================

	switch (ActionOrder)
	{
	case Action_Order::End:
	{

	}
	break;

	case Action_Order::LeanLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntLeanLeft");
		velocity.x = 0;
		LookDirection = Right;
	}
	break;

	case Action_Order::LeanRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntLeanRight");
		velocity.x = 0;
		LookDirection = Left;
	}
	break;

	case Action_Order::RunLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleLeft");
			break;
		}
			

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntRunLeft");
		force.x -= scaleA * 250000;

		Run = true;
	}
	break;

	case Action_Order::RunRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntRunRight");
		force.x += scaleA * 250000;

		Run = true;

	}
	break;

	case Action_Order::TurnLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntTurnLeft");
		
	}
	break;

	case Action_Order::TurnRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntTurnRight");
	}
	break;

	case Action_Order::IdleRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
	}
	break;

	case Action_Order::IdleLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleLeft");
	}
	break;

	case Action_Order::AttackLeft:
	{
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntAttackLeft");
			SlashFist(Left);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GruntAttackLeft")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleLeft");
		}
	
	}
	break;

	case Action_Order::AttackRight:
	{
		if (AttackTimer > AttackSpeed)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntAttackRight");
			SlashFist(Right);
			AttackTimer = 0;

		}

		else if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() != L"GruntAttackRight")
		{
			CheckAttackStop();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
		}
		
	}
	break;

	case Action_Order::WalkLeft:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleLeft");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntWalkLeft");
		force.x -= scaleA * 250000;

		Walk = true;
	}
	break;

	case Action_Order::WalkRight:
	{
		if (LaserExist)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
			break;
		}

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntWalkRight");
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

		
			if ( cos(HurtAngle) < 0)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyLeft");

			else
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyRight");
		

		LookDirection = 0;
		Hurt = true;
		

	}
	break;

	}
	
	AttackTimer += TimeMgr::Create()->dt();
}

void CGrunt::Render(HDC _dc)
{
	CObject::Render(_dc);
}



void CGrunt::AdjustHurtAnimation()
{
	
}

void CGrunt::SlashFist(int dir)
{
	CGruntFist* fist = new CGruntFist;

	fist->Direction = dir;

	fist->Owner = this;
	fist->Initialize();

	CEventMgr::Create()->Event_CreateObj(fist, GROUP_TYPE::MONSTER_PROJECTILE);
	
}

void CGrunt::CheckAttackStop()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	doublepoint diff = (player->GetPos() - Pos);

	if (diff.Norm() > 150 || diff.x * LookDirection < 0)
		MainOrder = Main_Order::PlayerDetected;
}
