#include "pch.h"
#include "CJumpCloud.h"
#include "CAnimator.h"
#include "CAnimation.h"

CJumpCloud::CJumpCloud()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Jumpcloud\\up", L"JumpCloud", doublepoint{ 0,0 }, doublepoint{ 32,51 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpCloud")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpCloud");

}

CJumpCloud::~CJumpCloud()
{
}

void CJumpCloud::Initialize()
{
	
}
