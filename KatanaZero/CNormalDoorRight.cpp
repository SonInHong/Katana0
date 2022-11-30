#include "pch.h"
#include "CNormalDoorRight.h"
#include "CAnimator.h"


CNormalDoorRight::CNormalDoorRight()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Normal_Door\\Idle\\right", L"NormalRightDoorIdle", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 1, 0.04, true
		, doublepoint{ scaleA * 30,0 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Normal_Door\\Open\\right", L"NormalRightDoorOpen", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 16, 0.04, false
		, doublepoint{ scaleA * 30,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"NormalRightDoorIdle");
}

CNormalDoorRight::~CNormalDoorRight()
{
}

void CNormalDoorRight::Update()
{
	if (m_Open)
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"NormalRightDoorOpen");
}
