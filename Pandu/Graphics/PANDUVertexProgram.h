/********************************************************************
	filename: 	PANDUVertexProgram
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUVertexShader_h__
#define __PANDUVertexShader_h__

#include "PANDUShaderProgram.h"

namespace Pandu
{
	class VertexProgram;

	typedef boost::shared_ptr<VertexProgram> TSharedVertexProgramPtr;

	class VertexProgram : public ShaderProgram
	{
	protected:

		friend class ShaderManager;

		VertexProgram(const String& _filename);

	public:

		virtual ~VertexProgram(); 
	};
}

#endif