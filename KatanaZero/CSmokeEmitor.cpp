#include "pch.h"
#include "CSmokeEmitor.h"
#include "CSmoke.h"
#include "CEventMgr.h"
#include "TimeMgr.h"
#include <random>
#include "KeyMgr.h"

CSmokeEmitor::CSmokeEmitor()
{
	m_PoolSize = 50;
}

CSmokeEmitor::~CSmokeEmitor()
{
}

void CSmokeEmitor::Initialize()
{
	for (int i = 0; i < m_PoolSize; ++i)
	{
		CSmoke* _particle = new CSmoke;
		_particle->Initialize();

		m_Particles.push_back(_particle);
	}
}

void CSmokeEmitor::Enter()
{
	for (int i = m_PoolSize-1; i >= 0; --i)
	{
		CEventMgr::Create()->Event_CreateObj(m_Particles[i], GROUP_TYPE::PARTICLE);
	}
}

void CSmokeEmitor::Exit()
{
}

void CSmokeEmitor::Update()
{
	Pos = Owner->GetPos() + OffSet;

	

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distrAtt(m_AttackSpeedRange.x, m_AttackSpeedRange.y);
	std::uniform_real_distribution<double> distrAng(m_AngleRange.x / 180 * M_PI, m_AngleRange.y / 180 * M_PI);
	std::uniform_real_distribution<double> distrDur(m_DurationRange.x, m_DurationRange.y);
	std::uniform_real_distribution<double> distrVel(m_VelocityRange.x, m_VelocityRange.y);
	std::uniform_real_distribution<double> distrXoff(m_XOffSetRange.x, m_XOffSetRange.y);
	std::uniform_real_distribution<double> distrYoff(m_YOffSetRange.x, m_YOffSetRange.y);




	if (m_Timer > m_TimeLimit) // 발사
	{
		CParticle* bullet = nullptr;
		for (int i = 0; i < m_Particles.size(); ++i)
		{
			if (m_Particles[i]->Valid == false)
			{
				bullet = m_Particles[i];
				break;
			}

		}



		if (bullet == nullptr)
		{
			bullet = new CParticle;
			bullet->Initialize();

			CEventMgr::Create()->Event_CreateObj(bullet, GROUP_TYPE::PARTICLE);
			m_PoolSize++;
		}

		if (!OnOff)
			return;

		if (bullet)
		{
			bullet->Valid = true;

			bullet->Pos = Pos;
			bullet->Pos.x += distrXoff(eng);
			bullet->Pos.y += distrYoff(eng);
			bullet->SetResize(Resize);
			bullet->Duration = distrDur(eng);

			double Angle = distrAng(eng);
			double velocity = distrVel(eng);

			bullet->Velocity = doublepoint{ cos(Angle),sin(Angle) } *velocity;

			bullet->Shoot();

			m_TimeLimit = distrAtt(eng); // 타임리밋 조정
			m_Timer = 0;
			m_count++;

		}






		///===================================================== 하나더 


		bullet = nullptr;
		for (int i = 0; i < m_Particles.size(); ++i)
		{
			if (m_Particles[i]->Valid == false)
			{
				bullet = m_Particles[i];
				break;
			}

		}

		if (bullet == nullptr)
		{
			bullet = new CParticle;
			bullet->Initialize();

			CEventMgr::Create()->Event_CreateObj(bullet, GROUP_TYPE::PARTICLE);
			m_PoolSize++;
		}

		if (bullet)
		{
			bullet->Valid = true;

			bullet->Pos = Pos;
			bullet->Pos.x += distrXoff(eng);
			bullet->Pos.y += distrYoff(eng);
			bullet->SetResize(Resize);
			bullet->Duration = distrDur(eng);

			double Angle = distrAng(eng);
			double velocity = distrVel(eng);

			bullet->Velocity = doublepoint{ cos(Angle),sin(Angle) } *velocity;

			bullet->Shoot();

			m_TimeLimit = distrAtt(eng); // 타임리밋 조정
			m_Timer = 0;
			m_count++;

		
		}

		///===================================================== 하나더 



		bullet = nullptr;
		for (int i = 0; i < m_Particles.size(); ++i)
		{
			if (m_Particles[i]->Valid == false)
			{
				bullet = m_Particles[i];
				break;
			}

		}

		if (bullet == nullptr)
		{
			bullet = new CParticle;
			bullet->Initialize();

			CEventMgr::Create()->Event_CreateObj(bullet, GROUP_TYPE::PARTICLE);
			m_PoolSize++;
		}

		if (bullet)
		{
			bullet->Valid = true;

			bullet->Pos = Pos;
			bullet->Pos.x += distrXoff(eng);
			bullet->Pos.y += distrYoff(eng);
			bullet->SetResize(Resize);
			bullet->Duration = distrDur(eng);

			double Angle = distrAng(eng);
			double velocity = distrVel(eng);

			bullet->Velocity = doublepoint{ cos(Angle),sin(Angle) } *velocity;

			bullet->Shoot();

			m_TimeLimit = distrAtt(eng); // 타임리밋 조정
			m_Timer = 0;
			m_count++;

			
		}

		///===================================================== 하나더 


		bullet = nullptr;
		for (int i = 0; i < m_Particles.size(); ++i)
		{
			if (m_Particles[i]->Valid == false)
			{
				bullet = m_Particles[i];
				break;
			}

		}

		if (bullet == nullptr)
		{
			bullet = new CParticle;
			bullet->Initialize();

			CEventMgr::Create()->Event_CreateObj(bullet, GROUP_TYPE::PARTICLE);
			m_PoolSize++;
		}

		if (bullet)
		{
			bullet->Valid = true;

			bullet->Pos = Pos;
			bullet->Pos.x += distrXoff(eng);
			bullet->Pos.y += distrYoff(eng);
			bullet->SetResize(Resize);
			bullet->Duration = distrDur(eng);

			double Angle = distrAng(eng);
			double velocity = distrVel(eng);

			bullet->Velocity = doublepoint{ cos(Angle),sin(Angle) } *velocity;

			bullet->Shoot();

			m_TimeLimit = distrAtt(eng); // 타임리밋 조정
			m_Timer = 0;
			m_count++;

			
		}


	}



	if (m_count >= m_Maxcount)
		OnOff = false;

	m_Timer += TimeMgr::Create()->dt();
}

void CSmokeEmitor::Render(HDC _dc)
{
}
