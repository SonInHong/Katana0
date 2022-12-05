#include "pch.h"
#include "CLaser.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CCameraMgr.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CCameraMgr.h"
#include "KeyMgr.h"
#include "CResourceMgr.h"

CLaser::CLaser()
    :Texture(nullptr)
    ,OnOff(true)
    , CollisionOn(false)
{
    
    CreateAnimator();
}

CLaser::~CLaser()
{
}

void CLaser::Initialize()
{
    CreateCollider();

    dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Laser\\NoCollision", L"LaserNoCollision", doublepoint{ 0,0 }, doublepoint{ 1,Scale.y*Resize.y/AnimationScaling.y }, 4, 0.03, true);
    dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Laser\\Collision", L"LaserCollision", doublepoint{ 0,0 }, doublepoint{ 2,Scale.y * Resize.y / AnimationScaling.y }, 4, 0.03, false);
    Texture = CResourceMgr::Create()->Load<CTexture>(L"Laser\\spr_ceiling_laser_on\\0.bmp");
   
    dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
        ->FindAnimation(L"LaserCollision")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"LaserNoCollision");

}

void CLaser::Enter()
{
}

void CLaser::Exit()
{
}

void CLaser::Update()
{
    if (KeyMgr::Create()->key(Key::L).key_state == KeyState::TAP)
    {
        if (OnOff == false)
            OnOff = true;

        else
            OnOff = false;
    }


    if (OnOff)
    {
        if(dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName()!=L"LaserCollision")
            dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"LaserNoCollision");
    }
       

    else
        dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Dont_Play_Anything();

    CollisionOn = false;
}

void CLaser::Render(HDC _dc)
{
    doublepoint pos = CCameraMgr::Create()->CameraCoordinate(Pos);
    doublepoint scale = CCameraMgr::Create()->CameraScale(Scale);

    TransparentBlt(_dc, pos.x - Texture->GetWidth() * Resize.x * AnimationScaling.x / 2, pos.y - scale.y * Resize.y/2 - AnimationScaling.y*Texture->GetHeight()
        , Texture->GetWidth() * Resize.x * AnimationScaling.x , Texture->GetHeight() * Resize.y * AnimationScaling.y, Texture->GetDC()
        , 0, 0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));

    CObject::Render(_dc);
}

bool CLaser::Collide(CObject* other)
{
    if (OnOff)
    {

        CMonster* mon = dynamic_cast<CMonster*>(other);

        if (mon)
        {
            mon->SetMainOrder(Main_Order::GetBurn);
        }

        CPlayer* ply = dynamic_cast<CPlayer*>(other);

        if (ply && ply->GetUnbeatable() == false)
        {
            ply->SetMainOrder(Main_Order::GetBurn);
        }

        CollisionOn = true;

        dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"LaserCollision");
        CCameraMgr::Create()->SetCameraEffect(CameraEffect::LITTLESHAKE);
    }

    return false;
}

bool CLaser::Colliding(CObject* other)
{
    if (OnOff)
    {

        CMonster* mon = dynamic_cast<CMonster*>(other);

        if (mon)
        {
            mon->SetMainOrder(Main_Order::GetBurn);
        }

        CPlayer* ply = dynamic_cast<CPlayer*>(other);

        if (ply && ply->GetRoll() == 0)
        {
            ply->SetMainOrder(Main_Order::GetBurn);
        }

        CollisionOn = true;
    }

    return false;
}
