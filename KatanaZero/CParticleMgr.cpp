#include "pch.h"
#include "CParticleMgr.h"

CParticleMgr::CParticleMgr()
{
}

CParticleMgr::~CParticleMgr()
{
}

void CParticleMgr::Initialize()
{
	for (int i = 0; i < 3; ++i)
	{
		BloodEmitor[i] = new CBloodEmitor;
		BloodEmitor[i]->Initialize();

		LaserParticleEmitor[i] = new CLaserparticleEmitor;
		LaserParticleEmitor[i]->Initialize();
	}
}

void CParticleMgr::Enter()
{
	for (int i = 0; i < 3; ++i)
	{
		
		BloodEmitor[i]->Enter();

		LaserParticleEmitor[i]->Enter();
	}
}
