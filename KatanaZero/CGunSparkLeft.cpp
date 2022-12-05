#include "pch.h"
#include "CGunSparkLeft.h"
#include "CAnimator.h"
#include "CAnimation.h"

CGunSparkLeft::CGunSparkLeft()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Effect\\Gunspark2\\left", L"GunSpark2Left", doublepoint{ 0,0 }, doublepoint{ 50,50 }, 8, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GunSpark2Left")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"GunSpark2Left");
}

CGunSparkLeft::~CGunSparkLeft()
{
}

void CGunSparkLeft::Initialize()
{
}
