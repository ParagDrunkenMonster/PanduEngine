/********************************************************************
	filename: 	PANDUOnActorInsideTrigger
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOnActorInsideTrigger_h__
#define __PANDUOnActorInsideTrigger_h__

#include "PANDUTrigger.h"
#include "../GameComponents/PANDUActorOrToken.h"
#include "../Components/PANDUEvent.h"
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class OnActorInsideTrigger : public Trigger
	{
	protected:

		TActorTokenWeakPtr m_ActorToCheck;

		TEventSharedPtr m_ObjectInsideEvent;

	public:

		OnActorInsideTrigger( const TriggerDesc& _desc
			,TActorTokenWeakPtr _actorToCheck);
		virtual ~OnActorInsideTrigger();

		virtual void Update();

		void AddCommand(TCommandAutoPtr _command);

	private:

		void ProcessObectsInsideGhostObjects(btAlignedObjectArray < btCollisionObject* >& objs, bool isPairCachingGhostObject);
	};
}

#endif