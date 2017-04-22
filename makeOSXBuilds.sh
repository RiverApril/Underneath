./build.py
./build.py -s
./makeApp.sh Underneath_SDLgfx_OSX

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/OSX/"

mkdir -p $BUILDSDIR

zip -r Underneath_OSX.zip Underneath_OSX LICENCE.txt
zip -r Underneath_SDLgfx_OSX.zip Underneath_SDLgfx_OSX.app LICENCE.txt

cp Underneath_OSX.zip $BUILDSDIR
cp Underneath_SDLgfx_OSX.zip $BUILDSDIR
