/********************************************************************
	filename: 	GamePlayerCharacterController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GamePlayerCharacterController_h__
#define __GamePlayerCharacterController_h__

#include "boost/smart_ptr.hpp"
#include "GameComponents/PANDUPlayerCharacterController.h"

namespace Game
{
	class GamePlayerCharacterController;
	typedef boost::shared_ptr<GamePlayerCharacterController> TGamePlayerCharacterControllerSharedPtr;

	class GamePlayerCharacterController : public Pandu::PlayerCharacterController
	{
	public:

		GamePlayerCharacterController(Pandu::TActorWeakPtr _character);
		virtual ~GamePlayerCharacterController();

	private:

		virtual void Update(const Pandu::FrameEvent& _frameEvent);

		virtual bool OnKeyDown(unsigned long _gameKey);
		virtual bool OnKeyUp(unsigned long _gameKey);
		virtual bool OnKeyHeld(unsigned long _gameKey);
		virtual bool OnKeyPressed(unsigned long _gameKey);
	};
}

#endif