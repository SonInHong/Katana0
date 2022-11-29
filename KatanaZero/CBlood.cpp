#include "pch.h"
#include "CBlood.h"
#include "CAnimator.h"

CBlood::CBlood()
{
	Gravity = 3;
	CreateAnimator();
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Blood\\spr_bloodsplatter_nondir", L"NondirBlood", doublepoint{ 0,0 }, doublepoint{ 16,16 }, 7, 0.1, false);
}

CBlood::~CBlood()
{
}

void CBlood::Shoot()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"NondirBlood");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
}
