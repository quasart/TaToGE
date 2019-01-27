TEMPLATE = app
TARGET = tatoge
DESTDIR = .

INCLUDEPATH += .
QT += widgets
SOURCES += main.cpp MainWindow.cpp Table.cpp AddWidgetDialog.cpp
HEADERS += widgets/*.h  MainWindow.h Table.h AddWidgetDialog.h
CONFIG += c++14

VERSION= 0.1.0
DEFINES += M_APP_VERSION=\\\"$$VERSION\\\"

### Translation ###

TRANSLATIONS = i18n/tatoge_fr.ts
RESOURCES   += i18n/tatoge_fr.qm

LRELEASE_TARGET.target = i18n/tatoge_fr.qm
LRELEASE_TARGET.depends = i18n/tatoge_fr.ts
LRELEASE_TARGET.commands = lrelease tatoge.pro
QMAKE_EXTRA_TARGETS += LRELEASE_TARGET
PRE_TARGETDEPS += i18n/tatoge_fr.qm

### Icon ###

RESOURCES   += images/dice.svg
RC_ICONS    += images/dice.ico

ICO_TARGET.target = images/dice.ico
ICO_TARGET.depends = images/dice.svg
ICO_TARGET.commands = convert -render -background transparent images/dice.svg -define icon:auto-resize=64,48,32,16 images/dice.ico
QMAKE_EXTRA_TARGETS += ICO_TARGET
win32:PRE_TARGETDEPS += images/dice.ico

RESOURCES   += images/8ball.svg
RESOURCES   += images/coin.svg
RESOURCES   += images/counter.svg
RESOURCES   += images/custom.svg
RESOURCES   += images/dice20.svg
RESOURCES   += images/dice2d6.svg
RESOURCES   += images/dice4df.svg
RESOURCES   += images/dice6.svg
RESOURCES   += images/dice8.svg
RESOURCES   += images/dice.ico
RESOURCES   += images/dice.svg
RESOURCES   += images/logo.svg
RESOURCES   += images/roulette.svg
RESOURCES   += images/sandtimer.svg
RESOURCES   += images/space.svg
RESOURCES   += images/videau.svg


### Forbid use of deprecated feature of Qt ###

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

