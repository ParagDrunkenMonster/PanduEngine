/********************************************************************
	filename: 	PANDUInputManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInputManager_h__
#define __PANDUInputManager_h__

#include "PANDUInputSystem.h"

namespace Pandu
{
	class InputManager
	{
	public:

	private:

		static InputManager *m_Instance;

		TInputSystemSharedPtr m_CurrentInputSystem;

	public:


		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static InputManager* Get();
		//--------------------------------------------------------------------

		InputManager();
		~InputManager();

		TInputSystemSharedPtr RegisterInputSystem(TInputSystemAutoPtr _newInputSystem);
		void UnregisterInputSystem();

		void Update(const FrameEvent& _frameEvent);
	};

#define gInputManager InputManager::Get()
}

#endif