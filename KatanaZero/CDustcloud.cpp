#include "pch.h"
#include "CDustcloud.h"
#include "CAnimator.h"

CDustcloud::CDustcloud()
{
	CreateAnimator();
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Dustcloud\\Dustcloud", L"DustCloud", doublepoint{ 0,0 }, doublepoint{ 19,19 }, 7, 0.1, false);
}

CDustcloud::~CDustcloud()
{
}

void CDustcloud::Shoot()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"DustCloud");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

	
}

void CDustcloud::Render(HDC _dc)
{
	if (!Valid)
		return;

	CParticle::Render(_dc);
}
