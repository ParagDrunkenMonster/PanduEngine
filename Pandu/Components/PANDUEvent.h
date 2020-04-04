/********************************************************************
	filename: 	PANDUEvent
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUEvent_h__
#define __PANDUEvent_h__

#include "PANDUCommand.h"
#include "../Utils/PANDUCoreDefines.h"
#include <list>

namespace Pandu
{
	class Event;

	typedef boost::shared_ptr<Event> TEventSharedPtr;
	typedef boost::weak_ptr<Event> TEventWeakPtr;
	typedef std::auto_ptr<Event> TEventAutoPtr;	

	class Event
	{
	public:
		
		typedef std::list<TCommandSharedPtr> TCommandSharedPtrList;

	protected:

		TCommandSharedPtrList m_AllCommands;

	public:

		Event();
		virtual ~Event();

		TCommandWeakPtr AddCommand(TCommandAutoPtr _command);
		virtual void Execute();

	private:
		
		void Clear();
	};
}

#endif