#pragma once
class CCameraMgr
{
private:
	CCameraMgr();
	~CCameraMgr();

public:
	static CCameraMgr* Create() { static CCameraMgr p; return &p; }

	doublepoint RealCoordinate(doublepoint CameraPos);
	doublepoint CameraCoordinate(doublepoint AbsPos);
	doublepoint CameraScale(doublepoint AbsScale);
	doublepoint RealScale(doublepoint CameraScale);

	void SetLeftTop(doublepoint _lt) { LeftTop = _lt; }
	doublepoint GetLeftTop() { return LeftTop; }
	void SetScale(doublepoint _sc) { Scale = _sc; }
	doublepoint GetScale() { return Scale; }
	void SetCenter(doublepoint _c) { LeftTop.x = _c.x - Scale.x / 2; LeftTop.y = _c.y - Scale.y / 2; }
	void Reset();
	void Update();
	void LateUpdate();
	void MoveCamera();

	void Initialize();
	HDC GetYellowDC() { return yellowDC; }
	HDC GetBlackDC() { return blackDC; }
	HDC GetcomDC() { return comDC; }
	HDC GetAquaDC() { return AquaDC; }

	HDC GetColorDC(PenColor c)
	{
		switch (c)
		{
		
		case PenColor::YELLOW:
			return yellowDC;
			break;


		case PenColor::AQUA:
			return AquaDC;
			break;

		case PenColor::BLACK:
			return blackDC;
			break;
		}

		return 0;
	}

	void SetCameraEffect(CameraEffect p) { effect = p; }
	void SetCameraMode(CameraMode p) { mode = p; }
	void SetCameraLimit(double left, double right, double upper, double bottom) { LeftLimit = left; RightLimit = right; UpperLimit = upper; BottomLimit = bottom; }
	void SetCameraRenderMode(CameraRenderMode p) { rendermode = p; }

	void Render(HDC _dc);

private:
	doublepoint LeftTop;
	doublepoint Scale;

	HDC yellowDC;
	HBITMAP yellowBitmap;
	HDC blackDC;
	HBITMAP blackBitmap;

	HDC AquaDC;
	HBITMAP AquaBitmap;

	HDC comDC;
	HBITMAP comBitmap;

	CameraEffect effect;
	double Timer;
	double shaker;
	doublepoint OffSet;
	doublepoint MarkPoint;

	double LeftLimit;
	double RightLimit;
	double UpperLimit;
	double BottomLimit;

	CameraMode mode;
	CameraRenderMode rendermode;

	double frametime;

};

