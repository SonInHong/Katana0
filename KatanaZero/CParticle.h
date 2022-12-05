#pragma once
#include "CObject.h"
#include "CTexture.h"

class CParticleEmitor;
class CBloodEmitor;
class CSmokeEmitor;

class CParticle:public CObject
{
public:
	CParticle();
	virtual ~CParticle();

	virtual void Initialize();
	virtual void Enter();
	virtual void Exit();
	virtual void Update();
	virtual void Render(HDC _dc);

	friend CParticleEmitor;
	friend CBloodEmitor;
	friend CSmokeEmitor;

	bool operator < (CParticle b)
	{
		return ((int)Valid < (int)b.Valid);
	}

	virtual void Shoot(); // 막 발사됐을때 해야할 일들
	bool& GetValid() { return Valid; }

protected:
	CTexture* Texture;  // 텍스쳐 이미지
	
	double Gravity; //적용받을 중력가속도
	double AirFriction; // 적용받을 공기저항계수

	doublepoint Velocity; // 현재 속도

	double Duration; // 수명
	double Timer;

	bool Valid;
};



