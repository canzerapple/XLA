#ifndef XLA_STRING_INCLUDE
#define XLA_STRING_INCLUDE
#include <XLA/TypeDefine.hpp>
#include <XLA/Exception.hpp>
#include <sstream>

namespace XLA
{

	XLA_STATIC_CONST( UInt32 , XLA_FLOAT_PRECISION = 10 );

	inline Bool IsDigit( CUInt32& ch )
	{
		return isdigit( ch ) != 0;
	}

	inline String ToString( CFloat64& cdValue , CInt32& precision = -1 )
	{

		String cdString;

		typedef std::basic_stringstream< Char > StdStringStream;
		StdStringStream cvtStringStream;

		if( precision == -1 )
			cvtStringStream.precision( XLA_FLOAT_PRECISION );
		else if( precision >= 0 || precision <= 50 )
			cvtStringStream.precision( precision );

		cvtStringStream << cdValue;
		cvtStringStream >> cdString;

		return cdString;
	}

	inline String ToString( CFloat32& cdValue , CInt32& precision = -1 )
	{

		String cdString;

		typedef std::basic_stringstream< Char > StdStringStream;
		StdStringStream cvtStringStream;

		if( precision == -1 )
			cvtStringStream.precision( XLA_FLOAT_PRECISION );
		else if( precision >= 0 || precision < 50 )
			cvtStringStream.precision( precision );

		cvtStringStream << cdValue;
		cvtStringStream >> cdString;

		return cdString;
	}

	XLA_STATIC_CONST( UInt32 , XLA_CDSTRINGMAX = 64 );

	inline String ToString( CInt32& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) ) );


		cdError = _i64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}



	inline String ToString( CUInt32& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) ) );


		cdError = _ui64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}



	inline String ToString( CInt16& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) ) );


		cdError = _i64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}



	inline String ToString( CUInt16& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) ) );


		cdError = _ui64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}



	inline String ToString( CInt64& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( 
				(ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) )) );


		cdError = _i64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}



	inline String ToString( CUInt64& cdValue , CInt32& radix = 10 )
	{
		Char    szBuffer[XLA_CDSTRINGMAX] = { 0 };
		errno_t cdError                   = 0;
		if( radix < 2 || radix > 36 )
			ExpThrow( ArgumentOutOfRangeException( _X("raidx") , Environment::GetResourceString(_X("ArgumentOutOfRange_radix")) ) );


		cdError = _ui64tow_s( cdValue , szBuffer , XLA_CDSTRINGMAX , radix );

		if( cdError == 0 )
			return szBuffer;
		return String();
	}


	inline UInt32 ToUInt32( CString& cdString )
	{
		typedef std::basic_stringstream< Char > StdStringStream;
		StdStringStream cvtStringStream;

		UInt32 cdValue = 0;

		cvtStringStream<<cdString;
		cvtStringStream>>cdValue;

		return cdValue;
	}

	inline Int32 ToInt32( CString& cdString )
	{
		typedef std::basic_stringstream< Char > StdStringStream;
		StdStringStream cvtStringStream;

		Int32 cdValue = 0;

		cvtStringStream<<cdString;
		cvtStringStream>>cdValue;

		return cdValue;
	}

	inline CString& ToString( CString& cdValue )
	{
		return cdValue;
	}

	inline String ToUpper( CString& cdValue )
	{
		String value = cdValue;

		String::iterator token = value.begin();
		String::iterator end   = value.end();

		for(;token!=end;++token)
		{
			*token = toupper(*token);
		}

		return value;
	}

	enum EEqulaOption
	{
		EqualDefault,
		EqualIgnoreCase
	};

	inline Bool Equal( CString& source , CString& cmp , EEqulaOption opt = EqualDefault )
	{
		if( source.size() != cmp.size() )
		{
			return false;
		}

		typedef String::const_iterator iterator;
		
		iterator token = source.begin();
		iterator end   = source.end();
		iterator now   = cmp.begin();

		while( token != end )
		{
			Char charToken = *token;
			Char charNow   = *now;

			if( opt == EqualIgnoreCase )
			{
				charToken = toupper(charToken);
				charNow   = toupper(charNow);
			}
			
			if( charToken != charNow )
				return false;

			token++;
			now++;
		}

		return true;
	}

	template< class T >
	inline String TemplateToString( const T& cdValue )
	{
		return ToString( cdValue );
	}

	template< class T >
	String ToString( const T& cdValue )
	{
		return cdValue.ToString();
	}

	template< class T >
	String ToString( const T* cdValue )
	{
		return cdValue->ToString();
	}

}


namespace XLA
{

	class CharTokenizer
		: public Object
	{
		CChar  _split;
		CChar* _token;
		CChar* _end;
		String _current;
	public:
		XLA_REGEDIT_CLASS( CharTokenizer );
		CharTokenizer( CString& _source , CChar& ch )
			: _split( ch )
		{
			_token = _source.c_str();
			_end   = _source.c_str() + _source.size();
		}

		Bool MoveNext()
		{
			if(_token == _end )
				return false;
			CChar* now = _token;
			while( _token != _end )
			{
				if(* _token == _split )
				{
					_current.assign(now,_token);
					++_token;
					return true;
				}
				++_token;
			}

			_current.assign( now , _token );
			return true;
		}

		String GetCurrent() const
		{
			return _current;
		}

		virtual String ToString() const
		{
			return _current;
		}
	};

	template< class ContainT >
	void Split( CString& source , CChar& ch , ContainT& contain )
	{
		CharTokenizer charTokenizer( source , ch );
		while(charTokenizer.MoveNext())
		{
			contain.push_back( charTokenizer.ToString() );
		}
	}

	
}

#include <XLA/String/StringFormat.hpp>

#endif