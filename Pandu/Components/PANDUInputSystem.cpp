#include "PANDUInputSystem.h"

namespace Pandu
{
	InputSystem::InputSystem()
	{
	}

	InputSystem::~InputSystem()
	{
		ClearAllInputMethods();
	}

	TInputMethodSharedPtr InputSystem::AddInputMethod(TInputMethodAutoPtr _newIpMethod)
	{
		TInputMethodSharedPtr newInputMethod(_newIpMethod.release());
		m_AllInputMethods.push_back(newInputMethod);

		return newInputMethod;
	}

	void InputSystem::ClearAllInputMethods()
	{
		TInputMethodSharedPtrList::iterator itr = m_AllInputMethods.begin();
		TInputMethodSharedPtrList::iterator end = m_AllInputMethods.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR( (*itr).use_count() == 1, "InputMethod is still in use, first clear the reference before delete.." );
			(*itr).reset();
		}

		m_AllInputMethods.clear();
	}

	void InputSystem::UpdateAllInputMethods(const FrameEvent& _frameEvent)
	{
		TInputMethodSharedPtrList::iterator itr = m_AllInputMethods.begin();
		TInputMethodSharedPtrList::iterator end = m_AllInputMethods.end();

		for( ; itr != end ; itr++ )
		{
			(*itr)->Update(_frameEvent);
		}
	}
}