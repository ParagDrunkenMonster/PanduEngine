#include "PANDUActorManager.h"

namespace Pandu
{
	ActorManager* ActorManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void ActorManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new ActorManager();
		}
	}

	void ActorManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	ActorManager* ActorManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	ActorManager::ActorManager()
	{
	}

	ActorManager::~ActorManager()
	{
		ClearAllActors();
	}

	void ActorManager::ClearAllActors()
	{
		TActorTokenSharedPtrList::iterator itr = m_AllActors.begin();
		TActorTokenSharedPtrList::iterator end = m_AllActors.end();

		for( ; itr != end ; itr++)
		{
			PANDU_ERROR( (*itr).use_count() == 1, "Actor is still in use, first clear the reference before delete.." );
			(*itr).reset();
		}

		m_AllActors.clear();
	}

	TActorTokenSharedPtr ActorManager::RegisterActor(TActorToken _initialCreator ,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _removeCond)
	{
		TActorTokenSharedPtr newActor(new ActorOrToken(_initialCreator,_creatCondition,_destroyCondition,_removeCond) );
		m_AllActors.push_back(newActor);
		return newActor;
	}

	TActorTokenSharedPtr ActorManager::AddActor(TActorAutoPtr _actor,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _removeCond)
	{
		TActorTokenSharedPtr newActor(new ActorOrToken(_actor,_creatCondition,_destroyCondition,_removeCond) );
		m_AllActors.push_back(newActor);
		return newActor;
	}

	void ActorManager::Update(const FrameEvent& _frameEvent)
	{
		TActorTokenSharedPtrList::iterator itr = m_AllActors.begin();
		TActorTokenSharedPtrList::iterator end = m_AllActors.end();

		for( ; itr != end ; itr++)
		{
			(*itr)->Update(_frameEvent);
			if((*itr)->CanRemoveActor())
			{
				PANDU_ERROR( (*itr).use_count() == 1, "Actor is still in use, first clear the reference before delete.." );
				(*itr).reset();
				itr = m_AllActors.erase(itr);
			}
		}

	}
}