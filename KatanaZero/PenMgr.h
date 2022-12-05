#pragma once
class PenMgr
{
private:
	PenMgr();
	~PenMgr();

public:
	static PenMgr* Create() { static PenMgr p; return &p; }

	void Initialize();
	HPEN GetGreenPen() { return GreenPen; }
	HPEN GetRedPen() { return RedPen; }
	HBRUSH GetSkyBrush() { return SkyBrush; }
	HBRUSH GetYellowBrush() { return YellowBrush; }
	HBRUSH GetBrush(PenColor c)
	{
		switch (c)
		{
		case PenColor::SKY:
			return SkyBrush;
			break;

		case PenColor::YELLOW:
			return YellowBrush;
			break;

		case PenColor::MAGENTA:
			return MagentaBrush;
			break;

		case PenColor::AQUA:
			return AquaBrush;
			break;

		case PenColor::BLACK:
			return BlackBrush;
			break;
		}

		return 0;
	}

	COLORREF GetRGB(PenColor c)
	{
		switch (c)
		{
		case PenColor::SKY:
			return RGB(102, 255, 255);
			break;

		case PenColor::YELLOW:
			return RGB(255, 212, 0);
			break;

		case PenColor::MAGENTA:
			return RGB(255, 0, 255);
			break;

		case PenColor::AQUA:
			return RGB(0, 255, 255);
			break;

		case PenColor::BLACK:
			return RGB(0, 0, 0);
			break;
		}

		return 0;
	}

private:
	HPEN GreenPen = 0;
	HPEN RedPen = 0;
	HBRUSH SkyBrush = 0;
	HBRUSH YellowBrush = 0;
	HBRUSH MagentaBrush = 0;
	HBRUSH AquaBrush = 0;
	HBRUSH BlackBrush = 0;
};

