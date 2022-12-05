#include "pch.h"
#include "CEffectMgr.h"


CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Initialize()
{

	for (int i = 0; i < 10; ++i)
	{
		JumpCloud.effect.push_back(new CJumpCloud);
		JumpCloud.effect[i]->Initialize();
	}

	for (int i = 0; i < 10; ++i)
	{
		JumpCloudRight.effect.push_back(new CJumpCloudRight);
		JumpCloudRight.effect[i]->Initialize();
	}

	for (int i = 0; i < 10; ++i)
	{
		JumpCloudLeft.effect.push_back(new CJumpCloudLeft);
		JumpCloudLeft.effect[i]->Initialize();
	}

	for (int i = 0; i < 10; ++i)
	{
		GunSparkLeft.effect.push_back(new CGunSparkLeft);
		GunSparkLeft.effect[i]->Initialize();
	}

	for (int i = 0; i < 10; ++i)
	{
		GunSparkRight.effect.push_back(new CGunSparkRight);
		GunSparkRight.effect[i]->Initialize();
	}

	for (int i = 0; i < 10; ++i)
	{
		BulletReflect.effect.push_back(new CBulletReflect);
		BulletReflect.effect[i]->Initialize();
	}

	
	HitEffect = new CHitEffect;
	HitEffect->Initialize();

	
}

void CEffectMgr::PutInBox() // 현재 씬의 오브젝트 목록에 집어넣음.
{

	JumpCloud.PutInBox();
	JumpCloudLeft.PutInBox();
	JumpCloudRight.PutInBox();
	LandCloud.PutInBox();
	GunSparkLeft.PutInBox();
	GunSparkRight.PutInBox();
	BulletReflect.PutInBox();
	
	CEventMgr::Create()->Event_CreateObj(HitEffect, GROUP_TYPE::FINALEFFECT);
	
}
