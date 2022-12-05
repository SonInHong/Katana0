#include "pch.h"
#include "CLaserParticle.h"
#include "CCameraMgr.h"
#include "CResourceMgr.h"


CLaserParticle::CLaserParticle()
{
	//텍스쳐 로딩
	Texture = CResourceMgr::Create()->Load<CTexture>(L"Laser\\LaserParticle\\0.bmp");

	

	Gravity = 3;
}

CLaserParticle::~CLaserParticle()
{
}

void CLaserParticle::Shoot()
{
}

void CLaserParticle::Render(HDC _dc)
{
	if (!Valid)
		return;

	if (Valid)
	{
		doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);

		BLENDFUNCTION func = {};
		func.AlphaFormat = AC_SRC_ALPHA;
		func.BlendFlags = 0;
		func.BlendOp = AC_SRC_OVER;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 255 * (Duration - Timer) / Duration;

		AlphaBlend(_dc, pos.x - Texture->GetWidth() * Resize.x / 2, pos.y - Texture->GetWidth() * Resize.y / 2, Texture->GetWidth() * Resize.x, Texture->GetWidth() * Resize.y
			, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), func);
	}
}
