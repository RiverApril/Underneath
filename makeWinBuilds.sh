./i686-w64-mingw32-windres icon.rc icon.o

./build.py -w --use32
./build.py -ws --use32

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/Win32/"

mkdir -p $BUILDSDIR

zip Underneath_Windows_32.zip Underneath_Windows_32.exe pdcurses.dll
zip Underneath_SDLgfx_Windows_32.zip Underneath_SDLgfx_Windows_32.exe zlib1.dll libpng16-16.dll SDL2.dll SDL2_image.dll font.png

cp Underneath_Windows_32.zip $BUILDSDIR
cp Underneath_SDLgfx_Windows_32.zip $BUILDSDIR
