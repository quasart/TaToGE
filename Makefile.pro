TEMPLATE = app
TARGET = uGameTools

INCLUDEPATH += .
QT += widgets
SOURCES += main.cpp
HEADERS += widgets/*.h  Window.h AddWidgetDialog.h

TRANSLATIONS = i18n/uGameTools_fr.ts
RESOURCES   += i18n/uGameTools_fr.qm


# Forbid use of deprecated feature of Qt

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

