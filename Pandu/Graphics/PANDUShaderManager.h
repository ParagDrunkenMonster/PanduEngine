/********************************************************************
	filename: 	PANDUShaderManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderManager_h__
#define __PANDUShaderManager_h__

#include "PANDUShaderEffect.h"
#include <map>

namespace Pandu
{
	class ShaderManager
	{
	private:

		typedef std::map<const String,unsigned int> TTechniqueNameIDMap;

		friend class ApplicationRoot;

		TNameShaderProgramMap m_AllShaderPrograms[ShaderProgram::SPT_Count];
		TTechniqueNameIDMap m_TechniqueMap;
	
		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		ShaderManager();
		~ShaderManager();

		static ShaderManager *m_Instance;

		void ClearAllShaderPrograms();
		void ClearAllShaderProgramsFromList(TNameShaderProgramMap& _programList);

	public:

		//--------------------------------------------------------------------
		//Static functions
		static ShaderManager* Get();
		//--------------------------------------------------------------------
		
		TSharedVertexProgramPtr CreateVertexProgram(const String& _filePath);
		TSharedFragmentProgramPtr CreateFragmentProgram(const String& _filePath);

		unsigned int ShaderTechniqueID(const String& _techniqueName);
	};

	#define gShaderManager ShaderManager::Get()
}

#endif