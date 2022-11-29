#include "pch.h"
#include "CGruntFist.h"
#include "TimeMgr.h"
#include "CPlayer.h"
#include "CRigidBody.h"
#include "CEventMgr.h"
#include "CMonster.h"

CGruntFist::CGruntFist()
	:Owner(nullptr)
	, Speed(0)
	, OffSet{}
	, Direction(0)
	, TimeLimit(0.5)
	, Valid(true)
{
	CreateCollider();
}

CGruntFist::~CGruntFist()
{
}

void CGruntFist::Initialize()
{
	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;
	Scale.x = 10;
	Scale.y = 10;

	Speed = 75;

	
}

void CGruntFist::Update()
{
	if (TimeLimit < 0)
	{
		CEventMgr::Create()->Event_DestroyObj(this);
		TimeLimit = 10000;
	}
	

	//¸÷ÀÌ Á×À¸¸é ºñÈ°¼ºÈ­
	if (((CMonster*)Owner)->GetDead())
		Valid = false;

	OffSet.x += Direction * Speed * TimeMgr::Create()->dt();
	//OffSet.y += Direction * Speed * TimeMgr::Create()->dt();

	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;

	TimeLimit -= TimeMgr::Create()->dt();
}

bool CGruntFist::Collide(CObject* other)
{
	if (!Valid)
		return false;

	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p)
	{
		if (p->GetRoll())
			return false;

		((CRigidBody*)p->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetHurt() = true;

		p->SetHurtAngle(-M_PI_2 + Direction * 0.7 * M_PI_2);

	}

	return true;
}
