#include "PANDUAutoShaderParameter.h"
#include "../Utils/PANDUCoreDefines.h"

namespace
{
	static const Pandu::String kAutoShaderParamNames[Pandu::AutoShaderParameter::ASPT_Count] = 
																			{
																				"PANDUModelMatrix",				//ASPT_ModelMatrix,
																				"PANDUModelShadowProjetionMatrix0",	//ASPT_ModelShadowProjectionMatrix0,
																			};

	static const Pandu::ShaderParameter::ParameterType kAutoShaderParameterType[Pandu::AutoShaderParameter::ASPT_Count] = 
																			{
																				Pandu::ShaderParameter::ePT_Float4x4,				//ASPT_ModelMatrix,
																				Pandu::ShaderParameter::ePT_Float4x4,				//ASPT_ModelShadowProjectionMatrix0,
																			};

	static const Pandu::AutoShaderParameter::AutoShaderParameterType kShadowProjMatParamStart = Pandu::AutoShaderParameter::ASPT_ModelShadowProjectionMatrix0;
	static const Pandu::AutoShaderParameter::AutoShaderParameterType kShadowProjMatParamEnd = Pandu::AutoShaderParameter::ASPT_ModelShadowProjectionMatrix0;

	static const unsigned short kShadowProjectionMatrixCount = 1 + (kShadowProjMatParamEnd - kShadowProjMatParamStart);
	static Pandu::Matrix44 kShadowProjectionMatrices[kShadowProjectionMatrixCount] = 
															{
																Pandu::Matrix44::IDENTITY,						//ASPT_ModelShadowProjectionMatrix0
															};
}

namespace Pandu
{
	const String& AutoShaderParameter::GetAutoShaderParameterName(AutoShaderParameterType _autoShaderParam)
	{
		BOOST_ASSERT(_autoShaderParam >= 0 && _autoShaderParam < ASPT_Count );
		return kAutoShaderParamNames[_autoShaderParam];
	}

	ShaderParameter::ParameterType AutoShaderParameter::GetAutoShaderParameterType(AutoShaderParameterType _autoShaderParam)
	{
		BOOST_ASSERT(_autoShaderParam >= 0 && _autoShaderParam < ASPT_Count );
		return kAutoShaderParameterType[_autoShaderParam];
	}

	unsigned short AutoShaderParameter::GetShadowProjectionMatrixParamCount()
	{
		return kShadowProjectionMatrixCount;
	}

	void AutoShaderParameter::SetShadowProjectionMatrix(AutoShaderParameterType _paramType, const Matrix44& _shadowProjMat)
	{
		if( _paramType < kShadowProjMatParamStart || _paramType > kShadowProjMatParamEnd )
		{
			gLogManager->WriteWarningLog("Can't set shadow projection matrix, wrong parameter type");
			return;
		}

		const unsigned short index = _paramType - kShadowProjMatParamStart;
		kShadowProjectionMatrices[index] = _shadowProjMat;
	}

	const Matrix44& AutoShaderParameter::GetShadowProjectionMatrix(AutoShaderParameterType _paramType)
	{
		if( _paramType < kShadowProjMatParamStart || _paramType > kShadowProjMatParamEnd )
		{
			gLogManager->WriteWarningLog("Can't get shadow projection matrix, wrong parameter type");
			return Matrix44::IDENTITY;
		}

		const unsigned short index = _paramType - kShadowProjMatParamStart;
		return kShadowProjectionMatrices[index];
	}
}