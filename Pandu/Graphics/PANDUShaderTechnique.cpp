#include "PANDUShaderTechnique.h"

namespace Pandu
{
	//===============================================================================
	//ShaderTechnique Implementation
	ShaderTechnique::ShaderTechnique(unsigned int _techniqueID)
		: m_TechniqueID(_techniqueID)
	{

	}

	ShaderTechnique::~ShaderTechnique()
	{
		ClearAllPasses();
	}

	void ShaderTechnique::ClearAllPasses()
	{
		unsigned short count = (unsigned short)m_AllPasses.size();

		for( int i = 0 ; i < count ; i++ )
		{
			PANDU_ERROR( m_AllPasses[i].use_count() == 1, "Shader pass is still in use, first clear the reference used" );
			m_AllPasses[i].reset();
		}

		m_AllPasses.clear();
	}

	TSharedShaderPassPtr ShaderTechnique::AddShaderPass(TSharedVertexProgramPtr _vertexProgram,TSharedFragmentProgramPtr _fragmentProgram)
	{
		unsigned short count = (unsigned short)m_AllPasses.size();

		PANDU_ERROR( count < MAX_PASS_COUNT, 
			(String("Shader pass can't be more than ") + MAX_PASS_COUNT).CString() );

		if(count < MAX_PASS_COUNT)
		{
			TSharedShaderPassPtr pass(new ShaderPass(_vertexProgram,_fragmentProgram));
			if(pass->Link())
			{
				m_AllPasses.push_back(pass);
				return pass;
			}
			else 
			{
				gLogManager->WriteErrorLog("Unable to link shader pass");
				pass.reset();
				return TSharedShaderPassPtr();
			}
		}
		return TSharedShaderPassPtr();
	}
}
