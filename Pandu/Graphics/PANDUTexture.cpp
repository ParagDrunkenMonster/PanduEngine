#include "PANDUTexture.h"

namespace
{
	using namespace Pandu;

	const GLenum kGLTextureType[Texture::TT_Count] = 
												{
													GL_TEXTURE_2D,				//TT_Texture2D
												};

	const GLenum kGLTextureParamName[Texture::TPN_Count] = 
												{
													GL_TEXTURE_MIN_FILTER,		//TPN_MinFilter,
													GL_TEXTURE_MAG_FILTER,		//TPN_MagFilter,
													GL_TEXTURE_WRAP_S,			//TPN_WrapS,
													GL_TEXTURE_WRAP_T,			//TPN_WrapT,
												};

	const GLint kGLTextureParamValueI[Texture::TPIV_Count] = 
												{
													-1,							//TPIV_None,
													GL_NEAREST,					//TPIV_Nearest,
													GL_LINEAR,					//TPIV_Linear,
													GL_REPEAT,					//TPIV_Repeat,
													GL_CLAMP_TO_EDGE,			//TPIV_ClampToEdge
												};

	const unsigned int kSupportedParamValueForParamName[Texture::TPN_Count] = 
							{
								(1 << Texture::TPIV_Nearest | 1 << Texture::TPIV_Linear),		//TPN_MinFilter,
								(1 << Texture::TPIV_Nearest | 1 << Texture::TPIV_Linear),		//TPN_MagFilter,
								(1 << Texture::TPIV_Repeat	| 1 << Texture::TPIV_ClampToEdge),	//TPN_WrapS,
								(1 << Texture::TPIV_Repeat	| 1 << Texture::TPIV_ClampToEdge),	//TPN_WrapT,
							};

	const Texture::TextureParamIValue kDefaultTextureParamValue[Texture::TPN_Count] = 
							{
								Texture::TPIV_Linear,			//TPN_MinFilter,
								Texture::TPIV_Linear,			//TPN_MagFilter,
								Texture::TPIV_Repeat,			//TPN_WrapS,
								Texture::TPIV_Repeat,			//TPN_WrapT,
							};

	const Texture::TextureFormat kTextureFormatForImageFormat[Image::ICF_Count] = 
							{
								Texture::TF_Unknown,			//Image::ICF_Unknown,
								Texture::TF_R8G8B8,				//Image::ICF_R8G8B8,
							};

	const GLint kGLTextureInternalFormat[Texture::TF_Count] = 
							{
								-1,								//TF_Unknown		= 0,
								GL_RGB8,						//TF_R8G8B8,
								GL_RGBA8,						//TF_R8G8B8A8,
								GL_DEPTH_COMPONENT24,			//TF_DEPTH_COMPONENT24,
							};

	const GLint kGLPixelFormat[Texture::TF_Count] = 
							{
								-1,								//TF_Unknown		= 0,
								GL_RGB,							//TF_R8G8B8,
								GL_RGBA,						//TF_R8G8B8A8,
								GL_DEPTH_COMPONENT,				//TF_DEPTH_COMPONENT24,
							};
}

namespace Pandu
{
	Texture::Texture(TextureType _type, const String& _filePath)
		: m_TextureType(_type>=TT_Count?TT_Texture2D:_type)
		, m_Image(CreateImage(GetImageType(_filePath),_filePath))
		, m_Name(m_Image->GetFileName())
		, m_TextureFormat(TF_Unknown)
		, m_Loaded(false)
		, m_TextureId(0)
		, m_TextureWidth(0)
		, m_TextureHeight(0)
	{
		memcpy(m_TextureParamValue,kDefaultTextureParamValue,sizeof(kDefaultTextureParamValue));
	}

	Texture::Texture(TextureType _type, const String& _name, TextureFormat _texFormat, unsigned int _texWidth, unsigned int _texHeight)
		: m_TextureType(_type>=TT_Count?TT_Texture2D:_type)
		, m_Image(NULL)
		, m_Name(_name)
		, m_TextureFormat(_texFormat)
		, m_Loaded(false)
		, m_TextureId(0)
		, m_TextureWidth(_texWidth)
		, m_TextureHeight(_texHeight)
	{
		memcpy(m_TextureParamValue,kDefaultTextureParamValue,sizeof(kDefaultTextureParamValue));
	}

	Texture::~Texture()
	{
		if( m_Image )
		{
			m_Image->Unload();
			delete m_Image;
			m_Image = NULL;
		}

		Unload();
 	}

	bool Texture::Load()
	{
		if( m_Loaded )
		{
			gLogManager->WriteWarningLog("Texture already loaded");
			return false;
		}

		const char* imageData = NULL;
			
		if( m_Image )
		{
			if( !m_Image->Load() )
			{
				return false;
			}

			if( m_Image->GetImageFormat() <= Image::ICF_Unknown || m_Image->GetImageFormat() >= Image::ICF_Count )
			{
				gLogManager->WriteErrorLog("Unknown image formate for texture");
				return false;
			}

			if( m_Image->GetWidth() == 0 || m_Image->GetHeight() == 0 
				|| (m_Image->GetWidth()%2) != 0 || (m_Image->GetHeight()%2) != 0 )
			{
				gLogManager->WriteErrorLog("Only power of two textures supported");
				return false;
			}

			m_TextureFormat = kTextureFormatForImageFormat[m_Image->GetImageFormat()];
			m_TextureWidth = m_Image->GetWidth();
			m_TextureHeight = m_Image->GetHeight();
			imageData = m_Image->GetImageData();
			if(imageData == NULL)
			{
				gLogManager->WriteErrorLog("Null Image data in image");
				return false;
			}
		}

		if( m_TextureFormat <= TF_Unknown || m_TextureFormat >= TF_Count )
		{
			gLogManager->WriteErrorLog("Unknown texture formate");
			return false;
		}
		
		glGenTextures(1, &m_TextureId);
		glBindTexture(kGLTextureType[m_TextureType], m_TextureId);
		glTexImage2D(kGLTextureType[m_TextureType], 0,kGLTextureInternalFormat[m_TextureFormat],m_TextureWidth,m_TextureHeight
					, 0, kGLPixelFormat[m_TextureFormat], GL_UNSIGNED_BYTE,imageData);

		SetTextureParamValues();
		CHECK_GL_ERROR();

		if( m_Image )
		{
			m_Image->Unload();
		}
		m_Loaded = true;

		return true;
	}

	bool Texture::Unload()
	{
		if( !m_Loaded )
		{
			return false;
		}

		glDeleteTextures(1,&m_TextureId);
		m_TextureId = 0;
		return true;
	}

	bool Texture::EnableTexture() const
	{
		if( !m_Loaded )
		{
			gLogManager->WriteWarningLog("Texture not loaded yet");
			return false;
		}

		glEnable(kGLTextureType[m_TextureType]);
		glBindTexture(kGLTextureType[m_TextureType], m_TextureId);
		SetTextureParamValues();

		return true;
	}

	void Texture::DisableTexture() const
	{
		if( !m_Loaded )
		{
			gLogManager->WriteWarningLog("Texture not loaded yet");
			return;
		}

		glDisable(kGLTextureType[m_TextureType]);
	}

	void Texture::SetTextureParamValues() const
	{
		for( unsigned int i = 0 ; i < TPN_Count ; i++ )
		{
			const unsigned textValueParamFlag = (1 << m_TextureParamValue[i]);
			if( ( kSupportedParamValueForParamName[i] & textValueParamFlag ) == textValueParamFlag )
			{
				glTexParameteri(kGLTextureType[m_TextureType],kGLTextureParamName[i],kGLTextureParamValueI[m_TextureParamValue[i]]);
			}
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}

	Image* Texture::CreateImage(Image::ImageType _imageType, const String& _filePath)
	{
		switch(_imageType)
		{
		case Image::IT_Bitmap:
			{
				return new BitmapImage(_filePath);
			}
			break;

		default:
			{

			}
			break;
		}
		return NULL;
	}

	Image::ImageType Texture::GetImageType(const String& _filePath)
	{
		return Image::IT_Bitmap;
	}

	GLenum Texture::GetGLTextureType() const
	{
		return kGLTextureType[m_TextureType];
	}
}