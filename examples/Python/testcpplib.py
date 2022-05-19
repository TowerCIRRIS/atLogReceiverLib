#test 1
import os
import ctypes
from ctypes import cdll, windll
from ctypes import c_char_p
from ctypes import byref as _byref


# load the library
windll.LoadLibrary("C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32\\lib\\libstdc++-6.dll")
lib = windll.LoadLibrary("C:\\python_workspace\\testLibcpp\\AtlogReceiver.dll")

#dllFile = ctypes.CDLL("C:\\python_workspace\\testLibcpp\\AtlogReceiver.dll")
print(lib)

#lib.getFileName.argtypes(c_int,c_char_p)


value = lib.openComm(5, 460800)

print("Resultat:" + str(value))
if (value):  
    fileCount = lib.refreshFileList()
    print("\n\rPY:Nombre de fichiers: " + str(fileCount))

    fileNameString = c_char_p(b"")

    for i in range(0, fileCount):

        #fileNameString = c_char_p(None)
        lib.getFileName(i, fileNameString)
        lib.downloadDataByName(fileNameString)

        print("Nom fichier:" +  "".join(map(chr, fileNameString.value )))

    lib.endComm()
else:
    print("\n\rPY:Open comm error")
print("\n\rPY:test terminé")
