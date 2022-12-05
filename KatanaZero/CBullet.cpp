#include "pch.h"
#include "CBullet.h"
#include "TimeMgr.h"
#include "CArchitecture.h"
#include "CEventMgr.h"
#include "CDoor.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CPlayer.h"
#include "CRigidBody.h"
#include "CSword.h"
#include "CEffectMgr.h"
#include "CMonster.h"

CBullet::CBullet()
	:Velocity{}
	, KillPlayer(true)
	,Valid(false)
{
	Scale.x = 10;
	Scale.y = 10;
	

	//텍스쳐 로딩
	Texture = new CTexture;
	Texture->Load(L"Bullet\\0.bmp");
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	CreateCollider();
}

void CBullet::Enter()
{
}

void CBullet::Exit()
{
}

void CBullet::Update()
{
	if (!Valid)
		return;

	Pos = Pos + Velocity * TimeMgr::Create()->dt();
}

void CBullet::Render(HDC _dc)
{
	if (!Valid)
		return;

	for (int i = 0; i < 17; ++i)
	{
		BLENDFUNCTION func = {};
		func.AlphaFormat = AC_SRC_ALPHA;
		func.BlendFlags = 0;
		func.BlendOp = AC_SRC_OVER;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255- 12*i;

		doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos - Velocity.Normalize()* 2 * i);

		AlphaBlend(_dc, pos.x - 1, pos.y - 1, 2, 2, Texture->GetDC(), 2 * i, 0, 2, 2, func);
	}

	CObject::Render(_dc);
}

bool CBullet::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
	if (arc)
	{
		Valid = false;

		CDoor* d = dynamic_cast<CDoor*>(arc);
		if (d)
		{
			if (d->GetOpen())
				Valid = true;
		}
	}
	


	double Angle = atan2(Velocity.y, Velocity.x);

	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p && KillPlayer)
	{
		if (p->GetUnbeatable())
			return false;

		((CRigidBody*)p->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt() = true;

		p->SetHurtAngle(Angle);
		p->GetSword()->GetValid() = false;
		Valid = false;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);

		CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
			, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
		, 30000, 2000);

	}

	CMonster* mon = dynamic_cast<CMonster*>(other);

	if (mon && mon->GetMainOrder() != Main_Order::End && mon->GetMainOrder() != Main_Order::GetHurt && mon->GetMainOrder() != Main_Order::GetBurn && KillPlayer == false)
	{
		mon->GetMainOrder() = Main_Order::GetHurt;
		mon->SetHurtAngle(Angle);
		Valid = false;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
		CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
			, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
		, 30000, 2000);

		

	}

	CSword* sword = dynamic_cast<CSword*>(other);

	if (sword && KillPlayer && sword->GetValid())
	{
		CEffectMgr::Create()->BulletReflect.Play(Pos - Velocity.Normalize() * 100, doublepoint{ 2,2 });
		
		
		double bulletVel = Velocity.Norm();

		Velocity.x = cos(sword->GetAngle()) * bulletVel;
		Velocity.y = sin(sword->GetAngle()) * bulletVel;
		KillPlayer = false;

		
		CCameraMgr::Create()->SetCameraEffect(CameraEffect::LITTLESHAKE);

	}

	return true;
	
}

bool CBullet::COlliding(CObject* other)
{
	if (!Valid)
		return false;

	CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
	if (arc)
	{
		Valid = false;

		CDoor* d = dynamic_cast<CDoor*>(arc);
		if (d)
		{
			if (d->GetOpen())
				Valid = true;
		}
	}

	return true;
}
