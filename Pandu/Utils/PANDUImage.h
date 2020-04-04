/********************************************************************
	filename: 	PANDUImage
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUImage_h__
#define __PANDUImage_h__

#include "PANDUStringTokenizer.h"
#include "PANDUFile.h"

namespace Pandu
{
	class Image
	{
	public:

		enum ImageType
		{
			IT_Bitmap,
		};

		enum ImageColourFormat
		{
			ICF_Unknown = 0,
			ICF_R8G8B8,
			
			ICF_Count
		};

	protected:

		const ImageType			m_ImageType;
		ImageColourFormat				m_ImageFormat;
		const String			m_FilePath;
		const String			m_FileName;
		unsigned int			m_ImageWidth;
		unsigned int			m_ImageHeight;

	public:
	
		inline Image(ImageType _imageType, const String& _filePath)
			: m_ImageType(_imageType)
			, m_ImageFormat(ICF_Unknown)
			, m_FilePath(_filePath)
			, m_FileName(File::GetFileName(_filePath))
			, m_ImageWidth(0)
			, m_ImageHeight(0)
		{
		}

		virtual ~Image(){}

		inline ImageType GetImageType() const			{	return m_ImageType;			}
		inline ImageColourFormat GetImageFormat() const		{	return m_ImageFormat;		}
		inline const String& GetFileName() const		{	return m_FileName;			}
		inline unsigned int GetWidth() const			{	return m_ImageWidth;		}
		inline unsigned int GetHeight() const			{	return m_ImageHeight;		}

		virtual const char* GetImageData() const = 0;

		virtual bool Load() = 0;
		virtual bool Unload() = 0;
	};
}

#endif //__PANDUImage_h__