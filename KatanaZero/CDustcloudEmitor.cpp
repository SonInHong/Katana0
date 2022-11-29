#include "pch.h"
#include "CDustcloudEmitor.h"
#include "CDustcloud.h"
#include "CEventMgr.h"

CDustcloudEmitor::CDustcloudEmitor()
{
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
		CEventMgr::Create()->Event_CreateObj(_particle, GROUP_TYPE::PARTICLE);

		m_Particles.push_back(_particle);
	}
}

void CDustcloudEmitor::Update()
{
	if(Owner)
		Pos = Owner->GetPos() + OffSet;

	CParticleEmitor::Update();
}
