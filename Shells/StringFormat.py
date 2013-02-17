'''
String StringFormat( CString& format ,'''


def MakeTemplate( count ):
    temp  = ""
    for idx in range( 0 , count  ):
        temp += ("class T%s ," % idx)
    return temp[0:len(temp)-1]

def MakeArgument( count ):
    temp = ""
    for idx in range( 0 , count ):
        temp += ("const T%s& a%s ," %( idx , idx ) )
    return temp[0:len(temp)-1]

def MakeList( count ):
    temp = ""
    for idx in range( 0 ,count ):
        temp += ("    StringFormatArgument( cdValue , format , a%s );\n" %idx )
    return temp;

def MakeCall( count ):
    temp = ""
    for idx in range( 0 , count ):
         temp += ("a%s ," % idx )
    return temp[0:len(temp)-1]

for ix in range(0,11):
    item = '''\
template< %s >
inline String StringFormat( CString& fmt , %s )
{
    CChar* format = fmt.c_str();
    String cdValue;
%s
    cdValue.append( format );
    return cdValue;
}
''' % ( MakeTemplate(ix) , MakeArgument(ix ) , MakeList( ix ) ) 
    print( item )


def Lc(tp):
    st = '''\
inline Bool IsStringFormatType( const %s & cdValue ) { return true; };''' % tp
    print(st)

def LcU( tp ):
    Lc(tp)
    Lc("U" + tp )

LcU("Int16")
LcU("Int32")
LcU("Int64")
Lc( "Float32" )
Lc( "Float64" )
Lc( "Char" )
Lc( "Byte" )

for ix in range(0,11):
    tmp = '''
template< %s >
void WriteLog( CString& fmt , %s )
{ WriteLog( StringFormat( fmt , %s) ); } ''' % (MakeTemplate(ix) , MakeArgument(ix) , MakeCall(ix) )
    print(tmp)
