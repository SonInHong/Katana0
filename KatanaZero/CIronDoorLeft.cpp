#include "pch.h"
#include "CIronDoorLeft.h"
#include "CAnimator.h"

CIronDoorLeft::CIronDoorLeft()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Door\\spr_door_animation_iron\\Idle\\right", L"IronLeftDoorIdle", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 1, 0.04, true
		, doublepoint{ -scaleA * 40,0 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Door\\spr_door_animation_iron\\Open\\right", L"IronLeftDoorOpen", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 20, 0.04, false
		, doublepoint{ -scaleA * 40,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IronLeftDoorIdle");
}

CIronDoorLeft::~CIronDoorLeft()
{
}

void CIronDoorLeft::Update()
{
	if (m_Open)
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IronLeftDoorOpen");
}
