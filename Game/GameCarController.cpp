#include "GameCarController.h"


namespace Game
{
	GameCarController::GameCarController(Pandu::TActorWeakPtr _car)
		: m_Car(_car)
		, m_LastFrameAcclDecl(false)
		, m_CurrSteering(0.0f)
		, m_LastFrameSteering(false)
		, m_LastFrameThrottleKey(GIK_Num)
	{
		Pandu::TActorSharedPtr actor = m_Car.lock();
		PANDU_ERROR( actor != NULL, "Actor is NULL" );
	}

	GameCarController::~GameCarController()
	{
		//m_Car.release()
	}

	bool GameCarController::CanRemoveController() const
	{
		return false;
	}

	void GameCarController::Update(const Pandu::FrameEvent& _frameEvent)
	{
		if(!m_LastFrameAcclDecl)
		{
			Pandu::TActorSharedPtr actor = m_Car.lock();
			PANDU_ERROR( actor != NULL, "Actor is NULL" );

			Pandu::Car *chr = dynamic_cast<Pandu::Car*>(actor.get());

			chr->SetThrottle(0.0f);
			chr->SetBrake(0.05f);//default brake
		}

		if(!m_LastFrameSteering)
		{
			Pandu::TActorSharedPtr actor = m_Car.lock();
			PANDU_ERROR( actor != NULL, "Actor is NULL" );

			Pandu::Car *chr = dynamic_cast<Pandu::Car*>(actor.get());

			m_CurrSteering = chr->GetSteering();
		}

		m_LastFrameAcclDecl = false;
		m_LastFrameSteering = false;
	}

	bool GameCarController::OnKeyDown(unsigned long _gameKey)
	{
		return false;
	}

	bool GameCarController::OnKeyUp(unsigned long _gameKey)
	{
		return false;
	}

	bool GameCarController::OnKeyHeld(unsigned long _gameKey)
	{
		Pandu::TActorSharedPtr actor = m_Car.lock();
		PANDU_ERROR( actor != NULL, "Actor is NULL" );

		Pandu::Car *chr = dynamic_cast<Pandu::Car*>(actor.get());


		if(chr)
		{
			switch((GameImputKeys)_gameKey)
			{
			case GIK_Left:
				{
					m_CurrSteering += 0.02f;
					m_CurrSteering = chr->SetSteering(m_CurrSteering);
					m_LastFrameSteering = true;
				}
				break;

			case GIK_Right:
				{
					m_CurrSteering -= 0.02f;
					m_CurrSteering = chr->SetSteering(m_CurrSteering);
					m_LastFrameSteering = true;
				}
				break;

			case GIK_Up:
				{
					if(m_LastFrameThrottleKey == GIK_Up && chr->ComputeSpeed() < 0.0f)
					{
						chr->SetBrake(1.0f);
						chr->SetThrottle(0.0f);
					}
					else
					{
						chr->SetBrake(0.0f);
						chr->SetThrottle(1.0f);
					}

					m_LastFrameAcclDecl = true;
					m_LastFrameThrottleKey = GIK_Up;
				}
				break;

			case GIK_Down:
				{
					if(m_LastFrameThrottleKey == GIK_Down && chr->ComputeSpeed() > 0.0f)
					{
						chr->SetBrake(1.0f);
						chr->SetThrottle(0.0f);
					}
					else
					{
						chr->SetBrake(0.0f);
						chr->SetThrottle(-1.0f);
					}

					m_LastFrameAcclDecl = true;
					m_LastFrameThrottleKey = GIK_Down;
				}
				break;
			
			case GIK_Jump:
				{
				}
				break;
			}

			return true;
		}
		else
		{
			PANDU_ERROR( chr != NULL, "Actor is NULL" );
		}

		return false;
	}

	bool GameCarController::OnKeyPressed(unsigned long _gameKey)
	{
		return false;
	}
}