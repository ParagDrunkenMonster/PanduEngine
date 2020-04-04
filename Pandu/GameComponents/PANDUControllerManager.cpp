#include "PANDUControllerManager.h"

namespace Pandu
{
	ControllerManager* ControllerManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void ControllerManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new ControllerManager();
		}
	}

	void ControllerManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	ControllerManager* ControllerManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	ControllerManager::ControllerManager()
	{
	}

	ControllerManager::~ControllerManager()
	{
		ClearAllControllers();
	}

	void ControllerManager::ClearAllControllers()
	{
		TControllerSharedPtrList::iterator itr = m_AllControllers.begin();
		TControllerSharedPtrList::iterator end = m_AllControllers.end();

		for( ; itr != end ; itr++)
		{
			PANDU_ERROR( (*itr).use_count() == 1, "Controller is still in use, first clear the reference before delete.." );
			(*itr).reset();
		}

		m_AllControllers.clear();
	}

	void ControllerManager::AddController(TControllerSharedPtr _newController)
	{
		m_AllControllers.push_back(_newController);
	}

	void ControllerManager::Update(const FrameEvent& _frameEvent)
	{
		TControllerSharedPtrList::iterator itr = m_AllControllers.begin();
		TControllerSharedPtrList::iterator end = m_AllControllers.end();

		for( ; itr != end ; itr++)
		{
			(*itr)->Update(_frameEvent);
			if((*itr)->CanRemoveController())
			{
				PANDU_ERROR( (*itr).use_count() == 1, "Controller is still in use, first clear the reference before delete.." );
				(*itr).reset();
				itr = m_AllControllers.erase(itr);
			}
		}

	}
}