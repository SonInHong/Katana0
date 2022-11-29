#include "pch.h"
#include "CJumpCloudRight.h"
#include "CAnimator.h"
#include "CAnimation.h"

CJumpCloudRight::CJumpCloudRight()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Jumpcloud\\right", L"JumpCloudRight", doublepoint{ 0,0 }, doublepoint{ 51,32 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpCloudRight")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpCloudRight");
}

CJumpCloudRight::~CJumpCloudRight()
{
}

void CJumpCloudRight::Initialize()
{
}
