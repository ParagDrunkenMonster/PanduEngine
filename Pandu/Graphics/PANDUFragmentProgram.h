/********************************************************************
	filename: 	PANDUFragmentProgram
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFragmentProgram_h__
#define __PANDUFragmentProgram_h__

#include "PANDUShaderProgram.h"

namespace Pandu
{
	class FragmentProgram;

	typedef boost::shared_ptr<FragmentProgram> TSharedFragmentProgramPtr;

	class FragmentProgram : public ShaderProgram
	{
	protected:

		friend class ShaderManager;

		FragmentProgram(const String& _filename);

	public:

		virtual ~FragmentProgram();

	};
}

#endif