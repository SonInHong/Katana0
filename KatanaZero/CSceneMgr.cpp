#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CScene_Stage2.h"
#include "CCore.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "CCameraMgr.h"

CSceneMgr::CSceneMgr()
	:m_arrScene{}
	, m_pCurScene{}
{
}

CSceneMgr::~CSceneMgr()
{
}

void CSceneMgr::Initialize()
{
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Stage2;

	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::STAGE_02];

	
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
}

void CSceneMgr::ComponentUpdate()
{
	m_pCurScene->ComponentUpdate();


}

void CSceneMgr::Clean()
{
	m_pCurScene->Clean();
}

void CSceneMgr::Render(HDC _dc)
{
	WindowData temp = CCore::Create()->GetWindowData();

	HBRUSH oldbrush = (HBRUSH)SelectObject(_dc, GetStockObject(BLACK_BRUSH));
	Rectangle(_dc, -1, -1, temp.width + 1, temp.height + 1);
	SelectObject(_dc, oldbrush);

	m_pCurScene->Render(_dc);
	
	BitBlt(temp.hdc, 0, 0, temp.width, temp.height, _dc, 0, 0, SRCCOPY);
}
