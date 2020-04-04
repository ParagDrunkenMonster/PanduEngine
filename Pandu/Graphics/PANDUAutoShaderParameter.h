/********************************************************************
	filename: 	PANDUAutoShaderParameter
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUAutoShaderParameter_h__
#define __PANDUAutoShaderParameter_h__

#include "../Utils/PANDUString.h"
#include "PANDUShaderParameter.h"

namespace Pandu
{
	class AutoShaderParameter
	{
	public:

		friend class ShadowMapCamera;

		enum AutoShaderParameterType
		{
			ASPT_ModelMatrix = 0,						//Parameter Name: PANDUModelMatrix
			ASPT_ModelShadowProjectionMatrix0,			//Parameter Name: PANDUModelShadowProjetionMatrix0

			ASPT_Count
		};

		static const String& GetAutoShaderParameterName(AutoShaderParameterType _autoShaderParam);
		static ShaderParameter::ParameterType GetAutoShaderParameterType(AutoShaderParameterType _autoShaderParam);

		static unsigned short GetShadowProjectionMatrixParamCount();
		static const Matrix44& GetShadowProjectionMatrix(AutoShaderParameterType _paramType);

	private:

		static void SetShadowProjectionMatrix(AutoShaderParameterType _paramType, const Matrix44& _shadowProjMat);

	};
}

#endif