/********************************************************************
	filename: 	PANDUActorOrToken
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUActorOrToken_h__
#define __PANDUActorOrToken_h__

#include "PANDUActor.h"
#include "../Utils/PANDUCoreDefines.h"


namespace Pandu
{
	class ActorOrToken;

	typedef boost::shared_ptr<ActorOrToken> TActorTokenSharedPtr;
	typedef boost::weak_ptr<ActorOrToken> TActorTokenWeakPtr;
	
	typedef boost::function<bool ()> TActorCrDstCondition;
	typedef boost::function<void (TActorWeakPtr)> TOnActorCrDstListener;

	class ActorOrToken
	{
	private:

		TActorToken m_ActorCreator;
		TActorSharedPtr m_Actor;
		
		TActorCrDstCondition m_CreateCondition;
		TActorCrDstCondition m_DestroyCondition;
		TActorCrDstCondition m_RemoveActorCond;

		TOnActorCrDstListener m_OnActorCreate;
		TOnActorCrDstListener m_OnActorDestroy;
		

		void DestroyActor();

	public:

		ActorOrToken(TActorToken _initialCreator ,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _canRemoveActorCond);

		ActorOrToken(TActorAutoPtr _actor ,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _canRemoveActorCond);

		virtual ~ActorOrToken();

		inline TActorWeakPtr GetActor()													{	return m_Actor;							}
		inline void RegisterOnCreateListener(TOnActorCrDstListener _onCreateActor)		{	m_OnActorCreate = _onCreateActor;		}	
		inline void RegisterOnDestroyListener(TOnActorCrDstListener _onDestroyActor)	{	m_OnActorDestroy = _onDestroyActor;		}

		bool CanRemoveActor();

		void Update(const FrameEvent& _frameEvent);

	};
}

#endif