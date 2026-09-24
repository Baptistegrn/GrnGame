to build do : ./gradlew assembleDebug or ./gradlew assembleRelease

app icon is in src/main/res

dont forget to install gradlew and android sdk system 

edit local.properties if the compilation failed

you can set your app key in app/src/build.gradle

run this before build in release (for create the key) :

 keytool -genkeypair -v -keystore release-key.jks -alias grngame -keyalg RSA -keysize 2048 -validity 10000

