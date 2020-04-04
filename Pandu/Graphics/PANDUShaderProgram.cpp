#include "PANDUShaderProgram.h"

namespace Pandu
{
	ShaderProgram::ShaderProgram(ShaderProgramType _shaderType,const String& _filePath)
		: m_FilePath(_filePath)
		, m_ShaderType(_shaderType)
		, m_Loaded(false)
		, m_Compiled(false)
		, m_ProgramName(File::GetFileName(_filePath))
	{

	}

	ShaderProgram::~ShaderProgram()
	{
		Destroy();
	}

	void ShaderProgram::Destroy()
	{
		if (m_Compiled)
		{ 
			m_Loaded = false;
			m_Compiled = false;
			glDeleteObjectARB(m_GLShaderObject);
			//TODO: Check GL Error
			//CHECK_GL_ERROR();
		}
	}

	bool ShaderProgram::Load()
	{
		if( m_Loaded )
		{
			gLogManager->WriteLog("ShaderProgram already loaded");
			return false;
		}

		File file(m_FilePath);

		if(file.Open(File::eFM_Read))
		{
			unsigned long len = file.GetFileLength();

			if ( len == 0 )
			{
				return false;
			}

			char* shaderSource = (char*)malloc(len + 1);

			if ( shaderSource == NULL )
			{
				gLogManager->WriteLog("Memory not available to load shader " + m_FilePath );
				return false;
			}

			file.LoadStringToBuffer(shaderSource,len + 1);
			m_ShaderSource = shaderSource;
			free(shaderSource);
			m_Loaded = true;
		}
		else
		{
			gLogManager->WriteLog("Unable to read shader file : " + m_FilePath);
			return false;
		}
		return true;
	}

	bool ShaderProgram::Compile()
	{
		//if (!useGLSL) return false;

		if( !m_Loaded )
		{
			gLogManager->WriteLog("ShaderProgram file " + m_FilePath + " is not loaded");
			return false;
		}

		if( !m_Compiled )
		{
			GLint compiled = 0;

			GLint	length = (GLint) m_ShaderSource.Length();
			const char* source = m_ShaderSource.CString();
			glShaderSourceARB(m_GLShaderObject, 1, (const GLcharARB **)&source, &length);
			CHECK_GL_ERROR();

			glCompileShaderARB(m_GLShaderObject); 
			CHECK_GL_ERROR();
			glGetObjectParameterivARB(m_GLShaderObject, GL_COMPILE_STATUS, &compiled);
			CHECK_GL_ERROR();
			if( !compiled )
			{
				int maxLength = 0;
				glGetShaderiv(m_GLShaderObject, GL_INFO_LOG_LENGTH, &maxLength);
 
				if( maxLength > 0 )
				{
					/* The maxLength includes the NULL character */
					char *fragmentInfoLog = new char[maxLength];
				 
					glGetShaderInfoLog(m_GLShaderObject, maxLength, &maxLength, fragmentInfoLog);
				 
					/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
					/* In this simple program, we'll just leave */
					delete [] fragmentInfoLog;
				}
			}

			if (compiled) 
			{
				m_Compiled = true;
			}

			return m_Compiled;
		}
		else
		{
			return true;
		}
		return m_Compiled;
	}

	GLint ShaderProgram::GetAttribLocation(const String& _attribName)
	{
		return glGetAttribLocationARB(m_GLShaderObject, _attribName.CString());
	}
}