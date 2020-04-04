#include "PANDUInputManager.h"

namespace Pandu
{
	InputManager* InputManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void InputManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new InputManager();
		}
	}

	void InputManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	InputManager* InputManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
		UnregisterInputSystem();
	}

	void InputManager::Update(const FrameEvent& _frameEvent)
	{
		if(m_CurrentInputSystem != NULL)
		{
			m_CurrentInputSystem->Update(_frameEvent);
		}
	}

	TInputSystemSharedPtr InputManager::RegisterInputSystem(TInputSystemAutoPtr _newInputSystem)
	{
		PANDU_ERROR( m_CurrentInputSystem.use_count() == 0, "Already has input system first unregister current input system" );
		m_CurrentInputSystem.reset(_newInputSystem.release());

		return m_CurrentInputSystem;
	}

	void InputManager::UnregisterInputSystem()
	{
		PANDU_ERROR( m_CurrentInputSystem.use_count() <= 1, "InputSystem is still in use, first clear the reference before delete.." );
		m_CurrentInputSystem.reset();
	}
}