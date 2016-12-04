QT  += network widgets
QT  += core

CONFIG += c++11

TARGET = UpdMulticastSender
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sender.cpp

HEADERS += \
    sender.h
