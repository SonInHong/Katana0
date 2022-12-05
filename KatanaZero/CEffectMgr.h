#pragma once
#include "CJumpCloud.h"
#include "CJumpCloudLeft.h"
#include "CJumpCloudRight.h"
#include "CLandCloud.h"
#include "CHitEffect.h"
#include "CGunSparkLeft.h"
#include "CGunSparkRight.h"
#include "CBulletReflect.h"
#include "CEventMgr.h"


class CEffectMgr
{
private:
	CEffectMgr();
	~CEffectMgr();

public:
	struct EffectPool
	{
		std::vector<CEffect*> effect;

		void Play(doublepoint pos, doublepoint resize)
		{
			for (int i = 0; i < effect.size(); ++i)
			{
				if (effect[i]->Valid == false)
				{
					effect[i]->Play(pos, resize);
					return;
				}
			}
		}

		void PutInBox()
		{
			for (int i = 0; i < effect.size(); ++i)
			{
				CEventMgr::Create()->Event_CreateObj(effect[i], GROUP_TYPE::FINALEFFECT);
			}
		}
	};


	static CEffectMgr* Create() { static CEffectMgr p; return &p; }

	void Initialize();
	void PutInBox();

	EffectPool JumpCloud;
	EffectPool JumpCloudLeft;
	EffectPool JumpCloudRight;
	EffectPool LandCloud;
	EffectPool GunSparkLeft;
	EffectPool GunSparkRight;
	EffectPool BulletReflect;

	CHitEffect* HitEffect;
};

