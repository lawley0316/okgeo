include(okgeo.pri)

SOURCES += main.cpp

win32: RC_ICONS += static/logo.ico
macx: ICON = static/logo.icns
