#pragma once
#include "CObject.h"
#include "CTexture.h"

class CParticleEmitor;
class CBloodEmitor;

class CParticle:public CObject
{
public:
	CParticle();
	virtual ~CParticle();

	virtual void Initialize();
	virtual void Update();
	virtual void Render(HDC _dc);

	friend CParticleEmitor;
	friend CBloodEmitor;

	bool operator < (CParticle b)
	{
		return ((int)Valid < (int)b.Valid);
	}

	virtual void Shoot(); // �� �߻������ �ؾ��� �ϵ�

protected:
	CTexture* Texture;  // �ؽ��� �̹���
	
	double Gravity; //������� �߷°��ӵ�
	double AirFriction; // ������� �������װ��

	doublepoint Velocity; // ���� �ӵ�

	double Duration; // ����
	double Timer;

	bool Valid;
};



