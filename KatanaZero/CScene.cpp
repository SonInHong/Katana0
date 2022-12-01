#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CCameraMgr.h"

CScene::CScene()
	:m_arrObj{}
	, m_strName{}
{
}

CScene::~CScene()
{
}

void CScene::Update()
{
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (m_arrObj[i][j]->IsDead())
				continue;
			m_arrObj[i][j]->Update();
		}
	}
}

void CScene::ComponentUpdate()
{
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (m_arrObj[i][j]->IsDead())
				continue;
			m_arrObj[i][j]->ComponentUpdate();
		}
	}
}

void CScene::Render(HDC _dc)
{
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		std::vector<CObject*>::iterator iter = m_arrObj[i].begin();


		for (; iter != m_arrObj[i].end(); )
		{
			if ((*iter)->IsDead() == true)
				iter = m_arrObj[i].erase(iter);

						
			else
			{
				(*iter)->Render(_dc);
				++iter;
			}
				
		}

	}

	CCameraMgr::Create()->Render(_dc);

	
	CObject* pla = m_arrObj[(UINT)GROUP_TYPE::PLAYER][0];
	pla->ReRender(_dc);


		
}

void CScene::Load(const std::wstring& Path)
{
	
}

void CScene::ClearObject(GROUP_TYPE _eType)
{
	for (int i = 0; i < m_arrObj[(UINT)_eType].size(); ++i)
	{
		m_arrObj[(UINT)_eType][i]->Destroy();
		delete m_arrObj[(UINT)_eType][i];
	}

	m_arrObj[(UINT)_eType].clear();
}

void CScene::Clean()
{
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		std::vector<CObject*>::iterator iter = m_arrObj[i].begin();

		while (iter != m_arrObj[i].end())
		{
			if ((*iter)->IsDead())
			{
				delete (*iter);
				iter = m_arrObj[i].erase(iter);
			}

			else
			{
				++iter;
			}
		}
	}
}


