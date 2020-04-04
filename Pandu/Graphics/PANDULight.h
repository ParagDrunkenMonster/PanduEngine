/********************************************************************
	filename: 	PANDULight
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDULight_h__
#define __PANDULight_h__

#include "../Math/PANDUVector3.h"
#include "../Math/PANDUMatrix44.h"
#include "../Utils/PANDUString.h"
#include "../boost/smart_ptr.hpp"
#include <list>

namespace Pandu
{
	class Light;

	typedef boost::shared_ptr<Light> TSharedLightPtr;
	typedef std::list<TSharedLightPtr> TLightList;

	class Light
	{
	public:

		enum LightType
		{
			LT_Point,
			LT_Directional,
			LT_Spot
		};

	private:

		String				m_Name;
		LightType			m_LightType;
		Vector3				m_LightPos;
		Vector3				m_LightDirection;

		//Color m_AmbientColor;
		//Color m_DiffuseColor;
		//Color m_SpecularColor;

		bool				m_Enabled;

		void ApplyPointLight(int _glLightID);
		void ApplyDirectionalLight(int _glLightID);
		void ApplySpotLight(int _glLightID);

	public:

		Light(const String& _name);
		virtual ~Light();

		const String& GetName();
		void SetDirection(const Vector3& _direction);
		void SetPosition(const Vector3& _position);
		void ApplyLight(const Matrix44& _mat, int _glLightID);
		inline void SetLightType(LightType _lightType)			{	m_LightType = _lightType;			}
		
		inline const Vector3& GetLightDirection() const			{	return m_LightDirection;			}
	};
}

#endif