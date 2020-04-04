/********************************************************************
	filename: 	PANDUTexture
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUTexture_h__
#define __PANDUTexture_h__

#include "../boost/smart_ptr.hpp"
#include "../Utils/PANDUCoreDefines.h"
#include "Utils/PANDUBitmapImage.h"
#include "PANDUGlInclude.h"
#include "PANDUGLError.h"
#include <map>
#include <vector>

namespace Pandu
{
	class Texture;
	
	typedef boost::shared_ptr<Texture> TSharedTexturePtr;

	typedef std::map<const String,TSharedTexturePtr> TNameSharedTexturePtrMap;
	typedef std::vector<TSharedTexturePtr> TSharedTextureArray;

	class Texture
	{
	public:

		enum TextureType
		{
			TT_Texture2D = 0,

			TT_Count
		};

		enum TextureParamName
		{
			TPN_MinFilter = 0,
			TPN_MagFilter,
			TPN_WrapS,
			TPN_WrapT,

			TPN_Count
		};

		enum TextureParamIValue//texture param integer value
		{
			TPIV_None = 0,
			TPIV_Nearest,
			TPIV_Linear,
			TPIV_Repeat,
			TPIV_ClampToEdge,

			TPIV_Count
		};

		enum TextureFormat
		{
			TF_Unknown		= 0,
			TF_R8G8B8,
			TF_R8G8B8A8,
			TF_DEPTH_COMPONENT24,

			TF_Count
		};

	private:

		const TextureType				m_TextureType;
		Image							*m_Image;
		const String					m_Name;
		Texture::TextureFormat			m_TextureFormat;
		bool							m_Loaded;
		GLuint							m_TextureId;
		unsigned int					m_TextureWidth;
		unsigned int					m_TextureHeight;
		TextureParamIValue				m_TextureParamValue[TPN_Count];
	
	public:
	
		Texture(TextureType _type, const String& _filePath);
		Texture(TextureType _type, const String& _name, TextureFormat _texFormat, unsigned int _texWidth, unsigned int _texHeight);
		~Texture();

		inline const String& GetName() const			{	return m_Name;							}
		inline bool IsLoaded() const					{	return m_Loaded;						}
		inline unsigned int GetTextureWidth() const		{	return m_TextureWidth;					}
		inline unsigned int GetTextureHeight() const	{	return m_TextureHeight;					}
		inline TextureType GetTextureType() const		{	return m_TextureType;					}
		inline TextureFormat GetTextureFormat() const	{	return m_TextureFormat;					}
		GLenum GetGLTextureType() const;
		inline GLuint GetGLTextureId() const			{	return m_TextureId;						}

		bool Load();
		bool Unload();

		bool EnableTexture() const;
		void DisableTexture() const;

		void SetTextureParam(TextureParamName _paramName, TextureParamIValue _value)
		{
			m_TextureParamValue[_paramName] = _value;
		}

	private:

		void SetTextureParamValues() const;
		Image* CreateImage(Image::ImageType _imageType, const String& _filePath);
		Image::ImageType GetImageType(const String& _filePath);
	};
}

#endif //__PANDUTexture_h__