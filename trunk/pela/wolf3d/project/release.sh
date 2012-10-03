ant release&& cd bin && jarsigner -verbose -keystore key DemoActivity-unsigned.apk xianle &&rm -rf temp.apk &&zipalign -v 4 DemoActivity-unsigned.apk temp.apk && adb install -r temp.apk
