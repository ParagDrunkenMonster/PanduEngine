/********************************************************************
	filename: 	PANDUTransformer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUTransformer_h__
#define __PANDUTransformer_h__

#include "../boost/smart_ptr.hpp"
#include "../Graphics/PANDUFrameEvent.h"

namespace Pandu
{
	class Transformer;
	typedef boost::shared_ptr<Transformer> TTransformerSharedPtr;
	typedef std::auto_ptr<Transformer> TTransformerAutoPtr;

	class Transformer
	{
	private:

	public:

		Transformer()
		{
		}

		virtual ~Transformer()
		{
		}

		virtual void Update(const FrameEvent& _frameEvent) = 0;
	};
}

#endif