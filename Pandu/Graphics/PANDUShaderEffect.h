/********************************************************************
	filename: 	PANDUShaderEffect
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderEffect_h__
#define __PANDUShaderEffect_h__

#include "PANDUShaderTechnique.h"

namespace Pandu
{
	class ShaderEffect
	{
	private:

		friend class Material;

		TTechniqueIDShaderPtrMap m_AllTechniques;
		unsigned int m_TechniqueCount;

		void ClearAllTechniques();

	public:

		ShaderEffect();
		~ShaderEffect();

		TSharedShaderTechniquePtr CreateShaderTechnique(unsigned int _techniqueID);
		inline unsigned int GetTechniqueCount() const			{	return m_TechniqueCount;			}

		TSharedShaderTechniquePtr GetTechnique(unsigned short _techniqueID);
	};
}

#endif //__PANDUShaderEffect_h__