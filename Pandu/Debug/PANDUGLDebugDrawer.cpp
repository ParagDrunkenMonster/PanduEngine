#include "PANDUGLDebugDrawer.h"
#include "../Math/PANDUMathUtils.h"

namespace Pandu
{
	GLDebugDrawer::GLDebugDrawer()
		: m_bulletDebugMode(1)
	{
		m_GLUquadric = gluNewQuadric();
	}

	GLDebugDrawer::~GLDebugDrawer()
	{
		gluDeleteQuadric(m_GLUquadric);
		m_GLUquadric = NULL;
	}

	//DebugDrawer overrides
	void GLDebugDrawer::DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _fromColor, const Colour& _toColor)
	{
		glBegin(GL_LINES);
			glColor4f(_fromColor.r, _fromColor.g, _fromColor.b,_fromColor.a);
			glVertex3d(_from.x, _from.y, _from.z);
			glColor4f(_toColor.r, _toColor.g, _toColor.b,_toColor.a);
			glVertex3d(_to.x, _to.y, _to.z);
		glEnd();
	}

	void GLDebugDrawer::DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _color)
	{
		DrawLine(_from,_to,_color,_color);
	}

	void GLDebugDrawer::DrawSphere (const Vector3& _p, float _radius, int _slices, int _stacks, const Colour& _color)
	{
		glColor4f (_color.r, _color.g, _color.b, _color.a);
		glPushMatrix ();
			glTranslatef (_p.x, _p.y, _p.z);
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			gluSphere(m_GLUquadric,_radius,_slices,_stacks);
		glPopMatrix();
	}

	void GLDebugDrawer::DrawBox(const AxisAlignedBox3& _box, const Colour& _color)
	{
		Vector3 halfExtent = (_box.GetMax() - _box.GetMin()) * 0.5f;
		Vector3 center = (_box.GetMax() + _box.GetMin()) * 0.5f;
		//glEnable(GL_BLEND);     // Turn blending On
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f (_color.r, _color.g, _color.b, _color.a);
		glPushMatrix ();
		glTranslatef (center.x, center.y, center.z);
		glScaled(2.0f * halfExtent.x, 2.0f * halfExtent.y, 2.0f * halfExtent.z);
		//glutSolidCube(1.0);
		glPopMatrix ();
		//glDisable(GL_BLEND);
	}


	//btIDebugDraw overridees 
	void GLDebugDrawer::drawLine(const btVector3& _from,const btVector3& _to,const btVector3& _fromColor, const btVector3& _toColor)
	{
		DrawLine(ConvertVector3(_from),ConvertVector3(_to),ConvertColor(_fromColor),ConvertColor(_toColor));
	}

	void GLDebugDrawer::drawLine(const btVector3& _from,const btVector3& _to,const btVector3& _color)
	{
		drawLine(_from,_to,_color,_color);
	}

	void GLDebugDrawer::drawSphere (const btVector3& _p, btScalar _radius, const btVector3& _color)
	{
		DrawSphere(ConvertVector3(_p),_radius,5,5,ConvertColor(_color));
	}

	void GLDebugDrawer::drawBox (const btVector3& _boxMin, const btVector3& _boxMax, const btVector3& _color, btScalar _alpha)
	{
		DrawBox(AxisAlignedBox3(ConvertVector3(_boxMin),ConvertVector3(_boxMax)), ConvertColor(_color,_alpha));
	}

	void GLDebugDrawer::drawTriangle(const btVector3& _a,const btVector3& _b,const btVector3& _c,const btVector3& _color,btScalar _alpha)
	{
	//	if (m_debugMode > 0)
		{
			const btVector3	n=btCross(_b-_a,_c-_a).normalized();
			glBegin(GL_TRIANGLES);		
			glColor4f(_color.getX(), _color.getY(), _color.getZ(),_alpha);
			glNormal3d(n.getX(),n.getY(),n.getZ());
			glVertex3d(_a.getX(),_a.getY(),_a.getZ());
			glVertex3d(_b.getX(),_b.getY(),_b.getZ());
			glVertex3d(_c.getX(),_c.getY(),_c.getZ());
			glEnd();
		}
	}

	void GLDebugDrawer::setDebugMode(int _debugMode)
	{
		m_bulletDebugMode = _debugMode;
	}

	void GLDebugDrawer::draw3dText(const btVector3& _location,const char* textString)
	{
		glRasterPos3f(_location.x(),  _location.y(),  _location.z());
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
	}

	void GLDebugDrawer::reportErrorWarning(const char* _warningString)
	{
		printf("%s\n",_warningString);
	}

	void GLDebugDrawer::drawContactPoint(const btVector3& _pointOnB,const btVector3& _normalOnB,btScalar _distance,int _lifeTime,const btVector3& _color)
	{
		btVector3 _to=_pointOnB+_normalOnB*_distance;
		const btVector3&_from = _pointOnB;
		glColor4f(_color.getX(), _color.getY(), _color.getZ(),1.f);
		//glColor4f(0,0,0,1.f);

		glBegin(GL_LINES);
		glVertex3d(_from.getX(), _from.getY(), _from.getZ());
		glVertex3d(_to.getX(), _to.getY(), _to.getZ());
		glEnd();

		
		glRasterPos3f(_from.x(),  _from.y(),  _from.z());
		char buf[12];
		sprintf(buf," %d",_lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
	}
}