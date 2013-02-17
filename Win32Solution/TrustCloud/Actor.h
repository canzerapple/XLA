#ifndef ACTOR_INCLUDE
#define ACTOR_INCLUDE



#include <XLA/Encoding/UTF7Encoding.hpp>
#include <XLA/IO/StreamReader.hpp>
#include <XLA/io/MemoryStream.hpp>
#include <XLA/XML/XmlReader.hpp>
#include <iostream>
#include <map>

using namespace XLA;

#define DAY(d) (1<<d)

#define WEEKDAY(d,x) (1<<(d*7+x))

#define WEEK(w) ((DAY(0)|DAY(1)|DAY(2)|DAY(3)|DAY(4)|DAY(5)|DAY(6))<<(7*w))

#define MONTH (WEEK(0)|WEEK(1)|WEEK(2)|WEEK(3))


XLA_STATIC_CONST( UInt32 , MAXMASK = 32 );


class MarkTable
	: public Object
{
	Bool _data[MAXMASK];
public:
	XLA_REGEDIT_CLASS( MarkTable );

	MarkTable( const Bool (&data)[MAXMASK] )
	{
		for( UInt32 idx = 0 ; idx < MAXMASK ; ++idx )
			_data[idx] = data[idx];
	}

	MarkTable()
	{
		for(UInt32 idx = 0 ; idx < MAXMASK ; ++ idx )
			_data[idx] = false;
	}

	Bool& operator[]( UInt32 index )
	{
		return _data[index];
	}

	MarkTable GetTable( CUInt32& mask ) const
	{
		MarkTable table;

		for(UInt32 idx = 0 ; idx < 32 ; ++idx)
		{
			if( mask & (1<<idx) )
				table[idx] = _data[idx];
		}

		return table;
	}

	MarkTable GetTable() const
	{
		return _data;
	}

	UInt32 GetCount( CUInt32& mask , CBool& status ) const
	{
		UInt32 matchCount = 0;

		for(UInt32 idx = 0 ; idx < 32 ; ++idx)
		{
			if( mask & (1<<idx) && _data[idx] == status )
				matchCount++;
		}

		return matchCount;
	}

	UInt32 GetCount( CBool& status ) const
	{
		UInt32 matchCount = 0;
		for( UInt32 idx = 0 ; idx < 32 ; ++idx )
		{
			if( _data[idx] == status )
				matchCount ++;
		}

		return matchCount;
	}

	virtual String ToString() const
	{
		String markString;

		markString.reserve(32);

		for( UInt32 idx = 0 ; idx < 32 ; ++idx )
		{
			if( _data[idx] == true )
				markString.append(1,_X('1'));
			else
				markString.append(1,_X('0'));
		}

		return markString;
	}
};

class Actor 
	: public Object
{
	String    _guid;
	MarkTable _online;
	MarkTable _entracne;

public:
	XLA_REGEDIT_CLASS( Actor );

	void Initalize()
	{

	}

	Actor()
	{
		Initalize();
	}

	Actor( CString& guid )
		: _guid( guid )
	{
		Initalize();
	}

	void SetEntrance( UInt32 mask )
	{
		for(UInt32 idx = 0 ; idx<32;++idx)
		{
			if( mask & (1<<(idx)) )
				_entracne[idx] = true;
		}
	}

	void SetOnline( UInt32 mask )
	{
		for( UInt32 idx = 0 ; idx < 32 ; ++idx )
		{
			if( mask & (1<<idx) )
				_online[idx] = true;
		}
	}

	MarkTable GetOnline() const
	{
		return _online;
	}

	MarkTable GetOnline( CUInt32& mark )
	{
		return _online.GetTable( mark );
	}

	MarkTable GetEntrance() const
	{
		return _entracne;
	}

	MarkTable GetEntrance( CUInt32& mark )
	{
		return _entracne.GetTable( mark );
	}

	String GetGuid() const
	{
		return _guid;
	}

	virtual String ToString() const
	{
		String actorString = _guid;

		actorString = actorString + _X("[") + _entracne.ToString() + _X("][") + _online.ToString() + _X("]");

		return actorString;
	}
};

typedef const Actor CActor;

Bool operator==( CActor& actor , CString& guid )
{
	return Equal(actor.GetGuid(),guid,EqualIgnoreCase);
}

Bool operator==( CActor& actor , CActor& cmp )
{
	return actor.GetGuid() == cmp.GetGuid();
}

Bool operator<( CActor& actor , CActor& cmp )
{
	return actor.GetGuid() < cmp.GetGuid();
}

class ActorEnumator
	: public Object
{
	typedef std::map< String , Actor >::const_iterator iterator;

	Actor    _current;
	iterator _token;
	iterator _end;
public:
	XLA_REGEDIT_CLASS( ActorEnumator );

	Bool MoveNext()
	{
		if(_token == _end)
			return false;
		_current = (_token++)->second;
		return true;
	}

	Actor GetCurrent() const
	{
		return _current;
	}

	ActorEnumator( iterator token , iterator end )
		: _token( token )
		, _end(end)
	{

	}

	virtual String ToString() const
	{
		return _current.ToString();
	}
};

class ActorCollection
{
	typedef std::map< String , Actor > ActorIndex;
	typedef std::vector< String > StringArray;

	ActorIndex _index;
public:
	Actor GetActor( CString& guid )
	{
		ActorIndex::iterator token = _index.find(guid);
		if( token == _index.end() )
		{
			_index[guid] = Actor(guid);
			return Actor(guid);
		}

		return token->second;
	}

	void SetActor( CActor& actor )
	{
		_index[actor.GetGuid()] = actor;
	}

	void SetEntracne( CString& guid , UInt32& mask )
	{
		Actor actor = GetActor( guid );

		actor.SetEntrance( mask );

		SetActor( actor );
	}

	void SetOnline( CString& guid , UInt32& mask )
	{
		Actor actor = GetActor( guid );

		actor.SetOnline( mask );

		SetActor( actor );
	}

	void LoadEntrance( CString& fileName )
	{
		StreamReader entranceReader(fileName);
		String       entranceLine;
		StringArray  lineArray;

		UInt32 cnt = 0;

		while( !entranceReader.EndOfStream() )
		{
			lineArray.clear();

			entranceReader.ReadLine( entranceLine );
			Split( entranceLine , _X('\t') , lineArray );

			if(entranceLine.empty())
				continue;

			String guid     = ToUpper(lineArray[0]);
			UInt32 entracne = 0;

			for( UInt32 idx = 5 ; idx <= 8 ; ++idx )
			{
				UInt32 entranceMask = ToUInt32( lineArray[idx] );

				entracne |= entranceMask;
			}

			SetEntracne( guid , entracne );

		}
	}

	void LoadOnline( CString& fileName )
	{
		StreamReader onlineReader( fileName );
		String       onlineLine;
		StringArray  lineArray;

		while( !onlineReader.EndOfStream() )
		{
			lineArray.clear();
			onlineReader.ReadLine( onlineLine );

			if(onlineLine.empty())
				continue;

			Split(onlineLine,_X('\t'),lineArray);

			String guid = ToUpper(lineArray[0]);

			if( !Contain(guid) )
				continue;

			UInt32 online = 0;

			UInt32 lineSize = static_cast<UInt32>(lineArray.size());
			for( UInt32 idx = 1 ; idx < lineSize ; ++ idx )
			{
				if( lineArray[idx] == _X("1") )
					online |= (1<<(idx-1));
			}

			SetOnline( guid , online );
		}

	}

	void LoadData( CString& entrance , CString& online )
	{
		LoadEntrance( entrance );
		LoadOnline( online );
	}

	UInt32 GetSize() const
	{
		return static_cast<UInt32>(_index.size());
	}

	Bool Contain( CString& guid ) const
	{
		ActorIndex::const_iterator token = _index.find( guid );
		if( token == _index.end() )
			return false;
		return true;
	}

	void Add( const Actor& actor )
	{
		SetActor(actor);
	}

	ActorEnumator GetEnumator() const
	{
		return ActorEnumator( _index.begin() , _index.end() );
	}
};

#endif