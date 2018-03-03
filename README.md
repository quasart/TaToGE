# TableTop Game Equipment, a.k.a. TaToGE

This project provides a bunch of simple widgets to emulate the common game equipment needed when playing board games or card games : Dice, sandtimers, score counters...

It focuses on simplicity, and on the idea it will be used as a companion during "real" table-top gaming.

## Features

- Widgets for game state display:
  - `Timer` for virtual sand-clock
  - `Counter`
  - `CountDown`/`Sequence` steps follower
- Widgets to generate random:
  - `Dice`
  - `DiceCup` to roll a group of dice
  - `CardDrawer` for draw without replacement (Note that draw *with* replacement can be performed with `DiceCup` widget)
- General status saving/loading (in json format)


# Developer guide

## Improvement ideas

- New widgets:
   - Stopwatch
   - Score table
   - random "Go" signal
- Add sounds on events
- General reset button
- General roll button
- Tools dynamic delete/order
- Dice/Cards random values could be icons
- Widget labels could be icons

## Known issues

- Keyboard navigation does not trigger auto-scroll in AddEquipmentDialog.
- Command line help is not translated.
- TODO : Counter increments serialization

## Build dependency

Needs:
- Qt5.9
- C++11 compiler
- qmake to generate Makefile, and lrelease to generate translation file
- Imagemagick to render icon files

That means on debian/ubuntu:
```
sudo apt-get install qtbase5-dev qt5-default qttools5-dev-tools imagemagick
lrelease tatoge.pro
./images/render.sh
qmake
make
```

