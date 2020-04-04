/********************************************************************
	filename: 	PANDURenderable
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDURenderable_h__
#define __PANDURenderable_h__

#include "../boost/smart_ptr.hpp"
#include "../Math/PANDUMatrix44.h"
#include <list>

namespace Pandu
{
	class Renderable;
	typedef boost::shared_ptr<Renderable> TSharedRenderablePtr;
	typedef std::list<TSharedRenderablePtr> TRenderablesList;

	class Renderable
	{
	public:
		Renderable(){}
		virtual ~Renderable(){}
		virtual void Render(const Matrix44& _transform, unsigned int _techniqueID, bool _useFixedFunctionIfNoTechniqueAvailable) = 0;
	};
}

#endif