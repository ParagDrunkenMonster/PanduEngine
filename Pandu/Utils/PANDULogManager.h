/********************************************************************
	filename: 	PANDULogManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDULogManager_h__
#define __PANDULogManager_h__

#include <vector>
#include "PANDULogOutputTarget.h"

namespace Pandu
{
	class ApplicationRoot;

	class LogManager
	{
	private:

		friend class ApplicationRoot;

		typedef std::vector<TSharedLogOPTargetPtr> TLogOutputTargetsList;

		TLogOutputTargetsList m_OutputTargetsList;

	private:

		LogManager();
		~LogManager();

		void Destroy();

		static LogManager *m_Instance;

	public:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static LogManager* Get();
		//--------------------------------------------------------------------

		void WriteLog(const String& _log);
		void WriteErrorLog(const String& _log);
		void WriteWarningLog(const String& _log);

		void AddLogOutputTarget(TAutoLogOPTargetPtr _outputTarget);
	};

#define gLogManager LogManager::Get()
}

#endif