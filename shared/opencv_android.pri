#ANDROID
OPENCVSDKNATIVE = /home/uche/Downloads/opencv-3.4.3-android-sdk/OpenCV-android-sdk/sdk/native

INCLUDEPATH += $$OPENCVSDKNATIVE/jni/include
LIBS +=\
    -L"$$OPENCVSDKNATIVE/3rdparty/libs/armeabi-v7a"\
    -L"$$OPENCVSDKNATIVE/staticlibs/armeabi-v7a"\
     -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -llibtiff -llibjpeg-turbo -llibjasper -llibpng -lIlmImf -llibprotobuf -llibwebp -ltbb -lcpufeatures -ltegra_hal -lopencv_videoio

ANDROID_EXTRA_LIBS += $$OPENCVSDKNATIVE/libs/armeabi-v7a/libopencv_java3.so
