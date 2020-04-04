/********************************************************************
	filename: 	PANDULogOutputTargetOutputWindow
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDULogOutputTargetOutputWindow_h__
#define __PANDULogOutputTargetOutputWindow_h__

#include "PANDULogOutputTarget.h"

namespace Pandu
{
	class LogOutputTargetOutputWindow : public LogOutputTarget
	{
	public:

		LogOutputTargetOutputWindow();
		~LogOutputTargetOutputWindow();

	private:

		virtual void WriteErrorLog(const String& _log);
		virtual void WriteWarningLog(const String& _log);
		virtual void WriteLog(const String& _log);
	};
}

#endif