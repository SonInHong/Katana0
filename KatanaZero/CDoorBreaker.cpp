#include "pch.h"
#include "CDoorBreaker.h"
#include "TimeMgr.h"
#include "CDoor.h"
#include "CEventMgr.h"
#include "CCameraMgr.h"

CDoorBreaker::CDoorBreaker()
	:Owner(nullptr)
	, Speed(scaleA * 100)
	, OffSet{}
	, dir(0)
{
}

CDoorBreaker::~CDoorBreaker()
{
}

void CDoorBreaker::Initialize()
{	
	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;
	Scale.x = scaleA * 30;
	Scale.y = scaleA * 30;
	CreateCollider();
}

void CDoorBreaker::Enter()
{
}

void CDoorBreaker::Exit()
{
}

void CDoorBreaker::Update()
{
	OffSet.x += dir * Speed * TimeMgr::Create()->dt();

	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y;
}

bool CDoorBreaker::Collide(CObject* other)
{
	CDoor* p = dynamic_cast<CDoor*>(other);
	
	if (p)
	{
		p->SetOpen(true);
		CEventMgr::Create()->Event_DestroyObj(this);
		CCameraMgr::Create()->SetCameraEffect(CameraEffect::LITTLESHAKE);
	}
		
	
	return false;
}
