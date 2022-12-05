#include "pch.h"
#include "CBulletReflect.h"
#include "CAnimator.h"
#include "CAnimation.h"

CBulletReflect::CBulletReflect()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Effect\\spr_bulletreflect", L"BulletReflect", doublepoint{ 0,0 }, doublepoint{ 70,64 }, 5, 0.1, false
	);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"BulletReflect")->m_CompleteEvent = std::bind(&CEffect::Pause, this);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"BulletReflect");
}

CBulletReflect::~CBulletReflect()
{
}

void CBulletReflect::Initialize()
{
}
