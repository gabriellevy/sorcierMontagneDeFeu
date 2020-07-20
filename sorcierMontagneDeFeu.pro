#-------------------------------------------------
#
# Project created by QtCreator 2019-09-12T21:02:06
#
#-------------------------------------------------

QT       += core gui sql widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sorcierMontagneDeFeu
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

CONFIG += c++11
CONFIG += resources_big

include(../destinLib/abs/abs.pri)
include(../destinLib/gen/gen.pri)
include(../destinLib/exec/exec.pri)

SOURCES += \
    ../destinLib/gestcarac.cpp \
    ../destinLib/aspectratiolabel.cpp \
    ../destinLib/dbmanager.cpp \
    ../destinLib/glisseur.cpp \
    ../destinLib/reglages.cpp \
    ../destinLib/aleatoire.cpp \
    combat.cpp \
    equipement.cpp \
    gensorcmontagnefee.cpp \
    heros.cpp \
    main.cpp \
    ldoelh.cpp \
    numeros1-50.cpp \
    numeros51-100.cpp

HEADERS += \
    ../destinLib/gestcarac.h \
    ../destinLib/aspectratiolabel.h \
    ../destinLib/dbmanager.h \
    ../destinLib/glisseur.h \
    ../destinLib/reglages.h \
    ../destinLib/aleatoire.h \
    combat.h \
    equipement.h \
    gensorcmontagnefee.h \
    heros.h \
    ldoelh.h

FORMS += \
    ldoelh.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
