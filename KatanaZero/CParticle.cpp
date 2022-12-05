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

}

void CParticle::Enter()
{
}

void CParticle::Exit()
{
}

void CParticle::Update()
{


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

}

void CParticle::Shoot()
{
	

}
