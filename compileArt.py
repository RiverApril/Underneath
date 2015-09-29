#! /usr/bin/env python

import os
import glob

artPath = "art/"
srcPath = "src/"

filePaths = glob.glob(artPath+"*.txt")

artList = []

for filePath in filePaths:
    file = open(filePath, "r")
    artList.append([os.path.splitext(os.path.basename(filePath))[0], "".join(file.readlines())])
    file.close()

for art in artList:
    art[1] = art[1].replace("\\", "\\\\")
    art[1] = art[1].replace("\"", "\\\"")
    art[1] = art[1].replace("\n", "\",\"")
    art[1] = "{\""+art[1]+"\"}"

file = open(srcPath+"ArtFiles.hpp", "w")
file.write("\n")
file.write("//File automaticly generated via compileArt.py")
file.write("\n")
file.write("\n")
file.write("#ifndef __Underneath__ArtFiles__\n")
file.write("#define __Underneath__ArtFiles__\n")
file.write("\n")
file.write("#include <vector>\n")
file.write("#include <string>\n")
file.write("\n")
file.write("namespace ArtFiles {\n")
for art in artList:
    file.write("    std::vector<std::string> "+art[0]+" = "+art[1]+";\n")

file.write("}\n")
file.write("\n")
file.write("#endif /*defined(__Underneath__ArtFiles__)*/\n")
file.close();
