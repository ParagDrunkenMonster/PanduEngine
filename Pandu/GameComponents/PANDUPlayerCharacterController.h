/********************************************************************
	filename: 	PANDUPlayerCharacterController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUPlayerCharacterController_h__
#define __PANDUPlayerCharacterController_h__

#include "PANDUController.h"
#include "../Components/PANDUInputMethodKeyListener.h"
#include "PANDUCharacter.h"

namespace Pandu
{
	class PlayerCharacterController : public Controller , public InputMethodKeyListener
	{
	protected:

		TActorWeakPtr m_Character;

	public:

		PlayerCharacterController(TActorWeakPtr _character);
		virtual ~PlayerCharacterController();

	private:

		virtual bool CanRemoveController() const;
		virtual void Update(const FrameEvent& _frameEvent) = 0;

		virtual bool OnKeyDown(unsigned long _gameKey) = 0;
		virtual bool OnKeyUp(unsigned long _gameKey) = 0;
		virtual bool OnKeyHeld(unsigned long _gameKey) = 0;
		virtual bool OnKeyPressed(unsigned long _gameKey) = 0;
	};
}

#endif