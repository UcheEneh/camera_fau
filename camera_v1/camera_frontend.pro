#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T23:31:43
#
#-------------------------------------------------

QT += core gui multimedia multimediawidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera_frontend
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


# To include DESKTOP opencv (method 1)
# INCLUDEPATH += /usr/local/include/
# LIBS += `pkg-config --libs opencv`

# To include ANDROID opencv
# include(../../QtProject/shared/opencv_android.pri)

SOURCES += \
    main.cpp \
    camera.cpp \
    imagesettings.cpp \
    #edit_info.cpp \
    ClipScene.cpp \
    cropping_dialog.cpp \
    edit_dialog.cpp \
    SizeGripItem.cpp

HEADERS += \
    camera.h \
    imagesettings.h \
    #edit_info.h \
    ClipScene.h \
    cropping_dialog.h \
    edit_dialog.h \
    SizeGripItem.h

FORMS += \
    camera.ui \
    imagesettings.ui \
    #edit_info.ui \
    cropping_dialog.ui \
    edit_dialog.ui

CONFIG += mobility
MOBILITY = 

QT+=widgets
include(../../QtProject/shared/shared.pri)

RESOURCES += \
    files.qrc
