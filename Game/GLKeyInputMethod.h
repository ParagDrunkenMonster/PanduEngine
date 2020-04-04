/********************************************************************
	filename: 	GLKeyInputMethod
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GLKeyInputMethod_h__
#define __GLKeyInputMethod_h__

#include "Components/PANDUInputMethodKey.h"
#include "../NeHeGL.h"
#include "../boost/smart_ptr.hpp"
#include "GameInputKeys.h"

namespace Game
{
	
	class GLKeyInputMethod : public Pandu::InputMethodKey
	{
	private:

		Keys* m_Keys;

		bool m_PhysicalPrevPressed[GIK_Num];

	public:

		GLKeyInputMethod(Pandu::TInputBindingAutoPtr _inputBinding,Keys* _keys);
		virtual ~GLKeyInputMethod();

		virtual void Update(const Pandu::FrameEvent& _frameEvent);

	private:

		void ResetPreviousPressed();
		void UpdateGameKey(int _gameKey);
	};
}

#endif