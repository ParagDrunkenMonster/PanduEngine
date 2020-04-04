#include "PANDUShaderParameter.h"

namespace
{
	const unsigned int kElementCount[Pandu::ShaderParameter::ePT_Count] =
																	{
																		1,		//ePT_Texture,
																		1,		//ePT_Float,
																		2,		//ePT_Float2,
																		3,		//ePT_Float3,
																		4,		//ePT_Float4,
																		4,		//ePT_Float2x2,
																		9,		//ePT_Float3x3,
																		16,		//ePT_Float4x4,
																	};

	const unsigned int kElementSize[Pandu::ShaderParameter::ePT_Count] =
																	{
																		sizeof(GLint),		//ePT_Texture,
																		sizeof(GLfloat),	//ePT_Float,
																		sizeof(GLfloat),	//ePT_Float2,
																		sizeof(GLfloat),	//ePT_Float3,
																		sizeof(GLfloat),	//ePT_Float4,
																		sizeof(GLfloat),	//ePT_Float2x2,
																		sizeof(GLfloat),	//ePT_Float3x3,
																		sizeof(GLfloat),	//ePT_Float4x4,
																	};
}

namespace Pandu
{
	ShaderParameter::ShaderParameter(const String& _name, GLint _paramLocation, ParameterType _parameterType, unsigned int _count)
		: m_ParamName(_name)
		, m_ParameterLocation(_paramLocation)
		, m_ParameterType(_parameterType)
		, m_DataCount(_count)
		, m_DataSize(0)
		, m_Buffer(NULL)
		, m_Dirty(true)
	{
		AllocateBufferMemory();
	}

	ShaderParameter::~ShaderParameter()
	{
		if(m_Buffer)
		{
			std::free(m_Buffer);
			m_Buffer = NULL;
		}
	}

	void ShaderParameter::SetValue(float _value)
	{
		PANDU_ERROR(m_Buffer,"Data buffer not created");
		if( _value != *m_Buffer )
		{
			memcpy(m_Buffer,&_value,m_DataSize);
			m_Dirty = true;
		}
	}

	void ShaderParameter::SetValue(int _value)
	{
		PANDU_ERROR(m_Buffer,"Data buffer not created");
		if( _value != *m_Buffer )
		{
			memcpy(m_Buffer,&_value,m_DataSize);
			m_Dirty = true;
		}
	}

	void ShaderParameter::SetValue(const Vector2& _value)
	{
		PANDU_ERROR(m_Buffer,"Data buffer not created");
		memcpy(m_Buffer,_value.arr,m_DataSize);
		m_Dirty = true;
	}

	void ShaderParameter::SetValue(const Matrix44& _matrix)
	{
		PANDU_ERROR(m_Buffer,"Data buffer not created");
		unsigned short p = 0;
		float *buffer = (float*)m_Buffer;
		for(unsigned short i = 0 ; i < 4 ; i++)
		{
			for(unsigned short j = 0 ; j < 4 ; j++)
			{
				if( p < kElementCount[m_ParameterType] )
				{
					if( buffer[p] != _matrix[j][i] )
					{
						buffer[p] = _matrix[j][i];
						m_Dirty = true;
					}
				}
				else
				{
					break;
				}
				p++;
			}
		}
	}

	void ShaderParameter::SetToGL()
	{
		if( !m_Dirty )
		{
			return;
		}
		switch(m_ParameterType)
		{
		case ePT_Texture:
			{
				GLint* fVal = (GLint*)m_Buffer;
				glUniform1i(m_ParameterLocation,*fVal);
				//CHECK_GL_ERROR();
			}
			break;

		case ePT_Float:
			{
				GLfloat* fVal = (GLfloat*)m_Buffer;
				glUniform1f(m_ParameterLocation,*fVal);
				//CHECK_GL_ERROR();
			}
			break;

		case ePT_Float2:
			{
				GLfloat* fVal = (GLfloat*)m_Buffer;
				glUniform2f(m_ParameterLocation,*fVal,*(fVal + 1));
				//CHECK_GL_ERROR();
			}
			break;

		case ePT_Float3:
			{
			}
			break;

		case ePT_Float4:
			{
			}
			break;

		case ePT_Float2x2:
			{
			}
			break;

		case ePT_Float3x3:
			{
			}
			break;

		case ePT_Float4x4:
			{
				GLfloat* fVal = (GLfloat*)m_Buffer;
				glUniformMatrix4fv(m_ParameterLocation,m_DataCount,GL_FALSE,fVal);
			}
			break;
		
		default:
			{
			}
			break;
		}

		m_Dirty = false;
	}

	void ShaderParameter::AllocateBufferMemory()
	{
		if( !m_Buffer )
		{
			m_DataSize = kElementSize[m_ParameterType]*kElementCount[m_ParameterType]*m_DataCount;
			m_Buffer = static_cast<char*>(malloc(m_DataSize));
		}
	}
}