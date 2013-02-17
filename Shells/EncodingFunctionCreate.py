class EncodingInfo( object ):
    def __init__( self , name , func , dis ):
        self.Name = name
        self.Func = func
        self.Display = dis
        pos = func.find("(")
        self.Encoding = func[6:pos]

def Main():
    fd = open("EncodingNames.txt")
    EncodingList = [];
    for line in fd:
        lineSplits   = line.split('\t')

        name = lineSplits[0].strip()
        func = lineSplits[1].strip()
        dis  = lineSplits[2].strip()
        EncodingList.append( EncodingInfo( name , func , dis ) )

    print("struct StaticEncodings")
    print("{")
    for info in EncodingList:
        print("    Encoding* _%s;" % info.Name )
    print("")
    print("    StaticEncodings()")
    print("    {")
    for info in EncodingList:
        print("        INIT_STATIC_ENCODING(_%s);" % info.Name )
    print("    }")
    print("};")

    print("")
    print("")
    for info in EncodingList:
        print("case Encoding::CodePage%s :" % info.Name )
        print("{")
        print("    if( staticEncoding._%s == 0 )" % info.Name )
        print("    {")
        print("        LockEncodings();")
        print("        if( staticEncoding._%s == 0 )" % info.Name)
        print("            staticEncoding._%s = Detail::%s;"%( info.Name,info.Func))
        print("        UnlockEncodings();")
        print("    }")
        print("    *encoding = staticEncoding._%s;" % info.Name ) 
        print("    break;")
        print("}")

    print("")
    print("")

    fmtString = '''\
encodingTable->AddEncoding(
                  EncodingInfo(
                  %s::StaticGetClassName(),
                  _X("%s"),
                  Encoding::CodePage%s ) );'''
    for info in EncodingList:
        print( fmtString % ( info.Encoding,info.Display , info.Name ) );
        print("")

    print("")
    print("")
    for info in EncodingList:
        fmtString = '''\
inline static Encoding* %s()
{
    return Encoding::GetEncoding( CodePage%s );
}
'''
        print( fmtString % ( info.Name , info.Name ) )
    

Main()
