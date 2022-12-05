#include "pch.h"
#include "CPlayer.h"
#include "CResourceMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CSword.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "CEventMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CGroundBreaker.h"
#include "CFloor.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CDustcloudEmitor.h"
#include "CJumpCloud.h"
#include "CLandCloud.h"
#include "CDoorBreaker.h"
#include "CBloodEmitor.h"
#include "CEffectMgr.h"
#include "CLaserparticleEmitor.h"
#include "CParticleMgr.h"

#define Right 1
#define Left -1
#define JumpVelocity -scaleA*550
#define Animator0 dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
#define Animator1 dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])
#define AttackSpeed 0.0
#define StunTime 0.5

CPlayer::CPlayer()
	:Texture(nullptr)
	,LookDirection(1)
	,PrevPos{}
	,PrevOnFloor(false)
	,AttackOnAir(false)
	,AttackTimer(AttackSpeed+1)
	, AttackTimerSwitch(false)
	,PlayerDead(false)
	,burn(false)
	,Rolling(false)
	, PlayerSword(nullptr)
	, Unbeatable(false)
	,StunTimer(StunTime)
	, Stunned(false)
	,MP(100)
	,CantClimb(false)
	,Item(nullptr)
{
	MainOrder = Main_Order::End;
}

CPlayer::~CPlayer()
{

}

void CPlayer::Initialize()
{
	//충돌체
	CreateCollider(doublepoint{ 0,7 });

	//dynamic_cast<CCollider*>(m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetOffSet(doublepoint{ 0,7 });

	//애니메이터
	CreateAnimator();
	CreateAnimator();  // 칼 애니메이션용 애니메이터
	CreateAnimator(); // UI용 애니메이터
	CreateRigidBody();

	//CResourceMgr::Create()->MakeSpriteSheet(L"Player\\spr_idle", L"PlayerIdle");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle\\right", L"IdleRight", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 11, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle\\left", L"IdleLeft", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 10, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\right", L"IdleToRunRight", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\left", L"IdleToRunLeft", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\right", L"RollToRunRight", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\left", L"RollToRunLeft", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run\\right", L"RunRight", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run\\left", L"RunLeft", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 10, 0.07, true);


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run_to_idle\\right", L"RunToIdleRight", doublepoint{ 0,0 }, doublepoint{ 52,36 }, 5, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run_to_idle\\left", L"RunToIdleLeft", doublepoint{ 0,0 }, doublepoint{ 52,36 }, 5, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_attack\\right", L"AttackRight", doublepoint{ 0,0 }, doublepoint{ 62,42 }, 7, 0.03, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_attack\\left", L"AttackLeft", doublepoint{ 0,0 }, doublepoint{ 62,42 }, 7, 0.03, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_jump\\right", L"JumpRight", doublepoint{ 0,0 }, doublepoint{ 32,42 }, 4, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_jump\\left", L"JumpLeft", doublepoint{ 0,0 }, doublepoint{ 32,42 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_fall\\right", L"FallRight", doublepoint{ 0,0 }, doublepoint{ 42,48 }, 4, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_fall\\left", L"FallLeft", doublepoint{ 0,0 }, doublepoint{ 42,48 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_land\\right", L"LandRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 5, 0.07, false, doublepoint{ 0,-5 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_land\\left", L"LandLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 5, 0.07, false, doublepoint{ 0,-5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_precrouch\\right", L"PreCrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false, doublepoint{ 0,-5 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_precrouch\\left", L"PreCrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false, doublepoint{ 0,-5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_postcrouch\\right", L"PostCrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false, doublepoint{ 0,-5 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_postcrouch\\left", L"PostCrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false, doublepoint{ 0,-5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_crouch\\right", L"CrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 1, 0.07, true, doublepoint{ 0,-5 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_crouch\\left", L"CrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 1, 0.07, true, doublepoint{ 0,-5 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_wallslide\\right", L"WallGrabRight", doublepoint{ 0,0 }, doublepoint{ 46,42 }, 1, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_wallslide\\left", L"WallGrabLeft", doublepoint{ 0,0 }, doublepoint{ 46,42 }, 1, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_roll\\right", L"RollRight", doublepoint{ 0,0 }, doublepoint{ 48,33 }, 7, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_roll\\left", L"RollLeft", doublepoint{ 0,0 }, doublepoint{ 48,33 }, 7, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_player_flip\\right", L"FlipRight", doublepoint{ 0,0 }, doublepoint{ 50,45 }, 11, 0.02, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_player_flip\\left", L"FlipLeft", doublepoint{ 0,0 }, doublepoint{ 50,45 }, 11, 0.02, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_doorbreak\\right", L"DoorBreakRight", doublepoint{ 0,0 }, doublepoint{ 50,44 }, 6, 0.07, false
		, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_doorbreak\\left", L"DoorBreakLeft", doublepoint{ 0,0 }, doublepoint{ 50,44 }, 6, 0.07, false
		, doublepoint{ 0,-10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtfly_begin\\right", L"HurtFlyBeginRight", doublepoint{ 0,0 }, doublepoint{ 50,43 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtfly_begin\\left", L"HurtFlyBeginLeft", doublepoint{ 0,0 }, doublepoint{ 50,43 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtfly_loop\\right", L"HurtFlyLoopRight", doublepoint{ 0,0 }, doublepoint{ 50,43 }, 4, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtfly_loop\\left", L"HurtFlyLoopLeft", doublepoint{ 0,0 }, doublepoint{ 50,43 }, 4, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtground\\right", L"HurtGroundRight", doublepoint{ 0,0 }, doublepoint{ 57,25 }, 6, 0.07, false, doublepoint{ 0,scaleA * 10 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_hurtground\\left", L"HurtGroundLeft", doublepoint{ 0,0 }, doublepoint{ 57,25 }, 6, 0.07, false, doublepoint{ 0,scaleA * 10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollToRunRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollToRunLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PreCrouchRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"CrouchRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PreCrouchLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"CrouchLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RunToIdleRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RunToIdleLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->m_CompleteEvent = std::bind(&CRigidBody::SetAttack, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetAttack, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"LandRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"LandLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PostCrouchRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PostCrouchLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RollToRunRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->m_CompleteEvent = std::bind(&CRigidBody::SetRoll, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RollToRunLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetRoll, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetFlip, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->m_CompleteEvent = std::bind(&CRigidBody::SetFlip, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"DoorBreakRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"DoorBreakLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	//벽에 붙으면 y축 속도를 죽인다
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"WallGrabLeft")->m_StartEvent = std::bind(&CRigidBody::StartWallGrab, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"WallGrabRight")->m_StartEvent = std::bind(&CRigidBody::StartWallGrab, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]));


	//잔상효과
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->AfterImageOn(PenColor::SKY);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->AfterImageOn(PenColor::SKY);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->AfterImageOn(PenColor::SKY);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->AfterImageOn(PenColor::SKY);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->AfterImageOn(PenColor::SKY);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->AfterImageOn(PenColor::SKY);

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateAnimation(CResourceMgr::Create()->Find<CTexture>(L"PlayerIdle"), L"Idle", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 10, 0.1, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"HurtFlyBeginLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"HurtFlyLoopLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"HurtFlyBeginRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"HurtFlyLoopRight");


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IdleRight");
	LookDirection = Right;

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();



	//=========================
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunRight")->m_CompleteEvent = std::bind(&CPlayer::SetRunDustcloudOption, this, 20, doublepoint{ 2,2 }, doublepoint{ 170,210 }, doublepoint{ 150,200 },  //출발 구름 
			doublepoint{ 0.1,0.5 }, doublepoint{ 0.005,0.01 }, doublepoint{ 0,0 }, doublepoint{ -20,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunLeft")->m_CompleteEvent = std::bind(&CPlayer::SetRunDustcloudOption, this, 20, doublepoint{ 2,2 }, doublepoint{ -30,10 }, doublepoint{ 150,200 },
			doublepoint{ 0.1,0.5 }, doublepoint{ 0.005,0.01 }, doublepoint{ 0,0 }, doublepoint{ -20,0 });

	//==================================

	// 칼
	PlayerSword = new CSword;
	PlayerSword->Owner = this;
	PlayerSword->SetResize(doublepoint{ 1.5,1.5 });
	PlayerSword->SetValid(false);
	PlayerSword->Initialize();
	CEventMgr::Create()->Event_CreateObj(PlayerSword, GROUP_TYPE::PLAYER_PROJECTILE);


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\0", L"SlashRight0", doublepoint{ 0,0 }, doublepoint{ 106,32 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\10", L"SlashRight10", doublepoint{ 0,0 }, doublepoint{ 110,50 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-10", L"SlashRight-10", doublepoint{ 0,0 }, doublepoint{ 110,50 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\20", L"SlashRight20", doublepoint{ 0,0 }, doublepoint{ 118,69 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-20", L"SlashRight-20", doublepoint{ 0,0 }, doublepoint{ 118,69 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\30", L"SlashRight30", doublepoint{ 0,0 }, doublepoint{ 129,89 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-30", L"SlashRight-30", doublepoint{ 0,0 }, doublepoint{ 129,89 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\40", L"SlashRight40", doublepoint{ 0,0 }, doublepoint{ 143,111 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-40", L"SlashRight-40", doublepoint{ 0,0 }, doublepoint{ 143,111 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\50", L"SlashRight50", doublepoint{ 0,0 }, doublepoint{ 161,135 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-50", L"SlashRight-50", doublepoint{ 0,0 }, doublepoint{ 161,135 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\60", L"SlashRight60", doublepoint{ 0,0 }, doublepoint{ 182,161 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-60", L"SlashRight-60", doublepoint{ 0,0 }, doublepoint{ 182,161 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\70", L"SlashRight70", doublepoint{ 0,0 }, doublepoint{ 208,191 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-70", L"SlashRight-70", doublepoint{ 0,0 }, doublepoint{ 208,191 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\80", L"SlashRight80", doublepoint{ 0,0 }, doublepoint{ 239,225 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-80", L"SlashRight-80", doublepoint{ 0,0 }, doublepoint{ 239,225 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\90", L"SlashRight90", doublepoint{ 0,0 }, doublepoint{ 275,264 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\right\\-90", L"SlashRight-90", doublepoint{ 0,0 }, doublepoint{ 275,264 }, 5, 0.04, false);



	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\0", L"SlashLeft0", doublepoint{ 0,0 }, doublepoint{ 106,32 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\10", L"SlashLeft10", doublepoint{ 0,0 }, doublepoint{ 110,50 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-10", L"SlashLeft-10", doublepoint{ 0,0 }, doublepoint{ 110,50 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\20", L"SlashLeft20", doublepoint{ 0,0 }, doublepoint{ 118,69 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-20", L"SlashLeft-20", doublepoint{ 0,0 }, doublepoint{ 118,69 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\30", L"SlashLeft30", doublepoint{ 0,0 }, doublepoint{ 129,89 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-30", L"SlashLeft-30", doublepoint{ 0,0 }, doublepoint{ 129,89 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\40", L"SlashLeft40", doublepoint{ 0,0 }, doublepoint{ 143,111 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-40", L"SlashLeft-40", doublepoint{ 0,0 }, doublepoint{ 143,111 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\50", L"SlashLeft50", doublepoint{ 0,0 }, doublepoint{ 161,135 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-50", L"SlashLeft-50", doublepoint{ 0,0 }, doublepoint{ 161,135 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\60", L"SlashLeft60", doublepoint{ 0,0 }, doublepoint{ 182,161 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-60", L"SlashLeft-60", doublepoint{ 0,0 }, doublepoint{ 182,161 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\70", L"SlashLeft70", doublepoint{ 0,0 }, doublepoint{ 208,191 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-70", L"SlashLeft-70", doublepoint{ 0,0 }, doublepoint{ 208,191 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\80", L"SlashLeft80", doublepoint{ 0,0 }, doublepoint{ 239,225 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-80", L"SlashLeft-80", doublepoint{ 0,0 }, doublepoint{ 239,225 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\90", L"SlashLeft90", doublepoint{ 0,0 }, doublepoint{ 275,264 }, 5, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->CreateSpriteAndAnimation(L"Player\\spr_slash\\left\\-90", L"SlashLeft-90", doublepoint{ 0,0 }, doublepoint{ 275,264 }, 5, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-90")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-80")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-70")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-60")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-50")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-40")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-30")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-20")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight-10")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight0")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight90")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight80")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight70")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight60")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight50")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight40")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight30")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight20")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashRight10")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-90")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-80")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-70")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-60")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-50")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-40")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-30")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-20")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft-10")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft0")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft90")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft80")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft70")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft60")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft50")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft40")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft30")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft20")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->FindAnimation(L"SlashLeft10")->m_CompleteEvent = std::bind(&CAnimator::Dont_Play_Anything, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1]));


	CAnimal::Initialize();
}

void CPlayer::Enter()
{
	CAnimal::Enter();

	RollDustcloud->Enter();
	RunDustcloud->Enter();
	WallDustcloud->Enter();
}

void CPlayer::Exit()
{
}

void CPlayer::GetInput()
{
	doublepoint& velocity = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity();
	doublepoint& force = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetForce();

	bool& OnGround = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround();
	bool& Run = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun();
	bool& Walk = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk();
	int& OnStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair();
	int& OnWall = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnWall();
	int& OnDoor = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnDoor();
	int& WallGrab = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWallGrab();
	int& Roll = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRoll();
	int& Flip = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetFlip();
	bool& AttackOnOff = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttack();
	double& AttackAngle = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttackAngle();
	bool& Hurt = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt();


	Run = false;
	Walk = false;
	

	if (OnGround)
		OnStair = 0;

	if (OnGround || OnStair)
		AttackOnAir = false;

	if (AttackOnOff == true)
	{
		WallGrab = false;
		OnGround = false;
		OnStair = 0;
		Roll = 0;
		Flip = 0;
		return;
	}

	if (AttackTimerSwitch == true)
	{
		AttackTimer += TimeMgr::Create()->dt();
		
		if (AttackTimer > AttackSpeed)
			AttackTimerSwitch = false;
	}
	

	if (KeyMgr::Create()->key(Key::M).pressed)
	{
		int a = 0;
	}

	if (Animator0->GetCurAnimation()->GetName() == L"DoorBreakRight"            // 문부수는 모션은 취소불가
		|| Animator0->GetCurAnimation()->GetName() == L"DoorBreakLeft")
		return;

	//=========================================================================================
//여기에 피격 애니메이션 로직 설정
	//=========================================================================================

	if (Stunned)
	{
		StunTimer -= TimeMgr::Create()->dt();
		KeyMgr::Create()->DontGetInput();

		if (StunTimer < 0)
		{
			StunTimer = StunTime;
			Stunned = false;
		}

		
	}

	if (MainOrder == Main_Order::GetBurn)
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->BurnAnimation();
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().y = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetGravity(false);

		PlayerDead = true;
		burn = true;


		SetBloodEmitor(false);
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

		return;
		

	}

	if (MainOrder == Main_Order::Dead)
	{
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().y = 0;
		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetGravity(false);

		PlayerDead = true;
		return;
	}

	if ((OnGround || OnStair) && Animator0->GetCurAnimation()->GetName() == L"HurtFlyLoopRight")
		Animator0->StartPlaying(L"HurtGroundRight");

	if ((OnGround || OnStair) && Animator0->GetCurAnimation()->GetName() == L"HurtFlyLoopLeft")
		Animator0->StartPlaying(L"HurtGroundLeft");

	if (Hurt  && PlayerDead == false) //피격시 
	{
		PlayerDead = true;
		//피격 애니메이션 실행
		
		if (cos(HurtAngle) > 0)
			Animator0->StartPlaying(L"HurtFlyBeginRight");

		else
			Animator0->StartPlaying(L"HurtFlyBeginLeft");

		//피격 물리효과 실행
		velocity.x = scaleA * 1000 * cos(HurtAngle);
		velocity.y = scaleA * 1000 * sin(HurtAngle);
		OnGround = false;
		OnStair = 0;

		LookDirection = 0;
		PlayerDead = true;

		return;
	}

	if (PlayerDead)      // 죽었으면 입력 받지 않는다
		return;







//=========================================================================================
	//착지
	//=========================================================================================

	if ((OnGround == true || OnStair)) //PrevOnFloor == false && 
	{

		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetJump(false);

		if (LookDirection == Right)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FallRight"
				|| Animator0->GetCurAnimation()->GetName() == L"JumpRight"
				|| Animator0->GetCurAnimation()->GetName() == L"WallGrabRight")
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				WallGrab = 0;

				//착지 이펙트 필요


				CEffectMgr::Create()->LandCloud.Play(Pos + doublepoint{ 0,25 * scaleA }, Resize* scaleA * 1.3);
				
			}

		}

		if (LookDirection == Left)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FallLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"JumpLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"WallGrabLeft")
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				WallGrab = 0;

				//착지 이펙트 필요
				CEffectMgr::Create()->LandCloud.Play(Pos + doublepoint{ 0,25 * scaleA }, Resize* scaleA * 1.3);

			}
		}
	}

	//=========================================================================================
//
	//=========================================================================================


	if (KeyMgr::Create()->key(Key::A).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"RunLeft"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunLeft")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}



	}

	if (KeyMgr::Create()->key(Key::D).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"RunRight"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunRight")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

	}

	if (KeyMgr::Create()->key(Key::A).pressed == false && KeyMgr::Create()->key(Key::D).pressed == false)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"RunLeft"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunLeft")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

		if (Animator0->GetCurAnimation()->GetName() == L"RunRight"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunRight")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}
	}

	if (KeyMgr::Create()->key(Key::S).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"PreCrouchRight"
			|| Animator0->GetCurAnimation()->GetName() == L"CrouchRight")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"PostCrouchRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

		if (Animator0->GetCurAnimation()->GetName() == L"PreCrouchLeft"
			|| Animator0->GetCurAnimation()->GetName() == L"CrouchLeft")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"PostCrouchLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

	}

	//=========================================================================================
//좌측이동
	//=========================================================================================

	if (KeyMgr::Create()->key(Key::A).pressed == true)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight" //롤링 중인경우 입력 받지 않는다
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight" //플립 중일경우 입력 받지 않는다
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabRight")
		{
			LookDirection = Left;

			if (Animator0->GetCurAnimation()->GetName() == L"JumpRight")
			{
				int i = Animator0->GetCurAnimation()->GetCurIndex();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"JumpLeft");
				Animator0->GetCurAnimation()->GetCurIndex() = i;
			}

			if (Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
				&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
			{
				force.x = -scaleA * 100000;
				Run = true;

				//m_Velocity.y = -m_OnStair * m_Velocity.x
			}
				

			if (OnGround == true || OnStair)
			{
				if (OnWall == 0)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"IdleToRunLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"RollToRunLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"RunLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackRight"	)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"IdleToRunLeft");


					if (Animator0->GetCurAnimation()->GetName() == L"RunLeft")
					{
						force.x -= scaleA * 250000;
						Run = true;
					}
					
					if (OnDoor == Right)
					{
						SmashDoor();
					}
				}


			}

			if (OnWall == 1)
			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() == L"RunLeft"
						|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunLeft")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RunToIdleLeft");
				}

				else
				{
					if (Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft" && CantClimb == false)
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabLeft");
						WallGrab = 1;
					}

				}
			}
		}





	}


	if (KeyMgr::Create()->key(Key::D).pressed == true)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight"// 롤링 중인 경우 입력받지 않는다.
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight"//플립 중일 경우 입력받지 않는다.
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabRight"
			)
		{
			LookDirection = Right;

			if (Animator0->GetCurAnimation()->GetName() == L"JumpLeft")
			{
				int i = Animator0->GetCurAnimation()->GetCurIndex();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"JumpRight");
				Animator0->GetCurAnimation()->GetCurIndex() = i;
			}
				

			if (Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
				&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
			{
				force.x = scaleA * 100000;
				Run = true;
							
			}
				

			if (OnStair || OnGround == true)
			{
				if (OnWall == 0)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"IdleToRunRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RollToRunRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RunRight"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackRight"						)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"IdleToRunRight");


					if (Animator0->GetCurAnimation()->GetName() == L"RunRight")
					{
						force.x += scaleA * 250000;
						Run = true;
					}

					if (OnDoor == Left)
					{
						SmashDoor();
					}
						
				}


			}

			if (OnWall == -1)
			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() == L"RunRight"
						|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunRight")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RunToIdleRight");
				}

				else
				{
					if (Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft" && CantClimb == false)
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabRight");
						WallGrab = -1;
					}

				}
			}
		}




	}

	if (KeyMgr::Create()->key(Key::S).pressed == true)
	{
		if (OnGround == false && OnStair == 0)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FlipLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"FlipRight")
			{
				if (LookDirection == Right)
				{
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallRight");
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				}

				if (LookDirection == Left)
				{
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallLeft");
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				}
			}

			force.y += scaleA * 100000;
		}



		if (KeyMgr::Create()->key(Key::A).pressed == false  // S만 눌렸을 경우 
			&& KeyMgr::Create()->key(Key::D).pressed == false
			&& KeyMgr::Create()->key(Key::W).pressed == false)
		{
			if (OnGround == true || OnStair)
			{
				if (Animator0->GetCurAnimation()->GetName() != L"PreCrouchLeft"
					&& Animator0->GetCurAnimation()->GetName() != L"CrouchLeft"
					&& Animator0->GetCurAnimation()->GetName() != L"CrouchRight"
					&& Animator0->GetCurAnimation()->GetName() != L"PreCrouchRight"
					&& Animator0->GetCurAnimation()->GetName() != L"RollRight"
					&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
				{
					if (LookDirection == Right)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PreCrouchRight");

					else if (LookDirection == Left)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PreCrouchLeft");

					BreakGround();
				}

			}
		}


		else
		{


			if ((KeyMgr::Create()->key(Key::S).key_state == KeyState::TAP
				&& KeyMgr::Create()->key(Key::D).pressed)
				||
				(KeyMgr::Create()->key(Key::D).key_state == KeyState::TAP
					&& KeyMgr::Create()->key(Key::S).pressed))

			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RollRight");
						Roll = 1;

						SetRollDustcloudOption(doublepoint{ scaleA * 1.5,scaleA * 1.5 },doublepoint{ 170,240 }, doublepoint{ scaleA * 70,scaleA * 100 },
							doublepoint{ 0.5,0.7 }, doublepoint{ 0.001,0.002 }, doublepoint{ 0,0 }, doublepoint{ -scaleA * 20,0 });
					}
				}

				

			}

			if ((KeyMgr::Create()->key(Key::S).key_state == KeyState::TAP
				&& KeyMgr::Create()->key(Key::A).pressed)
				||
				(KeyMgr::Create()->key(Key::A).key_state == KeyState::TAP
					&& KeyMgr::Create()->key(Key::S).pressed))

			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RollLeft");
						Roll = -1;

						SetRollDustcloudOption(doublepoint{ scaleA * 1.5,scaleA * 1.5 }, doublepoint{ -60,10 }, doublepoint{ scaleA * 70,scaleA * 100 },
							doublepoint{ 0.5,0.7 }, doublepoint{ 0.001,0.002 }, doublepoint{ 0,0 }, doublepoint{ -scaleA * 20,0 });
					}
				}

			}
		}

	}

	if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			if (LookDirection == Right)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpRight");

			if (LookDirection == Left)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpLeft");

			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();


			velocity.y = JumpVelocity;

			State = Object_State::JUMP;
			dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetJump(true);

			OnGround = false;
			OnStair = 0;

			//점프 이펙트 필요
			CEffectMgr::Create()->JumpCloud.Play(Pos + doublepoint{ 0,0 }, Resize* scaleA * 1.3);


		}



	}


	if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::TAP)// && AttackTimer > AttackSpeed)
	{


		//마우스 커서 포지션 받아와서
		doublepoint mousepos = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::CURSOR)[0]->GetPos();

		//실제좌표 계산
		mousepos = CCameraMgr::Create()->RealCoordinate(mousepos);

		//캐릭터 포지션이랑 비교해서 어택앵글 계산
		AttackAngle = atan2(mousepos.y - Pos.y, mousepos.x - Pos.x);

		//
		AttackOnOff = true;
		WallGrab = false;
		OnGround = false;
		OnStair = 0;
		Roll = 0;
		Flip = 0;


		//
		if (mousepos.x >= Pos.x)
			LookDirection = Right;

		else
			LookDirection = Left;

		
		
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

		if (LookDirection == Right)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"AttackRight");

		if (LookDirection == Left)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"AttackLeft");

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

		velocity.x = scaleA * 600 * 0.8* cos(AttackAngle);
		velocity.y = scaleA * 600 * sin(AttackAngle);

		if (AttackOnAir && velocity.y < 0)
			velocity.y = 0;

		AttackOnAir = true;
		AttackTimer = 0;
		AttackTimerSwitch = true;

		SlashSword(AttackAngle);

		//
		return;
		


	}

	if (KeyMgr::Create()->key(RIGHTMOUSE).key_state == KeyState::TAP)
	{
		//마우스 커서 포지션 받아와서
		doublepoint mousepos = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::CURSOR)[0]->GetPos();

		//실제좌표 계산
		mousepos = CCameraMgr::Create()->RealCoordinate(mousepos);

		//캐릭터 포지션이랑 비교해서 어택앵글 계산
		AttackAngle = atan2(mousepos.y - Pos.y, mousepos.x - Pos.x);

		doublepoint Vel;
		Vel.x = 2000 * cos(AttackAngle);
		Vel.y = 2000 * sin(AttackAngle);

		if (Item)
			Item->Throw(Pos, Vel);
	}

	if (OnGround == false && OnStair == 0 && WallGrab == 0)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"JumpRight"
			&& Animator0->GetCurAnimation()->GetName() != L"JumpLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"AttackRight"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight"
			&& Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			)
		{
			if (LookDirection == Right)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}

			if (LookDirection == Left)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}
		}

	}

	

	if (WallGrab == Right)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			WallGrab = 0;

			//착지 이펙트 필요
			CEffectMgr::Create()->LandCloud.Play(Pos + doublepoint{ 0,25 * scaleA }, Resize* scaleA * 1.3);

			
		}

		else
		{
			if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
			{
				LookDirection = Right;
				Flip = Right;
				WallGrab = 0;
				OnWall = 0;

				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FlipRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				CEffectMgr::Create()->JumpCloudRight.Play(Pos + doublepoint{ 20,0 }, Resize* scaleA * 1.3);
				
			}

if (KeyMgr::Create()->key(Key::D).pressed)
{
	if (KeyMgr::Create()->key(Key::D).key_state == KeyState::TAP)
		TimeMgr::Create()->StartStopWatch(0.05);

	if (TimeMgr::Create()->Timeover())
	{
		WallGrab = 0;

	}
}

if (KeyMgr::Create()->key(Key::D).key_state == KeyState::RELEASE)
TimeMgr::Create()->EndStopWatch();

		}
	}

	if (WallGrab == Left)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			WallGrab = 0;

			//착지 이펙트 필요
			CEffectMgr::Create()->LandCloud.Play(Pos + doublepoint{ 0,25 * scaleA }, Resize* scaleA * 1.3);

		}

		else
		{
			if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
			{
				LookDirection = Left;
				Flip = Left;
				WallGrab = 0;
				OnWall = 0;

				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FlipLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				CEffectMgr::Create()->JumpCloudLeft.Play(Pos + doublepoint{ -20,0 }, Resize* scaleA * 1.3);
				
			}

			if (KeyMgr::Create()->key(Key::A).pressed)
			{
				if (KeyMgr::Create()->key(Key::A).key_state == KeyState::TAP)
					TimeMgr::Create()->StartStopWatch(0.05);

				if (TimeMgr::Create()->Timeover())
				{

					WallGrab = 0;

				}
			}

			if (KeyMgr::Create()->key(Key::A).key_state == KeyState::RELEASE)
				TimeMgr::Create()->EndStopWatch();
		}
	}

	if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
		&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
	{		
		Roll = 0;
	}


	if (Animator0->GetCurAnimation()->GetName() != L"FlipRight"
		&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft")
	{
		
		Flip = 0;
	}


	if (Animator0->GetCurAnimation()->GetName() == L"FlipRight")
	{
		if (OnWall != 0)
		{


			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabRight");
			WallGrab = -1;
		}
	}

	if (Animator0->GetCurAnimation()->GetName() == L"FlipLeft")
	{
		if (OnWall != 0)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabLeft");
			WallGrab = 1;
		}
	}

	Rolling = Roll; //롤링중에는 총알, 레이저 등을 적용받지 않기위해 롤링값을 넣어준다.
}

void CPlayer::ManageEffector()
{
	int& WallGrab = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWallGrab();
	doublepoint& velocity = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity();

	if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
		&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
	{
		SetRollDustcloud(false);
	}


	if (WallGrab == Right && velocity.y > scaleA * 50)
	{
		SetWallDustcloudOption(doublepoint{ 2,2 }, doublepoint{ -30,30 }, doublepoint{ 70,100 },
			doublepoint{ 0.3,1 }, doublepoint{ 0.02,0.05 }, doublepoint{ -5,0 }, doublepoint{ 0,0 });
	}

	else if (WallGrab == Right && velocity.y < -scaleA * 50)
	{
		SetWallDustcloudOption(doublepoint{ 2,2 }, doublepoint{ -30,30 }, doublepoint{ 70,100 },
			doublepoint{ 0.3,1 }, doublepoint{ 0.02,0.05 }, doublepoint{ -5,0 }, doublepoint{ 0,0 });
	}

	else if (WallGrab == Left && velocity.y > scaleA * 50)
	{
		SetWallDustcloudOption(doublepoint{ 2,2 }, doublepoint{ 150,210 }, doublepoint{ 70,100 },
			doublepoint{ 0.5,0.7 }, doublepoint{ 0.02,0.05 }, doublepoint{ -5,0 }, doublepoint{ 0,0 });
	}

	else if (WallGrab == Left && velocity.y < -scaleA * 50)
	{
		SetWallDustcloudOption(doublepoint{ 2,2 }, doublepoint{ 150,210 }, doublepoint{ 70,100 },
			doublepoint{ 0.5,0.7 }, doublepoint{ 0.02,0.05 }, doublepoint{ -5,0 }, doublepoint{ 0,0 });
	}

	else
		SetWallDustcloud(false);


}



void CPlayer::Update()
{	
	bool& OnGround = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround();
	int& OnStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair();


	if (MP > 0 && KeyMgr::Create()->key(LSHIFT).pressed)
	{
		SlowOn();
	}

	else
		SlowOff();

	GetInput();
	ManageEffector();

	

	PrevPos = Pos;


	//PrevOnFloor = OnGround || OnStair;


	// 공중에 떠있을 경우 recentfloor 추정
	doublepoint playerpos = {};
	double min = 100000;

	playerpos.y = ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetAbsPos().y
		+ ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetScale().y / 2 - 10; // 10만큼 보정

	playerpos.x = ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetAbsPos().x;


	if (OnGround == false && OnStair == 0)
	{


		for (int i = 0; i < CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR).size(); ++i)
		{
			CFloor* CF = dynamic_cast<CFloor*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);
			CStair* CS = dynamic_cast<CStair*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);

			if (CS)
			{
				double y = CS->GetPos().y + (CS->GetScale().y * CS->GetResize().y) / 2;
				double x1 = CS->GetPos().x - (CS->GetScale().x * CS->GetResize().x) / 2;
				double x2 = CS->GetPos().x + (CS->GetScale().x * CS->GetResize().x) / 2;

				if (y - playerpos.y > 0 && y - playerpos.y < min && playerpos.x > x1 && playerpos.x < x2)
					RecentFloor = CS;

			}

			else if (CF)
			{
				double y = CF->GetPos().y - (CF->GetScale().y * CF->GetResize().y) / 2;
				double x1 = CF->GetPos().x - (CF->GetScale().x * CF->GetResize().x) / 2;
				double x2 = CF->GetPos().x + (CF->GetScale().x * CF->GetResize().x) / 2;

				if (y - playerpos.y > 0 && y - playerpos.y < min && playerpos.x > x1 && playerpos.x < x2)
					RecentFloor = CF;
			}

		}
	}

	// 무적 판정 체크

	if (Animator0->GetCurAnimation()->GetName() == L"RollRight"
		|| Animator0->GetCurAnimation()->GetName() == L"RollLeft"
		|| Animator0->GetCurAnimation()->GetName() == L"FlipRight"
		|| Animator0->GetCurAnimation()->GetName() == L"FlipLeft")
	{
		Unbeatable = true;
	}
	
	else
	{
		Unbeatable = false;
	}


	// 오를수 있는 벽 판정 초기화
	CantClimb = false;

	CAnimal::Update();


	
		
}

void CPlayer::Render(HDC _dc)
{
	doublepoint CameraPos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint CameraScale = CCameraMgr::Create()->CameraScale(Scale);

	CObject::Render(_dc);

	
}

void CPlayer::SlashSword(double _Angle)
{
	
	PlayerSword->SetPos(Pos);
	PlayerSword->OffSet = doublepoint{ 0,0 };
	PlayerSword->SetAngle(_Angle);
	PlayerSword->SetResize(doublepoint{ 1.5,1.5 });
	PlayerSword->SetValid(true);
	PlayerSword->Timer = 0.2;

	if (cos(_Angle) > 0)
	{
		double a = _Angle / M_PI * 180;
		double s = sin(_Angle);
		double asf = sin((double)-5 / 180 * M_PI);
		double asfs = sin((double)5 / 180 * M_PI);

		if(sin(_Angle)>= sin((double)-5/180*M_PI) && sin(_Angle) < sin((double)5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight0");

		else if (sin(_Angle) >= sin((double)-15 / 180 * M_PI) && sin(_Angle) < sin((double)-5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-10");

		else if (sin(_Angle) >= sin((double)-25 / 180 * M_PI) && sin(_Angle) < sin((double)-15 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-20");

		else if (sin(_Angle) >= sin((double)-35 / 180 * M_PI) && sin(_Angle) < sin((double)-25 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-30");

		else if (sin(_Angle) >= sin((double)-45 / 180 * M_PI) && sin(_Angle) < sin((double)-35 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-40");

		else if (sin(_Angle) >= sin((double)-55 / 180 * M_PI) && sin(_Angle) < sin((double)-45 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-50");

		else if (sin(_Angle) >= sin((double)-65 / 180 * M_PI) && sin(_Angle) < sin((double)-55 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-60");

		else if (sin(_Angle) >= sin((double)-75 / 180 * M_PI) && sin(_Angle) < sin((double)-65 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-70");

		else if (sin(_Angle) >= sin((double)-85 / 180 * M_PI) && sin(_Angle) < sin((double)-75 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-80");

		else if (sin(_Angle) >= sin((double)-90 / 180 * M_PI) && sin(_Angle) < sin((double)-85 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight-90");


		else if (sin(_Angle) < sin((double)15 / 180 * M_PI) && sin(_Angle) >= sin((double)5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight10");
		
		else if (sin(_Angle) < sin((double)25 / 180 * M_PI) && sin(_Angle) >= sin((double)15 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight20");
		
		else if (sin(_Angle) < sin((double)35 / 180 * M_PI) && sin(_Angle) >= sin((double)25 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight30");
		
		else if (sin(_Angle) < sin((double)45 / 180 * M_PI) && sin(_Angle) >= sin((double)35 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight40");

		else if (sin(_Angle) < sin((double)55 / 180 * M_PI) && sin(_Angle) >= sin((double)45 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight50");

		else if (sin(_Angle) < sin((double)65 / 180 * M_PI) && sin(_Angle) >= sin((double)55 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight60");

		else if (sin(_Angle) < sin((double)75 / 180 * M_PI) && sin(_Angle) >= sin((double)65 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight70");

		else if (sin(_Angle) < sin((double)85 / 180 * M_PI) && sin(_Angle) >= sin((double)75 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight80");

		else if (sin(_Angle) <= sin((double)90 / 180 * M_PI) && sin(_Angle) >= sin((double)85 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashRight90");

		
	}
		

	else
	{
		double a = sin(_Angle);

		

		if (sin(_Angle) >= sin((double)-5 / 180 * M_PI) && sin(_Angle) < sin((double)5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft0");

		else if (sin(_Angle) >= sin((double)-15 / 180 * M_PI) && sin(_Angle) < sin((double)-5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-10");

		else if (sin(_Angle) >= sin((double)-25 / 180 * M_PI) && sin(_Angle) < sin((double)-15 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-20");

		else if (sin(_Angle) >= sin((double)-35 / 180 * M_PI) && sin(_Angle) < sin((double)-25 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-30");

		else if (sin(_Angle) >= sin((double)-45 / 180 * M_PI) && sin(_Angle) < sin((double)-35 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-40");

		else if (sin(_Angle) >= sin((double)-55 / 180 * M_PI) && sin(_Angle) < sin((double)-45 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-50");

		else if (sin(_Angle) >= sin((double)-65 / 180 * M_PI) && sin(_Angle) < sin((double)-55 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-60");

		else if (sin(_Angle) >= sin((double)-75 / 180 * M_PI) && sin(_Angle) < sin((double)-65 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-70");

		else if (sin(_Angle) >= sin((double)-85 / 180 * M_PI) && sin(_Angle) < sin((double)-75 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-80");

		else if (sin(_Angle) >= sin((double)-90 / 180 * M_PI) && sin(_Angle) < sin((double)-85 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft-90");


		else if (sin(_Angle) < sin((double)15 / 180 * M_PI) && sin(_Angle) >= sin((double)5 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft10");

		else if (sin(_Angle) < sin((double)25 / 180 * M_PI) && sin(_Angle) >= sin((double)15 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft20");

		else if (sin(_Angle) < sin((double)35 / 180 * M_PI) && sin(_Angle) >= sin((double)25 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft30");

		else if (sin(_Angle) < sin((double)45 / 180 * M_PI) && sin(_Angle) >= sin((double)35 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft40");

		else if (sin(_Angle) < sin((double)55 / 180 * M_PI) && sin(_Angle) >= sin((double)45 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft50");

		else if (sin(_Angle) < sin((double)65 / 180 * M_PI) && sin(_Angle) >= sin((double)55 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft60");

		else if (sin(_Angle) < sin((double)75 / 180 * M_PI) && sin(_Angle) >= sin((double)65 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft70");

		else if (sin(_Angle) < sin((double)85 / 180 * M_PI) && sin(_Angle) >= sin((double)75 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft80");

		else if (sin(_Angle) <= sin((double)90 / 180 * M_PI) && sin(_Angle) >= sin((double)85 / 180 * M_PI))
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->StartPlaying(L"SlashLeft90");

		else
		{
			int a = 0;
		}
	}
		
}

void CPlayer::BreakGround()
{
	CGroundBreaker* m = new CGroundBreaker;
	m->Owner = this;
	m->SetResize(doublepoint{ 1.5,1.5 });
	m->Initialize();

	CEventMgr::Create()->Event_CreateObj(m, GROUP_TYPE::PLAYER_PROJECTILE);
}

void CPlayer::SmashDoor()
{
	if (LookDirection == Right && Animator0->GetCurAnimation()->GetName() != L"DoorBreakRight")
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"DoorBreakRight");

		CDoorBreaker* DB = new CDoorBreaker;
		DB->Owner = this;
		DB->SetDir(LookDirection);
		DB->Initialize();
		CEventMgr::Create()->Event_CreateObj(DB, GROUP_TYPE::PLAYER_PROJECTILE);

		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = -LookDirection * 300;
	}
		

	else if (LookDirection == Left && Animator0->GetCurAnimation()->GetName() != L"DoorBreakLeft")
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"DoorBreakLeft");

		CDoorBreaker* DB = new CDoorBreaker;
		DB->Owner = this;
		DB->SetDir(LookDirection);
		DB->Initialize();
		CEventMgr::Create()->Event_CreateObj(DB, GROUP_TYPE::PLAYER_PROJECTILE);

		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity().x = -LookDirection * 300;
	}
		

	

}

void CPlayer::SlowOn()
{
	
		TimeMgr::Create()->GetTimeScale() -= TimeMgr::Create()->realdt();
		if (TimeMgr::Create()->GetTimeScale() < 0.5)
			TimeMgr::Create()->GetTimeScale() = 0.5;

		CCameraMgr::Create()->SetCameraRenderMode(CameraRenderMode::SubtleFadeOut);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRender(true);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->SetReRender(true);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->SetReRenderColor(PenColor::YELLOW);
	
}

void CPlayer::SlowOff()
{
	TimeMgr::Create()->GetTimeScale() = 1;
	CCameraMgr::Create()->SetCameraRenderMode(CameraRenderMode::SubtleFadeIn);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRender(false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])->SetReRender(false);
}
