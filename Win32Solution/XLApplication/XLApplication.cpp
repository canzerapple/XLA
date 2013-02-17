// XLApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>


#include <XLA/Application/Service.h>
#include <XLA/Time.hpp>
#include <vector>

#include <string>
#include <iostream>

using namespace XLA;




class AutomitService
	: public Service
{
public:
	virtual void OnRunService( EServiceControl& serviceControl )
	{
		Time time = Time::Now();
		time  = time.GetDate();

		TimeDelta timeDelta( Days(2) + Hours(3) );

		time += timeDelta;

		String str = time.ToString();

		WriteLog(_X("%s,%s") , 1,2);


	}
};

REGEDIT_APPLICATION( AutomitService )
