i686-w64-mingw32-windres icon.rc icon.o

./build.py -w --use32
./build.py -ws --use32

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/Win32/"

mkdir -p $BUILDSDIR

zip Underneath.Terminal.Windows32.zip Underneath_Windows_32.exe pdcurses.dll LICENCE.txt
zip Underneath.SDL.Windows32.zip Underneath_SDLgfx_Windows_32.exe zlib1.dll libpng16-16.dll SDL2.dll SDL2_image.dll font.png LICENCE.txt

cp Underneath.Terminal.Windows32.zip $BUILDSDIR
cp Underneath.SDL.Windows32.zip $BUILDSDIR
