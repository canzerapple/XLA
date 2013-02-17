#ifndef XLA_STREAM_INCLUDE
#define XLA_STREAM_INCLUDE

#include <XLA/TypeDefine.hpp>

namespace XLA
{
		XLA_STATIC_CONST( UInt32 , BEGIN     = 0 );
		XLA_STATIC_CONST( UInt32 , END       = 2 );
		XLA_STATIC_CONST( UInt32 , CURRENT   = 1 );


		class Stream :
			public Object
		{
		public:
			XLA_REGEDIT_CLASS( Stream );

			virtual Bool   CanRead()   const = 0;
			virtual Bool   CanWrite()  const = 0;
			virtual Bool   CanSeek()   const = 0;
			virtual Bool   CanSize()   const = 0;

			virtual UInt64 GetSize()     const = 0;
			virtual UInt64 GetPosition() const = 0;

			virtual UInt32 ReadBlock(   Byte* byteBuffer , UInt32 byteToRead  )  = 0;
			virtual UInt32 WriteBlock( CByte* byteBuffer , UInt32 byteToWrite )  = 0;

			virtual UInt64 Seek( UInt64 toMove , UInt32 moveMethod ) const = 0;

			virtual Bool   EndOfStream() const = 0;
			virtual Void   Close() = 0;

			UInt32 Read( Array<Byte>& byteBuffer )
			{
				return ReadBlock( byteBuffer , byteBuffer.GetSize() );
			}

			UInt32 Write( const Array<Byte>& byteBuffer )
			{
				return WriteBlock( byteBuffer , byteBuffer.GetSize() );
			}

			UInt32 Read(   Byte* byteBuffer , CUInt32& byteToRead  )
			{
				return ReadBlock( byteBuffer , byteToRead );
			}

			UInt32 Write( CByte* byteBuffer , CUInt32& byteToWrite )
			{
				return WriteBlock(byteBuffer,byteToWrite);
			}
		};

}

#endif