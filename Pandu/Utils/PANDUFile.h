/********************************************************************
	filename: 	PANDUFile
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFile_h__
#define __PANDUFile_h__

#include "PANDUString.h"
#include <iostream>
#include <fstream>
#include "PANDUFlag.h"
#include "PANDUStringUtils.h"
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class File
	{
	public:

		enum FileMode
		{
			eFM_None			= 0,
			eFM_Read			= 1 << 0,
			eFM_Write			= 1 << 1, 
			eFM_Binary			= 1 << 2,
		};

		File(const String& _filename);
		~File();

		bool Open(const TFlag32& _modes);
		bool Close();

		inline unsigned long GetFileLength() const	{	return m_FileLength;	}

		bool LoadStringToBuffer( char* _oBuffer, unsigned int _bufferSize );
		bool LoadBufferFromFile( char* _oBuffer, unsigned int _bufferSize );
		bool WriteBufferToFile( const char* _buffer, unsigned int _bufferSize );

		static String GetFileName(const String& _filePath);

	private:

		unsigned long GetFileLength(std::fstream& _file);
		std::ios_base::open_mode GetOpenMode(const TFlag32& _modes);
		bool IsError( bool _read ) const;

		const String m_FileName;
		unsigned long m_FileLength;

		std::fstream m_FileStream;
		bool m_IsFileOpen;
	};
}

#endif