#include "pch.h"
#include "CLandCloud.h"
#include "CAnimator.h"
#include "CAnimation.h"

CLandCloud::CLandCloud()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Landcloud", L"LandCloud", doublepoint{ 0,0 }, doublepoint{ 50,14 }, 7, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"LandCloud")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandCloud");
}

CLandCloud::~CLandCloud()
{
}

void CLandCloud::Initialize()
{
}
