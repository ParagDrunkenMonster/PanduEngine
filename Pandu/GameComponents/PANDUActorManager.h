/********************************************************************
	filename: 	PANDUActorManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUActorManager_h__
#define __PANDUActorManager_h__

#include "PANDUActorOrToken.h"
#include <list>
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class ActorManager
	{
	public:

		typedef std::list<TActorTokenSharedPtr> TActorTokenSharedPtrList;

	private:

		static ActorManager *m_Instance;

		TActorTokenSharedPtrList m_AllActors;

	public:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static ActorManager* Get();
		//--------------------------------------------------------------------

		ActorManager();
		~ActorManager();

		void ClearAllActors();

		TActorTokenSharedPtr RegisterActor(TActorToken _initialCreator ,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _removeCond);

		TActorTokenSharedPtr AddActor(TActorAutoPtr _actor,TActorCrDstCondition _creatCondition 
			,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _removeCond);

		void Update(const FrameEvent& _frameEvent);

	};

#define gActorManager ActorManager::Get()
}

#endif