#include "pch.h"
#include "CWall_CantClimb.h"
#include "CPlayer.h"

bool CWall_CantClimb::Collide(CObject* other)
{
	CWall::Collide(other);
	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p)
		p->SetCantClimb(true);

	return false;
}

bool CWall_CantClimb::Colliding(CObject* other)
{
	CWall::Collide(other);
	CPlayer* p = dynamic_cast<CPlayer*>(other);
	if (p)
		p->SetCantClimb(true);
	return false;
}
