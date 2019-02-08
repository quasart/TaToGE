TEMPLATE = app
TARGET = tatoge
DESTDIR = .
CONFIG += c++14
INCLUDEPATH += .
QT += widgets

SOURCES += main.cpp
SOURCES += MainWindow.cpp Table.cpp AddWidgetDialog.cpp JsonCodec.cpp
HEADERS += MainWindow.h   Table.h   AddWidgetDialog.h   JsonCodec.h
HEADERS += widgets/CardDrawer.h
HEADERS += widgets/Counter.h
HEADERS += widgets/Dice.h
HEADERS += widgets/Sequence.h
HEADERS += widgets/Timer.h
RESOURCES = tatoge.qrc

VERSION = 0.2.0
DEFINES += M_APP_VERSION=\\\"$$VERSION\\\"

unix {
  OBJECTS_DIR = .obj
  MOC_DIR     = .moc
  UI_DIR      = .ui
}


### Translation ###

TRANSLATIONS = i18n/tatoge_fr.ts

LRELEASE_TARGET.target = i18n/tatoge_fr.qm
LRELEASE_TARGET.depends = i18n/tatoge_fr.ts
LRELEASE_TARGET.commands = lrelease tatoge.pro
QMAKE_EXTRA_TARGETS += LRELEASE_TARGET
PRE_TARGETDEPS += i18n/tatoge_fr.qm

### Icons ###

RC_ICONS    += package/windows/tatoge.ico
ICO_TARGET.target= images/dice6.png
ICO_TARGET.depends = images/icons.svg
ICO_TARGET.commands = ./images/render.sh
QMAKE_EXTRA_TARGETS += ICO_TARGET
PRE_TARGETDEPS += images/dice6.png


### Forbid use of deprecated feature of Qt ###

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # disables all the APIs deprecated before Qt 6.0.0


### Install ###

unix {
  freedesktop.path = /usr/share/applications
  freedesktop.files = ./package/linux/tatoge.desktop
  INSTALLS += freedesktop

  install_icon.path = /usr/share/pixmaps
  install_icon.files = ./images/tatoge.svg
  INSTALLS += install_icon

  install_bin.path = /usr/bin
  install_bin.files = ./tatoge
  INSTALLS += install_bin

  examples.path = /usr/share/tatoge/examples
  examples.files += ./examples/backgammon.json
  examples.files += ./examples/chiffres_lettres.json
  examples.files += ./examples/duo.json
  examples.files += ./examples/pokerdice.json
  INSTALLS += examples
}

