#include "PANDUBitmapImage.h"

namespace Pandu
{
	BitmapImage::BitmapImage(const String& _filePath)
		: Image(IT_Bitmap,_filePath)
		, m_Colors(NULL)
		, m_PixelData(NULL)
		, m_Loaded(false)
	{

	}

	BitmapImage::~BitmapImage()
	{
		FreeData();
	}

	bool BitmapImage::Load()
	{
		if( m_Loaded )
		{
			gLogManager->WriteErrorLog("Image already loaded...");
			return false;
		}

		File file(m_FilePath);
		if( !file.Open( File::eFM_Binary | File::eFM_Read ) )
		{
			return false;
		}

		file.LoadBufferFromFile((char*)(&m_FileHeader),sizeof(BitmapFileHeader));
		file.LoadBufferFromFile((char*)(&m_FileInfoHeader),sizeof(BitmapInfoHeader));

		const unsigned int fileSize = file.GetFileLength();
		
		if( fileSize != m_FileHeader.fileSize )
		{
			gLogManager->WriteErrorLog("File is corrupted...");;
			return false;
		}

		// only 24 bit bitmap is supported
		if( m_FileInfoHeader.bitsPerPixel == 0 || m_FileInfoHeader.bitsPerPixel%24 != 0 )
		{
			gLogManager->WriteErrorLog("Not supported bitmap file, Currently only 24 bit bmp is supported...");;
			return false;
		}

		m_ImageWidth = abs(m_FileInfoHeader.imageWidth);
		m_ImageHeight = abs(m_FileInfoHeader.imageHeight);

		//const unsigned int numColors = 1 << m_FileInfoHeader.bitsPerPixel;
		const unsigned int dataSizeComputed = m_ImageWidth * m_ImageHeight * m_FileInfoHeader.bitsPerPixel/8;
		
		void* tempPixelData = malloc(dataSizeComputed);

		if( tempPixelData == NULL )
		{
			gLogManager->WriteErrorLog("Not enough memory to load image file...");
			return false;
		}

		if( !file.LoadBufferFromFile((char*)tempPixelData,dataSizeComputed) )
		{
			gLogManager->WriteErrorLog("Error loading image file...");
			return false;
		}

		file.Close();

		unsigned int byteWidth, padWidth;
		byteWidth = padWidth = m_ImageWidth * m_FileInfoHeader.bitsPerPixel/8;

		while( padWidth%4 != 0 ) 
		{
			padWidth++;
		}

		const bool converted = ConvertImagedataToPixelData((char*)tempPixelData,dataSizeComputed,byteWidth,padWidth);
		std::free(tempPixelData);

		if( !converted )
		{
			gLogManager->WriteErrorLog("Unable to convert to pixel data...");
			return false;
		}

		m_ImageFormat = ICF_R8G8B8;

		return true;
	}

	bool BitmapImage::Unload()
	{
		if( m_Loaded )
		{
			return false;
		}
		FreeData();
		return true;
	}

	bool BitmapImage::ConvertImagedataToPixelData(char* _data, unsigned int _dataSize, unsigned int _byteWidth, unsigned int _padWidth)
	{
		if( m_PixelData )
		{
			return false;
		}

		const unsigned int actualImageSize = m_ImageWidth * m_ImageHeight * gRGBByteSize;
		m_PixelData = (char*)malloc(actualImageSize);
		if( m_PixelData == NULL ) 
		{
			gLogManager->WriteErrorLog("Not enough memory to convert image file...");
			return false;
		}

		if( m_FileInfoHeader.imageHeight > 0 ) 
		{
			const unsigned int offset = _padWidth - _byteWidth;
			
			for( unsigned int i = 0 ; i < _dataSize ; i+=3 ) 
			{
				if( (i + 1) % _padWidth == 0 ) 
				{
					i += offset;
				}
			
				*(m_PixelData + i + 2) = *(_data + i + 0);
				*(m_PixelData + i + 1) = *(_data + i + 1);
				*(m_PixelData + i + 0) = *(_data + i + 2);
			}
		}
		else 
		{
			const unsigned int offset = _padWidth - _byteWidth;
			unsigned int j = _dataSize - 3;
			//count backwards so you start at the front of the image
			//here you can start from the back of the file or the front,
			//after the header  The only problem is that some programs
			//will pad not only the data, but also the file size to
			//be divisible by 4 bytes.
			for( unsigned int i = 0 ; i < _dataSize ; i+=3 ) 
			{
				if( (i + 1) % _padWidth == 0 ) 
				{
					i += offset;
				}
				*(m_PixelData + i + 2) = *(_data + i + 0);
				*(m_PixelData + i + 1) = *(_data + i + 1);
				*(m_PixelData + i + 0) = *(_data + i + 2);
				j -= 3;
			}
		}

		return true;
	}

	void BitmapImage::FreeData()
	{
		if(m_Colors)
		{
			std::free(m_Colors);
			m_Colors = NULL;
		}
		
		if(m_PixelData)
		{
			std::free(m_PixelData);
			m_PixelData = NULL;
		}

		m_Loaded = false;
	}
}