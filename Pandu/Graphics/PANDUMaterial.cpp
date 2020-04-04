#include "PANDUMaterial.h"
#include "PANDUGlInclude.h"

namespace
{
	static const unsigned int kGLPolygonModes[Pandu::Material::PFM_Count] = 
	{
		GL_FILL,			//PFM_Fill
		GL_LINE,			//PFM_Line
		GL_POINT,			//PFM_Point
	};
}

namespace Pandu
{
	Material Material::defaultMaterial = Material(String("defaultMaterial") );

	//===============================================================================
	Material::Material(const String& _matName)
		: m_MaterialName(_matName)
		, m_EnableCullFace(true)
		, m_FrontFace(GL_CCW)
		, m_CullFace(GL_BACK)
		, m_DepthTestEnabled(true)
		, m_GLShadeModel(GL_SMOOTH)
		, m_LightingEnabled(true)
		, m_AlphaTestEnable(false)
		, m_BlendingEnabled(false)
		, m_BlendingSFector(GL_SRC_ALPHA)
		, m_BlendingDFector(GL_ONE_MINUS_SRC_ALPHA)		
		, m_PolygonFillMode(PFM_Fill)
		, m_ShaderEffect(NULL)
		, m_MaterialShininess(0)
	{
		InitiliazeDefaultMaterialColours();
	}
	
	Material::~Material()
	{
		DisableMaterialProperties();

		if( m_ShaderEffect )
		{
			delete m_ShaderEffect;
			m_ShaderEffect = NULL;
		}
	}

	const String& Material::GetName() const
	{	
		return m_MaterialName;	
	}

	void Material::EnableMaterialProperties()
	{
		glPolygonMode( GL_FRONT_AND_BACK, kGLPolygonModes[m_PolygonFillMode] );

		EnableDisableGLProperty(GL_CULL_FACE,m_EnableCullFace);
		
		glFrontFace(m_FrontFace);
		glCullFace(m_CullFace);

		EnableDisableGLProperty(GL_ALPHA_TEST,m_AlphaTestEnable);

		EnableDisableGLProperty(GL_BLEND,m_BlendingEnabled);
		glBlendFunc(m_BlendingSFector, m_BlendingDFector);
		
		EnableDisableGLProperty(GL_DEPTH_TEST,m_DepthTestEnabled);

		glShadeModel(m_GLShadeModel);

		ApplyMaterialColorProperties();

		//glDisable(GL_DEPTH_WRITE);
		unsigned int texCount = 0;
		const unsigned short textureCount = (unsigned short)m_Textures.size();
		if( textureCount != 0 )
		{
			for( unsigned short i = 0 ; i < textureCount ; i++ )
			{
				glActiveTexture(GL_TEXTURE0 + texCount );
				if(m_Textures[i]->EnableTexture())
				{
					m_ActiveTextures.push_back(TActiveTexture(m_Textures[i],GL_TEXTURE0 + texCount));
					texCount++;
				}
			}
		}

		EnableDisableGLProperty(GL_LIGHTING,m_LightingEnabled);
	}

	void Material::DisableMaterialProperties()
	{
		const unsigned int count = (unsigned int)m_ActiveTextures.size();
		for( unsigned int i = 0 ; i < count ; i++ )
		{
			glActiveTexture(m_ActiveTextures[i].second);
			m_ActiveTextures[i].first->DisableTexture();
		}

		m_ActiveTextures.clear();
	}

	void Material::ApplyMaterialColorProperties()
	{
		if(m_LightingEnabled)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,m_MaterialColours[MCT_Ambient].colourArray);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_MaterialColours[MCT_Diffuse].colourArray);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_MaterialColours[MCT_Specular].colourArray);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_MaterialShininess);
		}
		else
		{
			glColor4f(m_MaterialColours[MCT_Ambient].r,m_MaterialColours[MCT_Ambient].g,m_MaterialColours[MCT_Ambient].b,m_MaterialColours[MCT_Ambient].a);
		}
	}

	void Material::EnableDisableGLProperty(unsigned int _glProperty,bool _enable) const
	{
		if(_enable)
		{
			glEnable(_glProperty);
		}
		else
		{
			glDisable(_glProperty);
		}
	}

	TSharedShaderTechniquePtr Material::CreateShaderTechnique(unsigned int _techniqueID)
	{
		if( !m_ShaderEffect )
		{
			m_ShaderEffect = new ShaderEffect();
		}

		return m_ShaderEffect->CreateShaderTechnique(_techniqueID);
	}

	void Material::InitiliazeDefaultMaterialColours()
	{
		m_MaterialColours[MCT_Ambient] = Colour( 0.05f, 0.05f,0.05f,1.0f);
		m_MaterialColours[MCT_Diffuse] = Colour( 0.5f, 0.5f, 0.5f, 1.0f );
		m_MaterialColours[MCT_Specular] = Colour( 0.0f, 0.0f, 0.0f, 1.0f );
	}
}