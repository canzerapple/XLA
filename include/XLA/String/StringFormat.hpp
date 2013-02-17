#ifndef XLA_STRINGFORMAT_INCLUDE
#define XLA_STRINGFORMAT_INCLUDE



namespace XLA
{
	namespace StringUnit
	{
		template< int N >
		struct fmtTable
		{
			static Int32 fmtIndex[256];
		};

		Int32 fmtTable<0>::fmtIndex[256] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
			0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,
			0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0};
	}

	template< class T >
	inline Bool IsStringFormatType( const T& fmtType ) { return false; };
	inline Bool IsStringFormatType( const Int16 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const UInt16 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Int32 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const UInt32 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Int64 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const UInt64 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Float32 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Float64 & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Char & cdValue ) { return true; };
	inline Bool IsStringFormatType( const Byte & cdValue ) { return true; };

	template< class T >
	inline void StringFormatArgument( String& cdValue , CChar* & fmt , const T& cdArgument )
	{
		CChar* token         = fmt;
		Char   szFormat[21]  = {0};
		Char   szData  [128] = {0};

		while( true )
		{
			if(*fmt == 0)
			{
				cdValue.append(token,fmt);
			}

			if( *fmt== _X('%') )
			{
				cdValue.append(token,fmt);
				token = fmt;
				fmt++;

				if( *fmt == _X('s') || *fmt == _X('S') )
				{
					cdValue.append( XLA::ToString(cdArgument) );
					fmt++;
					return;
				}

				while(true)
				{
					if( *fmt == 0 )
					{
						cdValue.append(token,fmt);
						return;
					}

					if( StringUnit::fmtTable<0>::fmtIndex[*fmt++] == 1 )
					{
						Size fmtSize = fmt - token;
						if( fmtSize > 20 )
						{
							cdValue.append(token,fmt);
						}

						for( Size ix = 0 ; ix < fmtSize ; ++ix )
						{
							szFormat[ix] = token[ix];
						}
						szFormat[fmtSize] = 0;

						if( !IsStringFormatType(cdArgument) )
						{
							cdValue.append(token,fmt);
							return;
						}
						else
						{
							memset(szData,0,sizeof(szData));
							swprintf_s( szData , 128 , szFormat , cdArgument );
							cdValue.append(szData);
							return;
						}
					}

				}
			}

			fmt++;

		}
	}

	inline void StringFormatExpand( String& cdValue , CChar* & fmt )
	{

	}

	template< class T , class... Args >
	inline void StringFormatExpand( String& cdValue , CChar* & fmt , const T& argument , const Args&... args )
	{
		StringFormatArgument( cdValue , fmt , argument );
		StringFormatExpand( cdValue , fmt , args... );
	}

	template< class... Args >
	inline String StringFormat( CString& fmt , const Args&... args )
	{
		CChar* format = fmt.c_str();
		String cdValue;
		StringFormatExpand( cdValue , format , args... );

		cdValue.append( format );
		return cdValue;
	}

}

#endif