QT += core gui widgets concurrent

HEADERS += \
    $$PWD/src/consts/api.h \
    $$PWD/src/fend/uiabout/uiabout.h \
    $$PWD/src/fend/uicom/dialog.h \
    $$PWD/src/fend/uicom/loading.h \
    $$PWD/src/fend/uicom/navitem.h \
    $$PWD/src/bend/gateway.h \
    $$PWD/src/fend/uiphenotype/uiphenotype.h \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.h \
    $$PWD/src/middle/language.h \
    $$PWD/src/middle/manager.h \
    $$PWD/src/middle/signals.h \
    $$PWD/src/utils/error.h \
    $$PWD/src/utils/helpers.h \
    $$PWD/src/utils/rapidcsv.h \
    $$PWD/src/fend/uimain/uimain.h \
    $$PWD/src/utils/stats.h \
    $$PWD/src/utils/str.h

SOURCES += \
    $$PWD/src/fend/uiabout/uiabout.cpp \
    $$PWD/src/fend/uicom/dialog.cpp \
    $$PWD/src/fend/uicom/loading.cpp \
    $$PWD/src/fend/uicom/navitem.cpp \
    $$PWD/src/bend/gateway.cpp \
    $$PWD/src/fend/uiphenotype/uiphenotype.cpp \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.cpp \
    $$PWD/src/middle/language.cpp \
    $$PWD/src/middle/manager.cpp \
    $$PWD/src/middle/signals.cpp \
    $$PWD/src/utils/helpers.cpp \
    $$PWD/src/fend/uimain/uimain.cpp

FORMS += \
    $$PWD/src/fend/uiabout/uiabout.ui \
    $$PWD/src/fend/uicom/dialog.ui \
    $$PWD/src/fend/uicom/loading.ui \
    $$PWD/src/fend/uimain/uimain.ui \
    $$PWD/src/fend/uiphenotype/uiphenotype.ui \
    $$PWD/src/fend/uiprobetogene/uiprobetogene.ui

RESOURCES += \
    $$PWD/static.qrc
