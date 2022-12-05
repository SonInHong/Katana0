#include "pch.h"
#include "CIronDoorRight.h"
#include "CAnimator.h"

CIronDoorRight::CIronDoorRight()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Door\\spr_door_animation_iron\\Idle\\right", L"IronRightDoorIdle", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 1, 0.04, true
		, doublepoint{ scaleA * 40,0 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Door\\spr_door_animation_iron\\Open\\right", L"IronRightDoorOpen", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 20, 0.04, false
		, doublepoint{ scaleA * 40,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IronRightDoorIdle");
}

CIronDoorRight::~CIronDoorRight()
{
}

void CIronDoorRight::Update()
{
	if (m_Open)
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IronRightDoorOpen");
}
