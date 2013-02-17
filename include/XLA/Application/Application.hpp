#ifndef XLA_APPLICATION_INCLUDE
#define XLA_APPLICATION_INCLUDE

#include <map>
#define ApplicationMain wmain

#define REGEDIT_APPLICATION( AppType ) XLA::Application::ApplicationInitalize<AppType> _loadAppInitalize;

namespace XLA
{
	class FileInfo
	{
		String _data;

		void Initalize( CString& data )
		{
			Char szBuffer[MAX_PATH] = {0};

			if( ::GetFullPathName( data.c_str() , MAX_PATH , szBuffer  , 0 ) == 0 )
			{
				ExpThrow( SystemException( Environment::GetResourceString(_X("FileInfo_Abstract")) ) );
			}

			_data = szBuffer;
		}
	public:
		FileInfo( CString& data )
			: _data( data )
		{
			Initalize(data);
		}

		FileInfo( CChar* data )
		{
			Initalize(data);
		}

		FileInfo()
		{

		}

		operator CString() const
		{
			return _data;
		}

		String GetParent() const
		{
			Size last  = _data.find_last_of(_X("\\"));
			Size first = _data.find_first_of(_X("\\"));

			if(last == String::npos || first == last )
				return _data;

			return String( _data.c_str() , last + 1 );
		}

		static void SetLocal( CString& dirName )
		{
			if( ::SetCurrentDirectory( dirName.c_str() ) == FALSE )
			{
				ExpThrow( SystemException(_X("FileInfo_SetLocal")) );
			}
		}
	};

	class Application;

	class CommandLineException
		: public RunTimeException
	{
		Int32 _argc;
		CChar* _argv;

	public:
		XLA_REGEDIT_CLASS( CommandLineException )
			CommandLineException( CInt32& argc , CChar* argv )
			: _argc( argc )
			, _argv( argv )
		{

		}

		virtual String ToString() const
		{
			return _X("[") + XLA::ToString(_argc) + _X("]:") + _argv;
		}
	};

	class CommandLine
		: public Object
	{
		typedef std::map< String , String > Arguments;
		Arguments _argument;

		template< class F >
		void Skip( CChar* & stream , F func )
		{
			while( *stream != 0 )
			{
				if( !func(*stream) )
					return;
				++stream;
			}
		}

	protected:

		static Bool CommandKeyPredicate( Char ch )
		{
			if( ch == _X('=') || isspace(ch) )
				return false;
			return true;
		}

		static Bool CommandKeySkipPredicate( Char ch )
		{
			return !CommandKeyPredicate(ch);
		}

		static Bool CommandValuePredicate( Char ch )
		{
			if( ch == _X('\t')  )
				return false;
			return true;
		}
	public:
		XLA_REGEDIT_CLASS( CommandLine );

		CommandLine()
		{
		}

		Bool Contain( CString& query ) const
		{
			String cdValue = ToUpper(query);

			Arguments::const_iterator token = _argument.find( cdValue );

			if(token == _argument.end())
				return false;
			return true;
		}

		

		UInt32 GetSize() const
		{
			return static_cast<UInt32>( _argument.size() );
		}

		void LoadCommandLines( Int32 argc , Str* argv )
		{
			for(Int32 idx = 1 ; idx < argc ; ++ idx )
			{
				CChar* currentArgv = argv[idx];

				if( *currentArgv != _X('-') && *currentArgv != _X('/') )
				{
					ExpThrow( CommandLineException( idx , currentArgv ) );
				}

				currentArgv++;

				String argumentKey = currentArgv;

				_argument[ToUpper(argumentKey)] = String();

				if( (idx + 1) == argc )
					continue;

				currentArgv = argv[++idx];

				if( *currentArgv == _X('-') || *currentArgv == _X('/') )
					continue;

				String argumentValue = currentArgv;

				_argument[ToUpper(argumentKey)] = argumentValue;
			}
		}
	};



	class Application
		: public Object
	{
		static Application* _runApplication;
	protected:
		CommandLine _commandLine;

	public:
		Application()
		{

		}

		virtual void ApplicationException( const Exception& e ) = 0;

		static Application* GetApplication()
		{
			return _runApplication;
		}

		template< class T >
		struct ApplicationInitalize
		{
			ApplicationInitalize()
			{
				_runApplication = (T*)malloc( sizeof(T) );
				new(_runApplication) T();
			}
		};

		Int32 RunApplication( Int32 argc , Str* argv )
		{

			try
			{
				_commandLine.LoadCommandLines( argc , argv );
				return Run();
			}
			catch(  const Exception& e )
			{
				ApplicationException(e);
			}

			return 0;

		}

		static Int32 Main( Int32 argc , Str* argv )
		{
			return GetApplication()->RunApplication( argc , argv );
		}

		virtual Int32 Run() = 0;
	};

	Application* Application::_runApplication = 0;
}

#endif