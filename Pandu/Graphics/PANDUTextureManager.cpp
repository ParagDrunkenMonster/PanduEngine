#include "PANDUTextureManager.h"

namespace Pandu
{
	//===============================================================================
	//TextureManager Implementation

	class TextureManager::Impl
	{
	private:

		TNameSharedTexturePtrMap m_TextureMap;		

	public:

		Impl()
		{
		}

		~Impl()
		{
			ClearAllTextures();
		}

		void ClearAllTextures()
		{
			TNameSharedTexturePtrMap::iterator itr = m_TextureMap.begin();
			TNameSharedTexturePtrMap::iterator end = m_TextureMap.end();

			for( ; itr != end ; itr++ )
			{
				PANDU_ERROR((*itr).second.use_count() == 1, (String("Texture named ") + (*itr).second->GetName()
					+ " is still in use, first clear the reference before clear.." ).CString() );

				(*itr).second.reset();
			}

			m_TextureMap.clear();
		}

		TSharedTexturePtr CreateTexture(Texture::TextureType _texType, const String& _filePath)
		{
			TSharedTexturePtr texturePtr = GetTexture(File::GetFileName(_filePath));

			if(texturePtr != NULL)
			{
				return texturePtr;
			}

			TSharedTexturePtr texture(new Texture(_texType,_filePath));
			bool loaded = texture->Load();
			if( loaded )
			{
				std::pair<const String, TSharedTexturePtr> nameTexturePair(texture->GetName(),texture);
				m_TextureMap.insert(nameTexturePair);
				return texture;
			}

			texture.reset();
			return texture;
		}

		TSharedTexturePtr CreateTexture(Texture::TextureType _texType, const String& _name, Texture::TextureFormat _texFormat, unsigned int _texWidth, unsigned int _texHeight)
		{
			TSharedTexturePtr texturePtr = GetTexture(_name);

			if(texturePtr != NULL)
			{
				return texturePtr;
			}

			TSharedTexturePtr texture(new Texture(_texType,_name,_texFormat,_texWidth,_texHeight));
			bool loaded = texture->Load();
			if( loaded )
			{
				std::pair<const String, TSharedTexturePtr> nameTexturePair(texture->GetName(),texture);
				m_TextureMap.insert(nameTexturePair);
				return texture;
			}

			texture.reset();
			return texture;
		}

		TSharedTexturePtr GetTexture(const String& _textureName)
		{
			TNameSharedTexturePtrMap::iterator end = m_TextureMap.end();
			TNameSharedTexturePtrMap::iterator itr = m_TextureMap.find(_textureName);

			if( itr != end )
			{
				return (*itr).second;
			}

			return TSharedTexturePtr();
		}

		void DestroyTexture(const String& _textureName)
		{
			TNameSharedTexturePtrMap::iterator end = m_TextureMap.end();

			TNameSharedTexturePtrMap::iterator itr = m_TextureMap.find(_textureName);

			if( itr != end )
			{
				PANDU_ERROR((*itr).second.use_count() == 1, (String("Texture named ") + _textureName
					+ " is still in use, first clear the reference before destroying.." ).CString() );
				(*itr).second.reset();
				m_TextureMap.erase(itr);
			}
		}
	};

	//===============================================================================

	TextureManager* TextureManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void TextureManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new TextureManager();
		}
	}

	void TextureManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	TextureManager* TextureManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	TextureManager::TextureManager()
		: m_Impl(new Impl())
	{
	}

	TextureManager::~TextureManager()
	{
	}

	TSharedTexturePtr TextureManager::CreateTexture(Texture::TextureType _texType, const String& _filePath)
	{
		return m_Impl->CreateTexture(_texType,_filePath);
	}

	TSharedTexturePtr TextureManager::CreateTexture(Texture::TextureType _texType, const String& _name, Texture::TextureFormat _texFormat, unsigned int _texWidth, unsigned int _texHeight)
	{
		return m_Impl->CreateTexture(_texType,_name,_texFormat,_texWidth, _texHeight);
	}

	TSharedTexturePtr TextureManager::GetTexture(const String& _textureName)
	{
		return m_Impl->GetTexture(_textureName);
	}

	void TextureManager::DestroyTexture(const String& _textureName)
	{
		m_Impl->DestroyTexture(_textureName);
	}
}