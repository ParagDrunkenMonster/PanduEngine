#include "PANDUActorEnterExitTrigger.h"

namespace Pandu
{

	ActorEnterExitTrigger::ActorEnterExitTrigger( const TriggerDesc& _desc, TActorTokenSharedPtr _actorToCheck)
		: Trigger(_desc)
		, m_ActorToCheck(_actorToCheck)
		, m_PrvObjectInside(false)
	{
		for(int i = 0 ; i < ET_Num ; i++ )
		{
			m_Events[i].reset(new Event());
		}
	}

	ActorEnterExitTrigger::~ActorEnterExitTrigger()
	{
		for(int i = 0 ; i < ET_Num ; i++ )
		{
			PANDU_ERROR( m_Events[i].use_count() == 1, "Event is still in use, first clear the reference before delete.." );
			m_Events[i].reset();
		}
		
	}

	void ActorEnterExitTrigger::Update()
	{
		if (m_BulletDynamicsWorld != NULL)
		{
			btAlignedObjectArray < btCollisionObject* > objsInsidePairCachingGhostObject;	// We might want this to be a member variable...							
			btAlignedObjectArray < btCollisionObject* >* pObjsInsideGhostObject = NULL;// = objsInsidePairCachingGhostObject;		// We will store a reference of the current array in this pointer
			
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

				pObjsInsideGhostObject = &go->getOverlappingPairs();	

				if(pObjsInsideGhostObject != NULL)
				{
					ProcessObectsInsideGhostObjects(*pObjsInsideGhostObject,false);
				}
			}
			
		}
	}

	void ActorEnterExitTrigger::ProcessObectsInsideGhostObjects(btAlignedObjectArray < btCollisionObject* >& objs, bool isPairCachingGhostObject)
	{
		TActorTokenSharedPtr actorToken = m_ActorToCheck;

		bool objectInside = false;
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
							if(!m_PrvObjectInside)
							{
								m_Events[ET_OnEnter]->Execute();
							}
							else
							{
								m_Events[ET_Inside]->Execute();
							}
							
							objectInside = true;
							break;
						}
					}
				}
			}

			if(m_PrvObjectInside == true && !objectInside)
			{
				m_Events[ET_OnExit]->Execute();
			}

			m_PrvObjectInside = objectInside;
		}
	}

	void ActorEnterExitTrigger::AddCommand(TCommandAutoPtr _command, ActorEnterExitTrigger::EventType _toEvent)
	{
		if(_toEvent >= 0 && _toEvent < ET_Num)
		{
			m_Events[_toEvent]->AddCommand(_command);
		}
	}

}