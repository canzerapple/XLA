
#include "EncodingLinkinfo.hpp"
#include <XLA/Encoding/Encoding.hpp>
#include <XLA/Encoding/EncodingInfo.hpp>
#include <map>
#include <algorithm>

#include <XLA/Encoding/UTF8Encoding.hpp>
#include <XLA/Encoding/UTF7Encoding.hpp>
#include <XLA/Encoding/UTF16Encoding.hpp>
#include <XLA/Encoding/UTF32Encoding.hpp>

#define INIT_STATIC_ENCODING( encoding ) (encoding = 0)  

namespace XLA
{
	class EncodingTable :
		public Object
	{
		typedef std::map< String , EncodingInfo > EncodingHashTable;
	protected:
		EncodingHashTable _encodingTable;
	public:
		XLA_REGEDIT_CLASS( EncodingTable );

		void AddEncoding( const EncodingInfo& encodingInfo )
		{
			_encodingTable[ToUpper( encodingInfo.GetDisplayName() )] = encodingInfo;
			_encodingTable[ToUpper( encodingInfo.GetName() )       ] = encodingInfo;
		}

		EncodingInfo GetEncodingInfo( CString& displayName )
		{


			EncodingHashTable::iterator token = _encodingTable.find( displayName );
			if( token == _encodingTable.end() )
				return EncodingInfo();
			return token->second;
		}
	};

	struct StaticEncodings
	{
		Encoding* _UTF8;
		Encoding* _Unicode;
		Encoding* _UnicodeBE;
		Encoding* _UTF32;
		Encoding* _UTF32BE;
		Encoding* _UTF7;

		EncodingTable* _EncodingTable;

		StaticEncodings()
		{
			INIT_STATIC_ENCODING(_UTF8);
			INIT_STATIC_ENCODING(_Unicode);
			INIT_STATIC_ENCODING(_UnicodeBE);
			INIT_STATIC_ENCODING(_UTF32);
			INIT_STATIC_ENCODING(_UTF32BE);
			INIT_STATIC_ENCODING(_UTF7);

			_EncodingTable = 0;
		}
	};

	StaticEncodings staticEncoding;

	void InitalizeEncodingTable( EncodingTable* encodingTable )
	{
		encodingTable->AddEncoding(
			EncodingInfo(
			UTF8Encoding::StaticGetClassName(),
			_X("UTF-8"),
			Encoding::CodePageUTF8 ) );

		encodingTable->AddEncoding(
			EncodingInfo(
			UTF16Encoding::StaticGetClassName(),
			_X("UTF-16"),
			Encoding::CodePageUnicode ) );

		encodingTable->AddEncoding(
			EncodingInfo(
			UTF16Encoding::StaticGetClassName(),
			_X("UTF-16 Big Endian"),
			Encoding::CodePageUnicodeBE ) );

		encodingTable->AddEncoding(
			EncodingInfo(
			UTF32Encoding::StaticGetClassName(),
			_X("UTF-32"),
			Encoding::CodePageUTF32 ) );

		encodingTable->AddEncoding(
			EncodingInfo(
			UTF32Encoding::StaticGetClassName(),
			_X("UTF-32 Big Endian"),
			Encoding::CodePageUTF32BE ) );

		encodingTable->AddEncoding(
			EncodingInfo(
			UTF7Encoding::StaticGetClassName(),
			_X("UTF-7"),
			Encoding::CodePageUTF7 ) );
	}
}

namespace XLA
{

	namespace Detail
	{
		UInt32 LockEncodings()
		{
			return 0;
		}

		UInt32 UnlockEncodings()
		{
			return 0;
		}

		UInt32 GetEncoding( CUInt32& codePage , Encoding** encoding )
		{
			switch( codePage )
			{
			case Encoding::CodePageUTF8 :
				{
					if( staticEncoding._UTF8 == 0 )
					{
						LockEncodings();
						if( staticEncoding._UTF8 == 0 )
							staticEncoding._UTF8 = Detail::CreateUTF8Encoding();
						UnlockEncodings();
					}
					*encoding = staticEncoding._UTF8;
					break;
				}
			case Encoding::CodePageUnicode :
				{
					if( staticEncoding._Unicode == 0 )
					{
						LockEncodings();
						if( staticEncoding._Unicode == 0 )
							staticEncoding._Unicode = Detail::CreateUTF16Encoding(false);
						UnlockEncodings();
					}
					*encoding = staticEncoding._Unicode;
					break;
				}
			case Encoding::CodePageUnicodeBE :
				{
					if( staticEncoding._UnicodeBE == 0 )
					{
						LockEncodings();
						if( staticEncoding._UnicodeBE == 0 )
							staticEncoding._UnicodeBE = Detail::CreateUTF16Encoding(true);
						UnlockEncodings();
					}
					*encoding = staticEncoding._UnicodeBE;
					break;
				}
			case Encoding::CodePageUTF32 :
				{
					if( staticEncoding._UTF32 == 0 )
					{
						LockEncodings();
						if( staticEncoding._UTF32 == 0 )
							staticEncoding._UTF32 = Detail::CreateUTF32Encoding(false);
						UnlockEncodings();
					}
					*encoding = staticEncoding._UTF32;
					break;
				}
			case Encoding::CodePageUTF32BE :
				{
					if( staticEncoding._UTF32BE == 0 )
					{
						LockEncodings();
						if( staticEncoding._UTF32BE == 0 )
							staticEncoding._UTF32BE = Detail::CreateUTF32Encoding(true);
						UnlockEncodings();
					}
					*encoding = staticEncoding._UTF32BE;
					break;
				}
			case Encoding::CodePageUTF7 :
				{
					if( staticEncoding._UTF7 == 0 )
					{
						LockEncodings();
						if( staticEncoding._UTF7 == 0 )
							staticEncoding._UTF7 = Detail::CreateUTF7Encoding();
						UnlockEncodings();
					}
					*encoding = staticEncoding._UTF7;
					break;
				}
			default:
				return EEncodingCodePageNotFound;
			}

			return EEncodingLoad;
		}

		UInt32 GetEncodingCodePage( CString& name , UInt32& codePage )
		{
			if( staticEncoding._EncodingTable == 0 )
			{
				LockEncodings();

				if( staticEncoding._EncodingTable == 0 )
				{
					staticEncoding._EncodingTable = new EncodingTable();
					InitalizeEncodingTable( staticEncoding._EncodingTable );
				}

				UnlockEncodings();
			}

			EncodingInfo encodingInfo = staticEncoding._EncodingTable->GetEncodingInfo( name );

			if( encodingInfo.GetCodePage() == 0xFFFFFFFF )
				return EEncodingNameNotFound;

			codePage = encodingInfo.GetCodePage();

			return EEncodingLoad;
		}
	}
	
}

