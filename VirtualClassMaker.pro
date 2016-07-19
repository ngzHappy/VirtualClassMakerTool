CONFIG += c++14
QT     += core gui
QT     += widgets

TARGET = VirtualClassMaker
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ClassNameReadWrite.cpp \
    VirtualClassMaker.cpp

HEADERS  += MainWindow.hpp \
    ClassNameReadWrite.hpp \
    VirtualClassMaker.hpp




