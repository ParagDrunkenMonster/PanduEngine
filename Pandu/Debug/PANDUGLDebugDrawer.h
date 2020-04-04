/********************************************************************
	filename: 	PANDUGLDebugDrawer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUGLDebugDrawer_h__
#define __PANDUGLDebugDrawer_h__

#include "../Utils/PANDUBulletPanduConversion.h"
#include "PANDUDebugDrawer.h"
#include "../Graphics/PANDUGlInclude.h"

namespace Pandu
{
	class GLDebugDrawer : public DebugDrawer
	{
	private:

		int m_bulletDebugMode;
		GLUquadric* m_GLUquadric;

	public:

		GLDebugDrawer();
		virtual ~GLDebugDrawer();

		//DebugDrawer overrides
		virtual void DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _fromColor, const Colour& _toColor);
		virtual void DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _color);
		virtual void DrawSphere (const Vector3& _p, float _radius, int _slices, int _stacks, const Colour& _color);
		virtual void DrawBox (const AxisAlignedBox3& _box, const Colour& _color);

		//btIDebugDraw overrides 
		virtual void drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _fromColor, const btVector3& _toColor);
		virtual void drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _color);
		virtual void drawSphere (const btVector3& _p, btScalar _radius, const btVector3& _color);
		virtual void drawBox (const btVector3& _boxMin, const btVector3& _boxMax, const btVector3& _color, btScalar _alpha);
		virtual void drawTriangle(const btVector3& _a, const btVector3& _b, const btVector3& _c, const btVector3& _color, btScalar _alpha);
		virtual void drawContactPoint(const btVector3& _pointOnB, const btVector3& _normalOnB, btScalar _distance, int _lifeTime, const btVector3& _color);
		virtual void reportErrorWarning(const char* _warningString);
		virtual void draw3dText(const btVector3& _location, const char* _textString);
		virtual void setDebugMode(int _debugMode);
		virtual int	getDebugMode() const					{ return m_bulletDebugMode;}
	};
}

#endif