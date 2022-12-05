#pragma once
#include "CBloodEmitor.h"
#include "CLaserparticleEmitor.h"

class CParticleMgr
{
private:
	CParticleMgr();
	~CParticleMgr();

public:
	static CParticleMgr* Create() { static CParticleMgr p; return &p; }
	
	void Initialize();
	void Enter();

	CBloodEmitor* BloodEmitor[3];
	CLaserparticleEmitor* LaserParticleEmitor[3];
};

