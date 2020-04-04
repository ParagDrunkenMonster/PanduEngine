#include "GamePlayerCharacterController.h"
#include "GameInputKeys.h"

namespace Game
{
	GamePlayerCharacterController::GamePlayerCharacterController(Pandu::TActorWeakPtr _character)
		: Pandu::PlayerCharacterController(_character)
	{
		Pandu::TActorSharedPtr actor = _character.lock();
		PANDU_ERROR( actor != NULL, "Actor is NULL" );
	}

	GamePlayerCharacterController::~GamePlayerCharacterController()
	{
	}

	void GamePlayerCharacterController::Update(const Pandu::FrameEvent& _frameEvent)
	{
		
	}

	bool GamePlayerCharacterController::OnKeyDown(unsigned long _gameKey)
	{
		return false;
	}

	bool GamePlayerCharacterController::OnKeyUp(unsigned long _gameKey)
	{
		return false;
	}

	bool GamePlayerCharacterController::OnKeyHeld(unsigned long _gameKey)
	{
		Pandu::TActorSharedPtr actor = m_Character.lock();
		PANDU_ERROR( actor != NULL, "Actor is NULL" );

		Pandu::Character *chr = dynamic_cast<Pandu::Character*>(actor.get());
		if(chr)
		{
			switch((GameImputKeys)_gameKey)
			{
			case GIK_Left:
				{
					chr->SetSpeed(-0.01f);
				}
				break;

			case GIK_Right:
				{
					chr->SetSpeed( 0.01f);
				}
				break;

			case GIK_Up:
				{
				}
				break;

			case GIK_Down:
				{
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

	bool GamePlayerCharacterController::OnKeyPressed(unsigned long _gameKey)
	{
		return false;
	}
}