#include "pch.h"
#include "CKnife.h"
#include "TimeMgr.h"
#include "CMonster.h"
#include "CEffectMgr.h"
#include "CCameraMgr.h"
#include "CArchitecture.h"
#include "CDoor.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "KeyMgr.h"
#include "CResourceMgr.h"
#include "CTopFloor.h"

#define pickupDistance 200

CKnife::CKnife()
{
	Texture = CResourceMgr::Create()->Load<CTexture>(L"ButcherKnife\\0.bmp");
	CreateAnimator();
	Scale.x = 80;
	Scale.y = 80;
}

CKnife::~CKnife()
{
}

void CKnife::Enter()
{
}

void CKnife::Exit()
{
}

void CKnife::Initialize()
{
	CreateCollider();
}

void CKnife::Update()
{
	if (!Valid)
		return;

	if (stick == true)
	{
		State = ThrowingObjState::Stick;
		Velocity.x = 0;
		Velocity.y = 0;
	}

	else
	{
		if (State == ThrowingObjState::Stick)
			State = ThrowingObjState::Neutral;
	}

	switch (State)
	{
	case ThrowingObjState::Neutral:
	{
		//중력으로 떨어짐
		Velocity.y += 1500 * TimeMgr::Create()->dt();
		Velocity.y = min(2000, Velocity.y);
		Pos = Pos + Velocity * TimeMgr::Create()->dt();
	}
	break;

	case ThrowingObjState::Stick:
	{
		//반경 범위 내에 플레이어가 존재하면 UI를 띄우고 플레이어가 rightclick하면 주워진다.
		if ((Owner->GetPos() - Pos).Norm() < pickupDistance)
		{
			if (KeyMgr::Create()->key(RIGHTMOUSE).key_state == KeyState::TAP && Owner->GetItem() == nullptr)
			{
				State = ThrowingObjState::OwnedByPlayer;
				Pos = Owner->GetPos();
				Owner->SetItem(this);

			}
				
		}
	}
	break;

	case ThrowingObjState::OwnedByPlayer:
	{
		Pos = Owner->GetPos();
	}
	break;

	case ThrowingObjState::Throwing:
	{
		Velocity.y += 1500 * TimeMgr::Create()->dt();
		Velocity.y = min(2000, Velocity.y);
		Pos = Pos + Velocity * TimeMgr::Create()->dt();
		
	}
	break;

	}

	stick = false;
}

void CKnife::Render(HDC _dc)
{
	if (!Valid)
		return;

	doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

	switch (State)
	{
	case ThrowingObjState::Neutral:
	{
		TransparentBlt(_dc, pos.x - AnimationScaling.x * scale.x / 2, pos.y - AnimationScaling.y * scale.y / 2, AnimationScaling.x * scale.x, AnimationScaling.y * scale.y
			, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));
	}
	break;

	case ThrowingObjState::Stick:
	{
		TransparentBlt(_dc, pos.x - AnimationScaling.x * scale.x / 2, pos.y - AnimationScaling.y * scale.y / 2, AnimationScaling.x * scale.x, AnimationScaling.y * scale.y
			, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));
	}
	break;

	case ThrowingObjState::OwnedByPlayer:
	{
		//렌더링 안함
	}
	break;

	case ThrowingObjState::Throwing:
	{
		TransparentBlt(_dc, pos.x - AnimationScaling.x * scale.x / 2, pos.y - AnimationScaling.y * scale.y / 2, AnimationScaling.x * scale.x, AnimationScaling.y * scale.y
			, Texture->GetDC(), 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));
	}
	break;

	}

	CObject::Render(_dc);
}

void CKnife::Throw(doublepoint pos, doublepoint velocity)
{
	State = ThrowingObjState::Throwing;
	Pos = pos;
	Velocity = velocity;

	Owner->SetItem(nullptr);
}

bool CKnife::Collide(CObject* other)
{
	if (!Valid)
		return false;

	switch (State)
	{
	case ThrowingObjState::Neutral:
	{
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			CDoor* d = dynamic_cast<CDoor*>(arc);
			if (d)
			{
				if (d->GetOpen())
					stick = stick || false;

				else
					stick = stick || true;
			}

			else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
			{
				stick = stick || false;
			}

			else
				stick = stick || true;
		}
	}
	break;
	
	case ThrowingObjState::Stick:
	{
		//아무것도 안함
	}
	break;

	case ThrowingObjState::OwnedByPlayer:
	{
		//아무것도 안함
	}
	break;

	case ThrowingObjState::Throwing:
	{
		//몬스터를 맞았을때
		CMonster* mon = dynamic_cast<CMonster*>(other);

		if (mon && mon->GetMainOrder() != Main_Order::End && mon->GetMainOrder() != Main_Order::GetHurt && mon->GetMainOrder() != Main_Order::GetBurn)
		{
			double Angle = atan2(Velocity.y, Velocity.x);

			mon->GetMainOrder() = Main_Order::GetHurt;
			mon->SetHurtAngle(Angle);
			Valid = false;

			CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
			CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
				, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
			, 30000, 2000);

		}

		//땅과 벽,천장, 문에 맞았을때
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			CDoor* d = dynamic_cast<CDoor*>(arc);
			if (d)
			{
				if (d->GetOpen())
					stick = stick || false;

				else
					stick = stick || true;
			}

			else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
			{
				if (t->GetPos().y-t->GetScale().y/2 > Pos.y+Scale.y/2-5 && Velocity.y > -0.01)
					stick = stick || true;

				else
					stick = stick || false;
			}

			else
				stick = stick || true;
		}
	}
	break;

	}

	return false;
}

bool CKnife::Colliding(CObject* other)
{
	if (!Valid)
		return false;

	switch (State)
	{
	case ThrowingObjState::Neutral:
	{
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			CDoor* d = dynamic_cast<CDoor*>(arc);
			if (d)
			{
				if (d->GetOpen())
					stick = stick || false;

				else
					stick = stick || true;
			}

			else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
			{
				stick = stick || false;
			}

			else
				stick = stick || true;
		}
	}
	break;
	
	case ThrowingObjState::Stick:
	{
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			CDoor* d = dynamic_cast<CDoor*>(arc);
			if (d)
			{
				if (d->GetOpen())
					stick = stick || false;

				else
					stick = stick || true;
			}

			else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
			{
				stick = stick || true;
			}

			else
				stick = stick || true;
		}
	}
	break;

	case ThrowingObjState::OwnedByPlayer:
	{
		//아무것도 안함
	}
	break;

	case ThrowingObjState::Throwing:
	{
		//몬스터를 맞았을때
		CMonster* mon = dynamic_cast<CMonster*>(other);

		if (mon && mon->GetMainOrder() != Main_Order::End && mon->GetMainOrder() != Main_Order::GetHurt && mon->GetMainOrder() != Main_Order::GetBurn)
		{
			double Angle = atan2(Velocity.y, Velocity.x);

			mon->GetMainOrder() = Main_Order::GetHurt;
			mon->SetHurtAngle(Angle);
			Valid = false;

			CCameraMgr::Create()->SetCameraEffect(CameraEffect::SHAKE);
			CEffectMgr::Create()->HitEffect->Shoot(doublepoint{ Pos.x - 1500 * cos(Angle), Pos.y - 1500 * sin(Angle) }
				, doublepoint{ Pos.x + 1500 * cos(Angle), Pos.y + 1500 * sin(Angle) }
			, 30000, 2000);

		}

		//땅과 벽,천장, 문에 맞았을때
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			CDoor* d = dynamic_cast<CDoor*>(arc);
			if (d)
			{
				if (d->GetOpen())
					stick = stick || false;

				else
					stick = stick || true;
			}

			else if (CTopFloor* t = dynamic_cast<CTopFloor*>(arc))
			{
				if (t->GetPos().y - t->GetScale().y / 2 > Pos.y + Scale.y/2 - 5 && Velocity.y > -0.01)
					stick = stick || true;

				else
					stick = stick || false;
			}

			else
				stick = stick || true;
		}
	}
	break;

	}

	return false;
}

bool CKnife::CollideEscape(CObject* other)
{
	switch (State)
	{
	case ThrowingObjState::Stick:
	{
		CArchitecture* arc = dynamic_cast<CArchitecture*>(other);
		if (arc)
		{
			stick = stick || false;
		}
		
	}
		break;
	}

	return false;
}

bool CKnife::DontCollide(CObject* other)
{
	return false;
}