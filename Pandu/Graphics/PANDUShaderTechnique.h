/********************************************************************
	filename: 	PANDUShaderTechnique
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShaderTechnique_h__
#define __PANDUShaderTechnique_h__

#include "PANDUShaderPass.h"
#include <map>

#define MAX_PASS_COUNT 1 //for now supporting only one pass

namespace Pandu
{
	class ShaderTechnique;

	typedef boost::shared_ptr<ShaderTechnique> TSharedShaderTechniquePtr;
	typedef std::map<unsigned int,TSharedShaderTechniquePtr> TTechniqueIDShaderPtrMap;

	class ShaderTechnique
	{
	private:

		friend class ShaderEffect;

		std::vector<TSharedShaderPassPtr> m_AllPasses;

		const unsigned int m_TechniqueID;

		void ClearAllPasses();

	public:

		ShaderTechnique(unsigned int _techniqueID);
		~ShaderTechnique();

		TSharedShaderPassPtr AddShaderPass( TSharedVertexProgramPtr _vertexProgram,TSharedFragmentProgramPtr _fragmentProgram);

		inline unsigned short GetPassCount() const	{	return (unsigned short)(m_AllPasses.size());		}
		inline TSharedShaderPassPtr GetPass(unsigned short _passIndex)
		{
			if( _passIndex >= GetPassCount() )
			{
				return TSharedShaderPassPtr();
			}
			return m_AllPasses[(int)_passIndex];
		}
	};
}

#endif //__ShaderTechnique_h__