/********************************************************************
	filename: 	PANDULogOutputTarget
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDULogOutputTarget_h__
#define __PANDULogOutputTarget_h__

#include "../boost/smart_ptr.hpp"
#include "PANDUString.h"

namespace Pandu
{
	class LogOutputTarget;

	typedef boost::shared_ptr<LogOutputTarget> TSharedLogOPTargetPtr;
	typedef std::auto_ptr<LogOutputTarget> TAutoLogOPTargetPtr;
	
	class LogManager;

	class LogOutputTarget
	{
	public:

		LogOutputTarget(){}
		virtual ~LogOutputTarget(){}

		virtual void WriteErrorLog(const String& _log) = 0;
		virtual void WriteWarningLog(const String& _log) = 0;
		virtual void WriteLog(const String& _log) = 0;
	};
}

#endif