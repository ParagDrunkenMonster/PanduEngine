/********************************************************************
	filename: 	PANDUShaderParameter
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderParameter_h__
#define __PANDUShaderParameter_h__

#include "../Math/PANDUVector2.h"
#include "../Math/PANDUMatrix44.h"
#include "../boost/smart_ptr.hpp"
#include "../Utils/PANDUString.h"
#include "../Utils/PANDUCoreDefines.h"
#include "PANDUGLError.h"
#include <map>

namespace Pandu
{
	class ShaderParameter;

	typedef boost::shared_ptr<ShaderParameter> TSharedShaderParameterPtr;
	typedef std::map<const String,TSharedShaderParameterPtr> TNameShaderParameterMap;

	class ShaderParameter
	{
	public:

		friend class ShaderPass;

		enum ParameterType
		{
			ePT_Texture = 0,
			ePT_Float,
			ePT_Float2,
			ePT_Float3,
			ePT_Float4,
			ePT_Float2x2,
			ePT_Float3x3,
			ePT_Float4x4,

			ePT_Count
		};
	private:

		const String			m_ParamName;
		const GLint				m_ParameterLocation;
		const ParameterType		m_ParameterType;
		const unsigned int		m_DataCount;
		unsigned int			m_DataSize;
		char*					m_Buffer;
		bool					m_Dirty;
	
		ShaderParameter(const String& _name, GLint _paramLocation, ParameterType _parameterType, unsigned int _count = 1);
		
	public:

		~ShaderParameter();

		void SetValue(int _value);
		void SetValue(float _value);
		void SetValue(const Vector2& _value);
		void SetValue(const Matrix44& _matrix);

		inline ParameterType GetParameterType() const		{	return m_ParameterType;			}
		inline const String& GetName() const				{	return m_ParamName;				}

	private:

		void SetToGL();
		void AllocateBufferMemory();
	};
}

#endif //__PANDUShaderParameter_h__