QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../okgeo/okgeo.pri)

INCLUDEPATH += ../okgeo

HEADERS +=  \
    testgeneexpressionmerger/tst_testgeneexprhelper.h \
    teststatsmedian/tst_teststatsmedian.h

SOURCES +=  \
    main.cpp \
    testgeneexpressionmerger/tst_testgeneexprhelper.cpp \
    teststatsmedian/tst_teststatsmedian.cpp
