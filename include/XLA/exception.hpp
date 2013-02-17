#ifndef XLA_CEXCEPTION_INCLUDE
#define XLA_CEXCEPTION_INCLUDE

#include <XLA/TypeDefine.hpp>


namespace XLA
{
	class Environment :
		public Object
	{
	public:
		static String GetResourceString( CString& resName )
		{
			return resName;
		}
	};

	typedef DWORD TErrorCode;

	class CompileTimeInfo 
	{
	public:
		XLA_REGEDIT_CLASS( CompileTimeInfo );

		String   SourceFile;
		UInt32   SoucreLine;
		String   SourceFunction;

		CompileTimeInfo( const String& file , const UInt32& line , const String& func ) 
			: SourceFile(file)
			, SoucreLine(line)
			, SourceFunction(func)
		{

		}

		CompileTimeInfo() 
			: SoucreLine(0)
		{

		}
	};

#define GetComplieTimeInfo() (XLA::CompileTimeInfo( _X(__FILE__) , __LINE__ ,  _X("")/*_X(__FUNCTION__)*/ ) )

#define GetSystemErrorCode() ::GetLastError()


	inline String GetSystemErrorMessage( const TErrorCode& ec )
	{
		DWORD  dwFlags   = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
		LPVOID lpMsgData = 0;
		DWORD  dwLanguage= MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT);
		String mesasge;

		DWORD code = ::FormatMessage( dwFlags , 0 , ec , dwLanguage , (LPTSTR)&lpMsgData , 0 , 0  );


		if( code != 0 )
		{
			mesasge = (LPTSTR)lpMsgData;
		}

		::LocalFree( lpMsgData );

		return mesasge;
	}

	class Exception
	{
		CompileTimeInfo _compileTimeInfo;

	public:

		XLA_REGEDIT_CLASS( Exception );

		Exception() 
		{

		}

		void SetCompileTimeInfo( const CompileTimeInfo& ctInfo )
		{
			_compileTimeInfo = ctInfo;
		}

		virtual String ToString() const
		{
			return _X("Generic Exception!");
		}

		String GetFunctionName() const
		{
			return _compileTimeInfo.SourceFunction;
		}
	};

	class RunTimeException
		: public Exception
	{
	protected:
		String GetResourceString( const UInt32& id )
		{
			return _X("Not Support!");
		}
	public:
		XLA_REGEDIT_CLASS( RunTimeException );
	};

	class ArgumentException :
		public RunTimeException
	{
		String _argument;
		String _message;
	public:
		XLA_REGEDIT_CLASS( ArgumentException );

		ArgumentException( const String& msg ) 
			: _message( msg ) 
		{

		}

		ArgumentException( const String& msg , const String& argument ) 
			: _message( msg )
			, _argument( argument )
		{

		}

		String GetArgument() const
		{
			return _argument;
		}

		virtual String ToString() const
		{
			if( _argument.empty() )
				return _message;
			return _message + _X("(") + _argument + _X(")");
		}
	};

	class ArgumentOutOfRangeException :
		public ArgumentException
	{
	public:
		XLA_REGEDIT_CLASS(ArgumentOutOfRangeException);

		ArgumentOutOfRangeException( CString& argument , CString& message )
			: ArgumentException( message , argument )
		{

		}

		ArgumentOutOfRangeException( CString& argument )
			: ArgumentException( _X("") , argument )
		{

		}
	};

	class NullArgumentException :
		public ArgumentException
	{
	public:
		XLA_REGEDIT_CLASS( NullArgumentException );

		NullArgumentException( CString& argument )
			: ArgumentException( _X("") , argument )
		{

		}
	};


	class SystemException :
		public Exception
	{
		TErrorCode _systemErrorCode;
		String     _message;
	public:
		XLA_REGEDIT_CLASS( SystemException );

		SystemException(const TErrorCode& ec ) 
			: _systemErrorCode(ec)
		{

		}

		SystemException( const TErrorCode& ec , CString& message )
			: _systemErrorCode( ec )
			,_message( message ) 
		{

		}

		SystemException(  CString& message )
			: _systemErrorCode( GetSystemErrorCode() )
			,_message( message ) 
		{

		}

		SystemException() 
			: _systemErrorCode( GetSystemErrorCode() )
		{

		}

		TErrorCode GetCode() const 
		{
			return _systemErrorCode;
		}

		virtual String FormatErrorCode( const TErrorCode& ec ) const
		{
			return GetSystemErrorMessage( ec );
		}

		virtual String ToString() const
		{
			if( _message.empty() )
				return FormatErrorCode( _systemErrorCode );
			return _message + _X(" ") + FormatErrorCode(_systemErrorCode);
		}
	};

	class MemroyAllocException :
		public SystemException
	{
		UInt32 _allocSize;

	public:
		XLA_REGEDIT_CLASS( MemroyAllocException );

		MemroyAllocException( CUInt32& allocSize )
			: _allocSize( allocSize )
		{

		}

		UInt32 AllocSize()
		{
			return _allocSize;
		}
	};

	typedef errno_t TErrno;

	class StandardException
		: public Exception
	{
		TErrno _errno;
	public:
		XLA_REGEDIT_CLASS( StandardException );

		StandardException( const TErrno& eno )
			: _errno( eno )
		{

		}

		TErrno GetErrno() const
		{
			return _errno;
		}

		virtual String ToString() const
		{
			Char szMessage[512] = {0};
			_wcserror_s( szMessage , GetErrno() );

			return szMessage;
		}
	};


	template< class ExceptionT>
	void ThrowException( ExceptionT& e , const CompileTimeInfo& ctInfo )
	{
		e.SetCompileTimeInfo(ctInfo);
		throw e;
	}

#define ExpThrow( e ) XLA::ThrowException( (e) , (GetComplieTimeInfo()) )
}

#endif