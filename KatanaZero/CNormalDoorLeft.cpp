#include "pch.h"
#include "CNormalDoorLeft.h"
#include "CAnimator.h"

CNormalDoorLeft::CNormalDoorLeft()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Normal_Door\\Idle\\left", L"NormalLeftDoorIdle", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 1, 0.04, true
		, doublepoint{ -scaleA * 30,0 });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Normal_Door\\Open\\left", L"NormalLeftDoorOpen", doublepoint{ 0,0 }, doublepoint{ 80,64 }, 16, 0.04, false
		, doublepoint{ -scaleA * 30,0 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"NormalLeftDoorIdle");
}

CNormalDoorLeft::~CNormalDoorLeft()
{
}

void CNormalDoorLeft::Update()
{
	if (m_Open)
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"NormalLeftDoorOpen");
}
