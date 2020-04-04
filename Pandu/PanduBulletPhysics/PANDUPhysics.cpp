#include "PANDUPhysics.h"

namespace Pandu
{
	//===============================================================================

	Physics* Physics::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions

	void Physics::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new Physics();
		}
	}

	Physics* Physics::Get()
	{
		return m_Instance;
	}

	void Physics::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	//--------------------------------------------------------------------

	Physics::Physics()
	{
	}

	Physics::~Physics()
	{
		ClearAllWorld();
	}

	void Physics::ClearAllWorld()
	{
		TDynamicsWorldsList::iterator itr = m_AllWorlds.begin();
		TDynamicsWorldsList::iterator end = m_AllWorlds.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1, (String("World named ") + (*itr)->GetName()
					+ " is still in use, first clear the reference before clear.." ).CString() );

			(*itr).reset();
		}

		m_AllWorlds.clear();
	}

	TDynamicsWorldSharedPtr Physics::CreateWorld(const String& _name , WorldType _worldType)
	{
		TDynamicsWorldSharedPtr found = GetWorld(_name);

		PANDU_ERROR(found == NULL, (String("World named ") + found->GetName()
						+ " already exists, choose different name" ).CString() );

		if(found == NULL)
		{
			found.reset(new DynamicsWorld(_name,_worldType));
			m_AllWorlds.push_back(found);
			return found;
		}

		return TDynamicsWorldSharedPtr();
	}

	TDynamicsWorldSharedPtr Physics::GetWorld(const String& _name)
	{
		TDynamicsWorldsList::iterator itr = m_AllWorlds.begin();
		TDynamicsWorldsList::iterator end = m_AllWorlds.end();

		TDynamicsWorldSharedPtr found;

		for( itr ; itr != end ; itr++ )
		{
			if( (*itr)->GetName() == _name )
			{
				found = (*itr);
				break;
			}
		}

		return found;
	}

	void Physics::DestroyWorld(const String& _name)
	{
		TDynamicsWorldsList::iterator itr = m_AllWorlds.begin();
		TDynamicsWorldsList::iterator end = m_AllWorlds.end();

		for( ; itr != end ; itr++ )
		{
			if((*itr)->GetName() == _name)
			{
				PANDU_ERROR((*itr).use_count() == 1, (String("World named ") + _name
						+ " is still in use, first clear the reference before destroying.." ).CString() );
				(*itr).reset();
				m_AllWorlds.erase(itr);
				break;
			}
		}
	}

	void Physics::StepSimulation(float _time)
	{
		TDynamicsWorldsList::iterator itr = m_AllWorlds.begin();
		TDynamicsWorldsList::iterator end = m_AllWorlds.end();

		for( ; itr != end ; itr++ )
		{
			(*itr)->StepSimulation(_time);
		}
	}
}