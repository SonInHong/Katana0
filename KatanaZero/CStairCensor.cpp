#include "pch.h"
#include "CStairCensor.h"
#include "CRigidBody.h"

CStairCensor::CStairCensor()
{
	Scale.x = 100; Scale.y = 5;
	CreateCollider();
}

CStairCensor::~CStairCensor()
{
}

bool CStairCensor::Collide(CObject* other)
{

	CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
	if (p)
		p->SetOnStair(true);

	return false;
}

bool CStairCensor::Colliding(CObject* other)
{
	CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
	if (p)
		p->SetOnStair(true);

	return false;
}

bool CStairCensor::CollideEscape(CObject* other)
{
	return false;
}

bool CStairCensor::DontCollide(CObject* other)
{
	return false;
}

void CStairCensor::Render(HDC _dc)
{
	CObject::Render(_dc);
}

void CStairCensor::Enter()
{
}

void CStairCensor::Exit()
{
}
