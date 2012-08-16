#!/bin/bash

# Set here your own NDK path if needed
# export PATH=$PATH:~/src/endless_space/android-ndk-r4b
# Set environment to CrystaX NDK with RTTI and exceptions instead of original NDK
# export PATH=$PATH:~/src/endless_space/android-ndk-r4-crystax/ndk-build

ant release&& cd bin && jarsigner -verbose -keystore cndroidkey  CoverActivity-release-unsigned.apk cndroid &&rm -rf temp.apk &&zipalign -v 4  CoverActivity-release-unsigned.apk temp.apk && adb install -r temp.apk

