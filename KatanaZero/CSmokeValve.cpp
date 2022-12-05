#include "pch.h"
#include "CSmokeValve.h"
#include "CResourceMgr.h"
#include "CCameraMgr.h"
#include "CTexture.h"
#include "CSword.h"
#include "CBullet.h"
#include "ThrowingObj.h"

CSmokeValve::CSmokeValve()
    :Open(false)
{
    //텍스쳐 로딩
    Texture_Open = CResourceMgr::Create()->Load<CTexture>(L"SmokeValve\\SmokeOn.bmp");
    Texture_Closed = CResourceMgr::Create()->Load<CTexture>(L"SmokeValve\\SmokeOff.bmp");
 }

CSmokeValve::~CSmokeValve()
{
}

void CSmokeValve::Initialize()
{
    CreateCollider();

    SmokeEmitor = new CSmokeEmitor;
    SmokeEmitor->SetOwner(this);
    SmokeEmitor->SetOffset(doublepoint{ 0,Scale.y/2 });
    SmokeEmitor->Initialize();
   
}

void CSmokeValve::Enter()
{
    SmokeEmitor->Enter();
}

void CSmokeValve::Update()
{
    SmokeEmitor->Update();
}

void CSmokeValve::Render(HDC _dc)
{
    doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
    doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

    if (Open)
    {
        TransparentBlt(_dc, pos.x - scale.x / 2, pos.y - scale.y / 2, scale.x, scale.y, Texture_Open->GetDC(), 0, 0, Texture_Open->GetWidth(), Texture_Open->GetHeight(), RGB(255, 255, 255));
    }

    if (!Open)
    {
        TransparentBlt(_dc, pos.x - scale.x / 2, pos.y - scale.y / 2, scale.x, scale.y, Texture_Closed->GetDC(), 0, 0, Texture_Closed->GetWidth(), Texture_Closed->GetHeight(), RGB(255, 255, 255));
    }

    CObject::Render(_dc);
}

bool CSmokeValve::Collide(CObject* other)
{
    if (dynamic_cast<CSword*>(other) || dynamic_cast<ThrowingObj*>(other) || dynamic_cast<CBullet*>(other))
    {
        if(Open == false)
            SmokeEmitor->SetOption(50,doublepoint{ 2,2 }, doublepoint{ -30,210 }, doublepoint{ 100,1000 },
            doublepoint{ 0.5,0.7 }, doublepoint{ 0.1,0.2 }, doublepoint{ 0,0 }, doublepoint{ 0,0 });

        Open = true;
        SmokeEmitor->SetOnOff(true);
    }
        
    

    CWall_CantClimb::Collide(other);

    return false;
}

bool CSmokeValve::Colliding(CObject* other)
{
    if (dynamic_cast<CSword*>(other) || dynamic_cast<ThrowingObj*>(other) || dynamic_cast<CBullet*>(other))
        Open = true;

    CWall_CantClimb::Colliding(other);

    return false;
}

bool CSmokeValve::CollideEscape(CObject* other)
{
    CWall::CollideEscape(other);

    return false;
}

bool CSmokeValve::DontCollide(CObject* other)
{
    CWall::DontCollide(other);

    return false;
}
