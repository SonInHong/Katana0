#include "pch.h"
#include "CDirtyWater.h"
#include "CAnimator.h"

CDirtyWater::CDirtyWater()
{
	Gravity = 10;
	AirFriction = 0.0001;

	Scale.x = 5;
	Scale.y = 5;

	CreateAnimator();
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Dustcloud\\Dustcloud", L"DirtyWater", doublepoint{ 0,0 }, doublepoint{ 19,19 }, 7, 0.2, false);
}

CDirtyWater::~CDirtyWater()
{
}

void CDirtyWater::Shoot()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"DirtyWater");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
}
