/********************************************************************
	filename: 	PANDUGLError
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUGLError_h__
#define __PANDUGLError_h__

#include "PANDUGlInclude.h"
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	// GL ERROR CHECK
	static int CheckGLError(char *file, int line)
	{
		GLenum glErr;
		int    retCode = 0;

		glErr = glGetError();
		while (glErr != GL_NO_ERROR) 
		{
			const GLubyte* sError = gluErrorString(glErr);

			if (sError)
			{
				//cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << endl;
				Pandu::gLogManager->WriteErrorLog("GL Error #" + String(" (") 
					+ (const char*)gluErrorString(glErr) + ") " + " in File " + file + " at line: " 
					+ line );
			}
			else
			{
				//cout << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << endl;
				Pandu::gLogManager->WriteErrorLog("GL Error #" + String(" (no message available)") + " in File " + file + " at line: " + line );
			}

			retCode = 1;
			glErr = glGetError();
		}
		return retCode;
	}

#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)
}

#endif