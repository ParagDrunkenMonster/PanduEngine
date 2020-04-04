/********************************************************************
	filename: 	PANDUInputMethodKeyListener
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInputMethodKeyListener_h__
#define __PANDUInputMethodKeyListener_h__

#include "../boost/smart_ptr.hpp"

namespace Pandu
{
	class InputMethodKeyListener;
	typedef boost::shared_ptr<InputMethodKeyListener> TInputMethodKeyListenerSharedPtr;
	typedef boost::weak_ptr<InputMethodKeyListener> TInputMethodKeyListenerWeakPtr;
	typedef std::auto_ptr<InputMethodKeyListener> TInputMethodKeyListenerAutoPtr;

	class InputMethodKeyListener
	{
	public:
		InputMethodKeyListener(){}
		virtual ~InputMethodKeyListener(){}

		virtual bool OnKeyDown(unsigned long _gameKey) = 0;
		virtual bool OnKeyUp(unsigned long _gameKey) = 0;
		virtual bool OnKeyHeld(unsigned long _gameKey) = 0;
		virtual bool OnKeyPressed(unsigned long _gameKey) = 0;

	};
}

#endif