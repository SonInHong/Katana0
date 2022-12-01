#include "pch.h"
#include "CEffectMgr.h"
#include "CEventMgr.h"

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Initialize()
{
	JumpCloud = new CJumpCloud;
	JumpCloud->Initialize();

	JumpCloudRight = new CJumpCloudRight;
	JumpCloudRight->Initialize();

	JumpCloudLeft = new CJumpCloudLeft;
	JumpCloudLeft->Initialize();

	LandCloud = new CLandCloud;
	LandCloud->Initialize();

	HitEffect = new CHitEffect;
	HitEffect->Initialize();
}

void CEffectMgr::PutInBox() // ���� ���� ������Ʈ ��Ͽ� �������.
{
	CEventMgr::Create()->Event_CreateObj(JumpCloud, GROUP_TYPE::FINALEFFECT);
	CEventMgr::Create()->Event_CreateObj(JumpCloudRight, GROUP_TYPE::FINALEFFECT);
	CEventMgr::Create()->Event_CreateObj(JumpCloudLeft, GROUP_TYPE::FINALEFFECT);
	CEventMgr::Create()->Event_CreateObj(LandCloud, GROUP_TYPE::FINALEFFECT);
	CEventMgr::Create()->Event_CreateObj(HitEffect, GROUP_TYPE::FINALEFFECT);
}
