/********************************************************************
	filename: 	GameInputBinding
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GameInputBinding_h__
#define __GameInputBinding_h__

#include "Components/PANDUInputBinding.h"
#include "GameInputKeys.h"
#include <windows.h>	

namespace Game
{
	
	class GameInputBinding;

	class GameInputBinding : public Pandu::InputBinding
	{
	private:
			
		int m_WindowsKeyForGameKey[GIK_Num];
		float m_TimeForGameInput[GIK_Num];
		
	public:
		GameInputBinding()
		{
			m_WindowsKeyForGameKey[GIK_Left]  = VK_LEFT;
			m_WindowsKeyForGameKey[GIK_Right] = VK_RIGHT;
			m_WindowsKeyForGameKey[GIK_Up] = VK_UP;
			m_WindowsKeyForGameKey[GIK_Down] = VK_DOWN;
			m_WindowsKeyForGameKey[GIK_Jump] = VK_SPACE;

			m_TimeForGameInput[GIK_Left]  = 1.0f;
			m_TimeForGameInput[GIK_Right] = 1.0f;
			m_TimeForGameInput[GIK_Up] = 1.0f;
			m_TimeForGameInput[GIK_Down] = 1.0f;
			m_TimeForGameInput[GIK_Jump] = 1.0f;

		}

		~GameInputBinding()
		{
		}

		virtual int GetGameInputFromPhysicalKey(int _key)
		{
			for(int i = 0 ; i < GIK_Num ; i++)
			{
				if(m_WindowsKeyForGameKey[i] == _key )
				{
					return i;
				}
			}
			return GIK_Num;
		}

		virtual int GetPhysicalKeyFromGameInput(int _gameInput)
		{
			if(_gameInput >= 0 && _gameInput < GIK_Num)
			{
				return m_WindowsKeyForGameKey[_gameInput];
			}

			return -1;
		}

		virtual float GetTimeForGameInput(int _gameInput)
		{
			return m_TimeForGameInput[_gameInput];
		}
	};
}

#endif