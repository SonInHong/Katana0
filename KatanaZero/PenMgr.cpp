#include "pch.h"
#include "PenMgr.h"

PenMgr::PenMgr()
{
}

PenMgr::~PenMgr()
{
}

void PenMgr::Initialize()
{
	GreenPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	RedPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	SkyBrush = CreateSolidBrush(RGB(102, 255, 255));  //
	YellowBrush = CreateSolidBrush(RGB(255, 212, 0));
	MagentaBrush = CreateSolidBrush(RGB(255, 0, 255));
	AquaBrush = CreateSolidBrush(RGB(0, 255, 255));
	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
}
