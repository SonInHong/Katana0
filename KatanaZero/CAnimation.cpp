#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "TimeMgr.h"
#include "CCameraMgr.h"
#include <functional>
#include "PenMgr.h"
#include "CAnimal.h"
#include "CLaserparticleEmitor.h"
#pragma comment(lib, "Msimg32.lib")

CAnimation::CAnimation()
	:m_Owner(nullptr)
	, m_Complete(false)
	, m_CurIndex(0)
	, m_FrameCount(0)
	, m_FrameTime(0)
	, m_LeftTop{}
	, m_Loop(false)
	, m_Name{}
	, m_SpriteSize{}
	, m_Texture(nullptr)
	, m_Start(true)
	, OffSet{}
	, AfterImageOnOff(false)
	,AfterImage{}
	,BurnIndex(0)
	,Burn(false)
	,Burn_Yalpha(0)
	, Aftert(0)
	, AfterTimeLimit(0)
	, AfterTimerOnOff(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Update()
{
	Timer += TimeMgr::Create()->dt();
	Aftert += TimeMgr::Create()->dt(); // 잔상효과 타이머

	if (m_Start == true)
	{
		m_StartEvent();
		m_Start = false;

		if (AfterTimerOnOff) // 잔상효과 타이머
		{
			Aftert = 0;
		}
	}

	if (Burn)
	{
		BurnIndex = m_CurIndex;
		Burn_Yalpha += 30*TimeMgr::Create()->dt();

		return;
	}

	

	if (Timer >= m_FrameTime)
	{
		if (m_CurIndex >= m_FrameCount - 1)
		{
			if (m_Loop == true)
			{
				m_EndEvent();
				m_CurIndex = 0;

			}


			else
			{
				if (m_Complete == false)
				{
					m_CompleteEvent();
					
					
				}
					

				m_Complete = true;

			}
		}

		else
			++m_CurIndex;

		//인덱스 체크해서 루프 애니메이션이면 인덱스 0으로 초기화 아니면 컴플리트

		Timer = 0;

		//타이머 초기화
	}
}

void CAnimation::Render(HDC _dc)
{
	

	doublepoint Pos = m_Owner->GetOwner()->GetPos() + OffSet;
	doublepoint Scale = doublepoint{ (double)m_SpriteSize.x, (double)m_SpriteSize.y };
	doublepoint Resize = m_Owner->GetOwner()->GetResize();
	doublepoint AnimationScaling = m_Owner->GetOwner()->GetAnimationScaling();

	if (m_Owner->GetOwner()->GetCameraONOFF() == true)
	{
		Pos = CCameraMgr::Create()->CameraCoordinate(Pos);
		Scale = CCameraMgr::Create()->CameraScale(Scale);
	}

	BLENDFUNCTION func0 = {};
	func0.AlphaFormat = AC_SRC_ALPHA;
	func0.BlendFlags = 0;
	func0.BlendOp = AC_SRC_OVER;
	func0.AlphaFormat = AC_SRC_ALPHA;
	func0.SourceConstantAlpha = 255;

	BLENDFUNCTION func = {};
	func.AlphaFormat = AC_SRC_ALPHA;
	func.BlendFlags = 0;
	func.BlendOp = AC_SRC_OVER;
	func.AlphaFormat = AC_SRC_ALPHA;
	func.SourceConstantAlpha = 10;

	BLENDFUNCTION func2 = {};
	func2.AlphaFormat = AC_SRC_ALPHA;
	func2.BlendFlags = 0;
	func2.BlendOp = AC_SRC_OVER;
	func2.AlphaFormat = 0;
	func2.SourceConstantAlpha = 7;

	BLENDFUNCTION func3 = {};
	func3.AlphaFormat = AC_SRC_ALPHA;
	func3.BlendFlags = 0;
	func3.BlendOp = AC_SRC_OVER;
	func3.AlphaFormat = 0;
	func3.SourceConstantAlpha = 120;

	//AlphaBlend(_dc, Pos.x - m_SpriteSize.x / 2, Pos.y - m_SpriteSize.y / 2, m_SpriteSize.x, m_SpriteSize.y 
		//,m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * m_CurIndex, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, func);


	if (Burn)
	{
		HDC com_DC = CCameraMgr::Create()->GetcomDC();

		doublepoint Scale2 = CCameraMgr::Create()->CameraScale(doublepoint{ (double)m_SpriteSize.x, (double)m_SpriteSize.y } - doublepoint{ 0,Burn_Yalpha });

		SelectObject(com_DC, PenMgr::Create()->GetYellowBrush());

		Rectangle(com_DC, -1, -1, m_SpriteSize.x + 1, m_SpriteSize.y + 1);
		
		TransparentBlt(com_DC, 0, Burn_Yalpha, m_SpriteSize.x, m_SpriteSize.y-Burn_Yalpha
			, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * BurnIndex, m_LeftTop.y+Burn_Yalpha
			, m_SpriteSize.x, m_SpriteSize.y - Burn_Yalpha, RGB(255,255,255));

		AlphaBlend(com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y
			, CCameraMgr::Create()->GetYellowDC(), 0, 0, m_SpriteSize.x, m_SpriteSize.y, func3);

		TransparentBlt(_dc, Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2, Pos.y - AnimationScaling.y * Resize.y * Scale.y / 2, AnimationScaling.x * Resize.x * Scale.x, AnimationScaling.y * Resize.y * Scale.y
			, com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y, RGB(255,212,0));

		SelectObject(com_DC, PenMgr::Create()->GetSkyBrush());

		//여기서 레이저파티클 이미터의 오프셋 지정
		CAnimal* p = dynamic_cast<CAnimal*>(m_Owner->GetOwner());
		if (p)
		{
			if (Burn_Yalpha > m_SpriteSize.y)
			{
				p->GetLaserparticleEmitor()->SetOnOff(false);
				p->GetMainOrder() = Main_Order::Dead;
			}
				
			
			p->GetLaserparticleEmitor()->SetOffset({ OffSet.x,OffSet.y - m_SpriteSize.y * AnimationScaling.y * Resize.y / 2 + Burn_Yalpha * AnimationScaling.y * Resize.y });


		}

		m_Owner->ReRendering = false;

		return;

	}

	if ((int)(Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2) > 2000 ||
		(int)(Pos.y - AnimationScaling.y * Resize.y * Scale.y / 2) > 1300 ||
			(int)(Pos.x + AnimationScaling.x * Resize.x * Scale.x / 2) < 0 ||
			(int)(Pos.y + AnimationScaling.y * Resize.y * Scale.y / 2) < 0 )
			return;


	TransparentBlt(_dc, (int)(Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2) , (int)(Pos.y - AnimationScaling.y * Resize.y* Scale.y / 2)
		, (int)(AnimationScaling.x* Resize.x* Scale.x), (int)(AnimationScaling.y * Resize.y* Scale.y)
		, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * m_CurIndex, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, RGB(255,255,255));

	if (AfterImageOnOff == true)
	{
		if (AfterTimerOnOff && Aftert > AfterTimeLimit)
			return;

		HBRUSH brush = PenMgr::Create()->GetBrush(AfterColor);
		HDC com_DC = CCameraMgr::Create()->GetcomDC();
		HBRUSH oldbrush = (HBRUSH)SelectObject(com_DC, brush);

		for (int i = AfterImage.size()-1; i >=0; --i)
		{
			if ((AfterImage[i].Pos - (m_Owner->GetOwner()->GetPos() + OffSet)).Norm() > 70)
				break;

			doublepoint pos = CCameraMgr::Create()->CameraCoordinate(AfterImage[i].Pos);
			doublepoint size = CCameraMgr::Create()->CameraScale(AfterImage[i].Size);
			size.x = size.x * Resize.x * AnimationScaling.x;
			size.y = size.y * Resize.y * AnimationScaling.y;
			int index = AfterImage[i].Index;

			Rectangle(com_DC, -1, -1, m_SpriteSize.x + 1, m_SpriteSize.y + 1);

			AlphaBlend(com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y
				, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * index, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, func2);

			AlphaBlend(_dc, pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y, com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y, func);

			
		}

		ImageSave IS = { m_CurIndex, m_Owner->GetOwner()->GetPos() + OffSet, doublepoint{ (double)m_SpriteSize.x, (double)m_SpriteSize.y } };
		AfterImage.push_back(IS);


		SelectObject(com_DC, oldbrush);
	}
}

void CAnimation::ReRender(HDC _dc)
{
	doublepoint Pos = m_Owner->GetOwner()->GetPos() + OffSet;
	doublepoint Scale = doublepoint{ (double)m_SpriteSize.x, (double)m_SpriteSize.y };
	doublepoint Resize = m_Owner->GetOwner()->GetResize();
	doublepoint AnimationScaling = m_Owner->GetOwner()->GetAnimationScaling();


	if (m_Owner->GetOwner()->GetCameraONOFF() == true)
	{
		Pos = CCameraMgr::Create()->CameraCoordinate(Pos);
		Scale = CCameraMgr::Create()->CameraScale(Scale);
	}

	BLENDFUNCTION func3 = {};
	func3.AlphaFormat = AC_SRC_ALPHA;
	func3.BlendFlags = 0;
	func3.BlendOp = AC_SRC_OVER;
	func3.AlphaFormat = 0;
	func3.SourceConstantAlpha = m_Owner->rerender_alpha;


	HDC com_DC = CCameraMgr::Create()->GetcomDC();

	SelectObject(com_DC, PenMgr::Create()->GetBrush(m_Owner->rerender_color));

	Rectangle(com_DC, -1, -1, m_SpriteSize.x + 1, m_SpriteSize.y + 1);   // 도화지 준비

	TransparentBlt(com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y
		, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * m_CurIndex, m_LeftTop.y 
		, m_SpriteSize.x, m_SpriteSize.y, RGB(255, 255, 255));  // 그 위에 그리고

	AlphaBlend(com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y
		, CCameraMgr::Create()->GetColorDC(m_Owner->rerender_color), 0, 0, m_SpriteSize.x, m_SpriteSize.y, func3); // 그 위에 색 덫칠

	TransparentBlt(_dc, Pos.x - AnimationScaling.x * Resize.x * Scale.x / 2, Pos.y - AnimationScaling.y * Resize.y * Scale.y / 2, AnimationScaling.x * Resize.x * Scale.x, AnimationScaling.y * Resize.y * Scale.y
		, com_DC, 0, 0, m_SpriteSize.x, m_SpriteSize.y, PenMgr::Create()->GetRGB(m_Owner->rerender_color));

	SelectObject(com_DC, PenMgr::Create()->GetSkyBrush());
}

void CAnimation::Reset()
{
	m_Start = true;
	m_Complete = false;
	m_CurIndex = 0;
	
	AfterImage.clear();
	//AfterImageOnOff = false;

}
