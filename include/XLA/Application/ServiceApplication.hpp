#ifndef XLA_SERVICE_APPLICATION_INCLUDE
#define XLA_SERVICE_APPLICATION_INCLUDE

#include <Windows.h>
#include <XLA/Exception.hpp>
#include <XLA/Application/Application.hpp>
#include <XLA/Application/Log.hpp>

namespace XLA
{
	class ServiceApplication
		: public Application
		, public LogSupport
	{

	protected:
		enum EServiceControl
		{
			ControlStop,
			ControlShutdown,
			ControlRuning,
			ControlInitalize
		};

		String _serviceName;
		Bool   _modeDebug;
		SERVICE_STATUS_HANDLE _service;
		SERVICE_STATUS        _status;

		EServiceControl       _control;



		template< class ActorT >
		void SetActor( ActorT& actor )
		{
			actor.SetApplication( GetService() );
		}

		void ControlService( DWORD controlCode )
		{
			switch( controlCode )
			{
			case SERVICE_CONTROL_STOP:
				{
					SetStatus( SERVICE_STOP_PENDING );
					if( StopService() )
						SetStatus(SERVICE_STOPPED);
					else
						SetStatus(SERVICE_RUNNING);
				}
			case SERVICE_CONTROL_SHUTDOWN:
				{
					_control = ControlShutdown;
					ShutdownService();
					SetStatus(SERVICE_STOPPED);
				}
			default:
				break;
			}
		}

		static VOID WINAPI ServiceControl( DWORD controlCode )
		{
			ServiceApplication* svc = ServiceApplication::GetService();
			svc->ControlService(controlCode);
		}

		static DWORD WINAPI ServiceMain( DWORD argc , LPTSTR* argv )
		{
			ServiceApplication* svc = ServiceApplication::GetService();
			svc->_service = RegisterServiceCtrlHandler( svc->_serviceName.c_str() , (LPHANDLER_FUNCTION)ServiceControl );

			if( svc->_service == 0 )
			{
				svc->_status.dwWin32ExitCode = ::GetLastError();
				svc->SetStatus(SERVICE_STOPPED);
				return svc->_status.dwWin32ExitCode;
			}

			return svc->InitalizeAndRunService();
		}

		void Initalize()
		{
			_status.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
			_status.dwCheckPoint              = 0;
			_status.dwControlsAccepted        = 0;
			_status.dwCurrentState            = SERVICE_START_PENDING;
			_status.dwServiceSpecificExitCode = 0;
			_status.dwWaitHint                = 0;
			_status.dwWin32ExitCode           = 0;

			_control = ControlInitalize;

			Add( new DebugViewLogger() );
		}

		virtual Bool StartService()
		{
			return true;
		}

		virtual void RunService()
		{
		}

		virtual Bool StopService()
		{
			_control = ControlStop;
			return true;
		}

		virtual Bool ShutdownService()
		{
			_control = ControlShutdown;
			return true;
		}

		virtual Bool SetStatus( CUInt32& state )
		{
			DWORD svcState  = state;
			DWORD svcAccept = 0;
			switch( svcState )
			{
			case SERVICE_RUNNING:
				svcAccept = SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_STOP;
				_control  = ControlRuning;
				break;
			case SERVICE_STOP_PENDING:
				_control  = ControlStop;
			default:
				svcAccept = 0;
				break;
			}

			_status.dwCurrentState     = svcState;
			_status.dwControlsAccepted = svcAccept;

			if( _modeDebug )
				return true;
			return (SetServiceStatus( _service , & _status ) == TRUE );
		}

		Int32 InitalizeAndRunService()
		{

			Initalize();

			SetStatus( SERVICE_START_PENDING );
			
			if( !StartService() )
			{
				SetStatus(SERVICE_STOPPED);
				return _status.dwWin32ExitCode;
			}

			SetStatus(SERVICE_RUNNING);
			RunService();

			return _status.dwWin32ExitCode;
		}

	public:
		XLA_REGEDIT_CLASS( ServiceApplication );

		ServiceApplication( CString& name  )
			: _serviceName(name)
		{

		}

		static ServiceApplication* GetService()
		{
			ServiceApplication* svcApplication = (ServiceApplication*)GetApplication();
			return svcApplication;
		}

		virtual Int32 Run()
		{
			_modeDebug = _commandLine.Contain(_X("Debug"));

			if( _modeDebug )
			{
				return InitalizeAndRunService();
			}
			else
			{
				Array<Char> serviceName( _serviceName.c_str() , (UInt32)(_serviceName.size() + 1) );

				SERVICE_TABLE_ENTRY serviceTableEntry [] =
				{
					{ serviceName.GetData() , (LPSERVICE_MAIN_FUNCTION)ServiceMain },
					{ 0 , 0 }
				};

				if( ::StartServiceCtrlDispatcher( serviceTableEntry ) == TRUE )
					return 0;
				
				LogSupport::Add( new ConsoleLogger() );
				ExpThrow( SystemException() );
				
			}
		}

		String GetServiceName() const
		{
			return _serviceName;
		}

		virtual void ApplicationException( const Exception& e )
		{
			SetStatus(SERVICE_STOP_PENDING);
			WriteLog( e.ToString() );
			SetStatus(SERVICE_STOPPED);
		}
	};
}

int ApplicationMain( XLA::Int32 argc , XLA::Char* argv[] )
{
	setlocale(LC_ALL,"chs");
	return XLA::ServiceApplication::Main( argc , argv );
}





#endif