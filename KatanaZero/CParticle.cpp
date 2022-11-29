#include "pch.h"
#include "TimeMgr.h"
#include "CParticle.h"
#include "CEventMgr.h"
#include "CParticleEmitor.h"
#include "CResourceMgr.h"
#include "CAnimator.h"
#include "KeyMgr.h"

CParticle::CParticle()
	:Texture(nullptr)
	,Gravity(0)
	,AirFriction(0)
	,Velocity{}
	,Duration(0)
	,Timer(0)
	,Valid(false)
{
	
}

CParticle::~CParticle()
{
	
}

void CParticle::Initialize()
{
	Scale.x = 10;
	Scale.y = 10;

	CreateAnimator();
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Dustcloud\\Dustcloud", L"Dustcloud1", doublepoint{ 0,0 }, doublepoint{ 19,19 }, 7, 0.5, false);

}

void CParticle::Update()
{

	if (KeyMgr::Create()->key(Key::M).pressed && Valid)
	{
		int a = 0;
	}

	if (!Valid)
		return;

	if (Timer > Duration)
	{
		Valid = false;
		Timer = 0;
	}
	
		

	Timer += TimeMgr::Create()->dt();

	Pos.x += Velocity.x * TimeMgr::Create()->dt();
	Pos.y += Velocity.y * TimeMgr::Create()->dt();

	doublepoint Accel = Velocity * (-AirFriction * Velocity.Norm());
	Accel.y += Gravity;

	Velocity = Velocity + Accel;
}

void CParticle::Render(HDC _dc)
{
	if (!Valid)
		return;

	CObject::Render(_dc);

	//TransparentBlt(_dc, Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2, Pos.y - AnimationScaling.y * Resize.y * Scale.y / 2, AnimationScaling.x*Resize.x * Scale.x, AnimationScaling.y*Resize.y * Scale.y
		//, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));

	//Rectangle(_dc, Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2, Pos.y - AnimationScaling.y * Resize.y * Scale.y / 2
		//, Pos.x + AnimationScaling.x * Resize.x * Scale.x / 2, Pos.y + AnimationScaling.y * Resize.y * Scale.y / 2);
}

void CParticle::Shoot()
{
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"Dustcloud1");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();


}
