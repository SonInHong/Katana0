#pragma once
#include "CJumpCloud.h"
#include "CJumpCloudLeft.h"
#include "CJumpCloudRight.h"
#include "CLandCloud.h"
#include "CHitEffect.h"

class CEffectMgr
{
private:
	CEffectMgr();
	~CEffectMgr();

public:
	static CEffectMgr* Create() { static CEffectMgr p; return &p; }

	void Initialize();
	void PutInBox();

	CJumpCloud* JumpCloud;
	CJumpCloudLeft* JumpCloudLeft;
	CJumpCloudRight* JumpCloudRight;
	CLandCloud* LandCloud;
	CHitEffect* HitEffect;

};

