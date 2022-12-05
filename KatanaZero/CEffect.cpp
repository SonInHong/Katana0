#include "pch.h"
#include "CEffect.h"
#include "CAnimator.h"

CEffect::CEffect()
	:Valid(false)
{
	CreateAnimator();
}

CEffect::~CEffect()
{
}

void CEffect::Play(doublepoint pos, doublepoint resize)
{
	Pos = pos;
	Resize = resize;
	Valid = true;
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
}

void CEffect::Pause()
{
	Valid = false;
}

void CEffect::Enter()
{
}

void CEffect::Exit()
{
}

void CEffect::Render(HDC _dc)
{
	if (Valid)
		CObject::Render(_dc);
}
