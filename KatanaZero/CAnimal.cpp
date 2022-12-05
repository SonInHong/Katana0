#include "pch.h"
#include "CAnimal.h"
#include "CRunDustcloudEmitor.h"
#include "CDustcloud.h"
#include "CBloodEmitor.h"
#include "CLaserparticleEmitor.h"
#include "CAnimator.h"

CAnimal::CAnimal()
	:BloodEmitor(nullptr)
	, RollDustcloud(nullptr)
	, RunDustcloud(nullptr)
	, WallDustcloud(nullptr)
	,LaserparticleEmitor(nullptr)
	, MainOrder(Main_Order::End)
	, Hiding(false)
{
}

CAnimal::~CAnimal()
{
}

void CAnimal::Initialize()
{
	//BloodEmitor = new CBloodEmitor; //피
	//BloodEmitor->SetOwner(this);
	//BloodEmitor->SetOffset(doublepoint{ 0,0 });
	//BloodEmitor->SetOnOff(false);
	//BloodEmitor->Initialize();

	RollDustcloud = new CDustcloudEmitor; //구르기 구름
	RollDustcloud->SetOwner(this);
	RollDustcloud->SetOffset(doublepoint{ 0,scaleA * 25 });
	RollDustcloud->SetOnOff(false);
	RollDustcloud->Initialize();

	RunDustcloud = new CDustcloudEmitor; //출발 구름
	RunDustcloud->SetOwner(this);
	RunDustcloud->SetOffset(doublepoint{ 0,scaleA * 25 });
	RunDustcloud->SetOnOff(false);
	RunDustcloud->Initialize();

	WallDustcloud = new CDustcloudEmitor; //벽 구름
	WallDustcloud->SetOwner(this);
	WallDustcloud->SetOffset(doublepoint{ 0,scaleA * 25 });
	WallDustcloud->SetOnOff(false);
	WallDustcloud->Initialize(); //씬에서 매번 수행되어야 하는 기능

	//LaserparticleEmitor = new CLaserparticleEmitor; //레이저 파티클
	//LaserparticleEmitor->SetOwner(this);
	//LaserparticleEmitor->SetOffset(doublepoint{ 0,-20 });
	//LaserparticleEmitor->SetOnOff(false);
	//LaserparticleEmitor->Initialize();
}

void CAnimal::Enter()
{
	
	//BloodEmitor->Enter();
	//LaserparticleEmitor->Enter();
}

void CAnimal::Exit()
{
}

void CAnimal::Update()
{
	if(BloodEmitor)
		BloodEmitor->Update();
	
	
	RollDustcloud->Update();
	RunDustcloud->Update();
	WallDustcloud->Update();
	
	if(LaserparticleEmitor)
		LaserparticleEmitor->Update();


	if (InSmoke)
		Hiding = true;
	else
		Hiding = false;

	InSmoke = false;
	
	if (Hiding)
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRender(true);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRenderColor(PenColor::BLACK);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRenderAlpha(230);
	}

	else
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRenderAlpha(120);
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->SetReRender(false);
	}
}


void CAnimal::SetRollDustcloud(bool b)
{
	RollDustcloud->SetOnOff(b);
}

void CAnimal::SetRollDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	RollDustcloud->SetOption(resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	RollDustcloud->SetOnOff(true);
}

void CAnimal::SetRunDustcloud(bool b)
{
	RunDustcloud->SetOnOff(b);
}

void CAnimal::SetRunDustcloudOption(int maxcount, doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	RunDustcloud->SetOption(maxcount, resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	RunDustcloud->SetOnOff(true);
}

void CAnimal::SetWallDustcloud(bool b)
{
	WallDustcloud->SetOnOff(b);
}

void CAnimal::SetWallDustcloudOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	WallDustcloud->SetOption(resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	WallDustcloud->SetOnOff(true);
}

void CAnimal::SetLaserParticle(bool b)
{
	if (nullptr == LaserparticleEmitor)
		return;
	LaserparticleEmitor->SetOnOff(b);
}

void CAnimal::SetLaserParticleOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	if (nullptr == LaserparticleEmitor)
		return;

	LaserparticleEmitor->SetOption(resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	LaserparticleEmitor->SetOnOff(true);
}

void CAnimal::SetBloodEmitor(bool b)
{
	if (nullptr == BloodEmitor)
		return;

	BloodEmitor->SetOnOff(b);
}

void CAnimal::SetBloodEmitorOption(doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	if (nullptr == BloodEmitor)
		return;

	BloodEmitor->SetOption(resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	BloodEmitor->SetOnOff(true);
}

void CAnimal::SetBloodEmitormaxOption(int maxcount, doublepoint resize, doublepoint anglerange, doublepoint velocityrange, doublepoint durationrange, doublepoint attackspeedrange, doublepoint XoffRange, doublepoint YoffRange)
{
	if (nullptr == BloodEmitor)
		return;

	BloodEmitor->SetOption(maxcount, resize, anglerange, velocityrange, durationrange, attackspeedrange, XoffRange, YoffRange);
	BloodEmitor->SetOnOff(true);
}
