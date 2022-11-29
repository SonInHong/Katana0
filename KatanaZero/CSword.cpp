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

	Scale.x = 30;
	Scale.y = 30;

	Speed = 250;
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
	if (Valid)
	{
		//if (OffSet.Norm() > 50 || OffSet.Norm() < -50)
			//Direction = -1;

		if (Timer < 0)
		{
			Valid = false;
			OffSet = doublepoint{ 0,0 };
			Pos.x = Owner->GetPos().x + OffSet.x;
			Pos.y = Owner->GetPos().y + OffSet.y;

			Timer = 1;

			return;
		}
			

		OffSet.x = cos(Angle) * 50;  //Speed //* TimeMgr::Create()->dt();
		OffSet.y = sin(Angle) * 50; //Speed //* TimeMgr::Create()->dt();

		Timer -= TimeMgr::Create()->dt();

		
	}
	
	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;

	
}

void CSword::Render(HDC _dc)
{
	//doublepoint CameraPos = CCameraMgr::Create()->CameraCoordinate(Pos);
	//doublepoint CameraScale = CCameraMgr::Create()->CameraScale(Scale);

	//TransparentBlt(_dc, CameraPos.x - CameraScale.x / 2, CameraPos.y - CameraScale.y / 2, CameraScale.x, CameraScale.y, Texture->GetDC(), 0, 0,
		//Texture->GetWidth(), Texture->GetHeight(), RGB(1, 1, 1));

	CObject::Render(_dc);
}

bool CSword::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CMonster* mon = dynamic_cast<CMonster*>(other);
	
	if (mon && mon->MainOrder != Main_Order::End)
	{
		mon->MainOrder = Main_Order::GetHurt;
		mon->HurtAngle = Angle;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
	}

	CDoor* p = dynamic_cast<CDoor*>(other);

	if (p)
	{
		p->SetOpen(true);
		
	}

	return true;
}
