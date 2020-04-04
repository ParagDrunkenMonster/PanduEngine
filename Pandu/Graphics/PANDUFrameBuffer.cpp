#include "PANDUFrameBuffer.h"

namespace Pandu
{
	FrameBuffer::FrameBuffer()
		: m_FrameBuffer(0)	
		, m_DepthBuffer(0)
		, m_Loaded(false)
		, m_Enabled(false)
		, m_HasDepthOperations(false)
		, m_DepthBufferWidthHeightSet(false)
	{

	}

	FrameBuffer::~FrameBuffer()
	{
		Unload();
	}

	bool FrameBuffer::Load(bool _needDepthBuffer, TSharedTexturePtr _depthTexture /*= NULL */ )
	{
		if(!m_Loaded && m_FrameBuffer == 0 && m_DepthBuffer == 0 )
		{
			glGenFramebuffersEXT(1,&m_FrameBuffer);
			if(m_FrameBuffer > 0)
			{
				if(_needDepthBuffer )
				{
					m_HasDepthOperations = true;
					if( _depthTexture == NULL )
					{
						glGenRenderbuffersEXT( 1, &m_DepthBuffer );
						if( m_DepthBuffer > 0 )
						{
							SetDepthBufferWidthHeight();
							m_Loaded = true;
						}
						else
						{
							glDeleteFramebuffersEXT(1, &m_FrameBuffer);
							m_FrameBuffer = 0;
						}
					}
					else
					{
						if( _depthTexture->GetTextureFormat() == Texture::TF_DEPTH_COMPONENT24 && _depthTexture->GetTextureType() == Texture::TT_Texture2D )
						{
							m_Loaded = true;
							m_DepthTexture = _depthTexture;
						}
						else
						{
							gLogManager->WriteWarningLog("Only TF_DEPTH_COMPONENT24 is supported for depth texture");
						}
					}
				}
				else
				{
					m_Loaded = true;
				}
			}

			if(!CheckFrameBufferError())
			{
				m_Loaded = true;
				Unload();
			}
		}

		return m_Loaded;
	}

	void FrameBuffer::Unload()
	{
		if( m_Loaded )
		{
			DisableFrameBuffer();
			if( m_DepthBuffer > 0 )
			{
				glDeleteRenderbuffersEXT(1,&m_DepthBuffer);
			}

			if( m_FrameBuffer > 0 )
			{
				glDeleteFramebuffersEXT(1, &m_FrameBuffer);
			}

			m_FrameBuffer = 0;
			m_DepthBuffer = 0;
			m_Loaded = false;
			m_HasDepthOperations = false;
			m_DepthBufferWidthHeightSet = false;
		}
	}

	bool FrameBuffer::EnableFrameBuffer()
	{
		if(m_Enabled)
		{
			return true;
		}

		const unsigned short colTexCount = (unsigned short)m_ColourTextures.size();

		if( m_Loaded && ( colTexCount > 0 || m_DepthTexture != NULL ) )
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);

			bool colourAssigned = false;
			for( unsigned short i = 0 ; i < colTexCount ; i++ )
			{
				TSharedTexturePtr currColTex = m_ColourTextures[i];
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, currColTex->GetGLTextureType(), currColTex->GetGLTextureId(), 0/*mipmap level*/);
				colourAssigned = true;
			}

			if( colourAssigned )
			{
				glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); 
				glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			}
			else
			{
				glDrawBuffer(GL_NONE); 
				glReadBuffer(GL_NONE);
				glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			}
		
			bool depthAssigned = false;
			if( m_HasDepthOperations )
			{
				if( m_DepthTexture != NULL )
				{
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, m_DepthTexture->GetGLTextureType(), m_DepthTexture->GetGLTextureId(), 0/*mipmap level*/);
					depthAssigned = true;
				}
				else
				{
					if( m_DepthBuffer > 0 && m_DepthBufferWidthHeightSet )
					{
						glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_DepthBuffer);
						depthAssigned = true;
					}
				}
			}

			if( depthAssigned )
			{
				glDepthMask(GL_TRUE);
			}
			else
			{
				glDepthMask(GL_FALSE);
			}

			m_Enabled = ( CheckFrameBufferError() && (colourAssigned || depthAssigned) );
		}

		return m_Enabled;
	}

	void FrameBuffer::DisableFrameBuffer()
	{
		if(m_Enabled)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
			//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);
			m_Enabled = false;
		}
	}

	void FrameBuffer::SetDepthBufferWidthHeight()
	{
		if(!m_DepthBufferWidthHeightSet && m_DepthBuffer > 0 && ((unsigned int)m_ColourTextures.size() > 0 ) )
		{
			TSharedTexturePtr currColTex = m_ColourTextures[0];
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_DepthBuffer );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, currColTex->GetTextureWidth(), currColTex->GetTextureHeight() );
			m_DepthBufferWidthHeightSet = true;
		}
	}

	bool FrameBuffer::CheckFrameBufferError() const
	{
		/*GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

		switch( status )
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			gLogManager->WriteWarningLog("FrameBuffer unsupported");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete, missing attachment");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete attachment");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete dimensions");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete format");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete draw buffer");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			gLogManager->WriteWarningLog("FrameBuffer incomplete read buffer");
			break;

		default:
			gLogManager->WriteWarningLog("FrameBuffer unknown error");
			break;
		}*/
		return true;
	}
}
