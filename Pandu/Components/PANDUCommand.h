/********************************************************************
	filename: 	PANDUCommand
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCommand_h__
#define __PANDUCommand_h__

#include "../boost/smart_ptr.hpp"

namespace Pandu
{
	class Command;

	typedef boost::shared_ptr<Command> TCommandSharedPtr;
	typedef boost::weak_ptr<Command> TCommandWeakPtr;
	typedef std::auto_ptr<Command> TCommandAutoPtr;	

	class Command
	{
	public:
		Command(){}
		virtual ~Command(){}

		virtual void Execute() = 0;

	};
}

#endif