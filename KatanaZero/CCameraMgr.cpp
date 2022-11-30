#include "pch.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "PenMgr.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"

CCameraMgr::CCameraMgr()
    :LeftTop{}
    , Scale{}
    , effect(CameraEffect::End)
    , Timer(0)
    , OffSet{}
    , MarkPoint{}
    , shaker(0)
    , mode(CameraMode::DontMove)
    , LeftLimit(-10000)
    ,RightLimit(10000)
    ,UpperLimit(-10000)
    ,BottomLimit(10000)
    ,frametime(0)
{
}

CCameraMgr::~CCameraMgr()
{
}

doublepoint CCameraMgr::RealCoordinate(doublepoint CameraPos)
{
    double height = (double)CCore::Create()->GetWindowData().height;
    double width = (double)CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = CameraPos.x * (Scale.x / width) + LeftTop.x;
    y.y = CameraPos.y * (Scale.y / height) + LeftTop.y;

    return y;
}

doublepoint CCameraMgr::CameraCoordinate(doublepoint AbsPos)
{
    double height = (double)CCore::Create()->GetWindowData().height;
    double width = (double)CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = floor((AbsPos.x - LeftTop.x) * (width / Scale.x));
        y.y = floor((AbsPos.y - LeftTop.y) * (height / Scale.y)); 

    return y;
}

doublepoint CCameraMgr::CameraScale(doublepoint AbsScale)
{
    double height = (double)CCore::Create()->GetWindowData().height;
    double width = (double)CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = floor( AbsScale.x* (width / Scale.x));
    y.y = floor(AbsScale.y * (height / Scale.y));

    return y;
}

doublepoint CCameraMgr::RealScale(doublepoint CameraScale)
{
    double height = (double)CCore::Create()->GetWindowData().height;
    double width = (double)CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = CameraScale.x * (Scale.x / width);
    y.y = CameraScale.y * (Scale.y / height);

    return y;
}

#define alpha 1

void CCameraMgr::Reset()
{
    LeftTop.x = 0;
    LeftTop.y = 0;

    MarkPoint.x = 0;
    MarkPoint.y = 0;

    OffSet.x = 0;
    OffSet.y = 0;

    Timer = 0;

    Scale.y = floor(alpha * CCore::Create()->GetWindowData().height);
    Scale.x = floor(alpha * CCore::Create()->GetWindowData().width);
}

void CCameraMgr::Update()
{
    MoveCamera();

    
    
}

void CCameraMgr::LateUpdate()
{
    switch (effect)
    {
    case CameraEffect::End:
    {} break;

    case CameraEffect::SHAKE:
    {
        if (Timer > 0.25)
        {
            shaker -= TimeMgr::Create()->dt();
        }

        else if (Timer <= 0.25)
        {
            shaker += TimeMgr::Create()->dt();
        }

        if (Timer > 0.5)
        {
            effect = CameraEffect::End;
            Timer = 0;
            shaker = 0;
            OffSet = doublepoint{};
        }

        Timer += TimeMgr::Create()->dt();

        OffSet = doublepoint{ 30 * shaker * cos(100 * shaker),30 * shaker * sin(100 * shaker) };

    }
    break;

    case CameraEffect::LITTLESHAKE:
    {
        if (Timer > 0.1)
        {
            shaker -= TimeMgr::Create()->dt();
        }

        else if (Timer <= 0.1)
        {
            shaker += TimeMgr::Create()->dt();
        }

        if (Timer > 0.2)
        {
            effect = CameraEffect::End;
            Timer = 0;
            shaker = 0;
            OffSet = doublepoint{};
        }

        Timer += TimeMgr::Create()->dt();

        OffSet = doublepoint{ 20 * shaker * cos(70 * shaker),20 * shaker * sin(70 * shaker) };

    }
    break;

    }




    //=========================================================================================================
    switch (mode)
    {
    case CameraMode::FollowPlayer:
    {
        CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);

        if (player)
        {
            doublepoint PlcameraPos = CameraCoordinate(player->GetPos());
            doublepoint Center = doublepoint{ (double)CCore::Create()->GetWindowData().width / 2, (double)CCore::Create()->GetWindowData().height / 2 };

            if ((PlcameraPos - Center).Norm() > 10)
                MarkPoint = MarkPoint + (PlcameraPos - Center) * 3 * TimeMgr::Create()->dt();
        }

    }
    break;

    }

    //=========================================================================================================


    if (MarkPoint.x < LeftLimit)
        MarkPoint.x = LeftLimit;

    if (MarkPoint.x + Scale.x > RightLimit)
        MarkPoint.x = RightLimit - Scale.x;

    if (MarkPoint.y < UpperLimit)
        MarkPoint.y = UpperLimit;

    if (MarkPoint.y + Scale.y > BottomLimit)
        MarkPoint.y = BottomLimit - Scale.y;




    LeftTop.x = floor(MarkPoint.x + OffSet.x);
    LeftTop.y = floor(MarkPoint.y + OffSet.y);
    
}

#define MOVESPEED 200

void CCameraMgr::MoveCamera()
{
    if (KeyMgr::Create()->key(Key::UP).pressed == true)
    {
        MarkPoint.y -= MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::DOWN).pressed == true)
    {
        MarkPoint.y += MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::LEFT).pressed == true)
    {
        MarkPoint.x -= MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::RIGHT).pressed == true)
    {
        MarkPoint.x += MOVESPEED * TimeMgr::Create()->dt();
    }
}

void CCameraMgr::Initialize()
{
    yellowDC = CreateCompatibleDC(CCore::Create()->GetToolWindowData().hdc);
    yellowBitmap = CreateCompatibleBitmap(CCore::Create()->GetToolWindowData().hdc, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);

    SelectObject(yellowDC, yellowBitmap);

    blackDC = CreateCompatibleDC(CCore::Create()->GetToolWindowData().hdc);
    blackBitmap = CreateCompatibleBitmap(CCore::Create()->GetToolWindowData().hdc, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);

    SelectObject(blackDC, blackBitmap);

    HBRUSH yellow = CreateSolidBrush(RGB(255, 212, 0));
    DeleteObject((HBRUSH)SelectObject(yellowDC, yellow));

    DeleteObject((HBRUSH)SelectObject(blackDC, GetStockObject(BLACK_BRUSH)));

    Rectangle(yellowDC, -1, -1, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);
    Rectangle(blackDC, -1, -1, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);

    comDC = CreateCompatibleDC(CCore::Create()->GetToolWindowData().hdc);
    comBitmap = CreateCompatibleBitmap(CCore::Create()->GetToolWindowData().hdc, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);
    HBITMAP temp = (HBITMAP)SelectObject(comDC, comBitmap);
    DeleteObject(temp);
    SelectObject(comDC, PenMgr::Create()->GetSkyBrush());

}
