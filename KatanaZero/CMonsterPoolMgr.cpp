#include "pch.h"
#include "CMonsterPoolMgr.h"

CMonsterPoolMgr::CMonsterPoolMgr()
{
}

CMonsterPoolMgr::~CMonsterPoolMgr()
{
}

void CMonsterPoolMgr::Initialize()
{
	for (int i = 0; i < poolsize; ++i)
	{
		Grunt[i] = new CGrunt;
		Pomp[i] = new CPomp;
		Gangster[i] = new CGangster;

		Grunt[i]->Initialize();
		Pomp[i]->Initialize();
		Gangster[i]->Initialize();
	}
}
