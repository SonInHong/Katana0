#include "pch.h"
#include "CDirtyWaterEmitor.h"
#include "CEventMgr.h"
#include "CDirtyWater.h"

CDirtyWaterEmitor::CDirtyWaterEmitor()
{
	SetOption(doublepoint{ 2,2 }, doublepoint{ 90,180 }, doublepoint{ 10,50 }, doublepoint{ 2,2 }, doublepoint{ 0,0.05 }, doublepoint{ 0,0 }, doublepoint{ 0,0 });

	for (int i = 0; i < m_PoolSize; ++i)
	{
		CDirtyWater* _particle = new CDirtyWater;
		_particle->Initialize();
		CEventMgr::Create()->Event_CreateObj(_particle, GROUP_TYPE::PARTICLE);

		m_Particles.push_back(_particle);
	}
}

CDirtyWaterEmitor::~CDirtyWaterEmitor()
{
}

void CDirtyWaterEmitor::Initialize()
{
}
