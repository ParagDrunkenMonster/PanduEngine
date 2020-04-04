/********************************************************************
	filename: 	PANDUCoreDefines
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCoreDefines_h__
#define __PANDUCoreDefines_h__

#include "PANDULogManager.h"
#include "../boost/assert.hpp"

#ifndef PANDU_ERROR
	#define PANDU_ERROR(expression, error)	if( Pandu::gLogManager && !(expression) )	Pandu::gLogManager->WriteErrorLog(error);							\
											BOOST_ASSERT(expression);
#endif


#endif
