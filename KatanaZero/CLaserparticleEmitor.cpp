#include "pch.h"
#include "CLaserparticleEmitor.h"
#include "CLaserParticle.h"
#include "CEventMgr.h"

CLaserparticleEmitor::CLaserparticleEmitor()
	:Owner(nullptr)
	,OffSet{}
{
}

CLaserparticleEmitor::~CLaserparticleEmitor()
{
}

void CLaserparticleEmitor::Initialize()
{
	for (int i = 0; i < m_PoolSize; ++i)
	{
		CLaserParticle* _particle = new CLaserParticle;
		_particle->Initialize();
		
		m_Particles.push_back(_particle);
	}
}

void CLaserparticleEmitor::Enter()
{
	for (int i = 0; i < m_PoolSize; ++i)
	{
		CEventMgr::Create()->Event_CreateObj(m_Particles[i], GROUP_TYPE::PARTICLE);

	}
}

void CLaserparticleEmitor::Exit()
{
}

void CLaserparticleEmitor::Update()
{
	if (!OnOff)
		return;

	if (Owner)
		Pos = Owner->GetPos() + OffSet;

	CParticleEmitor::Update();
}
