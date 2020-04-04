/********************************************************************
	filename: 	PANDUShaderAttribute
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderAttribute_h__
#define __PANDUShaderAttribute_h__

#include "../boost/smart_ptr.hpp"
#include "../Utils/PANDUString.h"
#include "PANDUGlInclude.h"
#include "PANDUGLError.h"
#include <map>

namespace Pandu
{
	class ShaderAttribute;

	typedef boost::shared_ptr<ShaderAttribute> TSharedShaderAttributePtr;
	typedef std::map<const String,TSharedShaderAttributePtr> TNameShaderAttributeMap;

	class ShaderAttribute
	{
	public:

		enum HardwareAttributeLocations
		{
			eHAL_Vertex			= 0,		//reserved can't be used
			eHAL_Normal			= 2,		//reserved can't be used
			eHAL_Color			= 3,
			eHAL_SecondaryColor	= 4,
			eHAL_FogCoord		= 5,
			eHAL_MultiTexCoord0	= 8,		//reserved can't be used
			eHAL_MultiTexCoord1	= 9,
			eHAL_MultiTexCoord2	= 10,
			eHAL_MultiTexCoord3	= 11,
			eHAL_MultiTexCoord4	= 12,
			eHAL_MultiTexCoord5	= 13,
			eHAL_MultiTexCoord6	= 14,
			eHAL_MultiTexCoord7	= 15,
		};

		enum AttributeLocationsAvailable
		{
			eALA_Location0		= 1,
			eALA_Location1		= eHAL_Color,
			eALA_Location2		= eHAL_SecondaryColor,
			eALA_Location3		= eHAL_FogCoord,
			eALA_Location4		= 6,
			eALA_Location5		= 7,
			eALA_Location6		= eHAL_MultiTexCoord1,
			eALA_Location7		= eHAL_MultiTexCoord2,
			eALA_Location8		= eHAL_MultiTexCoord3,
			eALA_Location9		= eHAL_MultiTexCoord4,
			eALA_Location10		= eHAL_MultiTexCoord5,
			eALA_Location11		= eHAL_MultiTexCoord6,
			eALA_Location12		= eHAL_MultiTexCoord7,
		};

	private:

		friend class ShaderPass;

		const String m_AttributeName;
		const GLint m_AttributeLocation;
	
		ShaderAttribute(const String& _name, GLint _attributeLocation);

	public:

		~ShaderAttribute();

		inline const String& GetName() const				{	return m_AttributeName;			}
		
		void SetValue1(const float* _attributeArray);
		void SetValue2(const float* _attributeArray);
		void SetValue3(const float* _attributeArray);
		void SetValue4(const float* _attributeArray);

		void EnableAttribute();
		void DisableAttribute();

		inline GLint GetGLAttributeLocation() const				{	return m_AttributeLocation;		}
	};
}

#endif //__PANDUShaderAttribute_h__