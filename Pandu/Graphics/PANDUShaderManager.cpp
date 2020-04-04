#include "PANDUShaderManager.h"

namespace
{
	using namespace Pandu;

	static bool glslExtensionsInitialized = false;
	static bool glslAvailable = false;
	static bool glslGPUShader4 = false;

	bool HasGLSLSupport();
	bool InitOpenGLExtensions();
	bool HasGeometryShaderSupport();
	bool HasShaderModel4();

	bool InitOpenGLExtensions()
	{
		if (glslExtensionsInitialized) 
		{
			return true;
		}

		glslExtensionsInitialized = true;

		GLenum err = glewInit();

		String error;

		if (GLEW_OK != err)
		{
			error = error + "Error:";
			error = error + (char*)(glewGetErrorString(err));
			glslExtensionsInitialized = false;
			return false;
		}

		error = error + "OpenGL Vendor: ";
		error = error + (char*)(glGetString(GL_VENDOR));
		error = error + "\n";
		error = error + "OpenGL Renderer: ";
		error = error + (char*)(glGetString(GL_RENDERER));
		error = error + ("\n");
		error = error + "OpenGL Version: ";
		error = error + (char*)(glGetString(GL_VERSION));
		error = error + "\n\n";
		//cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n\n";
		
		gLogManager->WriteLog(error);
		
		HasGLSLSupport();

		return true;
	}


	bool HasGLSLSupport()
	{
		glslGPUShader4     = HasShaderModel4();

		if (glslAvailable) 
		{
			return true;  // already initialized and GLSL is available
		}

		glslAvailable = true;

		String mssg;

		if (GLEW_VERSION_2_0)
		{
			mssg = mssg + "OpenGL 2.0 (or higher) is available!";
		}
		else if (GLEW_VERSION_1_5)
		{
			mssg = mssg + "OpenGL 1.5 core functions are available";
		}
		else if (GLEW_VERSION_1_4)
		{
			mssg = mssg + "OpenGL 1.4 core functions are available";
		}
		else if (GLEW_VERSION_1_3)
		{
			mssg = mssg + "OpenGL 1.3 core functions are available";
		}
		else if (GLEW_VERSION_1_2)
		{
			mssg = mssg + "OpenGL 1.2 core functions are available";
		}

		if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
		{
			mssg = mssg + "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
			glslAvailable = false;
		}

		if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
		{
			mssg = mssg + "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
			glslAvailable = false;
		}

		if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
		{
			mssg = mssg + "[WARNING] GL_ARB_shader_objects extension is not available!\n";
			glslAvailable = false;
		}

		if (glslAvailable)
		{
			mssg = mssg + "[OK] OpenGL Shading Language is available!\n\n";
		}
		else
		{
			mssg = mssg + "[FAILED] OpenGL Shading Language is not available...\n\n";
		}   

		gLogManager->WriteLog(mssg);

		return glslAvailable;
	}

	bool HasGeometryShaderSupport(void)
	{
		if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
			return false;

		return true;
	}

	bool HasShaderModel4(void)
	{
		if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
			return false;

		return true;
	}
}

namespace Pandu
{
	ShaderManager* ShaderManager::m_Instance = NULL;
	
	ShaderManager::ShaderManager()
	{
		InitOpenGLExtensions();
	}

	ShaderManager::~ShaderManager()
	{
		ClearAllShaderPrograms();
	}
	
	void ShaderManager::ClearAllShaderPrograms()
	{
		for( unsigned int i = 0 ; i < ShaderProgram::SPT_Count ; i++ )
		{
			ClearAllShaderProgramsFromList(m_AllShaderPrograms[i]);
		}
	}

	void ShaderManager::ClearAllShaderProgramsFromList(TNameShaderProgramMap& _programList)
	{
		TNameShaderProgramMap::iterator itr = _programList.begin();
		TNameShaderProgramMap::iterator end = _programList.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).second.use_count() == 1, (String("Shader program named ") + (*itr).first
				+ " is still in use, first clear the reference before clear.." ).CString() );

			(*itr).second.reset();
		}

		_programList.clear();
	}

	TSharedVertexProgramPtr ShaderManager::CreateVertexProgram(const String& _filePath)
	{
		TNameShaderProgramMap::iterator itr = m_AllShaderPrograms[ShaderProgram::SPT_Vertex].find(File::GetFileName(_filePath));

		if( itr != m_AllShaderPrograms[ShaderProgram::SPT_Vertex].end() )
		{
			if( (*itr).second->GetShaderType() == ShaderProgram::SPT_Vertex)
			{
				return boost::static_pointer_cast<VertexProgram>((*itr).second);
			}
			else
			{
				return TSharedVertexProgramPtr();
			}
		}
		else
		{
			TSharedVertexProgramPtr newVertexProgram(new VertexProgram(_filePath));
			
			if(newVertexProgram->Load())
			{
				if(newVertexProgram->Compile())
				{
					m_AllShaderPrograms[ShaderProgram::SPT_Vertex].insert(TNameShaderProgramPair(newVertexProgram->GetName(),newVertexProgram) );
					return newVertexProgram;
				}
				else
				{
					gLogManager->WriteLog("Unable to compile shader program");
					newVertexProgram.reset();
				}
			}
			else
			{
				gLogManager->WriteLog("Unable to load shader program");
				newVertexProgram.reset();
			}
		}

		return TSharedVertexProgramPtr();
	}

	TSharedFragmentProgramPtr ShaderManager::CreateFragmentProgram(const String& _filePath)
	{
		TNameShaderProgramMap::iterator itr = m_AllShaderPrograms[ShaderProgram::SPT_Fragment].find(File::GetFileName(_filePath));

		if( itr != m_AllShaderPrograms[ShaderProgram::SPT_Fragment].end() )
		{
			if( (*itr).second->GetShaderType() == ShaderProgram::SPT_Fragment)
			{
				return boost::static_pointer_cast<FragmentProgram>((*itr).second);
			}
			else
			{
				return TSharedFragmentProgramPtr();
			}
		}
		else
		{
			TSharedFragmentProgramPtr newFragmentProgram(new FragmentProgram(_filePath));

			if(newFragmentProgram->Load())
			{
				if(newFragmentProgram->Compile())
				{
					m_AllShaderPrograms[ShaderProgram::SPT_Fragment].insert(TNameShaderProgramPair(newFragmentProgram->GetName(),newFragmentProgram));
					return newFragmentProgram;
				}
				else
				{
					gLogManager->WriteLog("Unable to compile shader program");
					newFragmentProgram.reset();
				}
			}
			else
			{
				gLogManager->WriteLog("Unable to load shader program");
				newFragmentProgram.reset();
			}
		}

		return TSharedFragmentProgramPtr();
	}

	unsigned int ShaderManager::ShaderTechniqueID(const String& _techniqueName)
	{
		TTechniqueNameIDMap::iterator itr = m_TechniqueMap.find(_techniqueName);
		TTechniqueNameIDMap::iterator end = m_TechniqueMap.end();

		if( itr != end )
		{
			return (*itr).second;
		}
		
		unsigned int techniqueID = (unsigned int)m_TechniqueMap.size();
		m_TechniqueMap.insert(std::pair<const String,unsigned int>(_techniqueName,techniqueID));
		return techniqueID;
	}

	//--------------------------------------------------------------------
	//Static functions
	void ShaderManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new ShaderManager();
		}
	}

	void ShaderManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	ShaderManager* ShaderManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------
}