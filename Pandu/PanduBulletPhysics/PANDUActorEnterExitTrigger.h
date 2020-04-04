/********************************************************************
	filename: 	PANDUActorEnterExitTrigger
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUActorEnterExitTrigger_h__
#define __PANDUActorEnterExitTrigger_h__


#include "PANDUTrigger.h"
#include "../GameComponents/PANDUActorOrToken.h"
#include "../Components/PANDUEvent.h"

namespace Pandu
{

	class ActorEnterExitTrigger : public Trigger
	{
	public:

		enum EventType
		{
			ET_OnEnter = 0,
			ET_Inside,
			ET_OnExit,

			ET_Num
		};
	protected:

		TActorTokenSharedPtr m_ActorToCheck;

		bool m_PrvObjectInside;
		TEventSharedPtr m_Events[ET_Num];

	public:

		ActorEnterExitTrigger( const TriggerDesc& _desc ,TActorTokenSharedPtr _actorToCheck);
		virtual ~ActorEnterExitTrigger();

		virtual void Update();

		void AddCommand(TCommandAutoPtr _command, EventType _toEvent);

	private:

		void ProcessObectsInsideGhostObjects(btAlignedObjectArray < btCollisionObject* >& objs, bool isPairCachingGhostObject);
	};
}

#endif