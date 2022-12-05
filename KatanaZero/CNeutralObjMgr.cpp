#include "pch.h"
#include "CNeutralObjMgr.h"
#include "CEventMgr.h"

CNeutralObjMgr::CNeutralObjMgr()
{
}

CNeutralObjMgr::~CNeutralObjMgr()
{
}

void CNeutralObjMgr::Initialize()
{
	for (int i = 0; i < 100; ++i)
	{
		Bullet[i] = new CBullet;
		Bullet[i]->Initialize();
	}

	Knife = new CKnife;
	Knife->Initialize();
}

void CNeutralObjMgr::PutInBox()
{
	for (int i = 0; i < 100; ++i)
	{
		CEventMgr::Create()->Event_CreateObj(Bullet[i], GROUP_TYPE::NEUTRAL_OBJECT);
	}

	CEventMgr::Create()->Event_CreateObj(Knife, GROUP_TYPE::NEUTRAL_OBJECT);
}

void CNeutralObjMgr::ShootBullet(doublepoint Pos, doublepoint velocity, bool b)
{
	for (int i = 0; i < 100; ++i)
	{
		if (Bullet[i]->GetValid() == false)
		{
			Bullet[i]->SetVelocity(velocity);
			Bullet[i]->SetPos(Pos);
			Bullet[i]->SetKillPlayer(b);
			Bullet[i]->SetValid(true);

			return;
		}
	}

	
}
