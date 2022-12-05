#pragma once
#include "CGrunt.h"
#include "CGangster.h"
#include "CPomp.h"

#define poolsize 10

class CMonsterPoolMgr
{
private:
	CMonsterPoolMgr();
	~CMonsterPoolMgr();

public:
	static CMonsterPoolMgr* Create() { static CMonsterPoolMgr p; return &p; }
	void Initialize();

public:
	CGrunt* Grunt[poolsize];
	CGangster* Gangster[poolsize];
	CPomp* Pomp[poolsize];

};

