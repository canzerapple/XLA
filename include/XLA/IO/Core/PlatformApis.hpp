#ifndef BOOST_FILE_APIS_INCLUDE
#define BOOST_FILE_APIS_INCLUDE

#include <XLA/TypeDefine.hpp>
#include <Windows.h>




namespace XLA
{
	typedef HANDLE TFile;

	enum open_mode
	{
		OPEN_READ   = (1<<1),
		OPEN_WRITE  = (1<<2),
		OPEN_CREATE = (1<<3)
	};


	static const  TFile ERROR_FD = INVALID_HANDLE_VALUE;



	TFile OpenFile( const String& fileName , const UInt32& mode )
	{
		DWORD dwAccess = 0;

		if( ( mode & OPEN_READ   ) == OPEN_READ  )
			dwAccess |= GENERIC_READ;
		if( ( mode  & OPEN_WRITE ) == OPEN_WRITE )
			dwAccess |= GENERIC_WRITE;

		DWORD dwCreate = OPEN_ALWAYS;

		if( ( mode & OPEN_CREATE) == OPEN_CREATE )
			dwCreate = CREATE_ALWAYS;


		return ::CreateFile( fileName.c_str() , dwAccess , FILE_SHARE_READ , 0 , dwCreate , 0 , 0  );
	}

	Bool CloseFile( TFile fd )
	{
		return (CloseHandle(fd) == TRUE);
	}

	Bool ReadFile( TFile fd , Byte* byteBuffer , const UInt32& byteToRead , UInt32& byteRead )
	{
		DWORD dataRead = 0; 
		BOOL  fileRead =
			::ReadFile( fd , byteBuffer , byteToRead , &dataRead , NULL );

		byteRead = dataRead;

		return (fileRead == TRUE);
	}

	Bool WriteFile( TFile fd , const Byte* byteBuffer , const UInt32& byteToWrite , UInt32& byteWrite )
	{
		DWORD dataWrite = 0;
		BOOL  fileWrite = 
			::WriteFile( 
			fd , 
			byteBuffer  , 
			byteToWrite , 
			&dataWrite  , 
			NULL 
			);

		byteWrite = dataWrite;

		return (fileWrite == TRUE);
	}

	Bool SeekFile( TFile fd , const UInt64& toMove , UInt64& newPosition , const UInt32& moveMethod )
	{
		LARGE_INTEGER liToMove  = { 0 };
		LARGE_INTEGER liNewMove = { 0 };

		liToMove.QuadPart = toMove;

		BOOL posMove = ::SetFilePointerEx( fd , liToMove , &liNewMove , moveMethod );

		newPosition = liNewMove.QuadPart;

		return (posMove == TRUE);
	}

	Bool SizeFile( TFile fd , UInt64& fileSize )
	{
		LARGE_INTEGER liFileSize = { 0 };
		BOOL          Succeed    = FALSE;

		Succeed = GetFileSizeEx( fd , &liFileSize );

		fileSize = liFileSize.QuadPart;

		return ( Succeed == TRUE );
	}

}

#endif