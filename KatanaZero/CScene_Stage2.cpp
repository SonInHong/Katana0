#include "pch.h"
#include "CScene_Stage2.h"
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
#include "CIronDoorRight.h"
#include "CTable.h"
#include "CWallCam.h"
#include "CMonsterPoolMgr.h"
#include "CSmokeValve.h"
#include "CParticleMgr.h"

CScene_Stage2::CScene_Stage2()
{
}

CScene_Stage2::~CScene_Stage2()
{
}

void CScene_Stage2::Enter()
{
	CCameraMgr::Create()->Reset();

	//오브젝트 풀링
	CEffectMgr::Create()->PutInBox();
	CNeutralObjMgr::Create()->PutInBox();
	CParticleMgr::Create()->Enter();

	//Map 추가
	CMap* _pMap = new CMap;
	_pMap->Load(L"ClubStage2.tile" , scaleA *1.2);
	AddObject(_pMap, GROUP_TYPE::MAP);

	//테이블 추가
	CTable* _pTable = new CTable;
	_pTable->SetPos(doublepoint{ 1364,395 });
	_pTable->SetAnimationScaling(doublepoint{ 2,2 });
	_pTable->Initialize();
	AddObject(_pTable, GROUP_TYPE::BACKGROUND);

	//플레이어 추가
	CPlayer* _pObj = new CPlayer;

	_pObj->SetPos(doublepoint{ 100,300 });
	_pObj->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pObj->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pObj->Initialize();

	AddObject(_pObj, GROUP_TYPE::PLAYER);

	CGrunt* _pMon = CMonsterPoolMgr::Create()->Grunt[0];
	_pMon->SetPos(doublepoint{ 900, 400 });
	_pMon->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pMon->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pMon->SetRoamingPoint(doublepoint{ 500, 300 });
	_pMon->SetMainOrder(Main_Order::GetToRoamingPoint);
	_pMon->SetMainOrder(Main_Order::IdleLeft);
	AddObject(_pMon, GROUP_TYPE::MONSTER);

	CGrunt* _pMon2 = CMonsterPoolMgr::Create()->Grunt[1];
	_pMon2->SetPos(doublepoint{ 1670, 400 });
	_pMon2->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pMon2->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pMon2->SetRoamingPoint(doublepoint{ 2350, 400 });
	_pMon2->SetMainOrder(Main_Order::LeanLeft);
	AddObject(_pMon2, GROUP_TYPE::MONSTER);

	

	CGangster* _pGang = CMonsterPoolMgr::Create()->Gangster[0];
	_pGang->SetPos(doublepoint{ 2750,400 });
	_pGang->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pGang->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pGang->SetRoamingPoint(doublepoint{ 2350, 400 });
	_pGang->SetMainOrder(Main_Order::IdleLeft);
	AddObject(_pGang, GROUP_TYPE::MONSTER);

	CGangster* _pGang2 = CMonsterPoolMgr::Create()->Gangster[1];
	_pGang2->SetPos(doublepoint{ 2600, 400 });
	_pGang2->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pGang2->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pGang2->SetRoamingPoint(doublepoint{ 2350, 400 });
	_pGang2->SetMainOrder(Main_Order::IdleLeft);
	AddObject(_pGang2, GROUP_TYPE::MONSTER);

	CGangster* _pGang3 = CMonsterPoolMgr::Create()->Gangster[2];
	_pGang3->SetPos(doublepoint{ 2675, 400 });
	_pGang3->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pGang3->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pGang3->SetRoamingPoint(doublepoint{ 2350, 400 });
	_pGang3->SetMainOrder(Main_Order::IdleLeft);
	AddObject(_pGang3, GROUP_TYPE::MONSTER);

	CPomp* _pPom = CMonsterPoolMgr::Create()->Pomp[0];
	_pPom->SetPos(doublepoint{ 2300, 400 });
	_pPom->SetScale(doublepoint{ scaleA * 30,scaleA * 60 });
	_pPom->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pPom->SetRoamingPoint(doublepoint{ 2350, 400 });
	AddObject(_pPom, GROUP_TYPE::MONSTER);


	//커서 추가
	CCursor* _pCursor = new CCursor;
	AddObject(_pCursor, GROUP_TYPE::CURSOR);
	ShowCursor(false);

	CFloor* _pFloor1 = new CFloor;									//메인 바닥
	_pFloor1->SetScale(doublepoint{ 3600, 100 });
	_pFloor1->SetPos(doublepoint{ 1800,511.5});
	_pFloor1->Initialize();
	AddObject(_pFloor1, GROUP_TYPE::FLOOR);

	CFloorCeiling * _pCeiling = new CFloorCeiling;									//메인 바닥
	_pCeiling->SetScale(doublepoint{ 3600, 100 });
	_pCeiling->SetPos(doublepoint{ 1800,63.5 });
	_pCeiling->Initialize();
	AddObject(_pCeiling, GROUP_TYPE::FLOOR);

	CWall_CantClimb* _pWall = new CWall_CantClimb;
	_pWall->SetScale(doublepoint{100, 600 });
	_pWall->SetPos(doublepoint{ 0,300 });
	_pWall->Initialize();
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	CWall_CantClimb* _pWall2 = new CWall_CantClimb;
	_pWall2->SetScale(doublepoint{ 110, 200 });
	_pWall2->SetPos(doublepoint{ 1612,128 });
	_pWall2->Initialize();
	AddObject(_pWall2, GROUP_TYPE::FLOOR);

	CWallDoor* _pWallDoor = new CWallDoor;
	_pWallDoor->SetScale(doublepoint{ 70, 345 });
	_pWallDoor->SetPos(doublepoint{ 1067,287 });
	_pWallDoor->SetAnimationScaling(doublepoint{ 3,3 });
	_pWallDoor->Initialize();
	AddObject(_pWallDoor, GROUP_TYPE::FLOOR);

	CWallDoor* _pWallDoor2 = new CWallDoor;
	_pWallDoor2->SetScale(doublepoint{ 70, 345 });
	_pWallDoor2->SetPos(doublepoint{ 2913,287 });
	_pWallDoor2->SetAnimationScaling(doublepoint{ 3,3 });
	_pWallDoor2->Initialize();
	AddObject(_pWallDoor2, GROUP_TYPE::FLOOR);

	CWallCam* _pWallCam = new CWallCam;
	_pWallCam->SetAnimationScaling(doublepoint{ 2,2 });
	_pWallCam->SetPos(doublepoint{ 700,130 });
	_pWallCam->SetConnectDoor(_pWallDoor);
	_pWallCam->SetAngle((double)120 / (double)180 * M_PI);
	_pWallCam->SetDetectAngleRange(doublepoint{ (double)90 / (double)180 * M_PI,(double)135 / (double)180 * M_PI });
	_pWallCam->Initialize();

	AddObject(_pWallCam, GROUP_TYPE::BACKGROUND);

	CWallCam* _pWallCam2 = new CWallCam;
	_pWallCam2->SetAnimationScaling(doublepoint{ 2,2 });
	_pWallCam2->SetPos(doublepoint{ 2800,130 });
	_pWallCam2->SetConnectDoor(_pWallDoor2);
	_pWallCam2->SetAngle((double)120 / (double)180 * M_PI);
	_pWallCam2->SetDetectAngleRange(doublepoint{ (double)30 / (double)180 * M_PI,(double)135 / (double)180 * M_PI });
	_pWallCam2->SetState(WallCamState::Roam);
	_pWallCam2->Initialize();

	AddObject(_pWallCam2, GROUP_TYPE::BACKGROUND);

	CSmokeValve* _pValve = new CSmokeValve;
	_pValve->SetPos(doublepoint{ 2700,172 });
	_pValve->SetScale(doublepoint{ 115.2,115.2 });
	_pValve->Initialize();
	AddObject(_pValve, GROUP_TYPE::FLOOR);

	CSmokeValve* _pValve2 = new CSmokeValve;
	_pValve2->SetPos(doublepoint{ 2150,172 });
	_pValve2->SetScale(doublepoint{ 115.2,115.2 });
	_pValve2->Initialize();
	AddObject(_pValve2, GROUP_TYPE::FLOOR);

	//문
	CIronDoorRight* _pDoor = new CIronDoorRight;
	_pDoor->SetScale(doublepoint{ 50,230 });
	_pDoor->SetAnimationScaling(doublepoint{ scaleA * 2.35,scaleA * 2.35 });
	_pDoor->SetPos(doublepoint{ 1612,345 });
	_pDoor->Initialize();
	AddObject(_pDoor, GROUP_TYPE::FLOOR);

	//
	CNeutralObjMgr::Create()->Knife->SetPos(doublepoint{ 1000,400 });
	CNeutralObjMgr::Create()->Knife->SetOwner(_pObj);
	CNeutralObjMgr::Create()->Knife->SetValid(true);
	CNeutralObjMgr::Create()->Knife->SetState(ThrowingObjState::Neutral);


	//레이저
	CLaser* _pLaser = new CLaser;
	_pLaser->SetPos(doublepoint{ 1900,290 });
	_pLaser->SetScale(doublepoint{ scaleA * 30,scaleA * 200 });
	_pLaser->SetAnimationScaling(doublepoint{ scaleA * 2,scaleA * 2 });
	_pLaser->Initialize();
	AddObject(_pLaser, GROUP_TYPE::TRAP);


	//Obj 초기화
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
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::FLOOR, GROUP_TYPE::PARTICLE);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PARTICLE);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PARTICLE);

	//카메라 모드 지정
	CCameraMgr::Create()->SetCameraMode(CameraMode::FollowPlayer);
}

void CScene_Stage2::Exit()
{
}

void CScene_Stage2::Update()
{
	CScene::Update();
}

void CScene_Stage2::Render(HDC _dc)
{
	CScene::Render(_dc);
}
