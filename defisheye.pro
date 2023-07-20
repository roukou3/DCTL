QT += core gui xml widgets

CONFIG += c++17

HEADERS += \
    Meike_6.5mm_f2.0.dctl \
    dctl.h \
    defisheye.h \
    grwidget.h \
    mainwindow.h

SOURCES += \
    dctl.cpp \
    grwidget.cpp \
    main.cpp \
    mainwindow.cpp

FORMS += \
    grwidget.ui \
    mainwindow.ui
