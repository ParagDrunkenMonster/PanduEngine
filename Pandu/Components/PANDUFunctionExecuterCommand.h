/********************************************************************
	filename: 	PANDUFunctionExecuterCommand
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFunctionExecuterCommand_h__
#define __PANDUFunctionExecuterCommand_h__

#include "PANDUCommand.h"
#include <boost/function.hpp>

namespace Pandu
{
	typedef boost::function<void ()> TBoostFunction;

	class FunctionExecuterCommand : public Command
	{
	private:
		
		TBoostFunction m_Function;

	public:

		FunctionExecuterCommand(TBoostFunction _function)
			: Command()
			, m_Function(_function)
		{
		}

		virtual ~FunctionExecuterCommand()
		{
		}

		virtual void Execute()
		{
			m_Function();
		}
	};
}

#endif