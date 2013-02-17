#ifndef XLA_CONSOLE_APPLICATION_INCLUDE
#define XLA_CONSOLE_APPLICATION_INCLUDE
#include <XLA/TypeDefine.hpp>
#include <XLA/Exception.hpp>
#include <XLA/Application/Application.hpp>
#include <Windows.h>
#include <XLA/String/StringData.hpp>
#include <map>



namespace XLA
{

	class ConsoleApplication
		: public Application
		, public StringData
	{
		virtual void WriteLineData( CString& cdValue )
		{
			std::wcout<<cdValue<<std::endl;
		}

		virtual void WriteData( CString& cdValue )
		{
			std::wcout<<cdValue;
		}
	public:
		virtual void ApplicationException( const Exception& e )
		{
			this->WriteLine( _X("%s -- %s") , e.GetClassName() , e );
		}
	};
}

int ApplicationMain( XLA::Int32 argc , XLA::Char* argv[] )
{
	return XLA::ConsoleApplication::Main( argc , argv );
}

#endif