#! /usr/bin/env python

import argparse
import platform
import glob
import os
import errno
import shutil
import filecmp
#import datetime
from subprocess import call


#def modification_date(filename):
#    t = os.path.getmtime(filename)
#    return datetime.datetime.fromtimestamp(t)


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise


parser = argparse.ArgumentParser(description="Compile Underneath.")
parser.add_argument("-s", "--sdl", action="store_true")
parser.add_argument("-a", "--all", action="store_true")
parser.add_argument("-l", "--linkonly", action="store_true")
parser.add_argument("-r", "--run", action="store_true")
parser.add_argument("-e", "--release", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("-t", "--notart", action="store_true")
parser.add_argument("-w", "--windows", action="store_true")

args = parser.parse_args()


if not args.notart:
    print("    + Making ArtFiles.hpp")
    import compileArt

systemName = platform.system()
machine = platform.machine()


sourceDirectory = "src"
objectDirectory = "obj"
copyDirectory = "cpy"

sourceExtention = "cpp"
headerExtention = "hpp"
objectExtention = "opp"


compiler = "g++"
compilerFlags = "-std=c++11"
libraryFlags = "-lncurses"

optimization = "-g -O0"

executableName = "Underneath"

compileAll = args.all

if args.release:
    optimization = "-O3"
    executableName += "_Release"


if systemName == "Windows" or args.windows:
    compiler = "i686-w64-mingw32-c++"
    compilerFlags += " -D WIN32"


compilerFlags = optimization+" "+compilerFlags;

if args.sdl:
    executableName += "_SDL"
    compilerFlags += " -D useSDLLightCurses"

    if systemName == "Windows" or args.windows:
        print("    ! Compiling on Windows with SDL has not been implemented.")
    elif systemName == "Darwin":
        compilerFlags += " -I/usr/local/include"
        libraryFlags = "-L/usr/local/lib `sdl2-config --cflags` -lSDL2_image"
        if args.release:
            libraryFlags += " `sdl2-config --static-libs`"
        else:
            libraryFlags += " `sdl2-config --libs`"
    else:
        compilerFlags += " `sdl2-config --cflags`"
        if args.release:
            libraryFlags = " `sdl2-config --static-libs` -lSDL2_image"
        else:
            libraryFlags  = " `sdl2-config --libs` -lSDL2_image"

else:
    executableName += "_Term"
    if systemName == "Windows" or args.windows:
        libraryFlags = "-lpdcurses -static -static-libgcc -static-libstdc++"
    elif systemName == "Darwin":
        libraryFlags = "-lncurses"
    else:
        libraryFlags = "-lncurses"


if systemName == "Darwin":
    executableName += "_OSX_"+machine
elif systemName == "Windows" or args.windows:
    executableName += "_Windows_"+machine+".exe"
else:
    executableName += "_"+systemName+"_"+machine


objectDirectory += "/"+executableName
copyDirectory = objectDirectory+copyDirectory

cppList = [a.replace("\\", "/") for a in sorted(glob.glob(sourceDirectory+"/*."+sourceExtention))]
hppList = [a.replace("."+sourceExtention, "."+headerExtention) for a in cppList]
oppList = [a.replace(sourceDirectory+"/", objectDirectory+"/") for a in [a.replace("."+sourceExtention, "."+objectExtention) for a in cppList]]
cppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in cppList]
hppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in hppList]


mkdir_p(objectDirectory)
mkdir_p(copyDirectory)

returnCode = 0

if os.path.isfile(executableName):
    print("    * Renaming old executable to: "+executableName+"_outdated")
    if os.path.isfile(executableName+"_outdated"):
        os.remove(executableName+"_outdated")
    os.rename(executableName, executableName+"_outdated")

skipCount = 0

if not args.linkonly:
    print("    # Building: "+executableName);

    for i in range(len(cppList)):

        cppDiff = True
        hppDiff = True

        hppExists = os.path.isfile(hppList[i])

        if os.path.isfile(cppCopyList[i]):
            cppDiff = not filecmp.cmp(cppList[i], cppCopyList[i])

        if hppExists:
            if os.path.isfile(hppCopyList[i]):
                hppDiff = not filecmp.cmp(hppList[i], hppCopyList[i])
        else:
            hppDiff = False


        if compileAll or cppDiff or hppDiff:
            print("    + Compiling: "+cppList[i])
            command = compiler+" "+compilerFlags+" -c "+cppList[i]+" -o "+oppList[i]
            if args.debug:
                print("    . Executing: \""+command+"\"")
            returnCode = call(command, shell=True)
            if returnCode == 0:
                shutil.copy(cppList[i], cppCopyList[i])
                if hppExists:
                    shutil.copy(hppList[i], hppCopyList[i])
            else:
                break
        else:
            skipCount += 1


print("    . Skipped "+str(skipCount)+" compilations.")

if returnCode == 0:
    print("    ~ Linking: "+executableName);
    command = compiler+" "+compilerFlags+" "+(" ".join(oppList))+" -o "+ executableName+" "+libraryFlags
    if args.debug:
        print("    . Executing: \""+command+"\"")
    returnCode = call(command, shell=True)

    if returnCode == 0:
        if os.path.isfile(executableName+"_outdated") and os.path.isfile(executableName):
            print("    - Deleteing outdated executable: "+executableName+"_outdated")
            os.remove(executableName+"_outdated")
        if args.run and returnCode == 0:
            print("    > Running...")
            if systemName == "Windows":
                os.system(executableName)
            else:
                os.system("./"+executableName)
    else:
        print("    x Failed to link, Error Code: "+str(returnCode))

else:
    print("    x Failed to compile, Error Code: "+str(returnCode))
