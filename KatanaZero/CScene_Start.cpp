#include "pch.h"
#include "CScene_Start.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CMonster.h"
#include "CGrunt.h"
#include "CPomp.h"
#include "CGangster.h"
#include "CColliderMgr.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "CResourceMgr.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CFloor.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CWall.h"
#include "CFloorCeiling.h"
#include "CCursor.h"
#include "CNodeMgr.h"
#include "CParticleEmitor.h"
#include "CDirtyWaterEmitor.h"
#include "CDustcloudEmitor.h"
#include "CStairCensor.h"
#include "CDoor.h"
#include "CNormalDoorRight.h"
#include "CNormalDoorLeft.h"
#include "CLaser.h"
#include "CEffectMgr.h"
#include "CNeutralObjMgr.h"
#include "CWall_CantClimb.h"
#include "CWallDoor.h"
#include "CParticleMgr.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	//CCore::Create()->SetMenuBar(true);
	

	//ShowWindow(CCore::Create()->GetToolWindowData().hwnd, SW_NORMAL);
	//UpdateWindow(CCore::Create()->GetToolWindowData().hwnd);


	//노드 테스트
	

	//카메라리셋
	CCameraMgr::Create()->Reset();
		
	//이펙트매니저 이펙트들 넣기
	CEffectMgr::Create()->PutInBox();
	
	//투사체들 오브젝트 풀링
	CNeutralObjMgr::Create()->PutInBox();
	//파티클 오브젝트 풀링
	CParticleMgr::Create()->Enter();

	//Map 추가
	CMap* _pMap = new CMap;
	_pMap->Load(L"Map.tile",scaleA);
	AddObject(_pMap, GROUP_TYPE::MAP);

	//커서 추가
	CCursor* _pCursor = new CCursor;
	AddObject(_pCursor, GROUP_TYPE::CURSOR);
	ShowCursor(false);

	//Player 추가
	CPlayer* _pObj = new CPlayer;

	_pObj->SetPos(doublepoint{ scaleA*100,scaleA * 0 });
	_pObj->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pObj->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });

	AddObject(_pObj, GROUP_TYPE::PLAYER);

	//Monster 추가
	
	CGrunt* _pMon = new CGrunt;
	_pMon->SetPos(doublepoint{ scaleA * 400,scaleA * 200 });
	_pMon->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pMon->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pMon->SetRoamingPoint(doublepoint{ scaleA * 600,scaleA * 200 });
	_pMon->SetMainOrder(Main_Order::LeanLeft);
	AddObject(_pMon, GROUP_TYPE::MONSTER);
	
	CGangster* _pGang = new CGangster;
	_pGang->SetPos(doublepoint{ scaleA * 600,scaleA * 400 });
	_pGang->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pGang->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pGang->SetRoamingPoint(doublepoint{ scaleA * 600,scaleA * 500 });
	AddObject(_pGang, GROUP_TYPE::MONSTER);

	//_pMon = new CGrunt;
	//_pMon->SetPos(doublepoint{ scaleA * 600,scaleA * 400 });
	//_pMon->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	//_pMon->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	//_pMon->SetRoamingPoint(doublepoint{ scaleA * 300,scaleA * 500 });
	//AddObject(_pMon, GROUP_TYPE::MONSTER);
	
	CPomp* _pPom = new CPomp;
	_pPom->SetPos(doublepoint{ scaleA * 600,scaleA * 400 });
	_pPom->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pPom->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pPom->SetRoamingPoint(doublepoint{ scaleA * 300,scaleA * 500 });
	AddObject(_pPom, GROUP_TYPE::MONSTER);

	

	/*
	*/
	//Floor 추가
	
	CStair* _pStair1 = new CStair;                                 //오른쪽 큰 계단
	_pStair1->SetScale(doublepoint{ scaleA * 300, scaleA * 300 });
	_pStair1->SetPos(doublepoint{ scaleA * 905,scaleA * 438.5 });
	_pStair1->SetDir(Right);
	AddObject(_pStair1, GROUP_TYPE::FLOOR);


	CStair* _pStair2 = new CStair;                                         //왼쪽 작은 계단
	_pStair2->SetScale(doublepoint{ scaleA * 60,scaleA * 60 });
	_pStair2->SetPos(doublepoint{ scaleA * 510, scaleA * 318.5 });
	_pStair2->SetDir(Left);
	AddObject(_pStair2, GROUP_TYPE::FLOOR);

	CFloor* _pFloor1 = new CFloor;									//메인 바닥
	_pFloor1->SetScale(doublepoint{ scaleA * 800, scaleA * 60 });
	_pFloor1->SetPos(doublepoint{ scaleA * 370, scaleA * 607.5 });
	AddObject(_pFloor1, GROUP_TYPE::FLOOR);

	CFloor* _pFloor2 = new CFloor;											//2층계단 오른쪽 바닥
	_pFloor2->SetScale(doublepoint{ scaleA * 200,scaleA * 60 });
	_pFloor2->SetPos(doublepoint{ scaleA * 1156, scaleA * 318.5 });
	AddObject(_pFloor2, GROUP_TYPE::FLOOR);
		
	CFloor* _pFloor = new CFloor;
	_pFloor->SetScale(doublepoint{ scaleA * 480,scaleA * 60 });
	_pFloor->SetPos(doublepoint{ scaleA * 240, scaleA * 318.5 });
	//AddObject(_pFloor, GROUP_TYPE::FLOOR);

	

	CTopFloor* _pTopFloor1 = new CTopFloor;							//계단 왼쪽 바닥
	_pTopFloor1->SetScale(doublepoint{ scaleA * 575, scaleA * 60 });
	_pTopFloor1->SetPos(doublepoint{ scaleA * 767.5, scaleA * 318.5 });
	AddObject(_pTopFloor1, GROUP_TYPE::FLOOR);

	CTopFloor* _pTopFloor2 = new CTopFloor;										// 굴뚝 위 바닥
	 _pTopFloor2->SetScale(doublepoint{ scaleA * 200, scaleA * 35 });
	 _pTopFloor2->SetPos(doublepoint{ scaleA * 75, scaleA * 117 });
	AddObject(_pTopFloor2, GROUP_TYPE::FLOOR);

	CTopFloor* _pTopFloor3 = new CTopFloor;											//2층 문 뒤 바닥
	_pTopFloor3->SetScale(doublepoint{ scaleA * 480, scaleA * 60 });
	_pTopFloor3->SetPos(doublepoint{ scaleA * 240, scaleA * 318.5 });
	AddObject(_pTopFloor3, GROUP_TYPE::FLOOR);

	CFloorCeiling* _pCeiling = new CFloorCeiling;
	_pCeiling->SetScale(doublepoint{ scaleA * 60, scaleA * 10 });
	_pCeiling->SetPos(doublepoint{ scaleA * 480, scaleA * 220 });
	//AddObject(_pCeiling, GROUP_TYPE::FLOOR);


	//바닥 노드 연결 (몬스터 기준 이동방법)
	_pStair1->Connect(_pFloor1, Move_Order::MoveLeft);
	_pFloor1->Connect(_pStair1, Move_Order::MoveRight);

	_pStair1->Connect(_pFloor2, Move_Order::MoveRight);
	_pFloor2->Connect(_pStair1, Move_Order::ClimbDownLeft);

	_pFloor2->Connect(_pTopFloor1, Move_Order::MoveToTopFloorLeft);
	_pTopFloor1->Connect(_pFloor2, Move_Order::MoveRight);

	_pTopFloor1->Connect(_pTopFloor3, Move_Order::MoveToTopFloorLeft);
	_pTopFloor3->Connect(_pTopFloor1, Move_Order::MoveToTopFloorRight);

	_pTopFloor3->Connect(_pStair2, Move_Order::ClimbDownRight);
	_pStair2->Connect(_pTopFloor3, Move_Order::MoveToTopFloorLeft);

	//_pTopFloor3->Connect(_pTopFloor2, Move_Order::Stay);

	// 벽
	CWall* _pWall = new CWall;
	_pWall->SetScale(doublepoint{ scaleA * 35,scaleA * 700 });
	_pWall->SetPos(doublepoint{ scaleA * 17,scaleA * 300 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	_pWall = new CWall;
	_pWall->SetScale(doublepoint{ scaleA * 35,scaleA * 400 });
	_pWall->SetPos(doublepoint{ scaleA * 157,scaleA * 300 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	CWall_CantClimb* _pWall2 = new CWall_CantClimb;
	_pWall2->SetScale(doublepoint{ scaleA * 35,scaleA * 700 });
	_pWall2->SetPos(doublepoint{ scaleA * 1260,scaleA * 400 });
	AddObject(_pWall2, GROUP_TYPE::FLOOR);

	_pWall = new CWall;
	_pWall->SetScale(doublepoint{ scaleA * 60,scaleA * 225 });
	_pWall->SetPos(doublepoint{ scaleA * 480,scaleA * 112.5 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	CWallDoor* _pWallDoor = new CWallDoor;
	_pWallDoor->SetScale(doublepoint{ 60,300 });
	_pWallDoor->SetPos(doublepoint{ 800,200 });
	_pWallDoor->SetAnimationScaling(doublepoint{ 3,3 });
	AddObject(_pWallDoor, GROUP_TYPE::FLOOR);

	//문
	

	CNormalDoorRight* _pDoor = new CNormalDoorRight;
	_pDoor->SetScale(doublepoint{ scaleA * 30,scaleA * 127 });
	_pDoor->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pDoor->SetPos(doublepoint{ scaleA * 380,scaleA * 225 });
	AddObject(_pDoor, GROUP_TYPE::FLOOR);

	CNormalDoorLeft* _pDoor2 = new CNormalDoorLeft;
	_pDoor2->SetScale(doublepoint{ scaleA * 30,scaleA * 127 });
	_pDoor2->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pDoor2->SetPos(doublepoint{ scaleA * 680,scaleA * 225 });
	AddObject(_pDoor2, GROUP_TYPE::FLOOR);


	//레이저
	CLaser* _pLaser = new CLaser;
	_pLaser->SetPos(doublepoint{ scaleA * 900,scaleA * 180 });
	_pLaser->SetScale(doublepoint{ scaleA * 30,scaleA * 200 });
	_pLaser->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	AddObject(_pLaser, GROUP_TYPE::TRAP);

	
	//
	CNeutralObjMgr::Create()->Knife->SetPos(doublepoint{ 1000,400 });
	CNeutralObjMgr::Create()->Knife->SetOwner(_pObj);
	CNeutralObjMgr::Create()->Knife->SetValid(true);
	CNeutralObjMgr::Create()->Knife->SetState(ThrowingObjState::Neutral);


	CParticleMgr::Create()->Enter();

	//UI 추가
	/*CPanelUI* _pUI = new CPanelUI;
	CButtonUI* _cUI = new CButtonUI;
	_pUI->setOffset(doublepoint{ 700, 0 });
	_pUI->SetScale(doublepoint{ 600, 300 });
	_pUI->setWindowData(CCore::Create()->GetWindowData());
	_cUI->setOffset(doublepoint{ 0, 0 });
	_cUI->SetScale(doublepoint{ 100, 50 });

	_pUI->setChild(_cUI);

	AddObject(_pUI, GROUP_TYPE::UI);*/

	

	//Obj 초기화
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->Initialize();
		}
	}

	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->Enter();
		}
	}

	//충돌 그룹 지정
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::MONSTER);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::CENSOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER_PROJECTILE);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::TRAP);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TRAP);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::MONSTER_PROJECTILE);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::NEUTRAL_OBJECT, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::NEUTRAL_OBJECT, GROUP_TYPE::PLAYER);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::NEUTRAL_OBJECT, GROUP_TYPE::MONSTER);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::NEUTRAL_OBJECT, GROUP_TYPE::PLAYER_PROJECTILE);

	//카메라 모드 지정
	CCameraMgr::Create()->SetCameraMode(CameraMode::FollowPlayer);
	CCameraMgr::Create()->SetCameraLimit(_pMap->GetPos().x-300, _pMap->GetPos().x +_pMap->GetScale().x+300, _pMap->GetPos().y - 300, _pMap->GetPos().y + _pMap->GetScale().y + 300);
}

void CScene_Start::Exit()
{
	//충돌 그룹 해제
	CColliderMgr::Create()->Reset();

	//커서 다시 보이게
	ShowCursor(true);
}

void CScene_Start::Update()
{
	CScene::Update();
}

void CScene_Start::Render(HDC _dc)
{
	CScene::Render(_dc);

	
}
