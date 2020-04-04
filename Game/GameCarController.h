/********************************************************************
	filename: 	GameCarController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GameCarController_h__
#define __GameCarController_h__

#include "GameComponents/PANDUController.h"
#include "Components/PANDUInputMethodKeyListener.h"
#include "GameComponents/PANDUCar.h"
#include "GameInputKeys.h"

namespace Game
{
	class GameCarController;
	typedef boost::shared_ptr<GameCarController> TGameCarControllerSharedPtr;

	class GameCarController : public Pandu::Controller , public Pandu::InputMethodKeyListener
	{
	protected:

		Pandu::TActorWeakPtr m_Car;
		bool m_LastFrameAcclDecl;
		float m_CurrSteering;
		bool m_LastFrameSteering;
		GameImputKeys m_LastFrameThrottleKey;

	public:

		GameCarController(Pandu::TActorWeakPtr _car);
		virtual ~GameCarController();

	private:

		virtual bool CanRemoveController() const;
		virtual void Update(const Pandu::FrameEvent& _frameEvent);

		virtual bool OnKeyDown(unsigned long _gameKey);
		virtual bool OnKeyUp(unsigned long _gameKey);
		virtual bool OnKeyHeld(unsigned long _gameKey);
		virtual bool OnKeyPressed(unsigned long _gameKey);
	};
}

#endif