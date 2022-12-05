#include "pch.h"
#include "CBackgroundObj.h"
#include "CCameraMgr.h"

CBackgroundObj::CBackgroundObj()
{
}

CBackgroundObj::~CBackgroundObj()
{
}

void CBackgroundObj::Render(HDC _dc)
{
	doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

	TransparentBlt(_dc, pos.x - scale.x * AnimationScaling.x / 2, pos.y - scale.y * AnimationScaling.y / 2, scale.x * AnimationScaling.x, scale.y * AnimationScaling.y
		, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));
}

void CBackgroundObj::Enter()
{
}

void CBackgroundObj::Exit()
{
}
