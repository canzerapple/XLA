import os
import sys
import shutil



def Main():
    suffix = ["ncb","user","obj","suo","ilk","exp","idb","dep","tfignore","sdf","log","tlog","lastbuildstate","htm"]
    dirName = os.path.dirname( sys.argv[0] )
    dirName = os.path.dirname( dirName )
    suoName = dirName
    dirName = os.walk( dirName )

    fileNames = []
    for obj in dirName:
        dirName      = obj[0]
        fileNameList = obj[2]
        for fileName in fileNameList:
            fileNames.append( dirName + "\\" + fileName )

    for fileName in fileNames:
        NameSplit = fileName.split('''.''')
        ExtName   = ""

        if len( NameSplit ) > 0 :
            ExtName   = NameSplit[ len( NameSplit ) - 1 ]
            
        #if ExtName == "pdb":
        #    save = suoName + "\\PDB\\" + os.path.basename( fileName )
        #    shutil.move( fileName , save )
         #   print( save )
         #   continue
        
        if ExtName in suffix :
            print( fileName )
            os.remove( fileName )
            

Main()
