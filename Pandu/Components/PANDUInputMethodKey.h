/********************************************************************
	filename: 	PANDUInputMethodKey
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInputMethodKey_h__
#define __PANDUInputMethodKey_h__

#include "PANDUInputMethod.h"
#include "PANDUInputMethodKeyListener.h"
#include <list>

namespace Pandu
{
	class InputMethodKey;
	typedef boost::shared_ptr<InputMethodKey> TInputMethodKeysSharedPtr;
	typedef boost::weak_ptr<InputMethodKey> TInputMethodKeysWeakPtr;
	typedef std::auto_ptr<InputMethodKey> TInputMethodKeysAutoPtr;

	class InputMethodKey : public InputMethod
	{
	public:
		enum GameKeyStates
		{
			GKS_None = 0,
			GKS_KeyDown,
			GKS_KeyHeld,
			GKS_KeyUp,

			GKS_Count
		};


		//float value specifies how long to keep KeyPressed alive till KeyHeld event occured
		typedef std::pair<int,float> TKeyTimePair;
		typedef std::pair<TKeyTimePair,GameKeyStates> TKeyStatePair;
		typedef std::list<TKeyStatePair> TGameKeyList;
		typedef std::list<TInputMethodKeyListenerWeakPtr> TKeyListenersList;

	protected:
		
	
		TGameKeyList m_CurrentKeys;

		TKeyListenersList m_AllKeyListeners; 
		
	public:

		InputMethodKey(TInputBindingAutoPtr _inputBinding);
		virtual ~InputMethodKey();

		void AddGameKeyListener(TInputMethodKeyListenerWeakPtr _listener)
		{
			m_AllKeyListeners.push_back(_listener);
		}

	protected:

		void KeyEventHappened(GameKeyStates _state, int _gameKey, float _maxTime);
		void UpdateListeners(const FrameEvent& _frameEvent);

	private:

		bool CallAllOnKeyDown(int _gameKey);
		bool CallAllOnKeyUp(int _gameKey);
		bool CallAllOnKeyHeld(int _gameKey);
		bool CallAllOnKeyPressed(int _gameKey);

		void Destroy();

		virtual void Update(const FrameEvent& _frameEvent) = 0;
	};
}

#endif