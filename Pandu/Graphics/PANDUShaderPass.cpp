#include "PANDUShaderPass.h"

namespace Pandu
{
	ShaderPass::ShaderPass( TSharedVertexProgramPtr _vertexProgram,TSharedFragmentProgramPtr  _fragmentProgram)
		: m_VertexProgram(_vertexProgram)
		, m_FragmentProgram(_fragmentProgram)
		, m_ProgramObject(0)
		, m_Linked(false)
	{
		Create();
	}

	ShaderPass::~ShaderPass()
	{
		Destroy();
	}

	void ShaderPass::Create()
	{
		m_ProgramObject = glCreateProgram();
	}

	void ShaderPass::Destroy()
	{
		DestroyAllShaderParameters();
		CHECK_GL_ERROR();
		DestroyAllShaderAttributes();
		CHECK_GL_ERROR();
		DetachAllShaders();

		glDeleteProgram(m_ProgramObject);
		CHECK_GL_ERROR();

		m_VertexProgram.reset();
		m_FragmentProgram.reset();
		CHECK_GL_ERROR();
	}

	void ShaderPass::DetachAllShaders()
	{
		glDetachShader(m_ProgramObject, m_VertexProgram->GetShaderObject());
		CHECK_GL_ERROR();

		glDetachShader(m_ProgramObject, m_FragmentProgram->GetShaderObject());
		CHECK_GL_ERROR();
	}

	void ShaderPass::AttachAllShaders()
	{
		glAttachShader(m_ProgramObject, m_VertexProgram->GetShaderObject());
		CHECK_GL_ERROR();

		glAttachShader(m_ProgramObject, m_FragmentProgram->GetShaderObject());
		CHECK_GL_ERROR();

		m_Linked = false;
	}

	bool ShaderPass::Link()
	{
		if (m_Linked) 
		{
			DetachAllShaders();
		}

		AttachAllShaders();

		GLint linked; 
		glLinkProgram(m_ProgramObject);
		CHECK_GL_ERROR();
		glGetProgramiv(m_ProgramObject, GL_LINK_STATUS, &linked);
		CHECK_GL_ERROR();

		if (linked)
		{
			AddAutoShaderParameters();
			m_Linked = true;
			return true;
		}
		else
		{
			gLogManager->WriteLog("Unable to Link Shader");
		}

		return false;
	}

	//-----------------------------------------------------------------------------

	void ShaderPass::BeginPass()
	{
		if (m_Linked)
		{
			glUseProgram(m_ProgramObject);
			CHECK_GL_ERROR();
			TNameShaderParameterMap::iterator itr = m_AllShaderParameters.begin();
			TNameShaderParameterMap::iterator end = m_AllShaderParameters.end();

			while (itr != end )
			{
				(*itr).second->SetToGL();
				itr++;
			}

			for( unsigned int i = 0 ; i < AutoShaderParameter::ASPT_Count ; i++ )
			{
				if( m_AutoShaderParams[i] != NULL )
				{
					m_AutoShaderParams[i]->SetToGL();
				}
			}
		}
	}

	void ShaderPass::EndPass()
	{
		if (m_Linked)
		{
			glUseProgram(0);
			CHECK_GL_ERROR();
		}
	}

	TSharedShaderParameterPtr ShaderPass::CreateShaderParameter(const String& _paramName, ShaderParameter::ParameterType _parameterType, unsigned int _count)
	{
		TNameShaderParameterMap::iterator itr = m_AllShaderParameters.find(_paramName);
		if( itr != m_AllShaderParameters.end() || IsAutoShaderParameter(_paramName) )
		{
			gLogManager->WriteWarningLog("Parameter named '" + _paramName + "' already exist");
			return TSharedShaderParameterPtr();
		}

		TSharedShaderParameterPtr newParam = CreateShaderParameterInternal(_paramName,_parameterType,_count);
		if( newParam != NULL )
		{
			m_AllShaderParameters.insert(std::pair<const String,TSharedShaderParameterPtr>(_paramName,newParam) );
			return newParam;
		}

		gLogManager->WriteWarningLog("Parameter named '" + _paramName + "' does not exist");
		return TSharedShaderParameterPtr();
	}

	TSharedShaderParameterPtr ShaderPass::CreateShaderParameterInternal(const String& _paramName, ShaderParameter::ParameterType _parameterType, unsigned int _count)
	{
		GLint location = glGetUniformLocation(m_ProgramObject,_paramName.CString());
		if( location >= 0 )
		{
			TSharedShaderParameterPtr shaderParam(new ShaderParameter(_paramName,location,_parameterType,_count));
			return shaderParam;
		}

		return TSharedShaderParameterPtr();
	}

	TSharedShaderAttributePtr ShaderPass::CreateShaderAttributeForBuffer(VertexBuffer::BufferType _bt, unsigned int _attributeLocation)
	{
		if( _bt < 0 || _bt >= VertexBuffer::eBT_Count || !VertexBuffer::CanCreateAttributeOfVertexBufferType(_bt) 
				|| m_AllShaderAttributes[_bt] != NULL )
		{
			return TSharedShaderAttributePtr();
		}

		const String& attribName = VertexBuffer::CustomAttributeNames(_bt);

		glBindAttribLocation(m_ProgramObject,_attributeLocation,attribName.CString());

		if(Link())
		{
			GLint location = glGetAttribLocation(m_ProgramObject,attribName.CString());
			if( location >= 0 )
			{
				TSharedShaderAttributePtr shaderAttrib(new ShaderAttribute(attribName,location));
				m_AllShaderAttributes[_bt] = shaderAttrib;
				return shaderAttrib;
			}
		}

		gLogManager->WriteWarningLog("Attribute named '" + attribName + "' does not exist");
		return TSharedShaderAttributePtr();
	}

	TSharedShaderAttributePtr ShaderPass::GetShaderAttributeForBuffer(VertexBuffer::BufferType _bt)
	{
		if( _bt < 0 || _bt >= VertexBuffer::eBT_Count )
		{
			return TSharedShaderAttributePtr();
		}
		return m_AllShaderAttributes[_bt];
	}

	void ShaderPass::AddAutoShaderParameters()
	{
		for( int i = 0 ; i < AutoShaderParameter::ASPT_Count ; i++ )
		{
			if( m_AutoShaderParams[i] == NULL )
			{
				const AutoShaderParameter::AutoShaderParameterType type = (AutoShaderParameter::AutoShaderParameterType)i;
				TSharedShaderParameterPtr newParam = CreateShaderParameterInternal(AutoShaderParameter::GetAutoShaderParameterName(type),AutoShaderParameter::GetAutoShaderParameterType(type),1);
				if( newParam != NULL )
				{
					m_AutoShaderParams[i] = newParam;
				}
			}
		}
	}

	bool ShaderPass::IsAutoShaderParameter(const String& _paramName)
	{
		for( int i = 0 ; i < AutoShaderParameter::ASPT_Count ; i++ )
		{
			const AutoShaderParameter::AutoShaderParameterType type = (AutoShaderParameter::AutoShaderParameterType)i;
			if( _paramName == AutoShaderParameter::GetAutoShaderParameterName(type) )
			{
				return true;
			}
		}
		return false;
	}

	void ShaderPass::SetAutoShaderParameterValue(AutoShaderParameter::AutoShaderParameterType _paramType, const Matrix44& _value)
	{
		if( _paramType < 0 || _paramType >= AutoShaderParameter::ASPT_Count )
		{
			return;
		}

		if( m_AutoShaderParams[_paramType] )
		{
			m_AutoShaderParams[_paramType]->SetValue(_value);
		}
	}

	void ShaderPass::DestroyAllShaderParameters()
	{
		TNameShaderParameterMap::iterator itr = m_AllShaderParameters.begin();
		TNameShaderParameterMap::iterator end = m_AllShaderParameters.end();

		while (itr != end )
		{
			PANDU_ERROR((*itr).second.use_count() == 1, (String("Shader parameter named '") + (*itr).first 
				+ "' is still in use, first clear the reference before destroy" ).CString() );

			(*itr).second.reset();
			itr++;
		}

		m_AllShaderParameters.clear();

		for( int i = 0 ; i < AutoShaderParameter::ASPT_Count ; i++ )
		{
			PANDU_ERROR(m_AutoShaderParams[i].use_count() <= 1, (String("Shader parameter named '") + m_AutoShaderParams[i]->GetName() 
				+ "' is still in use, first clear the reference before destroy" ).CString() );
			m_AutoShaderParams[i].reset();
		}
	}

	void ShaderPass::DestroyAllShaderAttributes()
	{
		for( unsigned int i = 0 ; i < VertexBuffer::eBT_Count ; i++ )
		{
			PANDU_ERROR(m_AllShaderAttributes[i].use_count() <= 1, (String("Shader parameter named '") + m_AllShaderAttributes[i]->GetName()
				+ "' is still in use, first clear the reference before destroy" ).CString() );

			m_AllShaderAttributes[i].reset();
		}
	}
}
