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
    ,rendermode(CameraRenderMode::NONE)
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

        OffSet = doublepoint{ 100 * shaker * cos(200 * shaker),50 * shaker * sin(100 * shaker) };

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

        OffSet = doublepoint{ 40 * shaker * cos(70 * shaker),40 * shaker * sin(70 * shaker) };

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
    yellowDC = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);
    yellowBitmap = CreateCompatibleBitmap(CCore::Create()->GetWindowData().hdc, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);

    SelectObject(yellowDC, yellowBitmap);

    blackDC = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);
    blackBitmap = CreateCompatibleBitmap(CCore::Create()->GetWindowData().hdc, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);

    SelectObject(blackDC, blackBitmap);

    HBRUSH yellow = CreateSolidBrush(RGB(255, 212, 0));
    DeleteObject((HBRUSH)SelectObject(yellowDC, yellow));

    DeleteObject((HBRUSH)SelectObject(blackDC, GetStockObject(BLACK_BRUSH)));

    Rectangle(yellowDC, -1, -1, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);
    Rectangle(blackDC, -1, -1, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);

    comDC = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);
    comBitmap = CreateCompatibleBitmap(CCore::Create()->GetWindowData().hdc, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);
    HBITMAP temp = (HBITMAP)SelectObject(comDC, comBitmap);
    DeleteObject(temp);
    SelectObject(comDC, PenMgr::Create()->GetSkyBrush());

    AquaDC = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);
    AquaBitmap = CreateCompatibleBitmap(CCore::Create()->GetWindowData().hdc, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);
    HBITMAP temp2 = (HBITMAP)SelectObject(AquaDC, AquaBitmap);
    DeleteObject(temp2);
    SelectObject(AquaDC, PenMgr::Create()->GetBrush(PenColor::AQUA));
    Rectangle(AquaDC, -1, -1, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height);

}

void CCameraMgr::Render(HDC _dc)
{
    static double t = 0;
    
    switch (rendermode)
    {
    case CameraRenderMode::SubtleFadeOut:
    {
        t += 2*TimeMgr::Create()->realdt();
        if (t > 0.7)
            t = 0.7;
        
    }
    break;

    case CameraRenderMode::SubtleFadeIn:
    {
        t -= 2*TimeMgr::Create()->realdt();
        if (t < 0)
            t = 0;
    }
    break;
    }

    BLENDFUNCTION func = {};
    func.AlphaFormat = AC_SRC_ALPHA;
    func.BlendFlags = 0;
    func.BlendOp = AC_SRC_OVER;
    func.AlphaFormat = 0;
    func.SourceConstantAlpha = 255*t;

    if(t>0)
        AlphaBlend(_dc, 0, 0, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height, blackDC, 0, 0, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height
        , func);
}
