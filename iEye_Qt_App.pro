# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = iEye_Qt_App
QT += core gui qaxserver qaxcontainer
CONFIG += debug console
DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2
INCLUDEPATH += ../../../serial/event \
    D:/McMaster_WS/4OI6_repo\serial\event \
    ../../../../../Qt/4.7.4/qextserialport-1.2rc/src \
    ../../../../../opencv/include/opencv \
    ../../../../../opencv/include/opencv2 \
    ../../../../../opencv/build/include/opencv \
    ../../../../../opencv/build/include/opencv2 \
    ../../../../../opencv/build/include \
    ./../../Software_Model \
    ./debug \
    . \
    ./../../../../Qt/4.8.4/mkspecs/win32-g++ \
    ./GeneratedFiles

INCLUDEPATH += ../../../Software_Model \
    D:/opencv/include \
    D:/opencv/include/opencv \
    D:/opencv/include/opencv2 \
    D:/opencv/build/include \
    D:/opencv/build/include/opencv \
    D:/opencv/build/include/opencv2 \
    D:/Qt/Qt5.0.1/5.0.1/mingw47_32/qextserialport-1.2rc/examples/event

LIBS += -L"../../../../../opencv/build/x86/mingw/lib" \
    -lopencv_core243 \
    -lopencv_imgproc243 \
    -lopencv_highgui243 \
    -lopencv_ml243 \
    -lopencv_video243 \
    -lopencv_features2d243 \
    -lopencv_calib3d243 \
    -lopencv_objdetect243 \
    -lopencv_contrib243 \
    -lopencv_legacy243 \
    -lopencv_flann243 \
    -lsetupapi \
    -ladvapi32 \
    -luser32
DEPENDPATH += .
MOC_DIR += D:/McMaster_WS/4OI6_repo/Qt_App/iEye_Qt_App-build-Desktop_Qt_4_8_4_MinGW_32bit-Debug/debug
OBJECTS_DIR +=  D:/McMaster_WS/4OI6_repo/Qt_App/iEye_Qt_App-build-Desktop_Qt_4_8_4_MinGW_32bit-Debug/debug
include(iEye_Qt_App.pri)

HEADERS += \
    ieye3by3quantizedmonitor.h \
    iEyeMappingPosMonitor.h

SOURCES += \
    ieye3by3quantizedmonitor.cpp \
    iEyeMappingPosMonitor.cpp
