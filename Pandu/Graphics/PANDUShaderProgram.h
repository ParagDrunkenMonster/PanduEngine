/********************************************************************
	filename: 	PANDUShaderProgram
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShader_h__
#define __PANDUShader_h__

#include "../Utils/PANDUString.h"
#include "PANDUGlInclude.h"
#include "PANDUGLError.h"
#include <map>
#include "../Utils/PANDUFile.h"
#include "../Utils/PANDUStringTokenizer.h"

#include "../boost/smart_ptr.hpp"

namespace Pandu
{
	class ShaderProgram;

	typedef boost::shared_ptr<ShaderProgram> TSharedShaderProgramPtr;
	typedef std::pair<const String,TSharedShaderProgramPtr> TNameShaderProgramPair;
	typedef std::map<const String,TSharedShaderProgramPtr> TNameShaderProgramMap;

	class ShaderProgram
	{
	public:

		friend class ShaderPass;
		friend class ShaderManager;

		enum ShaderProgramType
		{
			SPT_Vertex = 0,
			SPT_Fragment,

			SPT_Count
		};

	protected:

		const String m_FilePath;
		const String m_ProgramName;
		ShaderProgramType m_ShaderType;
		String m_ShaderSource;
		GLuint m_GLShaderObject;
		bool m_Loaded;
		bool m_Compiled;

		ShaderProgram(ShaderProgramType _shaderType,const String& _filePath);

		void Destroy();
		
		GLint GetAttribLocation(const String& _attribName);
		inline ShaderProgramType GetShaderType() const					{	return m_ShaderType;		}
		
		inline GLuint GetShaderObject() const							{	return m_GLShaderObject;	}

		bool Load();
		bool Compile();

	public:

		virtual ~ShaderProgram();

		inline const String& GetFilePath() const				{	return m_FilePath;			}
		inline const String& GetName() const					{	return m_ProgramName;		}
	};

}

#endif