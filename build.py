import argparse
import platform
import glob
import os, errno
import datetime
from subprocess import call


def modification_date(filename):
    t = os.path.getmtime(filename)
    return datetime.datetime.fromtimestamp(t)


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

args = parser.parse_args()



systemName = platform.system()
machine = platform.machine()

compiler = "g++"
compilerFlags = "-g -std=c++11"
libraryFlags = "-lncurses"

sourceDirectory = "src"
objectDirectory = "obj"

sourceExtention = "cpp"
headerExtention = "hpp"
objectExtention = "opp"

executableName = "Underneath"

compileAll = args.all


if systemName == "Linux":
    executableName += "_Linux_"+machine
elif systemName == "Darwin":
    executableName += "_OSX_"+machine
elif systemName == "Windows":
    executableName += "_Windows_"+machine+".exe"
else:
    executableName += "_"+systemName+"_"+machine



if args.sdl:
    executableName += "_SDL"
    objectDirectory += "/"+executableName
    compilerFlags += " -D useSDLLightCurses"
    libraryFlags = "-lSDL2 -lSDL2_image"
else:
    executableName += "_Standard"
    objectDirectory += "/"+executableName


cppList = glob.glob(sourceDirectory+"/*."+sourceExtention)
hppList = [a.replace("."+sourceExtention, "."+headerExtention) for a in cppList]
oppList = [a.replace(sourceDirectory+"/", objectDirectory+"/") for a in [a.replace("."+sourceExtention, "."+objectExtention) for a in cppList]]

executableModifiedDate = datetime.datetime.min

mkdir_p(objectDirectory)

if os.path.isfile(executableName):
    executableModifiedDate = modification_date(executableName)


print("Building: "+executableName);

for i in range(len(cppList)):
    sourceFileDate = modification_date(cppList[i])

    headerFileDate = datetime.datetime.min
    if os.path.isfile(hppList[i]):
        headerFileDate = modification_date(hppList[i])

    if executableModifiedDate<sourceFileDate or executableModifiedDate<headerFileDate or compileAll:
        print("+ Compiling: "+cppList[i])
        call([compiler]+compilerFlags.split(" ")+["-c", cppList[i], "-o", oppList[i]])
    else:
        print("   Skipping: "+cppList[i])


print("Linking: "+executableName);
call([compiler]+compilerFlags.split(" ")+oppList+["-o", executableName]+libraryFlags.split(" "))
