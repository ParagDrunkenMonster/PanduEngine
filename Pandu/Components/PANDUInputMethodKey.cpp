#include "PANDUInputMethodKey.h"

namespace Pandu
{
	InputMethodKey::InputMethodKey(TInputBindingAutoPtr _inputBinding)
		: InputMethod(_inputBinding)
	{
	}

	InputMethodKey::~InputMethodKey()
	{
		Destroy();
	}

	void InputMethodKey::Destroy()
	{
		m_AllKeyListeners.clear();
		m_CurrentKeys.clear();
	}

	void InputMethodKey::KeyEventHappened(GameKeyStates _state,int _gameKey, float _maxTime)
	{
		if( _state > GKS_None && _state < GKS_Count )
		{
			m_CurrentKeys.push_back(TKeyStatePair(TKeyTimePair(_gameKey,_maxTime),_state));
		}
	}

	void InputMethodKey::UpdateListeners(const FrameEvent& _frameEvent)
	{
		TGameKeyList::iterator itr = m_CurrentKeys.begin();
		TGameKeyList::iterator end = m_CurrentKeys.end();

		for( ; itr != end ; itr++ )
		{
			if( (*itr).second == GKS_KeyDown )
			{
				CallAllOnKeyDown((*itr).first.first);
				itr = m_CurrentKeys.erase(itr);
			}
			else if( (*itr).second == GKS_KeyHeld )
			{
				CallAllOnKeyHeld((*itr).first.first);
				itr = m_CurrentKeys.erase(itr);
			}
			else if( (*itr).second == GKS_KeyUp )
			{
				CallAllOnKeyUp((*itr).first.first);
				(*itr).second = GKS_None;
			}
			else if( (*itr).second == GKS_None )
			{
				if(CallAllOnKeyPressed((*itr).first.first))
				{
					itr = m_CurrentKeys.erase(itr);
				}
				else
				{
					(*itr).first.second -= _frameEvent.deltaTimeSecond;
					if((*itr).first.second <= 0)
					{
						itr = m_CurrentKeys.erase(itr);
					}
				}
			}

			if(itr == end)
			{
				break;
			}
		}
	}

	bool InputMethodKey::CallAllOnKeyDown(int _gameKey)
	{
		TKeyListenersList::iterator itr = m_AllKeyListeners.begin();
		TKeyListenersList::iterator end = m_AllKeyListeners.end();

		for( ; itr != end ; itr++ )
		{
			TInputMethodKeyListenerSharedPtr currListener = (*itr).lock();
			if(currListener == NULL)
			{
				itr = m_AllKeyListeners.erase(itr);
			}
			else
			{
				if( currListener->OnKeyDown(_gameKey) )
				{
					return true;
				}
			}

			if(itr == end)
			{
				break;
			}
		}
		return false;
	}

	bool InputMethodKey::CallAllOnKeyUp(int _gameKey)
	{
		TKeyListenersList::iterator itr = m_AllKeyListeners.begin();
		TKeyListenersList::iterator end = m_AllKeyListeners.end();

		for( ; itr != end ; itr++ )
		{
			TInputMethodKeyListenerSharedPtr currListener = (*itr).lock();
			
			if(currListener == NULL)
			{
				itr = m_AllKeyListeners.erase(itr);
			}
			else
			{
				if( currListener->OnKeyUp(_gameKey) )
				{
					return true;
				}
			}

			if(itr == end)
			{
				break;
			}
		}
		return false;
	}

	bool InputMethodKey::CallAllOnKeyHeld(int _gameKey)
	{
		TKeyListenersList::iterator itr = m_AllKeyListeners.begin();
		TKeyListenersList::iterator end = m_AllKeyListeners.end();

		for( ; itr != end ; itr++ )
		{
			TInputMethodKeyListenerSharedPtr currListener = (*itr).lock();

			if(currListener == NULL)
			{
				itr = m_AllKeyListeners.erase(itr);
			}
			else
			{
				if( currListener->OnKeyHeld(_gameKey) )
				{
					return true;
				}
			}

			if(itr == end)
			{
				break;
			}
		}
		return false;
	}

	bool InputMethodKey::CallAllOnKeyPressed(int _gameKey)
	{
		TKeyListenersList::iterator itr = m_AllKeyListeners.begin();
		TKeyListenersList::iterator end = m_AllKeyListeners.end();

		for( ; itr != end ; itr++ )
		{
			TInputMethodKeyListenerSharedPtr currListener = (*itr).lock();

			if(currListener == NULL)
			{
				itr = m_AllKeyListeners.erase(itr);
			}
			else
			{
				if( currListener->OnKeyPressed(_gameKey) )
				{
					return true;
				}
			}

			if(itr == end)
			{
				break;
			}
		}
		return false;
	}
}