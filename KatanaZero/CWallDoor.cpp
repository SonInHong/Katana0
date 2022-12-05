#include "pch.h"
#include "CWallDoor.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCameraMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CCollider.h"

#define OpenSpeed 6

CWallDoor::CWallDoor()
	:Closedness(1)
	,Open(false)
{
	Texture = new CTexture;
	Texture->Load(L"elevatordoor\\spr_sliding_door\\0.bmp");

	
}

CWallDoor::~CWallDoor()
{
}

void CWallDoor::Initialize()
{
	CreateAnimator();

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"elevatordoor\\spr_sliding_door_base_green", L"SlidingDoorOpen", doublepoint{ 0,0 }, doublepoint{ 16,16 }, 3, 0.1, false
		, doublepoint{ 0,-Scale.y / 2 + 8 * AnimationScaling.y });
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"elevatordoor\\spr_sliding_door_base_red", L"SlidingDoorClosed", doublepoint{ 0,0 }, doublepoint{ 16,16 }, 3, 0.1, true
		, doublepoint{ 0,-Scale.y / 2 + 8 * AnimationScaling.y });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"SlidingDoorClosed");

	CWall::Initialize();
}

void CWallDoor::Update()
{
	


	if (KeyMgr::Create()->key(O).pressed)
		Open = true;
	
	if (Open)
	{
		Closedness -= OpenSpeed * TimeMgr::Create()->dt();
		Closedness = max(0, Closedness);

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"SlidingDoorOpen");
	}

	if (!Open)
	{
		Closedness += OpenSpeed * TimeMgr::Create()->dt();
		Closedness = min(1, Closedness);

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"SlidingDoorClosed");
	}

	dynamic_cast<CCollider*>(m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetScale(doublepoint{ Scale.x,16 * AnimationScaling.y + Closedness * (Scale.y - 16 * AnimationScaling.y) });
	dynamic_cast<CCollider*>(m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetOffSet(doublepoint{ 0, (16 * AnimationScaling.y + Closedness * (Scale.y - 16 * AnimationScaling.y)) / 2 - Scale.y / 2 });
}

void CWallDoor::Render(HDC _dc)
{
	CObject::Render(_dc);

	doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

	TransparentBlt(_dc, pos.x - scale.x / 2, pos.y - scale.y / 2 + 16*AnimationScaling.y, scale.x, Closedness*(scale.y - 16*AnimationScaling.y)
		, Texture->GetDC(), 0, (1-Closedness)*Texture->GetHeight(), Texture->GetWidth(), Closedness * Texture->GetHeight(), RGB(255, 255, 255));
}



bool CWallDoor::Collide(CObject* other)
{
	
	CWall_CantClimb::Collide(other);

	return false;
}

bool CWallDoor::Colliding(CObject* other)
{   
	

	CWall_CantClimb::Colliding(other);

	return false;
}

bool CWallDoor::CollideEscape(CObject* other)
{
	CWall::CollideEscape(other);

	return false;
}

bool CWallDoor::DontCollide(CObject* other)
{
	CWall::DontCollide(other);

	return false;
}



