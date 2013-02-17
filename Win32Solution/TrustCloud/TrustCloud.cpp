#include <stdio.h>
#include "Actor.h"



int main(int argc, Char* argv[])
{
	try
	{
		setlocale(LC_ALL,"chs");

		ActorCollection actors;

		String entrance = _X("C:\\DDA\\Entrance.txt");
		String online   = _X("C:\\DDA\\Online.txt");
		actors.LoadData(entrance,online);

		ActorEnumator actorEnumator = actors.GetEnumator();

		while( actorEnumator.MoveNext() )
		{
			Actor actor = actorEnumator.GetCurrent();
			
			MarkTable online   = actor.GetOnline();
			MarkTable entrance = actor.GetEntrance();

			MarkTable onlineWeeks   = online.GetTable(WEEK(0)|WEEK(1)|WEEK(2));
			MarkTable entranceWeeks = entrance.GetTable(WEEK(0)|WEEK(1)|WEEK(2));

			MarkTable onlineLast    = online.GetTable(WEEK(3));
			MarkTable entracneLast  = entrance.GetTable(WEEK(3));

			std::wcout<<actor.GetGuid() <<_X("\t")
					  <<onlineWeeks.GetCount(true)<<_X("\t")
					  <<onlineLast.GetCount(true)<<_X("\t")
					  <<entranceWeeks.GetCount(true)<<_X("\t")
					  <<entracneLast.GetCount(true)<<std::endl;
		}

	}
	catch (Exception& e)
	{
		std::wcout<<e.GetClassName() <<_X(" ")<<e.ToString();
	}


	return 0;
}
