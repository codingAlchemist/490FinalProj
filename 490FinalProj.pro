#-------------------------------------------------
#
# Project created by QtCreator 2014-04-13T19:52:11
#
#-------------------------------------------------

QT       += core gui
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 490FinalProj
TEMPLATE = app

INCLUDEPATH += /usr/local/Cellar/opencv/2.4.8.2/include/
LIBS += -L/usr/local/Cellar/opencv/2.4.8.2/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_features2d\
     -lopencv_highgui\
     -lopencv_objdetect \
     -lopencv_calib3d \
     -lopencv_video



SOURCES += main.cpp\
        mainwindow.cpp \
    image.cpp

HEADERS  += mainwindow.h \
    image.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    blackwhite.png \
    images/blackwhite.png

RESOURCES += \
    images/resources.qrc
