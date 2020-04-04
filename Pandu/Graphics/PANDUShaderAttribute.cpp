#include "PANDUShaderAttribute.h"

namespace Pandu
{
	ShaderAttribute::ShaderAttribute(const String& _name, GLint _attributeLocation)
		: m_AttributeName(_name)
		, m_AttributeLocation(_attributeLocation)
	{
	}

	ShaderAttribute::~ShaderAttribute()
	{
	}

	void ShaderAttribute::SetValue1( const float* _attributeArray )
	{
		glVertexAttribPointer(m_AttributeLocation,1,GL_FLOAT,GL_FALSE,0,_attributeArray);
	}

	void ShaderAttribute::SetValue2( const float* _attributeArray )
	{
		glVertexAttribPointer(m_AttributeLocation,2,GL_FLOAT,GL_FALSE,0,_attributeArray);
	}

	
	void ShaderAttribute::SetValue3( const float* _attributeArray )
	{
		glVertexAttribPointer(m_AttributeLocation,3,GL_FLOAT,GL_FALSE,0,_attributeArray);
	}

	void ShaderAttribute::SetValue4( const float* _attributeArray )
	{
		glVertexAttribPointer(m_AttributeLocation,4,GL_FLOAT,GL_FALSE,0,_attributeArray);
	}

	void ShaderAttribute::EnableAttribute()
	{
		glEnableVertexAttribArray(m_AttributeLocation);
		CHECK_GL_ERROR();
	}

	void ShaderAttribute::DisableAttribute()
	{
		glDisableVertexAttribArray(m_AttributeLocation);
		CHECK_GL_ERROR();
	}

}