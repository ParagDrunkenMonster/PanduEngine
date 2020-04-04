/********************************************************************
	filename: 	PANDUBitmapImage
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBitmapImage_h__
#define __PANDUBitmapImage_h__

#include "PANDUImage.h"
#include "PANDUFile.h"
#include "PANDULogManager.h"

namespace Pandu
{
	//currently only supports 24 bit image without alpha
	//TODO: add support to different type of bitmap formats 
	class BitmapImage : public Image
	{
	public:

		const static unsigned short gBitmapMagicNumber = 19778;
		const static unsigned int gRGBByteSize = 3;

#pragma pack(push,BitmapImageData,1)

		struct RGBQuad 
		{
			char blue;
			char green;
			char red;
			char unused;
		};

		struct BitmapFileHeader 
		{
			unsigned short	magicNumber;
			unsigned int	fileSize;
			unsigned short	reserved1;
			unsigned short	reserved2;
			unsigned int	offsetImageData;
		};

		struct BitmapInfoHeader 
		{
			unsigned int	fileHeaderSize;
			int				imageWidth;
			int				imageHeight;
			unsigned short	bitPlanes;			//usually always 1
			unsigned short	bitsPerPixel;		
			unsigned int	compression;		//0 if no compression
			unsigned int	imageData;			//size of image data, sometimes set to 0
			int				horzPixelsPerMeter;
			int				vertPixelsPerMeter;
			unsigned int	numberColors;		//number of colors used in color palette.
												//If the image is 24 bits per pixel, then this is usually 0.
			unsigned int	importantColorIndexesCount;
		};

#pragma pack(pop,BitmapImageData)

	private:

		BitmapFileHeader	m_FileHeader;
		BitmapInfoHeader	m_FileInfoHeader;
		RGBQuad				*m_Colors;
		char				*m_PixelData;
		bool				m_Loaded;
	
	public:
	
		BitmapImage(const String& _filePath);
		virtual ~BitmapImage();

		virtual const char* GetImageData() const		{	return m_PixelData;				}

		virtual bool Load();
		virtual bool Unload();

	private:

		void FreeData();
		bool ConvertImagedataToPixelData(char* _data, unsigned int _dataSize, unsigned int _byteWidth, unsigned int _padWidth);
		
	};
}

#endif //__PANDUBitmapImage_h__