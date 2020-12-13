TEMPLATE = app
CONFIG += c++17
CONFIG -= app_bundle
CONFIG += qt
QT     += core gui
QT     += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS += miem-logo.ico

SOURCES += \
    decisionnode.cpp \
    graphicsview.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    processnode.cpp \
    terminal.cpp \
    textdialog.cpp \
    textnode.cpp

HEADERS += \
    decisionnode.h \
    graphicsview.h \
    mainwindow.h \
    node.h \
    processnode.h \
    terminal.h \
    textdialog.h \
    textnode.h \
    vector.h

FORMS += \
    mainwindow.ui \
    textdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
