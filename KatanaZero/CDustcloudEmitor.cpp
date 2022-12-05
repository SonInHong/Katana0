#include "pch.h"
#include "CDustcloudEmitor.h"
#include "CDustcloud.h"
#include "CEventMgr.h"

CDustcloudEmitor::CDustcloudEmitor()
{
	m_PoolSize = 50;
}

CDustcloudEmitor::~CDustcloudEmitor()
{
}

void CDustcloudEmitor::Initialize()
{	
	for (int i = 0; i < m_PoolSize; ++i)
	{
		CDustcloud* _particle = new CDustcloud;
		_particle->Initialize();

		m_Particles.push_back(_particle);
	}
}

void CDustcloudEmitor::Enter()
{
	for (int i = 0; i < m_PoolSize; ++i)
	{
		CEventMgr::Create()->Event_CreateObj(m_Particles[i], GROUP_TYPE::PARTICLE);
	}
}

void CDustcloudEmitor::Exit()
{
}

void CDustcloudEmitor::Update()
{
	if(Owner)
		Pos = Owner->GetPos() + OffSet;

	CParticleEmitor::Update();
}
