#include "pch.h"
#include "CJumpCloudLeft.h"
#include "CAnimator.h"
#include "CAnimation.h"

CJumpCloudLeft::CJumpCloudLeft()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Jumpcloud\\left", L"JumpCloudLeft", doublepoint{ 0,0 }, doublepoint{ 51,32 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpCloudLeft")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpCloudLeft");
}

CJumpCloudLeft::~CJumpCloudLeft()
{
}

void CJumpCloudLeft::Initialize()
{
}
