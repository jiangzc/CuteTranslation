#-------------------------------------------------
#
# Project created by QtCreator 2019-10-11T12:13:17
#
#-------------------------------------------------

QT       += core gui x11extras webenginewidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CuteTranslation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 x11
PKGCONFIG += xcb xcb-util
LIBS += -lX11 -lXext -lXtst

SOURCES += \
        src/baidutranslate.cpp \
        src/configtool.cpp \
        src/configwindow.cpp \
        src/event_monitor.cpp \
        src/floatbutton.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
        src/picker.cpp \
        src/searchbar.cpp \
        src/shortcut.cpp \
        src/systemtrayicon.cpp \
        src/xdotool.cpp

HEADERS += \
        src/baidutranslate.h \
        src/configtool.h \
        src/configwindow.h \
        src/event_monitor.h \
        src/floatbutton.h \
        src/mainwindow.h \
        src/picker.h \
        src/searchbar.h \
        src/shortcut.h \
        src/systemtrayicon.h \
        src/xdotool.h

FORMS += \
        src/configwindow.ui \
        src/floatbutton.ui \
        src/mainwindow.ui \
        src/searchbar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
