#include "pch.h"
#include "CSword.h"
#include "CResourceMgr.h"
#include "TimeMgr.h"
#include "CCameraMgr.h"
#include "CEventMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CMonster.h"
#include "CDoor.h"
#include "CCameraMgr.h"
#include "CEffectMgr.h"

CSword::CSword()
	:Texture(nullptr)
	, Owner(nullptr)
	, Speed(0)
	,OffSet{}
	,Direction(1)
	, Angle(0)
{
	CreateCollider();
	//CreateAnimator();

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Effect\\spr_dragon_slash\\right", L"DragonSlashRight", doublepoint{ 0,0 }, doublepoint{ 94,38 }, 5, 0.07, false);

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->FindAnimation(L"DragonSlashRight")->m_CompleteEvent = std::bind(&CEventMgr::Event_DestroyObj, CEventMgr::Create(), this);

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Effect\\spr_dragon_slash\\left", L"DragonSlashLeft", doublepoint{ 0,0 }, doublepoint{ 94,38 }, 5, 0.07, false);

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->FindAnimation(L"DragonSlashLeft")->m_CompleteEvent = std::bind(&CEventMgr::Event_DestroyObj, CEventMgr::Create(), this);

	Scale.x = scaleA * 30;
	Scale.y = scaleA * 30;

	Speed = scaleA * 1000;

	
}

CSword::~CSword()
{
}

void CSword::Initialize()
{


	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;
	
	

	//if(cos(Angle)>0)
	//	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"DragonSlashRight");

	//else if(cos(Angle) < 0)
		//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"DragonSlashLeft");

	
}

void CSword::Update()
{
	if (Timer < 0 || !Valid)
	{
		Valid = false;
		OffSet = doublepoint{ 0,0 };
		Pos.x = Owner->GetPos().x + OffSet.x;
		Pos.y = Owner->GetPos().y + OffSet.y;

		Timer = 1;

		return;
	}

	if (Valid)
	{
		//if (OffSet.Norm() > 50 || OffSet.Norm() < -50)
			//Direction = -1;

		
			

	

		if (OffSet.Norm() < 70 * scaleA)
		{
			OffSet.x += cos(Angle) * Speed * TimeMgr::Create()->dt();
			OffSet.y += sin(Angle) * Speed * TimeMgr::Create()->dt();
		}
			

		Timer -= TimeMgr::Create()->dt();

		
	}
	
	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;

	
}

void CSword::Render(HDC _dc)
{	
	CObject::Render(_dc);
}

bool CSword::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CMonster* mon = dynamic_cast<CMonster*>(other);
	
	if (mon && mon->MainOrder != Main_Order::End && mon->MainOrder != Main_Order::GetHurt)
	{
		mon->MainOrder = Main_Order::GetHurt;
		mon->HurtAngle = Angle;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
		CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
			, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
		, 30000, 2000);
		

		//Valid = false;
		//OffSet = doublepoint{ 0,0 };
		//Pos.x = Owner->GetPos().x + OffSet.x;
		//Pos.y = Owner->GetPos().y + OffSet.y;

		//Timer = 1;

		return true;
	}

	CDoor* p = dynamic_cast<CDoor*>(other);

	if (p)
	{
		if(!p->GetOpen())
			CCameraMgr::Create()->SetCameraEffect(CameraEffect::LITTLESHAKE);

		p->SetOpen(true);
		
	}

	return true;
}

bool CSword::Colliding(CObject* other)
{
	if (!Valid)
		return false;

	CMonster* mon = dynamic_cast<CMonster*>(other);

	if (mon && mon->MainOrder != Main_Order::End && mon->MainOrder != Main_Order::GetHurt)
	{
		mon->MainOrder = Main_Order::GetHurt;
		mon->HurtAngle = Angle;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
		CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
			, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
		, 30000, 2000);

		//Valid = false;
		//OffSet = doublepoint{ 0,0 };
		//Pos.x = Owner->GetPos().x + OffSet.x;
		//Pos.y = Owner->GetPos().y + OffSet.y;

		//Timer = 1;

		return true;
	}

	CDoor* p = dynamic_cast<CDoor*>(other);

	if (p)
	{
		if (!p->GetOpen())
			CCameraMgr::Create()->SetCameraEffect(CameraEffect::LITTLESHAKE);

		p->SetOpen(true);

	}

	return true;
}
