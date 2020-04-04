#include "PANDULogOutputTargetOutputWindow.h"

namespace Pandu
{
	LogOutputTargetOutputWindow::LogOutputTargetOutputWindow()
	{

	}

	LogOutputTargetOutputWindow::~LogOutputTargetOutputWindow()
	{

	}

	void LogOutputTargetOutputWindow::WriteLog(const String& _log)
	{
		printf(_log.CString());
		printf("\n");
	}

	void LogOutputTargetOutputWindow::WriteErrorLog(const String& _log)
	{
		WriteLog("Error : ");
		WriteLog(_log);
	}

	void LogOutputTargetOutputWindow::WriteWarningLog(const String& _log)
	{
		WriteLog("Warning : ");
		WriteLog(_log);
	}
}