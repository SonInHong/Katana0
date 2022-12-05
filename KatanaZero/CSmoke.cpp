#include "pch.h"
#include "CSmoke.h"
#include "TimeMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CResourceMgr.h"
#include "CCameraMgr.h"
#include "CArchitecture.h"
#include "CDoor.h"
#include "CTopFloor.h"
#include "CSmokeValve.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CAnimal.h"

CSmoke::CSmoke()
	:Accelation{}
	,State(Smoke_State::Emitting)
{
	Texture = CResourceMgr::Create()->Load<CTexture>(L"SmokeValve\\SmokeOff.bmp");
}

CSmoke::~CSmoke()
{
}

void CSmoke::Initialize()
{
	AirFriction = 0.01;
	Scale.x = 70;
	Scale.y = 70;

	AnimationScaling.x = 2;
	AnimationScaling.y = 2;

	CreateAnimator();
	CreateCollider();

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->CreateSpriteAndAnimation(L"Smoke\\spr_smoke_appear\\Emiting", L"SmokeEmiting", doublepoint{ 0,0 }, doublepoint{ 28,28 }, 1, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->CreateSpriteAndAnimation(L"Smoke\\spr_smoke_appear\\Emitted", L"SmokeEmitted", doublepoint{ 0,0 }, doublepoint{ 28,28 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->CreateSpriteAndAnimation(L"Smoke\\spr_smoke_dissolve", L"SmokeDissolve", doublepoint{ 0,0 }, doublepoint{ 28,28 }, 7, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->CreateSpriteAndAnimation(L"Smoke\\spr_smoke_loop", L"SmokeLoop", doublepoint{ 0,0 }, doublepoint{ 28,28 }, 6, 0.1, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"SmokeEmitted")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"SmokeLoop");
}

void CSmoke::Update()
{
	if (!Valid)
		return;

	if (Velocity.Norm() < 100)
	{
		State = Smoke_State::Emitted;
		Velocity = doublepoint{ 0,0 };
	}

	Pos.x += Velocity.x * TimeMgr::Create()->dt();
	Pos.y += Velocity.y * TimeMgr::Create()->dt();

	doublepoint Accel = Velocity * (-AirFriction*Velocity.Norm());
	Accel.y += Gravity;

	Velocity = Velocity + Accel*TimeMgr::Create()->dt();

	switch (State)
	{
	case Smoke_State::Emitting:
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"SmokeEmiting");
		break;

	case Smoke_State::Emitted:
	{
		if(dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName()!=L"SmokeLoop")
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"SmokeEmitted");
	}
		
		break;
	}
}

void CSmoke::Render(HDC _dc)
{

	if (!Valid)
		return;

	doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
	
	CObject::Render(_dc);
}

bool CSmoke::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
	if (arc)
	{
		CDoor* d = dynamic_cast<CDoor*>(arc);
		if (d)
		{
			if (d->GetOpen())
			{
			}

			else
				Velocity = doublepoint{ 0,0 };
		}

		else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
		{

		}

		else if (dynamic_cast<CSmokeValve*>(arc))
		{

		}

		else
			Velocity = doublepoint{ 0,0 };
	}

	if (CAnimal* p = dynamic_cast<CAnimal*>(other))
	{
		p->SetInSmoke(true);
	}

	return false;
}

bool CSmoke::Colliding(CObject* other)
{
	if (!Valid)
		return false;

	if (CAnimal* p = dynamic_cast<CAnimal*>(other))
	{
		p->SetInSmoke(true);
	}


	return false;
}

bool CSmoke::CollideEscape(CObject* other)
{
	if (!Valid)
		return false;

	return false;
}

bool CSmoke::DontCollide(CObject* other)
{
	if (!Valid)
		return false;

	return false;
}
