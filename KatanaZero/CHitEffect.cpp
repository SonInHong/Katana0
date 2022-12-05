#include "pch.h"
#include "CHitEffect.h"
#include "TimeMgr.h"
#include "CCameraMgr.h"
#include "CResourceMgr.h"

CHitEffect::CHitEffect()
	:Valid(false)
	,Texture(nullptr)
{
	//텍스쳐 로딩
	Texture = CResourceMgr::Create()->Load<CTexture>(L"Effect\\HitEffect\\0.bmp");
	
}

CHitEffect::~CHitEffect()
{
}

void CHitEffect::Enter()
{
}

void CHitEffect::Exit()
{
}

void CHitEffect::Update()
{
	if (!Valid)
		return;

	if ((Pos - From).Norm() > (To - From).Norm())
		Valid = false;

	Pos = Pos + Velocity * TimeMgr::Create()->dt();
	t += (double)69/Time * TimeMgr::Create()->dt();

}

void CHitEffect::Render(HDC _dc)
{
	if (!Valid)
		return;

	BLENDFUNCTION func = {};
	func.AlphaFormat = AC_SRC_ALPHA;
	func.BlendFlags = 0;
	func.BlendOp = AC_SRC_OVER;
	func.AlphaFormat = AC_SRC_ALPHA;
	func.SourceConstantAlpha = 50;

	//5x5 사이즈 만큼 자른걸 알파블렌딩 해간다
	//AlphaBlend(_dc, Pos.x - 10, Pos.y - 10, 20, 20, Texture->GetDC(), t, 0, 5, 5, func);

	infos.push_back(info{ Pos,t });

	doublepoint vector = Velocity / Velocity.Norm();
	
	for (int i = 0; i < Length/4 ; ++i)
	{

		doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos - vector * 4* i);
		AlphaBlend(_dc, pos.x - 5, pos.y - 5, scaleA * 10, scaleA * 10, Texture->GetDC(), t-80/Length*i, 0, 5, 5, func);
		
	}

}

void CHitEffect::Shoot(doublepoint from, doublepoint to, double velocity, double _Length)
{
	From = from; To = to;
	Velocity = (To-From) / (To - From).Norm() * velocity;

	Pos = From;
	Valid = true;

	Time = (to - from).Norm() / velocity;
	Length = _Length;
	t = 20;

}
