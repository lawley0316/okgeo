include(okgeo.pri)

SOURCES += main.cpp

win32: RC_ICONS += static/logo.ico
macx: ICON = static/logo.icns

TRANSLATIONS += \
    static/translations/app_en.ts \
    static/translations/app_zh_CN.ts
