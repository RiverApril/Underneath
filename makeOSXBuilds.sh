./build.py
./build.py -s
./makeApp.sh Underneath_SDLgfx_OSX

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/OSX/"

mkdir -p $BUILDSDIR

zip -r Underneath.Terminal.OSX.zip Underneath_OSX LICENCE.txt
zip -r Underneath.SDL.OSX.zip Underneath_SDLgfx_OSX.app LICENCE.txt

cp Underneath.Terminal.OSX.zip $BUILDSDIR
cp Underneath.SDL.OSX.zip $BUILDSDIR
