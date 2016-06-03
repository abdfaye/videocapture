TEMPLATE = app
CONFIG -= app_bundle
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += av

QT_XCB_GL_INTEGRATION=xcb_egl

LIBS += -lQt5AVWidgets -lQt5AV
SOURCES += main.cpp \
    playerwindow.cpp

HEADERS += \
    playerwindow.h
