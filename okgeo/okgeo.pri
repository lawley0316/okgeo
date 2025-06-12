QT += core gui widgets concurrent

HEADERS += \
    $$PWD/src/consts/api.h \
    $$PWD/src/fend/uicom/dialog.h \
    $$PWD/src/fend/uicom/loading.h \
    $$PWD/src/fend/uicom/navitem.h \
    $$PWD/src/bend/gateway.h \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.h \
    $$PWD/src/middle/manager.h \
    $$PWD/src/middle/signalmanager.h \
    $$PWD/src/utils/error.h \
    $$PWD/src/utils/helpers.h \
    $$PWD/src/utils/rapidcsv.h \
    $$PWD/src/fend/uimain/uimain.h

SOURCES += \
    $$PWD/src/fend/uicom/dialog.cpp \
    $$PWD/src/fend/uicom/loading.cpp \
    $$PWD/src/fend/uicom/navitem.cpp \
    $$PWD/src/bend/gateway.cpp \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.cpp \
    $$PWD/src/middle/manager.cpp \
    $$PWD/src/middle/signalmanager.cpp \
    $$PWD/src/utils/helpers.cpp \
    $$PWD/src/fend/uimain/uimain.cpp

FORMS += \
    $$PWD/src/fend/uicom/dialog.ui \
    $$PWD/src/fend/uicom/loading.ui \
    $$PWD/src/fend/uimain/uimain.ui \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.ui

RESOURCES += \
    $$PWD/static.qrc
