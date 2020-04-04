#include "PANDUShaderEffect.h"

namespace Pandu
{
	//===============================================================================
	//ShaderEffect Implementation

	ShaderEffect::ShaderEffect()
		: m_TechniqueCount(0)
	{

	}

	ShaderEffect::~ShaderEffect()
	{
		ClearAllTechniques();
	}

	void ShaderEffect::ClearAllTechniques()
	{
		TTechniqueIDShaderPtrMap::iterator itr = m_AllTechniques.begin();
		TTechniqueIDShaderPtrMap::iterator end = m_AllTechniques.end();

		while( itr != end )
		{
			PANDU_ERROR( (*itr).second.use_count() == 1, "Shader pass is still in use, first clear the reference used" );
			(*itr).second.reset();
			itr++;
		}

		m_AllTechniques.clear();
	}

	TSharedShaderTechniquePtr ShaderEffect::CreateShaderTechnique(unsigned int _techniqueID)
	{
		TTechniqueIDShaderPtrMap::iterator itr = m_AllTechniques.find(_techniqueID);
		TTechniqueIDShaderPtrMap::iterator end = m_AllTechniques.end();

		if( itr != end )
		{
			PANDU_ERROR( false , (String("Technique id '") + _techniqueID + "' already exist in shader effect").CString() );
			return TSharedShaderTechniquePtr();
		}

		TSharedShaderTechniquePtr newTechnique(new ShaderTechnique(_techniqueID));
		m_AllTechniques.insert(std::pair<unsigned int,TSharedShaderTechniquePtr>(_techniqueID,newTechnique));

		m_TechniqueCount = (unsigned int)m_AllTechniques.size();
		
		return newTechnique;
	}

	TSharedShaderTechniquePtr ShaderEffect::GetTechnique(unsigned short _techniqueID)
	{
		TTechniqueIDShaderPtrMap::iterator itr = m_AllTechniques.find(_techniqueID);
		TTechniqueIDShaderPtrMap::iterator end = m_AllTechniques.end();

		if( itr == end )
		{
			return TSharedShaderTechniquePtr();
		}

		return (*itr).second;
	}
}
