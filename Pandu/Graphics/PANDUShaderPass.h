/********************************************************************
	filename: 	PANDUShaderPass
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderPass_h__
#define __PANDUShaderPass_h__

#include "PANDUVertexProgram.h"
#include "PANDUFragmentProgram.h"
#include "PANDUShaderParameter.h"
#include "PANDUShaderAttribute.h"
#include "PANDUVertexBuffer.h"
#include "PANDUAutoShaderParameter.h"

namespace Pandu
{
	class ShaderPass;

	typedef boost::shared_ptr<ShaderPass> TSharedShaderPassPtr;
	typedef std::vector<TSharedShaderPassPtr> TSharedShaderPassArray;
	typedef std::vector<GLuint> TShaderParameterLocationArray;
	
	class ShaderPass
	{
	protected:

		friend class ShaderTechnique;

		GLuint						m_ProgramObject;                      // GLProgramObject
		bool						m_Linked;
		
		TSharedVertexProgramPtr		m_VertexProgram;
		TSharedFragmentProgramPtr	m_FragmentProgram;
		TNameShaderParameterMap		m_AllShaderParameters;
		TSharedShaderAttributePtr	m_AllShaderAttributes[VertexBuffer::eBT_Count];
		TSharedShaderParameterPtr	m_AutoShaderParams[AutoShaderParameter::ASPT_Count];
	
		ShaderPass(TSharedVertexProgramPtr _vertexProgram, TSharedFragmentProgramPtr _fragmentProgram);

		void Create();
		void Destroy();
		void DetachAllShaders();
		void AttachAllShaders();

		bool Link();

		void AddAutoShaderParameters();
		bool IsAutoShaderParameter(const String& _paramName);

		TSharedShaderParameterPtr CreateShaderParameterInternal(const String& _paramName, ShaderParameter::ParameterType _parameterType, unsigned int _count);	

		void DestroyAllShaderParameters();
		void DestroyAllShaderAttributes();

	public:

		~ShaderPass();
		TSharedShaderParameterPtr CreateShaderParameter(const String& _paramName, ShaderParameter::ParameterType _parameterType, unsigned int _count);	
		TSharedShaderAttributePtr CreateShaderAttributeForBuffer(VertexBuffer::BufferType _bt, unsigned int _attributeLocation);
		
		TSharedShaderAttributePtr GetShaderAttributeForBuffer(VertexBuffer::BufferType _bt);

		void SetAutoShaderParameterValue(AutoShaderParameter::AutoShaderParameterType _paramType, const Matrix44& _value);

		void BeginPass();
		void EndPass();
	};
}

#endif