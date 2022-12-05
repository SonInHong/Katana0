#pragma once
#include "CBullet.h"
#include "CKnife.h"

class CNeutralObjMgr
{
private:
	CNeutralObjMgr();
	~CNeutralObjMgr();

public:
	static CNeutralObjMgr* Create() { static CNeutralObjMgr p; return &p; }

	void Initialize();
	void PutInBox();

	CBullet* Bullet[100];
	CKnife* Knife;

	void ShootBullet(doublepoint Pos, doublepoint velocity , bool b);
	
};

