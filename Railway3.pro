QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    display.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    point.cpp \
    schedule.cpp \
    scheduleviewer.cpp \
    station.cpp \
    timePoint.cpp \
    train.cpp

HEADERS += \
    dialog.h \
    display.h \
    mainwindow.h \
    map.h \
    point.h \
    schedule.h \
    scheduleviewer.h \
    station.h \
    timePoint.h \
    train.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    scheduleviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
