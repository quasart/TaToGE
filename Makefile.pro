TEMPLATE = app
TARGET = uGameTools
DESTDIR = .

INCLUDEPATH += .
QT += widgets
SOURCES += main.cpp MainWindow.cpp Table.cpp AddWidgetDialog.cpp
HEADERS += widgets/*.h  MainWindow.h Table.h AddWidgetDialog.h
CONFIG += c++14

VERSION= 0.1.0
DEFINES += M_APP_VERSION=\\\"$$VERSION\\\"

### Translation ###

TRANSLATIONS = i18n/uGameTools_fr.ts
RESOURCES   += i18n/uGameTools_fr.qm

LRELEASE_TARGET.target = i18n/uGameTools_fr.qm
LRELEASE_TARGET.depends = i18n/uGameTools_fr.ts
LRELEASE_TARGET.commands = lrelease Makefile.pro
QMAKE_EXTRA_TARGETS += LRELEASE_TARGET
PRE_TARGETDEPS += i18n/uGameTools_fr.qm

### Icon ###

RESOURCES   += images/dice.svg
RC_ICONS    += images/dice.ico

ICO_TARGET.target = images/dice.ico
ICO_TARGET.depends = images/dice.svg
ICO_TARGET.commands = convert -render -background transparent images/dice.svg -define icon:auto-resize=64,48,32,16 images/dice.ico
QMAKE_EXTRA_TARGETS += ICO_TARGET
win32:PRE_TARGETDEPS += images/dice.ico

### Forbid use of deprecated feature of Qt ###

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

