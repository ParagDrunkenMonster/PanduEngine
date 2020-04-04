#include "PANDULogManager.h"
#include "PANDULogOutputTargetOutputWindow.h"

namespace Pandu
{
	LogManager *LogManager::m_Instance = NULL;

	LogManager::LogManager()
	{
		m_OutputTargetsList.push_back(TSharedLogOPTargetPtr(new LogOutputTargetOutputWindow()) );
	}

	LogManager::~LogManager()
	{
		Destroy();
	}

	void LogManager::Destroy()
	{
		int count = (int)m_OutputTargetsList.size();

		for(int i = 0 ; i < count ; i++)
		{
			BOOST_ASSERT_MSG(m_OutputTargetsList[i].use_count() == 1,"Output Target is still in use, please clear other references" );

			m_OutputTargetsList[i].reset();
		}
	}

	void LogManager::WriteLog(const String& _log)
	{
		int count = (int)m_OutputTargetsList.size();

		for(int i = 0 ; i < count ; i++)
		{
			m_OutputTargetsList[i]->WriteLog(_log);
		}
	}

	void LogManager::WriteErrorLog(const String& _log)
	{
		int count = (int)m_OutputTargetsList.size();

		for(int i = 0 ; i < count ; i++)
		{
			m_OutputTargetsList[i]->WriteErrorLog(_log);
		}
	}

	void LogManager::WriteWarningLog(const String& _log)
	{
		int count = (int)m_OutputTargetsList.size();

		for(int i = 0 ; i < count ; i++)
		{
			m_OutputTargetsList[i]->WriteWarningLog(_log);
		}
	}

	void LogManager::AddLogOutputTarget(TAutoLogOPTargetPtr _outputTarget)
	{
		m_OutputTargetsList.push_back(TSharedLogOPTargetPtr(_outputTarget.release()) );
	}

	//--------------------------------------------------------------------
	//Static functions
	void LogManager::CreateInstance()
	{
		if(!m_Instance)
		{
			m_Instance = new LogManager();
		}
	}

	void LogManager::DestroyInstance()
	{
		if(m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	LogManager* LogManager::Get()
	{
		return m_Instance;
	}
	//--------------------------------------------------------------------
}