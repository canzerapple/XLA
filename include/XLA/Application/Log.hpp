#ifndef XLA_LOG_INCLUDE
#define XLA_LOG_INCLUDE

#include <Windows.h>
#include <XLA/TypeDefine.hpp>
#include <XLA/String/StringData.hpp>
#include <list>
#include <iostream>

namespace XLA
{

	class Logger
		: public Object
	{
	public:
		XLA_REGEDIT_CLASS( Logger );
		virtual void WriteData( CString& logData ) = 0;
	};

	class DebugViewLogger
		: public Logger
	{
	public:
		XLA_REGEDIT_CLASS( DebugViewLogger );

		virtual void WriteData( CString& logData )
		{
			::OutputDebugString( logData.c_str() );
		}
	};

	class ConsoleLogger
		: public Logger
	{
	public:
		XLA_REGEDIT_CLASS( ConsoleLogger );

		virtual void WriteData( CString& logData )
		{
			std::wcout<<logData<<std::endl;
		}
	};

	class LogSupport
		: public Object
	{
		typedef std::list< Logger* > LoggerList;

		LoggerList _logger;
	public:
		XLA_REGEDIT_CLASS( LogSupport );
		virtual void WriteLog( CString& cdValue )
		{
			for (Logger* logger : _logger )
			{
				logger->WriteData( cdValue );
			}
		}

		void Add( Logger* logger )
		{
			_logger.push_back( logger );
		}

		template< class... Args >
		void WriteLog( CString& fmt , const Args&... args )
		{ 
			WriteLog( 
				StringFormat( fmt , args... ) 
			); } 
	};
}

#endif