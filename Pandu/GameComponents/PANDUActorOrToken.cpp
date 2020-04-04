#include "PANDUActorOrToken.h"

namespace 
{
	void OnActorCreate(Pandu::TActorWeakPtr )
	{
	}

	void OnActorDestroy(Pandu::TActorWeakPtr )
	{
	}
}

namespace Pandu
{
	ActorOrToken::ActorOrToken(TActorToken _initialCreator,TActorCrDstCondition _creatCondition
		,TActorCrDstCondition _destroyCondition , TActorCrDstCondition _canRemoveActorCond)
		: m_ActorCreator(_initialCreator)
		, m_CreateCondition(_creatCondition)
		, m_DestroyCondition(_destroyCondition)
		, m_RemoveActorCond(_canRemoveActorCond)
	{
		m_OnActorCreate = TOnActorCrDstListener(OnActorCreate);
		m_OnActorDestroy = TOnActorCrDstListener(OnActorDestroy);
	}

	ActorOrToken::ActorOrToken(TActorAutoPtr _actor ,TActorCrDstCondition _creatCondition 
		,TActorCrDstCondition _destroyCondition, TActorCrDstCondition _canRemoveActorCond)
		: m_Actor(_actor.release())
		, m_CreateCondition(_creatCondition)
		, m_DestroyCondition(_destroyCondition)
		, m_RemoveActorCond(_canRemoveActorCond)
	{
		m_OnActorCreate = TOnActorCrDstListener(OnActorCreate);
		m_OnActorDestroy = TOnActorCrDstListener(OnActorDestroy);
	}

	ActorOrToken::~ActorOrToken()
	{
		DestroyActor();
	}

	void ActorOrToken::Update(const FrameEvent& _frameEvent)
	{
		if(m_Actor != NULL)
		{
			m_Actor->Update(_frameEvent);

			if(m_Actor->TokenizeActor() && m_DestroyCondition())
			{
				m_ActorCreator = m_Actor->GetActorToken();
				DestroyActor();
			}
		}
		else if(m_CreateCondition())
		{
			m_Actor = m_ActorCreator();
			m_OnActorCreate(m_Actor);
		}
	}

	void ActorOrToken::DestroyActor()
	{
		if(m_Actor.use_count() == 1)
		{
			m_OnActorDestroy(m_Actor);
		}

		m_Actor.reset();
		PANDU_ERROR(m_Actor.use_count() == 0, "Actor is still in use, first clear the reference before clear.." );
	}

	bool ActorOrToken::CanRemoveActor()
	{
		return m_RemoveActorCond();
	}
}