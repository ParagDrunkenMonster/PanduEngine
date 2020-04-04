/********************************************************************
	filename: 	PANDUDebugDrawer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUDebugDrawer_h__
#define __PANDUDebugDrawer_h__

#include "../Math/PANDUVector3.h"
#include "../Graphics/PANDUColour.h"
#include "../Math/PANDUAxisAlignedBox3.h"
#include "../Utils/PANDUString.h"
#include "../boost/smart_ptr.hpp"
#include "../Bullet/LinearMath/btIDebugDraw.h"

namespace Pandu
{
	class DebugDrawer;

	typedef boost::shared_ptr<DebugDrawer> TSharedDebugDrawerPtr;
	typedef boost::weak_ptr<DebugDrawer> TDebugDrawerWeakPtr;
	typedef std::auto_ptr<DebugDrawer> TDebugDrawerAutoPtr;

	class DebugDrawer : public btIDebugDraw
	{
	public:

		DebugDrawer(){}
		virtual ~DebugDrawer(){}

		virtual void DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _fromColor, const Colour& _toColor) = 0;
		virtual void DrawLine(const Vector3& _from, const Vector3& _to, const Colour& _color) = 0;
		virtual void DrawSphere (const Vector3& _p, float _radius, int _slices, int _stacks, const Colour& _color) = 0;
		virtual void DrawBox (const AxisAlignedBox3& _box, const Colour& _color) = 0;
	};
}

#endif