#ifndef XLA_TYPEDEF_INCLUDE
#define XLA_TYPEDEF_INCLUDE

#include <string>
#include <Windows.h>


namespace XLA
{
	typedef                short   Int16;
	typedef const          short  CInt16;
	typedef unsigned       short  UInt16;
	typedef const unsigned short CUInt16;

	typedef                int   Int32;
	typedef const          int  CInt32;
	typedef unsigned       int  UInt32;
	typedef const unsigned int CUInt32;

	typedef                __int64   Int64;
	typedef const          __int64  CInt64;
	typedef unsigned       __int64  UInt64;
	typedef const unsigned __int64 CUInt64;

	typedef                float   Float32;
	typedef const          float  CFloat32;

	typedef                double   Float64;
	typedef const          double  CFloat64;

	typedef                unsigned char    Byte;
	typedef const          unsigned char   CByte;

	typedef       std::wstring  String;
	typedef const std::wstring CString;

	typedef wchar_t Char;
	typedef const wchar_t CChar;

	typedef wchar_t* 		Str;
	typedef const wchar_t* CStr; 

	typedef bool Bool;
	typedef const bool CBool;

	typedef void Void;
	typedef const void CVoid;

	typedef std::size_t Size;
}

#define XLA_CODE_TOSTRING_( code ) L##code
#define XLA_CODE_TOSTRING( code ) XLA_CODE_TOSTRING_( code )

#define XLA_CODE_ASSTRING_(x) #x
#define XLA_CODE_ASSTRING(x) XLA_CODE_ASSTRING_(x)

#define XLA_CODE_RAWSTRING_( code ) LR##code
#define XLA_CODE_RAWSTRING( code ) XLA_CODE_RAWSTRING_( code )

#if !defined( _X )
#define _X( code ) XLA_CODE_TOSTRING( code )
#endif

#if !defined( _R )
#define _R( x ) XLA_CODE_RAWSTRING( x )
#endif

#define _AT( x ) _X( XLA_CODE_ASSTRING(x) )


#define XLA_STATIC_CONST( type , assigment ) static const type assigment

#include <XLA/Object.hpp>
#include <XLA/String.hpp>
#include <XLA/Range.hpp>


#endif
