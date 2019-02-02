For android build
```
sudo apt-get install openjdk-8-jdk ant
#sudo apt-get install android-sdk google-android-ndk-installer

export JAVA_OPTS="-XX:+IgnoreUnrecognizedVMOptions --add-modules java.se.ee"
./tools/bin/sdkmanager "platforms;android-10"
./tools/bin/sdkmanager "system-images;android-25;google_apis;x86"
./tools/bin/sdkmanager "emulator"
./tools/bin/sdkmanager --licenses
./tools/bin/avdmanager create avd -n test -k "system-images;android-25;google_apis;x86"
```
