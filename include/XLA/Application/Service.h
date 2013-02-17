#ifndef XLA_SERVICE_INCLUDE
#define XLA_SERVICE_INCLUDE

#include <XLA/XML/LibXml.hpp>
#include <XLA/System/Time.hpp>
#include <XLA/Application/ServiceApplication.hpp>

namespace XLA
{
	class ServiceActor
		: public Object
	{
	protected:
		ServiceApplication* _service;
	public:
		ServiceActor()
		{
			_service = ServiceApplication::GetService();
		}

		void SetApplication( ServiceApplication* service )
		{
			_service = service;
		}
	};

	class ServiceMessage;

	class ServiceMessageProvide
	{
	public:
		virtual ServiceMessage* GetServiceMessage() = 0;
	};

	class ServiceMessage
	{
	protected:
		ServiceMessageProvide* _provider;
	public:
		virtual void MessageProcessed( Bool  ) = 0;
	};

	class ServiceMessageCollect
		: public ServiceActor
	{
		typedef std::list< ServiceMessageProvide* > MessageProvider;
		typedef MessageProvider::iterator           iterator;
		MessageProvider _provider;
		iterator        _current;
	public:
		ServiceMessageCollect()
		{
			_current = _provider.end();
		}

		ServiceMessage* GetServiceMessage()
		{
			if( _provider.empty() )
				return 0;

			if( _current == _provider.end() )
			{
				_current = _provider.begin();
			}

			ServiceMessage* message = (*_current)->GetServiceMessage();

			_current ++;

			return message;
		}
	};


	class ServiceSetting
		: public ServiceActor
	{
	public:
		FileInfo Session;
		FileInfo Logs;

		ServiceSetting()
		{
			Session = _X("Session");
			Logs    = _X("Logs");
		}

		void LoadFromFile( CString& name );
	};



	class Service
		: public ServiceApplication
	{
		ServiceSetting _setting;

	public:

		Service()
			: ServiceApplication( Service::StaticGetClassName() )
		{
		}

		virtual ServiceMessage* GetServiceMessage()
		{
			return 0;
		}

		virtual Bool StartService()
		{
			FileInfo localSetting =  _X("Setting.xml");
			_setting.LoadFromFile( localSetting );
			return true;
		}

		virtual void RunService()
		{

			while( _control == ControlRuning )
			{
				OnRunService( _control );
			}
		}

		virtual void OnRunService( EServiceControl& serviceContion )
		{
		}
	};

	void ServiceSetting::LoadFromFile( CString& name )
	{
		XmlDocument xmlDocument;

		xmlDocument.LoadFromFile( name );

		XmlNodeEnumator nodeEnumator = xmlDocument.GetEnumator();

		while( nodeEnumator.MoveNext() )
		{
			if( nodeEnumator.DataElement() )
			{
				String nodeName  = nodeEnumator.GetName();
				String nodeValue = nodeEnumator.GetValue();
				if( nodeName == _X("Session") )
				{
					Session =  nodeValue;
					continue;
				}
				if( nodeName == _X("Logs") )
				{
					Logs = nodeValue;
					continue;
				}
			}
		}
	}
}

#endif