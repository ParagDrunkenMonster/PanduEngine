/********************************************************************
	filename: 	PANDUTextureManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUTextureManager_h__
#define __PANDUTextureManager_h__

#include "../boost/smart_ptr.hpp"
#include "PANDUTexture.h"
#include "../Utils/PANDUCoreDefines.h"


namespace Pandu
{
	class TextureManager
	{
	public:

		friend class GraphicsRoot;

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static TextureManager *m_Instance;

	private:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		TextureManager();
		~TextureManager();

	public:

		//--------------------------------------------------------------------
		//Static functions
		static TextureManager* Get();
		//--------------------------------------------------------------------

		void ClearAllTextures();

		TSharedTexturePtr CreateTexture(Texture::TextureType _texType, const String& _filePath);
		TSharedTexturePtr CreateTexture(Texture::TextureType _texType, const String& _name, Texture::TextureFormat _texFormat, unsigned int _texWidth, unsigned int _texHeight);
		TSharedTexturePtr GetTexture(const String& _textureName);
		void DestroyTexture(const String& _textureName);

	};

	#define gTextureManager TextureManager::Get()
}

#endif