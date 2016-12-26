#! /usr/bin/env python

import argparse
import platform
import glob
import os
import errno
import shutil
import filecmp
import sys
import subprocess
from subprocess import call

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise


parser = argparse.ArgumentParser(description="Compile Underneath.")
parser.add_argument("-s", "--SDLGraphics", action="store_true")
parser.add_argument("-m", "--SDLAudio", action="store_true")
parser.add_argument("-a", "--all", action="store_true")
parser.add_argument("-l", "--linkonly", action="store_true")
parser.add_argument("-r", "--run", action="store_true")
parser.add_argument("-u", "--unoptimized", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("-t", "--notart", action="store_true")
parser.add_argument("-w", "--windows", action="store_true")
parser.add_argument("-i", "--dontCheckIncludes", action="store_true")
parser.add_argument("--use64", action="store_true")
parser.add_argument("--use32", action="store_true")
parser.add_argument("--compiler", help="specify compiler")
parser.add_argument("--app", action="store_true")

args = parser.parse_args()


if not args.notart:
    print("    + Making ArtFiles.hpp")
    import compileArt

systemName = platform.system()
system64Bit = sys.maxsize > 2**32
build64Bit = False
build32Bit = False

if system64Bit:
    build64Bit = True

if args.use64 and args.use32:
    print "Cannot use --use64 and --use32 at the same time."
    exit()

if args.use64:
    build64Bit = True

if args.use32:
    build32Bit = True

if systemName=="Windows":
    if args.windows:
        print "Already in windows, no need to use the -w flag."
    args.windows = True


sourceDirectory = "src"
objectDirectory = "obj"
copyDirectory = "cpy"

sourceExtention = "cpp"
headerExtention = "hpp"
objectExtention = "opp"


compiler = "g++"
compilerFlags = ""
libraryFlags = ""

optimization = ""

executableName = "Underneath"

compileAll = args.all

if args.windows:
    compilerFlags += "-std=gnu++11"
elif systemName == "Darwin":
    compilerFlags += "-stdlib=libc++ -std=gnu++11"
else:
    compilerFlags += "-std=gnu++11"

if args.unoptimized:
    executableName += "_Unoptim"
    optimization = "-g -O0 -Wall"
else:
    compilerFlags += " -D OPTIMIZED"
    optimization = "-Os"

compilerFlags = optimization+" "+compilerFlags;


if args.compiler:
    compiler = args.compiler;

if args.SDLGraphics:
    executableName += "_SDLgfx"
    compilerFlags += " -D useSDLGraphics"

if args.SDLAudio:
    executableName += "_SDLaud"
    compilerFlags += " -D useSDLAudio"

if args.windows:

    if systemName == "Darwin":
        compiler = "i586-mingw32-c++"
        compilerFlags += " -D WIN32"
    elif systemName == "Linux":
        compiler = "i686-w64-mingw32-c++"
        compilerFlags += " -D WIN32"

    print("    # The Windows executable will require the following dll files:")

    if args.SDLGraphics:
        libraryFlags += " -lSDL2main -lSDL2 -lSDL2_image"
        print("    #   SDL2.dll")
        print("    #   SDL2_image.dll")
        print("    #   zlib1.dll")
        print("    #   libpng16-16.dll")
    else:
        libraryFlags += " -lpdcurses"
        print("    #   pdcurses.dll")

    if args.SDLAudio:
        libraryFlags += " -lSDL2main  -lSDL2 -lSDL2_mixer"
        print("    #   SDL2_mixer.dll")
        print("    #   libogg-0.dll")

    libraryFlags += " -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic"


else:

    if systemName == "Darwin":
        compilerFlags += " -mmacosx-version-min=10.7"

    if args.SDLGraphics:
        if systemName == "Darwin":
            compilerFlags += " -I/usr/local/include"
            libraryFlags += " -L/usr/local/lib `sdl2-config --cflags` -lSDL2_image"
            libraryFlags += " `sdl2-config --libs`"
        elif systemName == "Linux":
            compilerFlags += " `sdl2-config --cflags`"
            libraryFlags  += " `sdl2-config --libs` -lSDL2_image"

    else:
        if systemName == "Darwin":
            libraryFlags += " -lncurses"
        elif systemName == "Linux":
            libraryFlags += " -lncurses"
        elif systemName == "Windows":
            libraryFlags += " -lpdcurses"

    if args.SDLAudio:
        if systemName == "Darwin":
            compilerFlags += " -stdlib=libc++"
            compilerFlags += " -I/usr/local/include"
            libraryFlags += " -L/usr/local/lib `sdl2-config --cflags` -lSDL2_mixer"
            libraryFlags += " `sdl2-config --libs`"
        elif systemName == "Linux":
            compilerFlags += " -stdlib=libc++"
            compilerFlags += " `sdl2-config --cflags`"
            libraryFlags  += " `sdl2-config --libs` -lSDL2_mixer"


if args.windows:
    if build64Bit:
        executableName += "_Windows_64.exe"
        compilerFlags += "  -m64"
    elif build32Bit:
        executableName += "_Windows_32.exe"
        compilerFlags += "  -m32"
    else:
        executableName += "_Windows.exe"
elif systemName == "Darwin":
    executableName += "_OSX"
    #OSX 32 bit is obsolete
else:
    if build64Bit:
        executableName += "_"+systemName+"_64"
        compilerFlags += "  -m64"
    elif build32Bit:
        executableName += "_"+systemName+"_32"
        compilerFlags += "  -m32"
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
        if args.app:
            print "    ~ Making App: "+executableName+".app";
            call("./makeApp.sh "+executableName, shell=True)
        if args.run:
            print("    > Running...")
            if systemName == "Windows":
                os.system(executableName)
            else:
                os.system("./"+executableName)
    else:
        print("    x Failed to link, Error Code: "+str(returnCode))

else:
    print("    x Failed to compile, Error Code: "+str(returnCode))
