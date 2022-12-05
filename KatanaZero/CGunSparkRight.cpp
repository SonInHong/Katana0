#include "pch.h"
#include "CGunSparkRight.h"
#include "CAnimator.h"
#include "CAnimation.h"

CGunSparkRight::CGunSparkRight()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Effect\\Gunspark2\\right", L"GunSpark2Right", doublepoint{ 0,0 }, doublepoint{ 50,50 }, 8, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GunSpark2Right")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"GunSpark2Right");
}

CGunSparkRight::~CGunSparkRight()
{
}

void CGunSparkRight::Initialize()
{
}
