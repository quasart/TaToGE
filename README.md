# µGameTools

This project provides a bunch of simple widgets to emulate basic tools needed when playing board games or card games.

It focuses on simplicity.

## Features

- Virtual board-game tools:
  - SandTimer
  - Counter
  - Dice
  - Sequence/CountDown follower
- Template file loading (in json format)


# Developer guide

## Improvement ideas

- New widgets:
   - Score table
   - "Go" signal
- Add sounds on events
- General reset button
- Tools dynamic Update/Delete
- Game status/template export
- Dice graphics with dots
- App icon
- Open source licence
- Expert mode (json edit) for adding widgets
- docker build environement (for Windows, Mobile...)

## Build command

Needs Qt5 and C++ compiler.

```
sudo apt-get install qtbase5-dev qt5-default
qmake
lrelease Makefile.pro
make
```

## Known issues


