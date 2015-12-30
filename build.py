#! /usr/bin/env python

import argparse
import platform
import glob
import os
import errno
import shutil
import filecmp
#import datetime
import subprocess
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
parser.add_argument("-o", "--optimize", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("-t", "--notart", action="store_true")
parser.add_argument("-w", "--windows", action="store_true")
parser.add_argument("-i", "--dontCheckIncludes", action="store_true")
parser.add_argument("--compiler", help="specify compiler")

args = parser.parse_args()


if not args.notart:
    print("    + Making ArtFiles.hpp")
    import compileArt

systemName = platform.system()


sourceDirectory = "src"
objectDirectory = "obj"
copyDirectory = "cpy"

sourceExtention = "cpp"
headerExtention = "hpp"
objectExtention = "opp"


compiler = "g++"
compilerFlags = "-std=gnu++11"
libraryFlags = ""

optimization = "-g -O0 -Wall"

executableName = "Underneath"

compileAll = args.all


if args.optimize:
    optimization = "-O3"
    executableName += "_Optim"

compilerFlags = optimization+" "+compilerFlags;



if args.compiler:
    compiler = args.compiler;

if args.sdl:
    executableName += "_SDL"
    compilerFlags += " -D useSDLLightCurses"
else:
    executableName += "_Term"


if args.windows:

    if systemName == "Darwin":
        compiler = "i586-mingw32-c++"
        compilerFlags += " -D WIN32"
        if args.sdl:
            libraryFlags += ""
    elif systemName == "Linux":
        compiler = "i686-w64-mingw32-c++"
        compilerFlags += " -D WIN32"

    if args.sdl:
        libraryFlags = "-lSDL2main -lSDL2 -lSDL2_image"
        print("    # The Windows executable will require the following dll files:")
        print("    #   SDL2.dll")
        print("    #   SDL2_image.dll")
        print("    #   zlib1.dll")
        print("    #   libpng16-16.dll")
    else:
        libraryFlags = "-lpdcurses -static -static-libgcc -static-libstdc++"
        print("    # The Windows executable will require the following dll file:")
        print("    #   pdcurses.dll")
else:
    if args.sdl:
        if systemName == "Darwin":
            compilerFlags += " -I/usr/local/include"
            libraryFlags = "-L/usr/local/lib `sdl2-config --cflags` -lSDL2_image"
            libraryFlags += " `sdl2-config --libs`"
        elif systemName == "Linux":
            compilerFlags += " `sdl2-config --cflags`"
            libraryFlags  = " `sdl2-config --libs` -lSDL2_image"

    else:
        if systemName == "Darwin":
            libraryFlags = "-lncurses"
        elif systemName == "Linux":
            libraryFlags = "-lncurses"

if args.windows:
    executableName += "_Windows.exe"
elif systemName == "Darwin":
    executableName += "_OSX"
else:
    executableName += "_"+systemName




objectDirectory += "/"+executableName
copyDirectory = objectDirectory+copyDirectory

cppList = [a.replace("\\", "/") for a in sorted(glob.glob(sourceDirectory+"/*."+sourceExtention))]
hppList = [a.replace("."+sourceExtention, "."+headerExtention) for a in cppList]
oppList = [a.replace(sourceDirectory+"/", objectDirectory+"/") for a in [a.replace("."+sourceExtention, "."+objectExtention) for a in cppList]]
cppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in cppList]
hppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in hppList]
hppDiffList = [False] * len(hppList)

mkdir_p(objectDirectory)
mkdir_p(copyDirectory)

returnCode = 0

if os.path.isfile(executableName):
    print("    * Renaming old executable to: "+executableName+"_outdated")
    if os.path.isfile(executableName+"_outdated"):
        os.remove(executableName+"_outdated")
    os.rename(executableName, executableName+"_outdated")

skipCount = 0

print("    # Using compiler: "+compiler)

if not args.linkonly:
    print("    # Building: "+executableName);

    headersChanged = False

    if not compileAll and not args.dontCheckIncludes:

        for i in range(len(hppList)):

            hppExists = os.path.isfile(hppList[i])

            if hppExists:
                if os.path.isfile(hppCopyList[i]):
                    hppDiffList[i] = not filecmp.cmp(hppList[i], hppCopyList[i])
                    if hppDiffList[i]:
                        headersChanged = True
            else:
                hppDiffList[i] = False


            if hppExists:
                shutil.copy(hppList[i], hppCopyList[i])


        #print hppDiffList


    for i in range(len(cppList)):



        cppDiff = True
        hppDiff = True
        otherHppDiff = False

        hppExists = os.path.isfile(hppList[i])

        if not compileAll:

            if os.path.isfile(cppCopyList[i]):
                cppDiff = not filecmp.cmp(cppList[i], cppCopyList[i])

            if hppExists:
                if os.path.isfile(hppCopyList[i]):
                    hppDiff = not filecmp.cmp(hppList[i], hppCopyList[i])
            else:
                hppDiff = False

            if not args.dontCheckIncludes and headersChanged:

                command = compiler+" -MM "+compilerFlags+" -c "+cppList[i]
                if args.debug:
                    print("    . Executing: \""+command+"\"")
                proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
                includeList = [x.strip() for x in proc.stdout.read().split(" ")]
                includeList = includeList[2:]
                includeList = [x for x in includeList if x != "" and x != "\\"]
                #print(includeList)
                for inc in includeList:
                    if inc in hppList:
                        if hppDiffList[hppList.index(inc)]:
                            #print inc
                            #print hppDiffList.index(inc)
                            #print hppDiffList[includeList.index(inc)]
                            otherHppDiff = True
                            break


        if compileAll or cppDiff or hppDiff or otherHppDiff:
            print("    + Compiling: "+cppList[i])
            command = compiler+" "+compilerFlags+" -c "+cppList[i]+" -o "+oppList[i]
            if args.debug:
                print("    . Executing: \""+command+"\"")
            returnCode = call(command, shell=True)
            if returnCode == 0:
                shutil.copy(cppList[i], cppCopyList[i])
            else:
                break
        else:
            skipCount += 1

if args.linkonly:
    print("    . Skipped all compilations.")
else:
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
