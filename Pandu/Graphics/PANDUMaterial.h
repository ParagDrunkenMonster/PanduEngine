/********************************************************************
	filename: 	PANDUMaterial
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMaterial_h__
#define __PANDUMaterial_h__

#include "../Utils/PANDUString.h"
#include "../boost/smart_ptr.hpp"
#include "PANDUShaderEffect.h"
#include "PANDUTexture.h"
#include "../Utils/PANDUCoreDefines.h"
#include "PANDUColour.h"
#include <list>

namespace Pandu
{
#define MAX_TEXTURES_PER_MATERIAL 8

	class Material;

	typedef boost::shared_ptr<Material> TSharedMaterialPtr;
	typedef std::vector<TSharedMaterialPtr> TSharedMaterialPtrArray;

	class Material
	{
	public:

		static Material defaultMaterial;

		enum PolygonFillMode
		{
			PFM_Fill = 0,
			PFM_Line,
			PFM_Point,

			PFM_Count
		};

		enum MaterialColourTypes
		{
			MCT_Ambient = 0,
			MCT_Diffuse,
			MCT_Specular,

			MCT_Count
		};

	private:

		typedef std::pair<TSharedTexturePtr,GLenum> TActiveTexture;
		typedef std::vector<TActiveTexture> TActiveTextureArray;
		
		const String			m_MaterialName;
		//Colour				m_AmbientColour;
		//Colour				m_DiffuseColour;
		bool					m_EnableCullFace;
		unsigned int			m_FrontFace;
		unsigned int			m_CullFace;
		bool					m_DepthTestEnabled;
		unsigned int			m_GLShadeModel;
		bool					m_LightingEnabled;
		bool					m_AlphaTestEnable;
		bool					m_BlendingEnabled;
		unsigned int			m_BlendingSFector;
		unsigned int			m_BlendingDFector;

		PolygonFillMode			m_PolygonFillMode;

		ShaderEffect			*m_ShaderEffect;
		TSharedTextureArray		m_Textures;

		Colour					m_MaterialColours[MCT_Count];
		float					m_MaterialShininess;

		TActiveTextureArray		m_ActiveTextures;

	public:

		Material(const String& _matName);
		~Material();

		const String& GetName() const;

		void EnableMaterialProperties();
		void DisableMaterialProperties();

		inline void SetLightingEnabled(bool _enabled)						{	m_LightingEnabled = _enabled;	}
		inline void SetPolygonFillMode(PolygonFillMode _fillMode)			{	m_PolygonFillMode = _fillMode;	}
	
		inline void AddTexture(TSharedTexturePtr _texture)					
		{
			const unsigned short textureCount = (unsigned short)m_Textures.size();
			if( textureCount >= MAX_TEXTURES_PER_MATERIAL )
			{
				gLogManager->WriteWarningLog(String("Only ") + MAX_TEXTURES_PER_MATERIAL + " textures supported per material");
				return;
			}
			if( _texture == NULL )
			{
				gLogManager->WriteWarningLog(String("Trying to add NULL texture to material"));
				return;
			}
			m_Textures.push_back(_texture);	
		}

		TSharedShaderTechniquePtr CreateShaderTechnique(unsigned int _techniqueID);
		
		inline ShaderEffect* GetShaderEffect()		{		return m_ShaderEffect;		}

		inline void SetMaterialColour(MaterialColourTypes _colourType, const Colour& _colour)
		{
			BOOST_ASSERT( _colourType < MCT_Count );
			m_MaterialColours[_colourType] = _colour;
		}

		inline void SetMaterialShininess(float _shininess)
		{
			m_MaterialShininess = _shininess;
		}

		inline void SetEnableCullFace(bool _enable)		{	m_EnableCullFace = _enable;		}
			
	private:

		
		void ApplyMaterialColorProperties();
		void EnableDisableGLProperty(unsigned int _glProperty,bool _enable) const;

		void InitiliazeDefaultMaterialColours();
	};
}

#endif