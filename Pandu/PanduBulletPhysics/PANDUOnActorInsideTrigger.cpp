#include "PANDUOnActorInsideTrigger.h"

namespace Pandu
{

	OnActorInsideTrigger::OnActorInsideTrigger( const TriggerDesc& _desc , TActorTokenWeakPtr _actorToCheck)
		: Trigger(_desc)
		, m_ActorToCheck(_actorToCheck)
		, m_ObjectInsideEvent(new Event())
	{
	}

	OnActorInsideTrigger::~OnActorInsideTrigger()
	{
		PANDU_ERROR( m_ObjectInsideEvent.use_count() == 1, "Event is still in use, first clear the reference before delete.." );
		m_ObjectInsideEvent.reset();
	}

	void OnActorInsideTrigger::Update()
	{
		if (m_BulletDynamicsWorld != NULL)
		{
			btAlignedObjectArray < btCollisionObject* > objsInsidePairCachingGhostObject;	// We might want this to be a member variable...							
			btAlignedObjectArray < btCollisionObject* >& pObjsInsideGhostObject = objsInsidePairCachingGhostObject;		// We will store a reference of the current array in this pointer
			
			btGhostObject* go = m_GhostObject;
		
			if (go)	
			{
				objsInsidePairCachingGhostObject.resize(0);
				btPairCachingGhostObject* pgo = dynamic_cast < btPairCachingGhostObject* > (go);	// No upcast functionality...
				
				//can be use pair catching ghost objects for checking in narrow phase.. for now broad phase is ok...
				//for better performance

				/*if (pgo)	
				{
					GetCollidingObjectsInsidePairCachingGhostObject(static_cast < btDiscreteDynamicsWorld* > (m_BulletDynamicsWorld),pgo,objsInsidePairCachingGhostObject);
					pObjsInsideGhostObject = &objsInsidePairCachingGhostObject;
				}
				else 
				{
					pObjsInsideGhostObject = &go->getOverlappingPairs();	// It's better not to try and copy the whole array, but to keep a reference to it!
					// Side Note: btAlignedObjectArray < btCollisionObject* > objs = go->getOverlappingPairs(); (at the moment) makes my program crash on my system...
					// Nevermind, that was the wrong way of doing it: btAlignedObjectArray < btCollisionObject* >& objs = go->getOverlappingPairs(); is much better.
				}	
				// Here pObjsInsideGhostObject should be valid.
				
				ProcessObectsInsideGhostObjects(*pObjsInsideGhostObject,pgo);*/

				pObjsInsideGhostObject = go->getOverlappingPairs();	

				ProcessObectsInsideGhostObjects(pObjsInsideGhostObject,false);
			}
			
		}
	}

	void OnActorInsideTrigger::ProcessObectsInsideGhostObjects(btAlignedObjectArray < btCollisionObject* >& objs, bool isPairCachingGhostObject)
	{
		TActorTokenSharedPtr actorToken = m_ActorToCheck.lock();

		if(actorToken != NULL)
		{
			TActorWeakPtr weakActor = actorToken->GetActor();
			TActorSharedPtr strongActor = weakActor.lock();

			if(strongActor != NULL)
			{
				TCollisionObjectWeakPtr weakCollision = strongActor->GetCollisionObject();
				TCollisionObjectSharedPtr strongCollision = weakCollision.lock();

				if(strongCollision != NULL)
				{
					int size = objs.size();
					for (int i = 0 ; i < size ; i++ ) 
					{
						if(strongCollision.get() == objs[i])
						{
							m_ObjectInsideEvent->Execute();
							break;
						}
					}
				}
			}
		}
	}

	void OnActorInsideTrigger::AddCommand(TCommandAutoPtr _command)
	{
		m_ObjectInsideEvent->AddCommand(_command);
	}

}