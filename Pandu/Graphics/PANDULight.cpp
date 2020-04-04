#include "PANDULight.h"
#include "PANDUGlInclude.h"

namespace Pandu
{
	Light::Light(const String& _name)
		: m_Name(_name)
		, m_LightType(LT_Directional)
		, m_LightDirection(Vector3(1.0f,1.0f,-1.0f))
		, m_LightPos(Vector3(100.0f,100.0f,100.0f))
		, m_Enabled(true)
	{
		m_LightDirection.Normalize();
	}
	
	Light::~Light()
	{

	}

	const String& Light::GetName()
	{
		return m_Name;
	}

	void Light::SetDirection(const Vector3& _direction)
	{
		m_LightDirection = _direction;
	}

	void Light::SetPosition(const Vector3& _position)
	{
		m_LightPos = _position;
	}

	void Light::ApplyLight(const Matrix44& _mat, int _glLightID)
	{
		float matrix[16];

		int p = 0;
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0 ; j < 4 ; j++)
			{
				matrix[p] = _mat[j][i];
				p++;
			}
		}

		glPushMatrix();
		glMultMatrixf(matrix);

		switch(m_LightType)
		{
		case LT_Point:
			{
				ApplyPointLight(_glLightID);
			}
			break;

		case LT_Directional:
			{
				ApplyDirectionalLight(_glLightID);
			}
			break;

		case LT_Spot:
			{
				ApplySpotLight(_glLightID);
			}
			break;
		}

		glPopMatrix();
	}

	void Light::ApplyPointLight(int _glLightID)
	{
		const float ambientColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float diffuseColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float specularColor[4] = {1.0f,1.0f,1.0f,1.0f};
		float* lightPos3 = m_LightPos.GetArray();
		float lightPos[4];

		memcpy(lightPos,lightPos3,3);
		lightPos[3] = 1.0f;

		glEnable(_glLightID);		
		glLightfv(_glLightID, GL_AMBIENT, ambientColor);
		glLightfv(_glLightID, GL_DIFFUSE, diffuseColor);
		glLightfv(_glLightID, GL_SPECULAR,specularColor);
		glLightfv(_glLightID, GL_POSITION,lightPos);
	}

	void Light::ApplyDirectionalLight(int _glLightID)
	{
		const float ambientColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float diffuseColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float specularColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float lightPos[4] = {m_LightDirection.x,m_LightDirection.y,m_LightDirection.z,0.0f};

		glEnable(_glLightID);		
		glLightfv(_glLightID, GL_AMBIENT, ambientColor);
		glLightfv(_glLightID, GL_DIFFUSE, diffuseColor);
		glLightfv(_glLightID, GL_SPECULAR,specularColor);
		glLightfv(_glLightID, GL_POSITION,lightPos );// m_LightPos.GetArray());
		//glLightfv(_glLightID, GL_SPOT_DIRECTION, m_LightDirection.GetArray());
	}

	void Light::ApplySpotLight(int _glLightID)
	{
		const float ambientColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float diffuseColor[4] = {1.0f,1.0f,1.0f,1.0f};
		const float specularColor[4] = {1.0f,1.0f,1.0f,1.0f};
		
		glEnable(_glLightID);		
		glLightfv(_glLightID, GL_AMBIENT, ambientColor);
		glLightfv(_glLightID, GL_DIFFUSE, diffuseColor);
		glLightfv(_glLightID, GL_SPECULAR,specularColor);
		glLightfv(_glLightID, GL_POSITION, m_LightPos.GetArray());
		glLightfv(_glLightID, GL_SPOT_DIRECTION, m_LightDirection.GetArray());
	}
}