#!/bin/bash

VERSION=`grep -w ^VERSION tatoge.pro | grep -o '[^ \s=]*$'`

export PATH=../mxe/usr/bin/:$PATH
i686-w64-mingw32.static-qmake-qt5
make

cp LICENSE release/
cp -r examples/ release/
rm release/examples/all.json

cd release/
zip -r tatoge-$VERSION-windows.zip tatoge.exe examples LICENSE

