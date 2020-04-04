/********************************************************************
	filename: 	PANDUInputSystem
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUInputSystem_h__
#define __PANDUInputSystem_h__

#include "PANDUInputMethod.h"
#include <list>

namespace Pandu
{
	class InputSystem;
	typedef boost::shared_ptr<InputSystem> TInputSystemSharedPtr;
	//typedef boost::weak_ptr<InputSystem> TInputSystemWeakPtr;
	typedef std::auto_ptr<InputSystem> TInputSystemAutoPtr;

	class InputSystem
	{
	public:

		typedef std::list<TInputMethodSharedPtr> TInputMethodSharedPtrList;

	protected:

		TInputMethodSharedPtrList m_AllInputMethods;

		void UpdateAllInputMethods(const FrameEvent& _frameEvent);

	public:

		InputSystem();
		virtual ~InputSystem();

		TInputMethodSharedPtr AddInputMethod(TInputMethodAutoPtr _newIpMethod);
		void ClearAllInputMethods();

		virtual void Update(const FrameEvent& _frameEvent) = 0;
	};
}

#endif