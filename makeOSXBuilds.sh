./build.py
./build.py -s
./makeApp.sh Underneath_SDLgfx_OSX

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/OSX/"

mkdir -p $BUILDSDIR

zip -r Underneath_SDLgfx_OSX.zip Underneath_SDLgfx_OSX.app

cp Underneath_OSX $BUILDSDIR
cp Underneath_SDLgfx_OSX.zip $BUILDSDIR
