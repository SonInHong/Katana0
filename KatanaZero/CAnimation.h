#pragma once
#include "CTexture.h"
#include "CCore.h"



class CAnimator;

class CAnimation
{
public:
	struct Event
	{
		std::vector<std::function<void()>> func;

		void operator = (std::function<void()> F)
		{
			func.push_back(std::move(F));
		}

		void operator () (void)
		{
			if (!func.empty())
			{
				for (int i = 0; i < func.size(); ++i)
					func[i]();
			}
		}
	};

	struct ImageSave
	{
		int Index;
		doublepoint Pos;
		doublepoint Size;
	};

	CAnimation();
	~CAnimation();

	void Update();
	void Render(HDC _dc);
	void ReRender(HDC _dc);

	std::wstring GetName() {
		if (this == nullptr)
			return L"";

		return m_Name;
		
	}

	void AfterImageOn( PenColor c)
	{
		AfterImage.clear();
		AfterImageOnOff = true;
		AfterColor = c;

	}

	void AfterImageOn(double timelimit , PenColor c)
	{
		AfterImage.clear();
		AfterImageOnOff = true;

		AfterTimeLimit = timelimit;
		AfterTimerOnOff = true;
		AfterColor = c;
	}

	void Reset();
	int& GetCurIndex() { return m_CurIndex; }
	
	void SetOffSet(doublepoint b) { OffSet = b; }

	friend class CAnimator;

private:
	CAnimator* m_Owner;
	std::wstring m_Name;
	CTexture* m_Texture;
	doublepoint m_LeftTop;
	doublepoint m_SpriteSize;
	int m_FrameCount;
	int m_CurIndex;
	double m_FrameTime;
	bool m_Loop;
	bool m_Complete;
	bool m_Start;
	int a = 0;

	double Timer;

	doublepoint OffSet;

	
	
	bool AfterImageOnOff;
	std::vector<ImageSave> AfterImage;
	CTexture* AfterTexture;

	


	void AfterImageOff() { AfterImageOnOff = false; AfterImage.clear(); }
	
	int BurnIndex;
	bool Burn;
	double Burn_Yalpha;

public:
	Event m_StartEvent, m_EndEvent, m_CompleteEvent;

	double Aftert;
	double AfterTimeLimit;
	bool AfterTimerOnOff;
	PenColor AfterColor;


};

