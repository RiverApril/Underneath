./build.py
./build.py -s
./makeApp.sh Underneath_SDLgfx_macOS

DATE=`date +%Y-%m-%d`
BUILDSDIR="builds/$DATE/macOS/"

mkdir -p $BUILDSDIR

zip -r Underneath.Terminal.macOS.zip Underneath_macOS LICENCE.txt
zip -r Underneath.SDL.macOS.zip Underneath_SDLgfx_macOS.app LICENCE.txt

cp Underneath.Terminal.macOS.zip $BUILDSDIR
cp Underneath.SDL.macOS.zip $BUILDSDIR
