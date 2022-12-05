#include "pch.h"
#include "CWallCam.h"
#include "CCameraMgr.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "TimeMgr.h"
#include "CResourceMgr.h"

CWallCam::CWallCam()
	:Speed(10)
	, Angle(M_PI_2)
	, AngleRange((double)20 / (double)180 * M_PI)
	, DetectAngleRange(doublepoint{ M_PI_4 * 1.25,M_PI_4 * 3 })
	, RoamingAngleRange(doublepoint{ M_PI_4 * 1.25,M_PI_4 * 3 })
	,State(WallCamState::Stay)
	,dir(Left)
{
	Scale.x = 32; Scale.y = 32;

	for (int i = 0; i < 8; ++i)
	{
		Texture_Green[i] = new CTexture;
		Texture_Red[i] = new CTexture;
	}

	Texture_Green[0] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\7.bmp");
	Texture_Green[1] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\6.bmp");
	Texture_Green[2] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\5.bmp");
	Texture_Green[3] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\4.bmp");
	Texture_Green[4] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\3.bmp");
	Texture_Green[5] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\2.bmp");
	Texture_Green[6] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\1.bmp");
	Texture_Green[7] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Green\\0.bmp");
	
	Texture_Red[0] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\7.bmp");
	Texture_Red[1] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\6.bmp");
	Texture_Red[2] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\5.bmp");
	Texture_Red[3] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\4.bmp");
	Texture_Red[4] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\3.bmp");
	Texture_Red[5] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\2.bmp");
	Texture_Red[6] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\1.bmp");
	Texture_Red[7] = CResourceMgr::Create()->Load<CTexture>(L"WallCam\\Red\\0.bmp");


}

CWallCam::~CWallCam()
{
}

void CWallCam::Initialize()
{
}

void CWallCam::Enter()
{
}

void CWallCam::Exit()
{
}

void CWallCam::Update()
{
	switch (State)
	{
	case WallCamState::FollowPlayer:
	{
		if (((CPlayer*)CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0])->GetHiding())  // 见绰芭 眉农
			State = WallCamState::Roam;

		doublepoint diff = ((CPlayer*)CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0])->GetPos() - Pos;

		if (diff.y > 0)
		{
			if (atan2(diff.y, diff.x) > Angle)
				Angle += Speed * TimeMgr::Create()->dt();

			if (atan2(diff.y, diff.x) < Angle)
				Angle -= Speed * TimeMgr::Create()->dt();
		}

		if (Angle > DetectAngleRange.y)
			Angle = DetectAngleRange.y;

		if (Angle < DetectAngleRange.x)
			Angle = DetectAngleRange.x;
	}
	break;

	case WallCamState::Roam:
	{
		if (dir == Left)
		{
			Angle += TimeMgr::Create()->dt();
			if (Angle > RoamingAngleRange.y)
			{
				Angle = RoamingAngleRange.y;
				dir = Right;
			}
			
		}

		else if(dir == Right)
		{
			Angle -= TimeMgr::Create()->dt();
			if (Angle < RoamingAngleRange.x)
			{
				Angle = RoamingAngleRange.x;
				dir = Left;
			}

		}

		if (PlayerInRange())
			State = WallCamState::FollowPlayer;
	}

	case WallCamState::Stay:
	{
		if (PlayerInRange())
			State = WallCamState::FollowPlayer;
	}

	default:
		break;
	}

	if (PlayerInRange())
		ConnectDoor->SetOpen(false);

	else
		ConnectDoor->SetOpen(true);
	
}

void CWallCam::Render(HDC _dc)
{
	double a = Angle / M_PI * 180;
	double b = (Angle - M_PI_4 *1.25) / (M_PI_4 / 4);
	int x = (int)floor((Angle - M_PI_4 * 1.25) / (M_PI_4 / 4));

	if (b < 0)
		x = 0;

	if (x >= 8)
		x = 7;

	doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

	if (State == WallCamState::FollowPlayer)
	{
		TransparentBlt(_dc, pos.x - scale.x * AnimationScaling.x / 2, pos.y - scale.y * AnimationScaling.y / 2, scale.x * AnimationScaling.x, scale.y * AnimationScaling.y
			, Texture_Red[x]->GetDC(), 0, 0, 32, 32, RGB(255, 255, 255));
	}

	else
		TransparentBlt(_dc, pos.x - scale.x * AnimationScaling.x / 2, pos.y - scale.y * AnimationScaling.y / 2, scale.x * AnimationScaling.x, scale.y * AnimationScaling.y
		, Texture_Green[x]->GetDC(), 0, 0, 32, 32, RGB(255, 255, 255));


	doublepoint A1 = CCameraMgr::Create()->CameraCoordinate(Pos + doublepoint{ cos(Angle + AngleRange), sin(Angle + AngleRange) } *300);
	doublepoint A2 = CCameraMgr::Create()->CameraCoordinate(Pos + doublepoint{ cos(Angle - AngleRange), sin(Angle - AngleRange) } *300);

	MoveToEx(_dc, A1.x, A1.y, NULL);
	LineTo(_dc, pos.x, pos.y);
	LineTo(_dc, A2.x, A2.y);

}

bool CWallCam::PlayerInRange()
{
	if (((CPlayer*)CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0])->GetHiding())  // 见绰芭 眉农
		return false;

	doublepoint diff = ((CPlayer*)CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0])->GetPos() - Pos;
	double diffangle = atan2(diff.y, diff.x);

	if (abs(diffangle - Angle) < AngleRange)
		return true;
	
	else
		return false;
}
