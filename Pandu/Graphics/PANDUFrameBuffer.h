/********************************************************************
	filename: 	PANDUFrameBuffer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFrameBuffer_h__
#define __PANDUFrameBuffer_h__

#include "PANDUTexture.h"
#include "../Utils/PANDUFlag.h"

namespace Pandu
{
	class FrameBuffer;

	typedef boost::shared_ptr<FrameBuffer> TSharedFrameBufferPtr;

	class FrameBuffer
	{
	public:
		static const unsigned short gMaxColourTexturePerFrameBuffer = 16;

	private:

		GLuint						m_FrameBuffer;	
		GLuint						m_DepthBuffer;
		bool						m_Loaded;
		bool						m_Enabled;
		TSharedTextureArray			m_ColourTextures;
		TSharedTexturePtr			m_DepthTexture;
		bool						m_HasDepthOperations;
		bool						m_DepthBufferWidthHeightSet;

	public:

		FrameBuffer();
		virtual ~FrameBuffer();

		bool Load(bool _needDepthBuffer, TSharedTexturePtr _depthTexture = TSharedTexturePtr() );
		void Unload();
		bool EnableFrameBuffer();
		void DisableFrameBuffer();

		inline bool IsLoaded() const			{	return m_Loaded;				}

		bool AddColourTexture(TSharedTexturePtr _colourTexture)
		{
			const unsigned short colTexCount = (unsigned short)m_ColourTextures.size();
			if( colTexCount >= gMaxColourTexturePerFrameBuffer )
			{
				gLogManager->WriteWarningLog("Can't add texture to frame buffer, max limit crossed");
				return false;
			}
			if(_colourTexture != NULL)
			{
				if(_colourTexture->GetTextureType() != Texture::TT_Texture2D )
				{
					gLogManager->WriteWarningLog("Can't add texture to frame buffer, only 2D texture supported");
					return false;
				}
				m_ColourTextures.push_back(_colourTexture);
				SetDepthBufferWidthHeight();
				return true;
			}
			return false;
		}

	private:

		void SetDepthBufferWidthHeight();
		bool CheckFrameBufferError() const;
		
	};
}

#endif