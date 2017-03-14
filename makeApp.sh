#!/bin/bash
rm -rf $@.app
mkdir -p $@.app
mkdir -p $@.app/Contents
mkdir -p $@.app/Contents/lib
mkdir -p $@.app/Contents/MacOS
mkdir -p $@.app/Contents/Resources
cp $@ $@.app/Contents/MacOS/
cp font.png $@.app/Contents/Resources/
cp icon.icns $@.app/Contents/Resources/
chmod a+x $@.app/Contents/MacOS/$@
otool -L $@ | awk '{if(NR>1)print}' | while read -r line ; do
    file=${line%%'('*}
    cp $file ./$@.app/Contents/lib/
    install_name_tool -change $file ../lib/$file $@.app/Contents/MacOS/$@
done

text="
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
  <key>CFBundleName</key>
  <string>$@</string>
  <key>CFBundleIconFile</key>
  <string>icon.icns</string>
  <key>CFBundleExecutable</key>
  <string>$@</string>
  <key>WorkingDirectory</key>
  <string>\$APP_PACKAGE/Resources</string>
</dict>
</plist>
"


printf "$text" > $@.app/Contents/Info.plist