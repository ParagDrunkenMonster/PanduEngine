#include "PANDUEvent.h"

namespace Pandu
{
	Event::Event()
	{
	}

	Event::~Event()
	{
		Clear();
	}

	TCommandWeakPtr Event::AddCommand(TCommandAutoPtr _command)
	{
		TCommandSharedPtr cmd(_command.release());
		m_AllCommands.push_back(cmd);
		return cmd;
	}

	void Event::Execute()
	{
		TCommandSharedPtrList::iterator itr = m_AllCommands.begin();
		TCommandSharedPtrList::iterator end = m_AllCommands.end();

		for( ; itr != end ; itr++ )
		{
			(*itr)->Execute();
		}
	}

	void Event::Clear()
	{
		TCommandSharedPtrList::iterator itr = m_AllCommands.begin();
		TCommandSharedPtrList::iterator end = m_AllCommands.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR( (*itr).use_count() == 1, "Command is in use, please clear the reference first" );
			(*itr).reset();
		}

		m_AllCommands.clear();
	}
}