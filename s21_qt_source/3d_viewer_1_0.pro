QT       += core gui opengl
include(../QtGifImage/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets
isEqual(QT_MAJOR_VERSION, 5): LIBS += -lGLU

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../s21_c_source/s21_moving.c \
    ../s21_c_source/s21_parsing.c \
    debugwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    s21_gl_view.cpp

HEADERS += \
    ../s21_c_source/s21_3d_viewer_1_0.h \
    debugwindow.h \
    mainwindow.h \
    s21_gl_view.h

FORMS += \
    debugwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
