QT  += network
QT  += core

CONFIG += c++11

TARGET = UpdMulticastSender
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sender.cpp \
    packageFormat.pb.cc \
    deliveryhandler.cpp

HEADERS += \
    sender.h \
    packageFormat.pb.h \
    pbuff.h \
    deliveryhandler.h

INCLUDEPATH += $$PWD/../../../../../../Games/protobuf1/src

win32: LIBS += -L$$PWD/./ -lprotobuf

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./protobuf.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/./libprotobuf.a
