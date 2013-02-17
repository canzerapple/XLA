#ifndef BOOST_MEMORY_STREAM_INCLUDE
#define BOOST_MEMORY_STREAM_INCLUDE

#include <vector>
#include <XLA/IO/Stream.hpp>
#include <XLA/Exception.hpp>

namespace XLA
{

	class MemoryStream :
		public Stream
	{
		mutable UInt32 _logicPosition;
		UInt32 _logicEnd;
		UInt32 _pageSize;

		XLA_STATIC_CONST( UInt32 , PAGE_SIZE = 512 );

		typedef std::vector< Byte* > TMemoryPageList;

		TMemoryPageList _memPage;

		UInt32 GetPageIndex( UInt32& logicPosition )
		{
			return (logicPosition/PAGE_SIZE);
		}

		void AllocEnoughSpace()
		{
			if( GetPageIndex(_logicPosition) >= _memPage.size() )
			{
				Byte* data = (Byte*)malloc(_pageSize);
				if( data == 0 )
					ExpThrow( MemroyAllocException( _pageSize ) );
				_memPage.push_back(data);
			}
		}

		Byte* GetCurrentPage()
		{
			UInt32 pageIndex = GetPageIndex( _logicPosition );
			return _memPage[pageIndex];
		}

		UInt32 GetCurrentPagePosition()
		{
			return _logicPosition - GetPageIndex(_logicPosition)*PAGE_SIZE;
		}

		UInt32 GetCurrentPageFreeSize()
		{
			return PAGE_SIZE - GetCurrentPagePosition();
		}

		Byte* GetCurrentPageData()
		{
			return GetCurrentPage() + GetCurrentPagePosition();
		}


		UInt32 WriteDataToPage( CByte* byteBuffer , UInt32 byteToWite )
		{
			AllocEnoughSpace();

			UInt32 pageFreeSize   = GetCurrentPageFreeSize();
			Byte*  pageData       = GetCurrentPageData();
			UInt32 pageWriteSize  = pageFreeSize > byteToWite ? byteToWite : pageFreeSize;

			memcpy( pageData , byteBuffer , pageWriteSize );

			_logicPosition += pageWriteSize;
			_logicEnd       = _logicPosition > _logicEnd ? _logicPosition : _logicEnd;

			return pageWriteSize;
		}

		UInt32 ReadDataFromPage( Byte* byteBuffer , UInt32 byteToRead )
		{
			UInt32 pageFreeSize = GetCurrentPageFreeSize();
			Byte*  pageData     = GetCurrentPageData();
			UInt32 pageReadSize = pageFreeSize > byteToRead ? byteToRead : pageFreeSize;

			memcpy( byteBuffer , pageData , pageReadSize );

			_logicPosition += pageReadSize;

			return pageReadSize;
		}
	public:

		XLA_REGEDIT_CLASS( MemoryStream );

		MemoryStream() 
			: _logicPosition(0)
			, _memPage(0)
			, _logicEnd(0)
			, _pageSize(PAGE_SIZE)
		{

		}

		virtual Bool CanRead()   const
		{
			return true;
		}

		virtual Bool CanWrite()  const
		{
			return true;
		}

		virtual Bool CanSeek()   const 
		{
			return true;
		}

		virtual Bool CanSize()   const 
		{
			return true;
		}

		virtual UInt64 GetSize() const
		{
			return _logicEnd;
		}

		virtual UInt64 GetPosition() const
		{
			return _logicPosition;
		}

		virtual UInt32 ReadBlock( Byte* byteBuffer , UInt32 byteToRead )
		{
			UInt32 logicFreeSize = _logicEnd - _logicPosition;
			UInt32 logicReadSize = byteToRead > logicFreeSize ? logicFreeSize:byteToRead;

			UInt32 sizeNeedRead = logicReadSize;

			while( sizeNeedRead > 0 )
			{
				UInt32 pageReadSize = 
					ReadDataFromPage( byteBuffer , sizeNeedRead );

				sizeNeedRead -= pageReadSize;
				byteBuffer   += pageReadSize;
			}

			return logicReadSize;
		}

		virtual UInt32 WriteBlock( CByte* byteBuffer , UInt32 byteToWrite )
		{
			UInt32 sizeNeedWrite = byteToWrite;
			while ( sizeNeedWrite > 0 )
			{
				UInt32 pageWriteSize = 
					WriteDataToPage( byteBuffer , sizeNeedWrite );
				sizeNeedWrite -= pageWriteSize;
				byteBuffer    += pageWriteSize;
			}
			return byteToWrite;
		}

		virtual UInt64 Seek( UInt64 toMove , UInt32 moveMethod ) const
		{
			UInt64 logicFreeSize = _logicEnd - _logicPosition;
			UInt64 logicMovePos  = toMove;
			if( moveMethod == CURRENT )
				logicMovePos += _logicPosition;
			else if( moveMethod == END )
			{
				if( _logicPosition < logicMovePos )
					ExpThrow( ArgumentOutOfRangeException(_X("toMove") ) );
				logicMovePos -= toMove;
			}

			if( logicMovePos > _logicEnd )
				ExpThrow( ArgumentOutOfRangeException(_X("toMove")) );

			_logicPosition = static_cast<UInt32>(logicMovePos);

			return _logicPosition;
		}

		virtual Bool   EndOfStream() const
		{
			return _logicPosition == _logicEnd;
		}

		virtual ~MemoryStream()
		{
			TMemoryPageList::iterator token = _memPage.begin();
			TMemoryPageList::iterator end   = _memPage.end();

			for(;token!=end;++token)
			{
				free( *token );
			}
		}
	};
}

#endif