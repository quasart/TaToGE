TEMPLATE = app
TARGET = tatoge
DESTDIR = .

INCLUDEPATH += .
QT += widgets
SOURCES += main.cpp MainWindow.cpp Table.cpp AddWidgetDialog.cpp
HEADERS += widgets/*.h  MainWindow.h Table.h AddWidgetDialog.h
CONFIG += c++14

VERSION = 0.1.2
DEFINES += M_APP_VERSION=\\\"$$VERSION\\\"

### Translation ###

TRANSLATIONS = i18n/tatoge_fr.ts
RESOURCES   += i18n/tatoge_fr.qm

LRELEASE_TARGET.target = i18n/tatoge_fr.qm
LRELEASE_TARGET.depends = i18n/tatoge_fr.ts
LRELEASE_TARGET.commands = lrelease tatoge.pro
QMAKE_EXTRA_TARGETS += LRELEASE_TARGET
PRE_TARGETDEPS += i18n/tatoge_fr.qm

### Icons ###

RC_ICONS    += images/dice.ico
RESOURCES   += images/dice.svg
ICO_TARGET.target = images/dice.ico
ICO_TARGET.depends = images/dice.svg
ICO_TARGET.commands = convert -render -background transparent images/dice.svg -define icon:auto-resize=64,48,32,16 images/dice.ico
QMAKE_EXTRA_TARGETS += ICO_TARGET
PRE_TARGETDEPS += images/dice.ico

RESOURCES   += images/8ball.png
RESOURCES   += images/coin.png
RESOURCES   += images/counter.png
RESOURCES   += images/custom.png
RESOURCES   += images/dice20.png
RESOURCES   += images/dice2d6.png
RESOURCES   += images/dice4df.png
RESOURCES   += images/dice6.png
RESOURCES   += images/dice8.png
RESOURCES   += images/dice.png
RESOURCES   += images/logo.png
RESOURCES   += images/roulette.png
RESOURCES   += images/sandtimer.png
RESOURCES   += images/space.png
RESOURCES   += images/videau.png
SVG2PNG_TARGET.target = images/dice6.png
SVG2PNG_TARGET.depends = images/dice6.svg
SVG2PNG_TARGET.commands = ./images/render.sh
QMAKE_EXTRA_TARGETS += SVG2PNG_TARGET
PRE_TARGETDEPS += images/dice6.png


### Forbid use of deprecated feature of Qt ###

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0

