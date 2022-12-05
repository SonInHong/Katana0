#include "pch.h"
#include "CGangsterGun.h"
#include "TimeMgr.h"
#include "CPlayer.h"
#include "CRigidBody.h"
#include "CEventMgr.h"
#include "CMonster.h"
#include "CSword.h"
#include "CCameraMgr.h"
#include "CBullet.h"

CGangsterGun::CGangsterGun()
	:Owner(nullptr)
	, Speed(0)
	, OffSet{}
	, Direction(0)
	, TimeLimit(0.3)
	, Valid(false)
{
	
}

CGangsterGun::~CGangsterGun()
{
}

void CGangsterGun::Initialize()
{
	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;
	Scale.x = scaleA * 30;
	Scale.y = scaleA * 30;

	Speed = scaleA * 700;

	CreateCollider();
}

void CGangsterGun::Enter()
{
}

void CGangsterGun::Exit()
{
}

void CGangsterGun::Update()
{
	if (TimeLimit < 0.3)
		Valid = true;

	if (TimeLimit < 0)
	{
		CEventMgr::Create()->Event_DestroyObj(this);
		TimeLimit = 10000;
	}


	//¸÷ÀÌ Á×À¸¸é ºñÈ°¼ºÈ­
	if (((CMonster*)Owner)->GetDead())
		Valid = false;


	//OffSet.y += Direction * Speed * TimeMgr::Create()->dt();

	if (abs(OffSet.x) < 50 * scaleA)
		OffSet.x += Direction * Speed * TimeMgr::Create()->dt();

	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;

	TimeLimit -= TimeMgr::Create()->dt();
}

bool CGangsterGun::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CSword* s = dynamic_cast<CSword*>(other);
	if (s)
	{
		if (s->GetValid())
		{
			CPlayer* pl = s->GetOwner();
			if (pl)
			{
				double x = ((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().x;

				pl->StunPlayer();
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().y = -400;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().x = -x;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround() = false;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair() = 0;

				s->GetValid() = false;
			}

			Valid = false;
			
			CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);

			return true;

		}
	}


	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p)
	{
		if (p->GetUnbeatable())
			return false;

		((CRigidBody*)p->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt() = true;

		p->SetHurtAngle(-M_PI_2 + Direction * 0.7 * M_PI_2);

		p->GetSword()->GetValid() = false;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
	}

	return true;
}

bool CGangsterGun::Colliding(CObject* other)
{
	if (!Valid)
		return false;

	CSword* s = dynamic_cast<CSword*>(other);
	if (s)
	{
		if (s->GetValid())
		{
			CPlayer* pl = s->GetOwner();
			if (pl)
			{
				double x = ((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().x;

				pl->StunPlayer();
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().y = -400;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity().x = -x;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround() = false;
				((CRigidBody*)pl->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair() = 0;

				s->GetValid() = false;
			}

			Valid = false;

			CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);

			return true;

		}
	}

	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p)
	{
		if (p->GetUnbeatable())
			return false;

		((CRigidBody*)p->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt() = true;

		p->SetHurtAngle(-M_PI_2 + Direction * 0.7 * M_PI_2);

		p->GetSword()->GetValid() = false;

		CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
	}

	return true;
}
